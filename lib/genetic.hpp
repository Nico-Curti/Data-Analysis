#include "os.hpp"
#include "fstat.hpp" // pair struct and argsort utilities
#include <memory> // std::unique_ptr
#include <algorithm> // std::generate
#include <type_traits> // std::result_of, std::invoke_result
#include <iostream> // std::cout 
#include <fstream> // outputlog
#include <string> // std::to_string
#include <omp.h> // parallel

struct
{
	// NEED:
	// Overload operator + for crossover
	// Overload operator ! for mutation
	// Constructor with unsigned int as rng
	// Default Constructor
	// FOR LOGFILE:
	// Overload operator <<
	template<typename genome, typename Func> auto operator()(const genome &target,
															 const int &n_population,
															 const int &max_iter,
															 Func fit,
															 float elit_rate = .1f,
															 float mutation_rate = .3f,
															 unsigned int seed = 0,
															 bool quiet = false // logfile
															 )
	{
		//typename std::invoke_result<Func(const genome &, const genome &)>::type res; // since c++17
		typename std::result_of<Func(const genome &, const genome &)>::type res; // since c++11

		int best, size, diff;
		const int half = n_population >> 1, 
				  elite = n_population * elit_rate, 
				  mutation = RAND_MAX * mutation_rate;
		decltype(res) *fitness = new decltype(res)[n_population];
		std::unique_ptr<genome[]> population(new genome[n_population]);
		srand(seed);
		std::generate(population.get(), population.get() + n_population, []{return genome(rand());});
		std::ofstream os;
		switch((int)quiet)
		{
			case 0:
			{
				os.open("genetic_" + type_name<genome>() + 
						"_n" + std::to_string(n_population) + 
						"_i" + std::to_string(max_iter) + 
						"_e" + std::to_string(elit_rate) +
						"_m" + std::to_string(mutation_rate) +
						"_s" + std::to_string(seed) +
						"_f" + type_name<Func>() +
						".log");
				os << "it\tBest\tfitness\ttarget" << std::endl;
			}break;
			case 1:
			break;
		}

		for(int i = 0; i < max_iter; ++i)
		{
#pragma omp parallel for
			for(int j = 0; j < n_population; ++j)
				fitness[j] = fit(population[j], target);

			// simple argsort for ranking of fitness (parallel version speedUp .5X)
			int *rank = argsort<true>(fitness, 0, n_population);
			best = rank[0];

			switch((int)quiet)
			{
				case 0:
					os << i << "\t" << population[best] << "\t" << fitness[best] << "\t" << target << std::endl;
				break;
				case 1:
				break;
			}

			if(fitness[best] == 0) break;
			std::unique_ptr<genome[]> new_gen(new genome[n_population]);
			//cross over and mutation
#pragma omp parallel for
			for(int j = 0; j < n_population; ++j)
			{
				// crossover
				new_gen[j] = (rank[j] < elite) ? population[rank[j]] :
											     population[rank[rand() % half]] + population[rank[rand() % half]];
				// random mutation
				if(rand() < mutation) !new_gen[j];
			}
			population = std::move(new_gen);
			delete[] rank;
		}
		delete[] fitness;
		if(!quiet) os.close();

		return population[best];
	}

} genetic;

