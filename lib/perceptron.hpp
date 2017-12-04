#include <time.h>

template<typename T> Perceptron<T>::Perceptron()
{ this->num_inputs = 0; this->size = 0; this->bias = (T)0.; this->weights = nullptr; }

template<typename T> Perceptron<T>::Perceptron(const int &Nsample, const int &Ninput)
{
	this->num_inputs = Ninput; this->size = Nsample;
	/*Initialize the perceptron with the bias and all weights
    set to 0.0. ``num_inputs`` is the number of input bits to the
    perceptron.*/
    this->weights = new T[this->num_inputs];
    srand((unsigned int)time(nullptr));
    std::generate(this->weights, this->weights + this->num_inputs, []{return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);});
	this->bias = static_cast <T> (rand()) / (static_cast <T> (RAND_MAX/(T)1.));
}

template<typename T> Perceptron<T>::Perceptron(const Perceptron<T> &perceptron)
{
	this->num_inputs = perceptron.num_inputs;
	this->size = perceptron.size;
	this->bias = perceptron.bias;
	this->weights = new T[this->num_inputs];
	std::memcpy(this->weights, perceptron.weights, this->num_inputs*sizeof(T));
}

template<typename T> Perceptron<T>& Perceptron<T>::operator=(const Perceptron<T> &perceptron)
{
	if(this->number != 0) delete[] weights;
	this->num_inputs = perceptron.num_inputs;
	this->bias = perceptron.bias;
	this->size = perceptron.size;
	this->weights = new T[this->num_inputs];
	std::memcpy(this->weights, perceptron.weights, this->num_inputs*sizeof(T));
	return *this;
}

template<typename T> Perceptron<T>::~Perceptron()
{ delete[] this->weights; }


template<typename T> template<bool par, parallel<par, T>> void Perceptron<T>::train(const Patterns<T> &pattern, T eta)
{
	/*Find a bias and a set of weights for a perceptron that
    computes the function ``f``. ``eta`` is the learning rate, and
    should be a small positive number.  Does not terminate when
    the function cannot be computed using a perceptron.*/
    // initialize the bias and weights with random values
	if(pattern.Nrow != this->size){std::cerr << "Wrong input dimensione! Number of inputs " << pattern.Nrow << " have to be the same of initialization (" << this->size << ")" << std::endl; exit(1);}
	if(pattern.Ncol != this->num_inputs){std::cerr << "Wrong input dimensione! Number of samples " << pattern.Ncol << " have to be the same of initialization (" << this->num_inputs << ")" << std::endl; exit(1);}

	int number_of_errors = -1, it = 0;
	T error, inner;
	while(number_of_errors != 0)
	{
		number_of_errors = 0;
		for(int i = 0; i < this->size; ++i)
		{
			/*Return the output (0 or 1) from the perceptron, with input pattern.input[i].*/
			inner = (T)0.;
#pragma omp parallel for reduction(+ : inner)
			for(int j = 0; j < this->num_inputs; ++j)
				inner += this->weights[j] * pattern.input[i][j];
			error = (inner + this->bias > 0) ? pattern.output[i] - 1 : pattern.output[i];
			if(error)
			{
				++number_of_errors;
				this->bias += eta * error;
#pragma omp parallel for
				for(int j = 0; j < this->num_inputs; ++j)
					this->weights[j] += eta * error * pattern.input[i][j];
			}
		}
		++it;
	}
	return;
}

template<typename T>template<bool par, serial<par, T>> void Perceptron<T>::train(const Patterns<T> &pattern, T eta)
{
	if(pattern.Nrow != this->size){std::cerr << "Wrong input dimensione! Number of inputs " << pattern.Nrow << " have to be the same of initialization (" << this->size << ")" << std::endl; exit(1);}
	if(pattern.Ncol != this->num_inputs){std::cerr << "Wrong input dimensione! Number of samples " << pattern.Ncol << " have to be the same of initialization (" << this->num_inputs << ")" << std::endl; exit(1);}

	int number_of_errors = -1, it = 0, error;
	T inner;
	while(number_of_errors != 0)
	{
		number_of_errors = 0;
		for(int i = 0; i < this->size; ++i)
		{
			/*Return the output (0 or 1) from the perceptron, with input pattern.input[i].*/
			inner = std::inner_product(this->weights, this->weights + this->num_inputs, 
									   pattern.input[i], (T)0., 
									   std::plus<T>(), 
									   [](const T &w, const T &i)
									   {
									   	return w*i;
									   });
			error = (inner + this->bias > 0) ? pattern.output[i] - 1 : pattern.output[i];
			if(error)
			{
				++number_of_errors;
				this->bias += eta * error;
				std::transform(pattern.input[i], pattern.input[i] + pattern.Ncol,
							   this->weights, this->weights, 
							   [&eta, &error](const T &in, const T &w)
							   {
							   	return w + eta*error*in;
							   });
			}
		}
		std::cout << "Number of errors: " << number_of_errors << std::endl;
		++it;
	}
	return;
}


template<typename T>template<bool par, parallel<par, T>> int* Perceptron<T>::test(const Patterns<T> &pattern)
{
	int *output = new int[pattern.Nrow];
	T inner;
	int n_thread;
#pragma omp parallel
	{
		n_thread = omp_get_max_threads();
#pragma omp parallel for private(inner)
		for(int i = 0; i < pattern.Nrow; ++i)
		{
			inner = std::inner_product(this->weights, this->weights + this->num_inputs, 
									   pattern.input[i], (T)0.,
									   std::plus<T>(), 
									   [](const T &w, const T &i)
									   {
									   	return w*i;
									   });
			output[i] = (inner + this->bias > 0) ? 1 : 0;
		}
	}
	return output;
}

template<typename T>template<bool par, serial<par, T>> int* Perceptron<T>::test(const Patterns<T> &pattern)
{
	int *output = new int[pattern.Nrow];
	T inner;
	for(int i = 0; i < pattern.Nrow; ++i)
	{
		inner = std::inner_product(this->weights, this->weights + this->num_inputs, 
								   pattern.input[i], (T)0.,
								   std::plus<T>(), 
								   [](const T &w, const T &i)
								   {
								   	return w*i;
								   });
		output[i] = (inner + this->bias > 0) ? 1 : 0;
	}
	return output;
}

