#pragma once
#include <memory> // std::unique_ptr
#include <chrono> // std::high_resolution_clock
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60
#include "fstat.hpp"
#include "score_coef.hpp"
#include "cv_classifier.hpp"

namespace grid_search
{
    void printProgress (const double &now, const double &total, const std::chrono::high_resolution_clock::time_point &start_time)
    {
        int val = (int) (now/total * 100);
        int lpad = (int) (now/total * PBWIDTH);
        printf ("\rOptimization Progress:\t%3d%% [%.*s%*s] %d/%d [%.3f sec]", 
                val, 
                lpad, 
                PBSTR, 
                PBWIDTH - lpad, 
                "",
                (int)now,
                (int)total,
                (double)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count());
        fflush (stdout);
    }
    // NEED:
    // Overload operator + for crossover
    // Overload operator ! for mutation
    // Overload operator () unsigned int as rng
    // Default Constructor
    // Overload operator =
    template<class Cls, typename T> auto genetic(const Patterns<T> &data,
                                                 const int &n_population, // number of dna
                                                 const int &max_iter, // max number of iteration
                                                 float elit_rate = .1f, // percentage of population to conserve
                                                 float mutation_rate = .3f, // probability of mutation
                                                 int K = 10, // number of folds
                                                 unsigned int seed = 0, // random seed
                                                 int nth = 1, // number of thread to use (power of two)
                                                 bool quiet = false, // cout
                                                 std::string log = "" // logfile name
                                                 )
    {
        int best, *fitness = new int[n_population], size, diff;
        const int half = n_population >> 1, 
                  elite = n_population * elit_rate, 
                  mutation = RAND_MAX * mutation_rate;
        std::unique_ptr<int[]> rank(new int[n_population]);
        std::unique_ptr<hyperparams<Cls>[]> population(new hyperparams<Cls>[n_population]);
        pair<int> *arr = new pair<int>[n_population];
        std::srand(seed);
        for(int i = 0; i < n_population; ++i) population[i](static_cast<unsigned int>(std::rand())); // change to generate
        std::ofstream os;
        auto start_time = std::chrono::high_resolution_clock::now();
        switch(int(log == ""))
        {
            case 0: // create log
            {
                os.open(log);
                os << "fitness\t";
                population[0].get_name(os, '\t');
                os << std::endl;
            } break;
            case 1:
            break;
        }
        switch((int)quiet)
        {
            case 0: std::cout << "Grid search algorithm (#population = " << n_population 
                                                        << "; max iterations = " << max_iter 
                                                        << "; elite rate = " << elit_rate 
                                                        << "; mutation rate = " << mutation_rate 
                                                        << ")" << std::endl;
            break;
            case 1: // silence
            break;
        }
        for(int i = 0; i < max_iter; ++i) // loop over number of iterations
        {
#pragma omp parallel num_threads(nth)
            {
#pragma omp for private(seed)
                for(int j = 0; j < n_population; ++j) // loop over population
                {
                    fitness[j] = cross_val_score<false, Cls>(data, 
                                                             std::bind( &CrossValidation::StratifiedKFold, 
                                                                        std::placeholders::_1, 
                                                                        data.output, K, 0, data.Nout, true, seed+j),
                                                             score::accuracy,
                                                             population[j]
                                                             );
                    // argsort variables
                    arr[j].value = fitness[j];
                    arr[j].idx = j;
                }

                // simple argsort for ranking of fitness (parallel version speedUp .5X)
                #pragma omp single
                {
                    nth = (nth % 2) ? nth-1 : nth;
                    size = ((diff = n_population % nth)) ? n_population - diff : n_population;
                    mergeargsort_parallel_omp(arr, fitness, 0, size, nth, std::greater<pair<int>>());
                }
            } // end of parallel zone
            if(diff)
            {
                std::sort(arr + size, arr + n_population);
                std::inplace_merge(arr, arr + size, arr + n_population, std::greater<pair<int>>());
            }
            for(int j = 0; j < n_population; ++j) rank[j] = arr[j].idx;
            best = rank[0];

            // break if converge
            if(fitness[best] == data.Nout) break;

            switch((int)quiet)
            {
                case 0: printProgress((double)fitness[best], (double)(data.Nout), start_time);//std::cout << "( it " << i << " ) Best = " << fitness[best] << " over " << data.Nout << " samples" << std::endl;
                break;
                case 1: // silence
                break;
            }
            switch(int(log == ""))
            {
                case 0: os << fitness[best] << "\t" << population[best] << std::endl;
                break;
                case 1:
                break;
            }

            //cross over and mutation
            std::unique_ptr<hyperparams<Cls>[]> new_gen(new hyperparams<Cls>[n_population]);
            for(int j = 0; j < n_population; ++j)
            {
                // crossover
                new_gen[j] = (rank[j] < elite) ? population[rank[j]] :
                                                 population[rank[std::rand() % half]] + population[rank[std::rand() % half]];
                // random mutation
                if(std::rand() < mutation) !new_gen[j];
            }
            population = std::move(new_gen);
        }
        if(log != "") os.close();
        delete[] arr;
        return population[best];
    }
        
} // end of namespace grid_search
