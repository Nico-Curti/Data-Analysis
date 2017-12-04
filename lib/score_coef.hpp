#pragma once

#include <set>
#include <vector>

template<typename T> class ScoreCoefficient
{
	int Nlabel;
	std::set<int> unique;
public:
	ScoreCoefficient(const std::set<int> &);
	~ScoreCoefficient() = default;

	T* ConfusionMatrix(const std::vector<int> &, const std::vector<int> &);
	T* ConfusionMatrix(int *, int *, const int &);
	T  MCC(const std::vector<int> &, const std::vector<int> &);
	T  MCC(int *, const int &, int *, const int &);
	T  Fbeta(const std::vector<int> &, const std::vector<int> &, const T &);
	T  Fbeta(int *, const int &, int *, const int &, const T &);
	T  PhiCoef(const std::vector<int> &, const std::vector<int> &);
	T  PhiCoef(int *, const int &, int *, const int &);
};



//==========================================================================================================================//
//	 _____  _____  _____ ______  _____      _____  _____  _____ ______ ______  _____  _____  _____  _____  _   _  _____ 	//
//	/  ___|/  __ \|  _  || ___ \|  ___|    /  __ \|  _  ||  ___||  ___||  ___||_   _|/  __ \|_   _||  ___|| \ | ||_   _|	//
//	\ `--. | /  \/| | | || |_/ /| |__      | /  \/| | | || |__  | |_   | |_     | |  | /  \/  | |  | |__  |  \| |  | |  	//
//	 `--. \| |    | | | ||    / |  __|     | |    | | | ||  __| |  _|  |  _|    | |  | |      | |  |  __| | . ` |  | |  	//
//	/\__/ /| \__/\\ \_/ /| |\ \ | |___     | \__/\\ \_/ /| |___ | |    | |     _| |_ | \__/\ _| |_ | |___ | |\  |  | |  	//
//	\____/  \____/ \___/ \_| \_|\____/      \____/ \___/ \____/ \_|    \_|     \___/  \____/ \___/ \____/ \_| \_/  \_/  	//
//	                                                                                                                    	//
//==========================================================================================================================//

template<typename T> ScoreCoefficient<T>::ScoreCoefficient(const std::set<int> &lblu)
{
	this->unique = lblu;
	this->Nlabel = (int)unique.size();
	if (Nlabel > 2)	{ std::cerr << "Error: Score Coefficients can be computed only for binary classes" << std::endl; exit(1); }
};

template<typename T> T* ScoreCoefficient<T>::ConfusionMatrix(const std::vector<int> &lbl_true, const std::vector<int> &lbl_pred)
{
	T *cfs_matrix = new T[this->Nlabel*this->Nlabel];
	std::memset(cfs_matrix, 0, this->Nlabel*this->Nlabel*sizeof(T));
	std::set<int>::iterator it1, it2;
	for (int i = 0; i < (int)lbl_true.size(); ++i)
	{
		it1 = this->unique.find(lbl_true[i]);
		it2 = this->unique.find(lbl_pred[i]);
		++cfs_matrix[std::distance(this->unique.begin(), it1) * this->Nlabel + std::distance(this->unique.begin(), it2)];
	}
	return cfs_matrix;
}

template<typename T> T* ScoreCoefficient<T>::ConfusionMatrix(int *lbl_true, int *lbl_pred, const int &size)
{
	T *cfs_matrix = new T[this->Nlabel*this->Nlabel];
	std::memset(cfs_matrix, 0, this->Nlabel*this->Nlabel*sizeof(T));
	std::set<int>::iterator it1, it2;
	for (int i = 0; i < size; ++i)
	{
		it1 = this->unique.find(lbl_true[i]);
		it2 = this->unique.find(lbl_pred[i]);
		++cfs_matrix[std::distance(this->unique.begin(), it1) * this->Nlabel + std::distance(this->unique.begin(), it2)];
	}
	return cfs_matrix;
}

template<typename T> T ScoreCoefficient<T>::MCC(const std::vector<int> &lbl_true, const std::vector<int> &lbl_pred)
{
	if (lbl_true.size() != lbl_pred.size()) { std::cerr << "The vector of true labels and the vector of labels predicted must have the same lenght" << std::endl; exit(1);}
	T *cfs_matrix = nullptr, mcc;
	cfs_matrix = ConfusionMatrix(lbl_true, lbl_pred, (int)lbl_true.size());

	if (Nlabel == 2)
	{
		if ((cfs_matrix[0] == 0 && cfs_matrix[3] != 0) || (cfs_matrix[0] != 0 && cfs_matrix[3] == 0) || (cfs_matrix[0] == 0 && cfs_matrix[3] == 0))
			return 0;
		mcc = (cfs_matrix[0] * cfs_matrix[3] - cfs_matrix[1] * cfs_matrix[2]) / sqrt((cfs_matrix[0] + cfs_matrix[2]) * (cfs_matrix[0] + cfs_matrix[1]) * (cfs_matrix[3] + cfs_matrix[1]) * (cfs_matrix[3] + cfs_matrix[2]));
	}
	else
	{
		T A1, A2, B1, B2, D1 = (T)0., D2 = (T)0., num = (T)0.;
		for (int k = 0; k < Nlabel; ++k)
		{
			A1 = (T)0.; A2 = (T)0.; B1 = (T)0.; B2 = (T)0.;
			for (int l = 0; l < Nlabel; ++l)
			{
				A1 += cfs_matrix[k*Nlabel + l];
				A2 += cfs_matrix[l*Nlabel + k];
				for (int m = 0; m < Nlabel; ++m)
				{
					num += cfs_matrix[k*Nlabel + k] * cfs_matrix[l*Nlabel + m] - cfs_matrix[k*Nlabel + l] * cfs_matrix[m*Nlabel + k];
					B1 += (m != k) ? cfs_matrix[m*Nlabel + l] : 0.;
					B2 += (m != k) ? cfs_matrix[l*Nlabel + m] : 0.;
				}
			}
			D1 += A1*B1;
			D2 += A2*B2;
		}
		mcc = num / (sqrt(D1 * D2));
	}
	return mcc;
}

template<typename T> T ScoreCoefficient<T>::MCC(int *lbl_true, const int &nlbl_true, int *lbl_pred, const int &nlbl_pred)
{
	if (nlbl_true != nlbl_pred) { std::cerr << "The vector of true labels and the vector of labels predicted must have the same lenght" << std::endl; exit(1);}
	T *cfs_matrix = nullptr, mcc;
	cfs_matrix = ConfusionMatrix(lbl_true, lbl_pred, nlbl_true);

	if (Nlabel == 2)
	{
		if ((cfs_matrix[0] == 0 && cfs_matrix[3] != 0) || (cfs_matrix[0] != 0 && cfs_matrix[3] == 0) || (cfs_matrix[0] == 0 && cfs_matrix[3] == 0))
			return 0;
		mcc = (cfs_matrix[0] * cfs_matrix[3] - cfs_matrix[1] * cfs_matrix[2]) / sqrt((cfs_matrix[0] + cfs_matrix[2]) * (cfs_matrix[0] + cfs_matrix[1]) * (cfs_matrix[3] + cfs_matrix[1]) * (cfs_matrix[3] + cfs_matrix[2]));
	}
	else
	{
		T A1, A2, B1, B2, D1 = (T)0., D2 = (T)0., num = (T)0.;
		for (int k = 0; k < Nlabel; ++k)
		{
			A1 = (T)0.; A2 = (T)0.; B1 = (T)0.; B2 = (T)0.;
			for (int l = 0; l < Nlabel; ++l)
			{
				A1 += cfs_matrix[k*Nlabel + l];
				A2 += cfs_matrix[l*Nlabel + k];
				for (int m = 0; m < Nlabel; ++m)
				{
					num += cfs_matrix[k*Nlabel + k] * cfs_matrix[l*Nlabel + m] - cfs_matrix[k*Nlabel + l] * cfs_matrix[m*Nlabel + k];
					B1 += (m != k) ? cfs_matrix[m*Nlabel + l] : 0.;
					B2 += (m != k) ? cfs_matrix[l*Nlabel + m] : 0.;
				}
			}
			D1 += A1*B1;
			D2 += A2*B2;
		}
		mcc = num / (sqrt(D1 * D2));
	}
	return mcc;
}

template<typename T> T ScoreCoefficient<T>::Fbeta(const std::vector<int> &lbl_true, const std::vector<int> &lbl_pred, const T &beta)
{
	if (lbl_true.size() != lbl_pred.size()) { std::cerr << "The vector of true labels and the vector of labels predicted must have the same lenght" << std::endl; exit(1);}
	T beta_sq = beta*beta, *cfs_matrix = ConfusionMatrix(lbl_true, lbl_pred, (int)lbl_true.size());
	if ((cfs_matrix[0] == 0 && cfs_matrix[3] != 0) || (cfs_matrix[0] != 0 && cfs_matrix[3] == 0) || (cfs_matrix[0] == 0 && cfs_matrix[3] == 0))
		return 0;
	return ((1 + beta_sq) * cfs_matrix[0]) / ((1 + beta_sq) * cfs_matrix[0] + beta_sq*cfs_matrix[1] + cfs_matrix[2]);
}

template<typename T> T ScoreCoefficient<T>::Fbeta(int *lbl_true, const int &nlbl_true, int * lbl_pred, const int &nlbl_pred, const T &beta)
{
	if (nlbl_true != nlbl_pred) { std::cerr << "The vector of true labels and the vector of labels predicted must have the same lenght" << std::endl; exit(1);}
	T beta_sq = beta*beta, *cfs_matrix = ConfusionMatrix(lbl_true, lbl_pred, nlbl_true);
	if ((cfs_matrix[0] == 0 && cfs_matrix[3] != 0) || (cfs_matrix[0] != 0 && cfs_matrix[3] == 0) || (cfs_matrix[0] == 0 && cfs_matrix[3] == 0))
		return 0;
	return ((1 + beta_sq) * cfs_matrix[0]) / ((1 + beta_sq) * cfs_matrix[0] + beta_sq*cfs_matrix[1] + cfs_matrix[2]);
}


template<typename T> T ScoreCoefficient<T>::PhiCoef(const std::vector<int> &lbl_true, const std::vector<int> &lbl_pred)
{
	if (lbl_true.size() != lbl_pred.size()) { std::cerr << "The vector of true labels and the vector of labels predicted must have the same lenght" << std::endl; exit(1);}
	T *cfs_matrix = ConfusionMatrix(lbl_true, lbl_pred, (int)lbl_true.size()), N;
	if ((cfs_matrix[0] == 0 && cfs_matrix[3] != 0) || (cfs_matrix[0] != 0 && cfs_matrix[3] == 0) || (cfs_matrix[0] == 0 && cfs_matrix[3] == 0))
		return 0;
	N = cfs_matrix[0] + cfs_matrix[1] + cfs_matrix[2] + cfs_matrix[3];
	return (cfs_matrix[0] * cfs_matrix[3] - cfs_matrix[1] * cfs_matrix[2]) / sqrt((cfs_matrix[0] + cfs_matrix[1])*(cfs_matrix[0] + cfs_matrix[1])*(N - cfs_matrix[0] + cfs_matrix[1])*(N - cfs_matrix[0] + cfs_matrix[1]));
}

template<typename T> T ScoreCoefficient<T>::PhiCoef(int *lbl_true, const int &nlbl_true, int *lbl_pred, const int &nlbl_pred)
{
	if (nlbl_true != nlbl_pred) { std::cerr << "The vector of true labels and the vector of labels predicted must have the same lenght" << std::endl; exit(1);}
	T *cfs_matrix = ConfusionMatrix(lbl_true, lbl_pred, nlbl_true), N;
	if ((cfs_matrix[0] == 0 && cfs_matrix[3] != 0) || (cfs_matrix[0] != 0 && cfs_matrix[3] == 0) || (cfs_matrix[0] == 0 && cfs_matrix[3] == 0))
		return 0;
	N = cfs_matrix[0] + cfs_matrix[1] + cfs_matrix[2] + cfs_matrix[3];
	return (cfs_matrix[0] * cfs_matrix[3] - cfs_matrix[1] * cfs_matrix[2]) / sqrt((cfs_matrix[0] + cfs_matrix[1])*(cfs_matrix[0] + cfs_matrix[1])*(N - cfs_matrix[0] + cfs_matrix[1])*(N - cfs_matrix[0] + cfs_matrix[1]));
}

