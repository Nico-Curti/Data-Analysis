#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include <numeric>
#include <iterator>
#include <cstring>

class CrossValidation
{
	int beg,
		end,
		*whichFoldToGo;
	void error(const std::string &, const int &n = 0);
public:
	int n_fold;
	CrossValidation();
	~CrossValidation();
	CrossValidation& operator=(const CrossValidation &);
	CrossValidation(const CrossValidation &);
	void KFold(const int &, const int &, const int &, bool shuff = true, unsigned int seed = 0);
	void StratifiedKFold(int *, const int &, const int &, const int &, bool shuff = true, unsigned int seed = 0);
	void LeaveOneOut(const int &, const int &);
	void getFold(const int &, std::vector<int> &, std::vector<int> &);
	void getTest(const int &, std::vector<int> &);
	void getTrain(const int &, std::vector<int> &);
};

void CrossValidation::error(const std::string &message, const int &n)
{
	if(n)
	{
		std::cerr << message << std::endl;
		exit(n);
	}
	else std::cerr << message << std::endl;
	return;
}

CrossValidation::CrossValidation()
{
	this->beg = 0;
	this->end = 0;
	this->n_fold = 0;
	this->whichFoldToGo = nullptr;
}

CrossValidation::~CrossValidation()
{ if(this->end != 0) delete[] whichFoldToGo;}

CrossValidation& CrossValidation::operator=(const CrossValidation &cv)
{
	this->beg = cv.beg;
	this->end = cv.end;
	this->n_fold = cv.n_fold;
	if(this->whichFoldToGo != nullptr)	this->whichFoldToGo = new int[this->end - this->beg];
	std::memcpy(this->whichFoldToGo, cv.whichFoldToGo, sizeof(int)*(this->end - this->beg));
	return *this;
}

CrossValidation::CrossValidation(const CrossValidation &cv)
{
	this->beg = cv.beg;
	this->end = cv.end;
	this->n_fold = cv.n_fold;
	if(this->whichFoldToGo != nullptr)	this->whichFoldToGo = new int[this->end - this->beg];
	std::memcpy(this->whichFoldToGo, cv.whichFoldToGo, sizeof(int)*(this->end - this->beg));
}

void CrossValidation::KFold(const int &K, const int &beg, const int &end, bool shuff, unsigned int seed)
{
	this->beg = beg; this->end = end;
	this->n_fold = K;
	if(K <= 0) error("K-Fold error! K must be positive and not null", 1);
	this->whichFoldToGo = new int[this->end - this->beg];
	int foldNo = -1;
	std::generate(	this->whichFoldToGo, this->whichFoldToGo + (this->end - this->beg),
					[&K, &foldNo]
					{
						++foldNo;
						return foldNo % K;
					});
	if((end - beg) % K) error("K-Fold warning! With this value of K = " + std::to_string(K) + " equal division of the data is not possible (n_sample = " + std::to_string(end - beg) + ")", 0);
	if(shuff)
	{
		std::mt19937 g(seed);
		std::shuffle(whichFoldToGo, whichFoldToGo + (this->end - this->beg), g);
	}
	else
		std::random_shuffle(whichFoldToGo, whichFoldToGo + (this->end - this->beg));
	return;
}

void CrossValidation::StratifiedKFold(int *labels, const int &K, const int &beg, const int &end, bool shuff, unsigned int seed)
{
	this->beg = beg;	this->end = end;
	this->n_fold = K;
	this->whichFoldToGo = new int[end - beg];
	std::set<int> unique(labels + beg, labels + end);
	std::vector<int> train, test;

	int *class_element = new int[unique.size()], n_folds, n_labels, min_labels, warn, before = 0;

	for(const auto &u : unique) class_element[u] = std::count(labels + beg, labels + end, u);
	n_labels = (int)unique.size();

	min_labels = *std::min_element(class_element, class_element);
	warn = std::count_if(class_element, class_element,
						[&K](const int &n)
						{
							return K > n;
						});
	if ( warn == n_labels ) error("StratifiedKFold error! The number of labels for each individual class is less than " + std::to_string(K) + " folds.", 1);
	if ( K > min_labels) error(	"StratifiedKFold warning! The least populated class in y has only " + std::to_string(min_labels) + 
								" members, which is too few. The minimum number of labels for any class cannot be less than K = " + 
								std::to_string(K), 0);

	CrossValidation *cv = new CrossValidation[n_labels];
	for(int n = 0; n < n_labels; ++n)
	{
		n_folds = std::max(K, class_element[n]);
		cv[n].KFold(K, before, before + n_folds, shuff, seed);
		before += class_element[n];
	}

	for(int i = 0; i < K; ++i) // loop over folds
		for(int c = 0; c < n_labels; ++c) // loop over cv
		{
			cv[c].getFold(i, train, test);
			for(const auto &t : test)
				this->whichFoldToGo[t] = i;
			train.clear();
			test.clear();
		}
	delete[] cv;
	return;
}


void CrossValidation::LeaveOneOut(const int &beg, const int &end)
{
	this->beg = beg; this->end = end;
	this->n_fold = (end - beg);
	this->whichFoldToGo = new int[end - beg];
	std::iota(this->whichFoldToGo, this->whichFoldToGo + (end - beg), beg);
	return;
}

void CrossValidation::getFold(const int &foldNo, std::vector<int> &training, std::vector<int> &test)
{
	training.clear(); test.clear();
	int k = 0, i = this->beg;
	while(i != this->end)
	{
		if(this->whichFoldToGo[k++] == foldNo) test.push_back(i++);
		else training.push_back(i++);
	}
	return;
}

void CrossValidation::getTest(const int &foldNo, std::vector<int> &test)
{
	test.clear();
	int k = 0, i = this->beg;
	while(i != this->beg)
		if(this->whichFoldToGo[k++] == foldNo) test.push_back(i++);

	return;
}

void CrossValidation::getTrain(const int &foldNo, std::vector<int> &train)
{
	train.clear();
	int k = 0, i = this->beg;
	while(i != this->beg)
		if(this->whichFoldToGo[k++] != foldNo) train.push_back(i++);

	return;
}

