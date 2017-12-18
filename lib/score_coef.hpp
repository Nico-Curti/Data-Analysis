#pragma once
#include <unordered_set> // std::unordered_set
#include <algorithm> // std::generate
#include <iterator> // std::distance
#include <cstring> // std::memset
#include "fmath.hpp"

namespace score
{
	template<typename T, class lbl>	T** confusion_matrix	(lbl *, lbl *, const int &);
	template<typename T, class lbl>	T 	matthews_corrcoef	(lbl *, lbl *, const int &);
	template<typename T, class lbl>	T 	fbeta 			 	(lbl *, lbl *, const int &, const T &);
	template<typename T, class lbl>	T 	phi_coef		 	(lbl *, lbl *, const int &);
}


//==========================================================================================================================//
//	 _____  _____  _____ ______  _____      _____  _____  _____ ______ ______  _____  _____  _____  _____  _   _  _____ 	//
//	/  ___|/  __ \|  _  || ___ \|  ___|    /  __ \|  _  ||  ___||  ___||  ___||_   _|/  __ \|_   _||  ___|| \ | ||_   _|	//
//	\ `--. | /  \/| | | || |_/ /| |__      | /  \/| | | || |__  | |_   | |_     | |  | /  \/  | |  | |__  |  \| |  | |  	//
//	 `--. \| |    | | | ||    / |  __|     | |    | | | ||  __| |  _|  |  _|    | |  | |      | |  |  __| | . ` |  | |  	//
//	/\__/ /| \__/\\ \_/ /| |\ \ | |___     | \__/\\ \_/ /| |___ | |    | |     _| |_ | \__/\ _| |_ | |___ | |\  |  | |  	//
//	\____/  \____/ \___/ \_| \_|\____/      \____/ \___/ \____/ \_|    \_|     \___/  \____/ \___/ \____/ \_| \_/  \_/  	//
//	                                                                                                                    	//
//==========================================================================================================================//


namespace score
{
	template<typename T, class lbl> T** confusion_matrix(lbl *true_lbl, lbl *predict_lbl, const int &n)
	{
		std::unordered_set<lbl> unique(true_lbl, true_lbl + n, sizeof(lbl)*n);
		int n_lbl = (int)unique.size();
		T **cfs_matrix = new T*[n_lbl];
		std::generate(cfs_matrix, cfs_matrix + n_lbl, [&n_lbl]{return new T[n_lbl];});
		std::for_each(cfs_matrix, cfs_matrix + n_lbl, [&n_lbl](T *x){std::memset(x, (T)0., sizeof(T)*n_lbl);});

		for(int i = 0; i < n; ++i) ++cfs_matrix[std::distance(unique.begin(), unique.find(lbl_true[i]))][std::distance(unique.begin(), unique.find(lbl_pred[i]))];
		return cfs_matrix;
	}

	template<typename T, class lbl> T matthews_corrcoef(lbl *true_lbl, lbl *predict_lbl, const int &n)
	{
		std::unordered_set<lbl> unique(true_lbl, true_lbl + n, sizeof(lbl)*n);
		int n_lbl = (int)unique.size();
		T **cfs = new T*[n_lbl], mcc;
		std::generate(cfs, cfs + n_lbl, [&n_lbl]{return new T[n_lbl];});
		std::for_each(cfs, cfs + n_lbl, [&n_lbl](T *x){std::memset(x, (T)0., sizeof(T)*n_lbl);});

		for(int i = 0; i < n; ++i) ++cfs[std::distance(unique.begin(), unique.find(lbl_true[i]))][std::distance(unique.begin(), unique.find(lbl_pred[i]))];
		
		switch(n_lbl)
		{
			case 2:
				mcc = (cfs[0][0] * cfs[1][1] - cfs[0][1] * cfs[1][0]) / 
					  sqrt((cfs[0][0] + cfs[1][0]) * (cfs[0][0] + cfs[0][1]) * (cfs[1][1] + cfs[0][1]) * (cfs[1][1] + cfs[1][0]));
				break;
			default:
				T A1, A2, B1, B2, D1 = (T)0., D2 = (T)0., num = (T)0.;
				for (int i = 0; i < n_lbl; ++i)
				{
					A1 = (T)0.; A2 = (T)0.; B1 = (T)0.; B2 = (T)0.;
					for (int j = 0; j < n_lbl; ++j)
					{
						A1 += cfs[i][j];
						A2 += cfs[j][i];
						for (int k = 0; k < n_lbl; ++k)
						{
							num += cfs[i][i] * cfs[j][k] - cfs[i][j] * cfs[k][i];
							B1 += (k != i) ? cfs[k][j] : (T)0.;
							B2 += (k != i) ? cfs[j][k] : (T)0.;
						}
					}
					D1 += A1*B1;
					D2 += A2*B2;
				}
				mcc = num / (sqrt(D1 * D2));
				break;
		}
		for(int i = 0; i < n_lbl; ++i) delete[] cfs[i];
		delete[] cfs;
		return mcc;
	}

	template<typename T, class lbl>	T fbeta(lbl *true_lbl, lbl *predict_lbl, const int &n, const T &beta)
	{
		T score, beta_sq = beta*beta, **cfs = nullptr;
		std::unordered_set<lbl> unique(true_lbl, true_lbl + n, sizeof(lbl)*n);
		int n_lbl = (int)unique.size();
		cfs = new T*[n_lbl];
		std::generate(cfs, cfs + n_lbl, [&n_lbl]{return new T[n_lbl];});
		std::for_each(cfs, cfs + n_lbl, [&n_lbl](T *x){std::memset(x, (T)0., sizeof(T)*n_lbl);});

		for(int i = 0; i < n; ++i) ++cfs[std::distance(unique.begin(), unique.find(lbl_true[i]))][std::distance(unique.begin(), unique.find(lbl_pred[i]))];
		
		score =	(((T)1. + beta_sq) * cfs[0][0]) / 
				(((T)1. + beta_sq) * cfs[0][0] + beta_sq*cfs[0][1] + cfs[1][0]);
		for(int i = 0; i < n_lbl; ++i) delete[] cfs;
		delete[] cfs;
		return score;
	}
	template<typename T, class lbl>	T phi_coef(lbl *true_lbl, lbl *predict_lbl, const int &n)
	{
		std::unordered_set<lbl> unique(true_lbl, true_lbl + n, sizeof(lbl)*n);
		int n_lbl = (int)unique.size();
		T **cfs = new T*[n_lbl], score, N;
		std::generate(cfs, cfs + n_lbl, [&n_lbl]{return new T[n_lbl];});
		std::for_each(cfs, cfs + n_lbl, [&n_lbl](T *x){std::memset(x, (T)0., sizeof(T)*n_lbl);});

		for(int i = 0; i < n; ++i) ++cfs[std::distance(unique.begin(), unique.find(lbl_true[i]))][std::distance(unique.begin(), unique.find(lbl_pred[i]))];
		
		N = cfs[0][0] + cfs[0][1] + cfs[1][0] + cfs[1][1];
		score = (cfs[0][0] * cfs[1][1] - cfs[0][1] * cfs[1][0]) /
				sqrt((cfs[0][0] + cfs[0][1]) * (cfs[0][0] + cfs[0][1]) * (N - cfs[0][0] + cfs[0][1]) * (N - cfs[0][0] + cfs[0][1]));

		for(int i = 0; i < n_lbl; ++i) delete[] cfs;
		delete[] cfs;
		return score;
	}

}

