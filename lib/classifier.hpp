#pragma once
#include "fmath.hpp"
#include "patterns.hpp"
#include <chrono>
#include <type_traits>
#include <omp.h>
#include <numeric>
#include <iterator>

template<bool c, typename T> using parallel = typename std::enable_if<c>::type*;
template<bool c, typename T> using serial = typename std::enable_if<!c>::type*;


template<typename T> class Perceptron
{
	int num_inputs, size;
	T bias, *weights;
public:
	Perceptron();
	Perceptron(const int &, const int &);
	Perceptron(const Perceptron<T> &);
	Perceptron<T>& operator=(const Perceptron<T> &);
	~Perceptron();

	template<bool par, parallel<par, T> = nullptr> void train(const Patterns<T> &, T eta = (T).1);
	template<bool par, serial<par, T> = nullptr> void train(const Patterns<T> &, T eta = (T).1);
	template<bool par, parallel<par, T> = nullptr> int* test(const Patterns<T> &);
	template<bool par, serial<par, T> = nullptr> int* test(const Patterns<T> &);
};

#include "perceptron.hpp"

namespace QuadraticCost
{
	template<typename T> inline T fn(T *, const int &, const int &);
	template<typename T> inline T * cost_delta(T *, T *, const int &, const int &);
}
namespace CrossEntropyCost
{
	template<typename T> inline T fn(T *, const int &, const int &);
	template<typename T> inline T* cost_delta(T* , T* , const int &, const int &);
}


template<typename T> class NeuralNetwork
{
	int num_layers;
	int *sizes;
	T **weights, **nabla_w, **delta_nabla_w, **biases, **nabla_b, **delta_nabla_b;
																					
	T(*fn)(T *, const int &, const int &);
	T*(*cost_delta)(T *, T *, const int &, const int &);
	std::string cost;
	inline T* vectorized_result(const int &, const int &);

	void default_weight_initializer();
	void large_weight_initializer();

	void update_mini_batch(const Patterns<T> &, int *, const int &, const T &, const T &);
	void backprop(const Patterns<T> &, const int &);
	T* feedforward(T *);
	inline int accuracy(const Patterns<T> &, const bool &);
	inline T total_cost(const Patterns<T> &, const bool &, const T &);

public:
	NeuralNetwork(int *, const int &, std::string cost_ = "CrossEntropy");
	void SGD(const Patterns<T> &, const int &, const int &, const T &, const Patterns<T> &, const T &, bool convert = false, bool monitor_evaluation_cost = false, bool monitor_evaluation_accuracy = false, bool monitor_training_cost = false, bool monitor_training_accuracy = false);
	void save(const std::string &, const Patterns<T> &, const int &, bool bin = false);
	//void load(std::string &filename);
	~NeuralNetwork();
};

#include "neural_network.hpp"

template<typename T> class diagQDA
{
	int n_class, *n_label;
	T **mean, **var, *prior	;
public:
	diagQDA();
	~diagQDA();

	void train(const Patterns<T> &, T *prior = nullptr);
	int* test(const Patterns<T> &);
};

#include "diagQDA.hpp"


template<typename T> class QDA
{
	int n_class, *n_label, N;
	T **mean, ***cov_inv, *prior, *det;
public:
	QDA();
	~QDA();

	void train(const Patterns<T> &, T *prior = nullptr);
	int* test(const Patterns<T> &);
};

#include "QDA.hpp"
