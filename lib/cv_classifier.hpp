#include "patterns.hpp"
#include "cross_validation.hpp"
#include "classifier.hpp"

// TO FIX parallel
template<	bool par,
			class Cls, 
			typename T, 
			typename CV, 
			typename Score,
			typename std::enable_if<par>::type* = nullptr
			> 
			auto cross_val_score(	const Patterns<T> &train,
									CV cross_validation,
									Score score_func,
									hyperparams<Cls> params
									)
{
	CrossValidation cv;
	cross_validation(cv); // for use std::bind(&CrossValidation::KFold, std::placeholders::_1, ...(parameters)...)
	std::vector<int> idx_test, // vector of test indices
					 idx_train; // vector of train indices
	int *predict_lbl = new int[train.Nout]; // vector of predicted labels

#pragma omp parallel for
	for(int fold = 0; fold < cv.n_fold; ++fold) // loop over folds
	{
		std::unique_ptr<Cls> classifier(new Cls); // unique ptr to allow default destruction at the end of loop
		cv.getFold(fold, idx_train, idx_test);
		classifier->cv_train(train, idx_train, params);
		for(const auto &test : idx_test) predict_lbl[test] = classifier->predict(train.input[test]);
			std::cout << fold << std::endl;
	}
	for(int i = 0; i < train.Nout; ++i) std::cout << predict_lbl[i] << " ";
		std::cout << std::endl;
	auto score = score_func(train.output, predict_lbl, train.Nout);
	delete[] predict_lbl;
	return score;
}

template<	bool par,
			class Cls, 
			typename T, 
			typename CV, 
			typename Score,
			typename std::enable_if<!par>::type* = nullptr
			> 
			auto cross_val_score(	const Patterns<T> &train,
									CV cross_validation,
									Score score_func,
									hyperparams<Cls> params
									)
{
	CrossValidation cv;
	cross_validation(cv); // for use std::bind(&CrossValidation::KFold, std::placeholders::_1, ...(parameters)...)
	std::vector<int> idx_test, // vector of test indices
					 idx_train; // vector of train indices
	int *predict_lbl = new int[train.Nout]; // vector of predicted labels

	for(int fold = 0; fold < cv.n_fold; ++fold) // loop over folds
	{
		std::unique_ptr<Cls> classifier(new Cls); // unique ptr to allow default destruction at the end of loop
		cv.getFold(fold, idx_train, idx_test);
		classifier->cv_train(train, idx_train, params);
		for(const auto &test : idx_test) predict_lbl[test] = classifier->predict(train.input[test]);
	}
	auto score = score_func(train.output, predict_lbl, train.Nout);
	delete[] predict_lbl;
	return score;
}
