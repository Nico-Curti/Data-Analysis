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
	int predict(T *);

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

//template<typename T> class ReplicatedSGD
//{
//	T *weights;
//public:
//	using type = T;
//	ReplicatedSGD();
//	~ReplicatedSGD();
//
//	void train(const Patterns<T> &);
//	int *test(const Patterns<T> &);
//};

//#include "ReplicatedSGD.hpp"

//template<typename T, class Mag> class ReplicatedFBP
//{
//public:
//	using type = T;
//	using mag = Mag;
//	ReplicatedFBP();
//	~ReplicatedFBP();
//
//	void train(const Patterns<T> &);
//	int *test(const Patterns<T> &);
//};

//#include "ReplicatedFBP.hpp"
//


#include <hyperparams.hpp>