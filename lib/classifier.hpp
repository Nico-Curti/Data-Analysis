#pragma once
#include "fmath.hpp"
#include "patterns.hpp"
#include <chrono>
#include <type_traits>
#include <omp.h>
#include <numeric>
#include <iterator>

template<typename Cls, typename enable = void> struct hyperparams;

template<typename T> class Perceptron
{
	int num_inputs, size;
	T bias, *weights;
public:
	using type = T;
	Perceptron();
	Perceptron(const Perceptron<T> &);
	Perceptron<T>& operator=(const Perceptron<T> &);
	~Perceptron();

	template<bool par = false, typename std::enable_if<par>::type* = nullptr> void train(const Patterns<T> &, T eta = (T).1, int max_iter = 100, unsigned int seed = 0);
	template<bool par = false, typename std::enable_if<!par>::type* = nullptr> void train(const Patterns<T> &, T eta = (T).1, int max_iter = 100, unsigned int seed = 0);
	template<bool par = false, typename std::enable_if<par>::type* = nullptr> void cv_train(const Patterns<T> &, const std::vector<int> &, T eta = (T).1, int max_iter = 100, unsigned int seed = 0);
	template<bool par = false, typename std::enable_if<!par>::type* = nullptr> void cv_train(const Patterns<T> &, const std::vector<int> &, T eta = (T).1, int max_iter = 100, unsigned int seed = 0);
	template<bool par = false, typename std::enable_if<par>::type* = nullptr> inline void train(const Patterns<T> &, const hyperparams<Perceptron> &);
	template<bool par = false, typename std::enable_if<!par>::type* = nullptr> inline void train(const Patterns<T> &, const hyperparams<Perceptron> &);
	template<bool par = false, typename std::enable_if<par>::type* = nullptr> inline void cv_train(const Patterns<T> &, const std::vector<int> &, const hyperparams<Perceptron> &);
	template<bool par = false, typename std::enable_if<!par>::type* = nullptr> inline void cv_train(const Patterns<T> &, const std::vector<int> &, const hyperparams<Perceptron> &);

	template<bool par = false, typename std::enable_if<par>::type* = nullptr> int* test(const Patterns<T> &);
	template<bool par = false, typename std::enable_if<!par>::type* = nullptr> int* test(const Patterns<T> &);
	template<bool par = false, typename std::enable_if<par>::type* = nullptr> int* cv_test(const Patterns<T> &, const std::vector<int> &);
	template<bool par = false, typename std::enable_if<!par>::type* = nullptr> int* cv_test(const Patterns<T> &, const std::vector<int> &);
	int predict(T *);
};

#include "perceptron.hpp"

struct
{
	template<typename T> inline T  operator()(T *, const int &, const int &); // fn
	template<typename T> inline T* operator()(T *, T *, const int &, const int &); // cost_delta
} QuadraticCost;
typedef decltype(QuadraticCost) Qcost;

struct
{
	template<typename T> inline T  operator()(T *, const int &, const int &); // fn
	template<typename T> inline T* operator()(T *, T *, const int &, const int &); // cost_delta
} CrossEntropyCost;
typedef decltype(CrossEntropyCost) Ecost;

template<typename T> class NeuralNetwork
{
	int num_layers, *sizes;
	T **weights, **biases;

	inline int accuracy(const Patterns<T> &);
	template<typename Up> inline T total_cost(const Patterns<T> &, const T &, Up);
	T* feedforward(T *);
	template<typename Up> void backprop(const Patterns<T> &, T **, T **, T **, T **, const int &, Up);
	template<typename Up> void update_mini_batch(const Patterns<T> &, T **, T **, T **, T **,int *, const int &, const T &, const T &,Up);
	template<typename Up> void SGD(const Patterns<T> &, T **,T **,T **,T **,const int &, const int &, Up,const T &, const T &);
public:
	using type = T;
	NeuralNetwork();
	~NeuralNetwork();

	template<typename Up> void train(const Patterns<T> &, int *, const int &, Up, const int &, const int &, const T &, const T &, unsigned int seed = 0);
	inline void train(const Patterns<T> &, const hyperparams<NeuralNetwork> &);
	template<typename Up> inline void cv_train(const Patterns<T> &, const std::vector<int> &, int *, const int &, Up, const int &, const int &, const T &, const T &, unsigned int seed = 0);
	inline void cv_train(const Patterns<T> &, const std::vector<int> &, const hyperparams<NeuralNetwork> &);


	int *test(const Patterns<T> &);
	int* cv_test(const Patterns<T> &, const std::vector<int> &);
	inline int predict(T *);

	void save(const std::string &, bool bin = false);
	void load(const std::string &, bool bin = false);
};

#include "neural_network.hpp"

template<typename T> class diagQDA
{
	int n_class, *n_label, n_sample;
	T **mean, **var, *prior;
public:
	using type = T;
	diagQDA();
	~diagQDA();

	void train(const Patterns<T> &, T *prior = nullptr);
	inline void train(const Patterns<T> &, hyperparams<diagQDA> &);
	void cv_train(const Patterns<T> &, const std::vector<int> &, T *prior = nullptr);
	inline void cv_train(const Patterns<T> &, const std::vector<int> &, hyperparams<diagQDA> &);
	
	int* test(const Patterns<T> &);
	int* cv_test(const Patterns<T> &, const std::vector<int> &);
	int predict(T *);
};

#include "diagQDA.hpp"


template<typename T> class QDA
{
	int n_class, *n_label, N;
	T **mean, ***cov_inv, *prior, *det;
public:
	using type = T;
	QDA();
	~QDA();

	void train(const Patterns<T> &, T *prior = nullptr);
	inline void train(const Patterns<T> &, hyperparams<QDA> &);
	void cv_train(const Patterns<T> &, const std::vector<int> &, T *prior = nullptr);
	inline void cv_train(const Patterns<T> &, const std::vector<int> &, hyperparams<QDA> &);

	int* test(const Patterns<T> &);
	int* cv_test(const Patterns<T> &, const std::vector<int> &);
	int predict(T *);
};

#include "QDA.hpp"

template<typename T> class ReplicatedSGD
{
	T *weights;
	int num_layers,
		input_size;
public:
	using type = T;
	ReplicatedSGD();
	~ReplicatedSGD();

	void inline train(const Patterns<T> &, const int &num_layers = 1, const int &y = 1, const int &batch = 5, const int &max_epochs = 1000, const T &eta = (T)2., const T &lambda = (T).1, const T &gamma = std::numeric_limits<T>::infinity(), const T &eta_factor = (T)1., const T &lambda_factor = (T)1., const T &gamma_step = (T).01, std::string formula = "simple", bool waitcenter = false, bool equal = true, bool center = false, int seed = 202, std::string outfile = "", bool quiet = false);
	void inline train(const Patterns<T> &, const hyperparams<ReplicatedSGD> &);
	void inline cv_train(const Patterns<T> &, const std::vector<int> &, const int &num_layers = 1, const int &y = 1, const int &batch = 5, const int &max_epochs = 1000, const T &eta = (T)2., const T &lambda = (T).1, const T &gamma = std::numeric_limits<T>::infinity(), const T &eta_factor = (T)1., const T &lambda_factor = (T)1., const T &gamma_step = (T).01, std::string formula = "simple", bool waitcenter = false, bool equal = true, bool center = false, int seed = 202, std::string outfile = "", bool quiet = false);
	void inline cv_train(const Patterns<T> &, const std::vector<int> &, const hyperparams<ReplicatedSGD> &);

	inline int* test(const Patterns<T> &);
	inline int* cv_test(const Patterns<T> &, const std::vector<int> &);
	inline int predict(T *);
};

#include "ReplicatedSGD.hpp"

// ReplicatedSGD Classifier
template<typename T> ReplicatedSGD<T>::ReplicatedSGD()
{
	this->num_layers = 0;
	this->input_size = 0;
	this->weights = nullptr;
}
template<typename T> ReplicatedSGD<T>::~ReplicatedSGD()
{
	if(this->weights != nullptr)
		delete[] this->weights;
}

template<typename T> inline void ReplicatedSGD<T>::train(const Patterns<T> &data, const int &num_layers, const int &y, const int &batch, const int &max_epochs, const T &eta, const T &lambda, const T &gamma, const T &eta_factor, const T &lambda_factor, const T &gamma_step, std::string formula, bool waitcenter, bool equal, bool center, int seed, std::string outfile, bool quiet)
{
	this->num_layers = num_layers;
	this->input_size = data.Ncol;
	this->weights = RSGD::train<T>(data, num_layers, y, batch, max_epochs, eta, lambda, gamma, eta_factor, lambda_factor, gamma_step, formula, waitcenter, equal, center, seed, outfile, quiet);
	return;
}

template<typename T> inline void ReplicatedSGD<T>::cv_train(const Patterns<T> &data, const std::vector<int> &idx, const int &num_layers, const int &y, const int &batch, const int &max_epochs, const T &eta, const T &lambda, const T &gamma, const T &eta_factor, const T &lambda_factor, const T &gamma_step, std::string formula, bool waitcenter, bool equal, bool center, int seed, std::string outfile, bool quiet)
{
	this->num_layers = num_layers;
	this->input_size = data.Ncol;
	Patterns<T> train;
	train.Nrow = (int)idx.size();
	train.Ncol = data.Ncol;
	train.Nout = (int)idx.size();
	train.output = new int[train.Nout];
	train.input = new T*[train.Nrow];
	std::generate(train.input, train.input + train.Nrow, [&data](){return new T[data.Ncol];});
	for(int i = 0; i < (int)idx.size(); ++i)
	{
		std::memcpy(train.input[i], data.input[idx[i]], sizeof(T)*train.Ncol);
		train.output[i] = data.output[idx[i]];
	}
	this->weights = RSGD::train<T>(data, num_layers, y, batch, max_epochs, eta, lambda, gamma, eta_factor, lambda_factor, gamma_step, formula, waitcenter, equal, center, seed, outfile, quiet);
	return;
}

template<typename T>inline void ReplicatedSGD<T>::train(const Patterns<T> &data, const hyperparams<ReplicatedSGD> &params)
{
	this->num_layers = num_layers;
	this->input_size = data.Ncol;
	this->weights = RSGD::train<T>(data, params.num_layers, params.y, params.batch, params.max_epochs, params.eta, params.lambda, params.gamma, params.eta_factor, params.lambda_factor, params.gamma_step, params.formula, params.waitcenter, params.equal, params.center, params.seed);
	return;
}

template<typename T> inline void ReplicatedSGD<T>::cv_train(const Patterns<T> &data, const std::vector<int> &idx, const hyperparams<ReplicatedSGD> &params)
{
	this->cv_train(data, idx, params.num_layers, params.y, params.batch, params.max_epochs, params.eta, params.lambda, params.gamma, params.eta_factor, params.lambda_factor, params.gamma_step, params.formula, params.waitcenter, params.equal, params.center, params.seed);
}

template<typename T> inline int* ReplicatedSGD<T>::test(const Patterns<T> &test)
{
	return RSGD::test<T>(test, this->weights, this->num_layers, this->input_size);
}
template<typename T> inline int* ReplicatedSGD<T>::cv_test(const Patterns<T> &test, const std::vector<int> &idx)
{
	int *results = new int[idx.size()]; 
	for(int i = 0; i < (int)idx.size(); ++i)
		results[i] = RSGD::compute_output(test.input[idx[i]], this->weights, this->num_layers, this->input_size);
	return results;
}
template<typename T> inline int ReplicatedSGD<T>::predict(T *input)
{
	return RSGD::compute_output(input, this->weights, this->num_layers, this->input_size);
}



//ReplicatedFBP
template<typename T> class ReplicatedFBP
{
	int input_size,
		num_layers,
		**weights;

public:
	using type = T;
	ReplicatedFBP();
	~ReplicatedFBP();

	template<class Mag> inline void train(const Patterns<T> &, const int &, const int &, const T &, const T &, const std::string &, const std::string &, const std::string &, int protocol_size = 101, int seed = 135, bool quiet = true, std::string output = "");
	template<class Mag> inline void train(const Patterns<T> &, const hyperparams<ReplicatedFBP> &);
	template<class Mag> inline void cv_train(const Patterns<T> &, const std::vector<int> &, const int &, const int &, const T &, const T &, const std::string &, const std::string &, const std::string &, int protocol_size = 101, int seed = 135, bool quiet = true, std::string output = "");
	template<class Mag> inline void cv_train(const Patterns<T> &, const std::vector<int> &, const hyperparams<ReplicatedFBP> &);

	inline int *test(const Patterns<T> &);
	inline int *cv_test(const Patterns<T> &, const std::vector<int> &);
	inline int predict(T *);
};

#include "ReplicatedFBP.hpp"

// ReplicatedFBP Classifier
template<typename T> ReplicatedFBP<T>::ReplicatedFBP()
{
	this->num_layers = 0;
	this->input_size = 0;
	this->weights = nullptr;
}
template<typename T> ReplicatedFBP<T>::~ReplicatedFBP()
{
	if(this->weights != nullptr)
		delete[] this->weights;
}
template<typename T>template<class Mag> inline void ReplicatedFBP<T>::train(const Patterns<T> &data, const int &num_layers, const int &max_iters, const T &randfact, const T &damping, const std::string &accuracy1, const std::string &accuracy2, const std::string &fprotocol, int protocol_size, int seed, bool quiet, std::string output)
{
	this->num_layers = num_layers;
	this->input_size = data.Ncol;
	this->weights = RFBP::train<T, Mag>(data, num_layers, seed, max_iters, randfact, damping, accuracy1, accuracy2, fprotocol, protocol_size, quiet, output);
	return;
}
template<typename T>template<class Mag> inline void ReplicatedFBP<T>::cv_train(const Patterns<T> &data, const std::vector<int> &idx, const int &num_layers, const int &max_iters, const T &randfact, const T &damping, const std::string &accuracy1, const std::string &accuracy2, const std::string &fprotocol, int protocol_size, int seed, bool quiet, std::string output)
{
	this->num_layers = num_layers;
	this->input_size = data.Ncol;
	Patterns<T> train;
	train.Nrow = (int)idx.size();
	train.Ncol = data.Ncol;
	train.Nout = (int)idx.size();
	train.output = new int[train.Nout];
	train.input = new T*[train.Nrow];
	std::generate(train.input, train.input + train.Nrow, [&data](){return new T[data.Ncol];});
	for(int i = 0; i < (int)idx.size(); ++i)
	{
		std::memcpy(train.input[i], data.input[idx[i]], sizeof(T)*train.Ncol);
		train.output[i] = data.output[idx[i]];
	}
	this->weights = RFBP::train<T, Mag>(train, num_layers, seed, max_iters, randfact, damping, accuracy1, accuracy2, fprotocol, protocol_size, quiet, output);
	return;
}
template<typename T>template<class Mag> inline void ReplicatedFBP<T>::train(const Patterns<T> &data, const hyperparams<ReplicatedFBP> &params)
{
	this->num_layers = num_layers;
	this->input_size = data.Ncol;
	this->weights = RFBP::train<T, Mag>(data, params.num_layers, params.seed, params.max_iters, params.randfact, params.damping, params.accuracy1, params.accuracy2, params.fprotocol, params.protocol_size);
	return;
}
template<typename T>template<class Mag> inline void ReplicatedFBP<T>::cv_train(const Patterns<T> &data, const std::vector<int> &idx, const hyperparams<ReplicatedFBP> &params)
{
	this->cv_train(data, idx, params.num_layers, params.seed, params.max_iters, params.randfact, params.damping, params.accuracy1, params.accuracy2, params.fprotocol, params.protocol_size);
}
template<typename T> inline int* ReplicatedFBP<T>::test(const Patterns<T> &test)
{
	return RFBP::test<T>(test, this->weights, this->num_layers, this->input_size);
}
template<typename T> inline int* ReplicatedFBP<T>::cv_test(const Patterns<T> &test, const std::vector<int> &idx)
{
	int *results = new int[idx.size()]; 
	for(int i = 0; i < (int)idx.size(); ++i)
		results[i] = RFBP::compute_output(test.input[idx[i]], this->weights, this->num_layers, this->input_size);
	return results;
}
template<typename T> inline int ReplicatedFBP<T>::predict(T *input)
{
	return RFBP::compute_output(input, this->weights, this->num_layers, this->input_size);
}


#include <hyperparams.hpp>