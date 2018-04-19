#pragma once
#include <unordered_set> // std::unordered_set
#include <unordered_map> // std::unordered_map
#include <algorithm> // std::generate
#include <numeric> // std::inner_product
#include <iterator> // std::distance
#include <cstring> // std::memset
#include "fmath.hpp"


//==========================================================================================================================//
//   _____  _____  _____ ______  _____      _____  _____  _____ ______ ______  _____  _____  _____  _____  _   _  _____     //
//  /  ___|/  __ \|  _  || ___ \|  ___|    /  __ \|  _  ||  ___||  ___||  ___||_   _|/  __ \|_   _||  ___|| \ | ||_   _|    //
//  \ `--. | /  \/| | | || |_/ /| |__      | /  \/| | | || |__  | |_   | |_     | |  | /  \/  | |  | |__  |  \| |  | |      //
//   `--. \| |    | | | ||    / |  __|     | |    | | | ||  __| |  _|  |  _|    | |  | |      | |  |  __| | . ` |  | |      //
//  /\__/ /| \__/\\ \_/ /| |\ \ | |___     | \__/\\ \_/ /| |___ | |    | |     _| |_ | \__/\ _| |_ | |___ | |\  |  | |      //
//  \____/  \____/ \___/ \_| \_|\____/      \____/ \___/ \____/ \_|    \_|     \___/  \____/ \___/ \____/ \_| \_/  \_/      //
//                                                                                                                          //
//==========================================================================================================================//


namespace score
{
    struct
    {
        template<typename lbl> inline auto operator()(lbl *true_lbl, lbl *predict_lbl, const int &n)
        {
            return std::inner_product(  true_lbl, true_lbl + n, 
                                        predict_lbl, 0,
                                        std::plus<int>(),
                                        [](const lbl &t, const lbl &p)
                                        {
                                            return (int)(t == p);
                                        });
        }
    } accuracy;

    struct
    {
        template<typename lbl> inline auto operator()(lbl *true_lbl, lbl *predict_lbl, const int &n)
        {
            std::unordered_map<lbl, int> perfs;         
            for(int i = 0; i < n; ++i) perfs[true_lbl[i]] += (true_lbl[i] == predict_lbl[i]) ? 1 : 0;
            return perfs;
        }
    } perfs;

    struct
    {
        template<typename lbl> inline auto operator()(lbl *true_lbl, lbl *predict_lbl, const int &n)
        {
            return predict_lbl;
        }
    } predict;

    struct
    {
        template<typename lbl> inline auto operator()(lbl *true_lbl, lbl *predict_lbl, const int &n)
        {
            std::unordered_set<lbl> unique(true_lbl, true_lbl + n, sizeof(lbl)*n);
            int n_lbl = (int)unique.size();
            int **cfs_matrix = new int*[n_lbl];
            std::generate(cfs_matrix, cfs_matrix + n_lbl, [&n_lbl]{return new int[n_lbl];});
            std::for_each(cfs_matrix, cfs_matrix + n_lbl, [&n_lbl](int *x){std::memset(x, 0, sizeof(int)*n_lbl);});

            for(int i = 0; i < n; ++i) ++cfs_matrix[std::distance(unique.begin(), unique.find(true_lbl[i]))][std::distance(unique.begin(), unique.find(predict_lbl[i]))];
            return cfs_matrix;
        }
    } confusion_matrix;
    
    struct
    {
        template<typename lbl> inline auto operator()(lbl *true_lbl, lbl *predict_lbl, const int &n)
        {
            std::unordered_set<lbl> unique(true_lbl, true_lbl + n, sizeof(lbl)*n);
            int n_lbl = (int)unique.size(),
                **cfs = new int*[n_lbl];
            float mcc;
            std::generate(cfs, cfs + n_lbl, [&n_lbl]{return new int[n_lbl];});
            std::for_each(cfs, cfs + n_lbl, [&n_lbl](int *x){std::memset(x, 0, sizeof(int)*n_lbl);});

            for(int i = 0; i < n; ++i) ++cfs[std::distance(unique.begin(), unique.find(true_lbl[i]))][std::distance(unique.begin(), unique.find(predict_lbl[i]))];
            
            switch(n_lbl)
            {
                case 2:
                    mcc = float(cfs[0][0] * cfs[1][1] - cfs[0][1] * cfs[1][0]) / 
                          sqrt(float( (cfs[0][0] + cfs[1][0]) * (cfs[0][0] + cfs[0][1]) * (cfs[1][1] + cfs[0][1]) * (cfs[1][1] + cfs[1][0]) ));
                    break;
                default:
                    int A1, A2, B1, B2, D1 = 0, D2 = 0, num = 0;
                    for (int i = 0; i < n_lbl; ++i)
                    {
                        A1 = 0; A2 = 0; B1 = 0; B2 = 0;
                        for (int j = 0; j < n_lbl; ++j)
                        {
                            A1 += cfs[i][j];
                            A2 += cfs[j][i];
                            for (int k = 0; k < n_lbl; ++k)
                            {
                                num += cfs[i][i] * cfs[j][k] - cfs[i][j] * cfs[k][i];
                                B1 += (k != i) ? cfs[k][j] : 0;
                                B2 += (k != i) ? cfs[j][k] : 0;
                            }
                        }
                        D1 += A1*B1;
                        D2 += A2*B2;
                    }
                    mcc = (float)num / (sqrt( float(D1 * D2)) );
                    break;
            }
            for(int i = 0; i < n_lbl; ++i) delete[] cfs[i];
            delete[] cfs;
            return mcc; 
        }
    } matthews_corrcoef;

    struct
    {   
        template<typename lbl> inline auto operator()(lbl *true_lbl, lbl *predict_lbl, const int &n, const auto &beta)
        {
            float score, beta_sq = beta*beta;
            int **cfs = nullptr;
            std::unordered_set<lbl> unique(true_lbl, true_lbl + n, sizeof(lbl)*n);
            int n_lbl = (int)unique.size();
            cfs = new int*[n_lbl];
            std::generate(cfs, cfs + n_lbl, [&n_lbl]{return new int[n_lbl];});
            std::for_each(cfs, cfs + n_lbl, [&n_lbl](int *x){std::memset(x, 0, sizeof(int)*n_lbl);});

            for(int i = 0; i < n; ++i) ++cfs[std::distance(unique.begin(), unique.find(true_lbl[i]))][std::distance(unique.begin(), unique.find(predict_lbl[i]))];
            
            score = ((1.f + beta_sq) * cfs[0][0]) / 
                    ((1.f + beta_sq) * cfs[0][0] + beta_sq*cfs[0][1] + cfs[1][0]);
            for(int i = 0; i < n_lbl; ++i) delete[] cfs;
            delete[] cfs;
            return score;
        }
    } fbeta; 

    struct
    {
        template<typename lbl> inline auto operator()(lbl *true_lbl, lbl *predict_lbl, const int &n)
        {
            std::unordered_set<lbl> unique(true_lbl, true_lbl + n, sizeof(lbl)*n);
            int n_lbl = (int)unique.size(),
                **cfs = new int*[n_lbl], N;
            float score;
            std::generate(cfs, cfs + n_lbl, [&n_lbl]{return new int[n_lbl];});
            std::for_each(cfs, cfs + n_lbl, [&n_lbl](int *x){std::memset(x, 0, sizeof(int)*n_lbl);});

            for(int i = 0; i < n; ++i) ++cfs[std::distance(unique.begin(), unique.find(true_lbl[i]))][std::distance(unique.begin(), unique.find(predict_lbl[i]))];
            
            N = cfs[0][0] + cfs[0][1] + cfs[1][0] + cfs[1][1];
            score = float(cfs[0][0] * cfs[1][1] - cfs[0][1] * cfs[1][0]) /
                    sqrt(float( (cfs[0][0] + cfs[0][1]) * (cfs[0][0] + cfs[0][1]) * (N - cfs[0][0] + cfs[0][1]) * (N - cfs[0][0] + cfs[0][1]) ));

            for(int i = 0; i < n_lbl; ++i) delete[] cfs;
            delete[] cfs;
            return score;
        }
    } phi_coef;
}
