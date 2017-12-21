
// TO FIX
template<typename T> T* VecMat(T *x, T *A, const int &Nrow, const int &Ncol)
{
	T *b = new T[Ncol];
	std::memset(b, 0, sizeof(T)*Ncol);
	for (int i = 0; i < Ncol; ++i)
		for (int k = 0; k < Nrow; ++k)
			b[i] += A[k * Ncol + i] * x[k];

	return b;
}
// TO FIX
template<typename T> T* MatVec(T *A, T *x, const int &Nrow, const int &Ncol)
{
	T* b = new T[Nrow];
	std::memset(b, 0, sizeof(T)*Nrow);
	for (int i = 0; i < Nrow; ++i)
		for (int k = 0; k < Ncol; ++k)
			b[i] += A[i*Ncol + k] * x[k];
	return b;
}
// TO FIX
template<typename T> T* VecVec(T *a, T *b, const int &Na, const int &Nb)
{
	T *A = new T[Na * Nb];
	for (int i = 0; i < Nb; ++i)
		for (int j = 0; j < Na; ++j)
			A[j * Nb + i] = a[j] * b[i];
	return A;
}

namespace QuadraticCost
{
	template<typename T> inline T fn(T *a, const int &y, const int &Na)
	{
		/*Return the cost associated with an output ``a`` and desired output ``y``.*/
		return (T).5 * std::accumulate(a, a + Na, (T)0., [&y](const T &i, const T &j) {return i + (j - y)*(j - y); });
	}
	template<typename T> inline T * cost_delta(T *zs, T *a, const int &y, const int &Nz)
	{
		/*Return the error delta from the output layer.*/
		T *res = new T[Nz];
		std::transform(a, a + Nz, zs, res,
					   [&y](const T &ai, const T &zi)
					   {
					   	return (ai - y) * ( ((T)1./((T)1. + exp(-zi))) * ((T)1. - (T)1./((T)1. + exp(-zi))) );
					   });
		return res;
	}
}

namespace CrossEntropyCost
{
	template<typename T> inline T fn(T *a, const int &y, const int &Na)
	{
		/*Return the cost associated with an output ``a`` and desired output
		``y``.  Note that np.nan_to_num is used to ensure numerical
		stability.  In particular, if both ``a`` and ``y`` have a 1.0
		in the same slot, then the expression (1-y)*np.log(1-a)
		returns nan.  The np.nan_to_num ensures that that is converted
		to the correct value (0.0).*/
		return std::accumulate(a, a + Na, (T)0., [&y](const T &i, const T &j) {return i + ((j == (T)1. && y == 1) ? (T)0. : (j == (T)1.) ? std::numeric_limits<T>::max() : -y*log(j) - (1 - y)*log(1 - j)); });
	}
	template <typename T> inline T* cost_delta(T* zs, T* a, const int &y, const int &Nz)
	{
		/*Return the error delta from the output layer.  Note that the
		parameter ``z`` is not used by the method.  It is included in
		the method's parameters in order to make the interface
		consistent with the delta method for other cost classes.*/
		T* res = new T[Nz];
		std::transform(a, a + Nz, res, [&y](const T &ai)
										{
											return ai - y;
										});
		return res;
	}
}

template<typename T> inline T* NeuralNetwork<T>::vectorized_result(const int &n, const int &j)
{
	T *e = new T[n];
	std::memset(e, 0, sizeof(T)*n);
	e[j] = (T)1.;
	return e;
}


template<typename T> NeuralNetwork<T>::~NeuralNetwork()
{
	delete[] this->sizes;
	for (int i = 0; i < this->num_layers - 1; ++i)
	{
		delete[] this->weights[i];
		delete[] this->delta_nabla_w[i];
		delete[] this->nabla_w[i];
		delete[] this->biases[i];
		delete[] this->delta_nabla_b[i];
		delete[] this->nabla_b[i];
	}
	delete[] this->weights;
	delete[] this->delta_nabla_w;
	delete[] this->nabla_w;
	delete[] this->biases;
	delete[] this->delta_nabla_b;
	delete[] this->nabla_b;
}

template<typename T> NeuralNetwork<T>::NeuralNetwork(int *size, const int &num_layers, std::string cost_, unsigned int seed)
{
	/*The list ``sizes`` contains the number of neurons in the
	respective layers of the network.  For example, if the list
	was [2, 3, 1] then it would be a three-layer network, with the
	first layer containing 2 neurons, the second layer 3 neurons,
	and the third layer 1 neuron.  The biases and weights for the
	network are initialized randomly, using a Gaussian
	distribution with mean 0, and variance 1.  Note that the first
	layer is assumed to be an input layer, and by convention we
	won't set any biases for those neurons, since biases are only
	ever used in computing the outputs from later layers.*/
	srand(seed);
	this->cost = cost_;
	if (this->cost == "CrossEntropy")
	{
		this->fn = &CrossEntropyCost::fn;
		this->cost_delta = &CrossEntropyCost::cost_delta;
	}
	else if (this->cost == "Quadratic")
	{
		this->fn = &QuadraticCost::fn;
		this->cost_delta = &QuadraticCost::cost_delta;
	}
	else
	{ std::cerr << "Invalid Cost function found! Given : " << cost_ << ". Possible values are only 'CrossEntropy' and 'Quadratic'!" << std::endl; exit(1);}

	this->num_layers = num_layers;
	this->sizes = new int[this->num_layers];
	std::transform(size, size + this->num_layers, this->sizes, [](const T &val){return val;});
	default_weight_initializer();
}

template<typename T> void NeuralNetwork<T>::default_weight_initializer()
{
	/*Initialize each weight using a Gaussian distribution with mean 0
	and standard deviation 1 over the square root of the number of
	weights connecting to the same neuron.  Initialize the biases
	using a Gaussian distribution with mean 0 and standard
	deviation 1.

	Note that the first layer is assumed to be an input layer, and
	by convention we won't set any biases for those neurons, since
	biases are only ever used in computing the outputs from later
	layers.
	*/
	T normalize;
	this->biases  = new T*[this->num_layers - 1]; 
	this->nabla_b = new T*[this->num_layers - 1]; 
	this->delta_nabla_b = new T*[this->num_layers - 1];

	this->weights = new T*[this->num_layers - 1]; 
	this->nabla_w = new T*[this->num_layers - 1]; 
	this->delta_nabla_w = new T*[this->num_layers - 1];

	for (int i = 0; i < this->num_layers - 1; ++i)
	{
		this->biases[i] = new T[this->sizes[i + 1]]; 
		this->nabla_b[i] = new T[this->sizes[i + 1]]; 
		this->delta_nabla_b[i] = new T[this->sizes[i + 1]];
		normalize = sqrt((T)this->sizes[i + 1]);

		this->weights[i] = new T[this->sizes[i] * this->sizes[i + 1]]; 
		this->nabla_w[i] = new T[this->sizes[i] * this->sizes[i + 1]];
		this->delta_nabla_w[i] = new T[this->sizes[i] * this->sizes[i + 1]];

		std::generate(this->biases[i],  this->biases[i]  + this->sizes[i + 1],[]{return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);});
		std::generate(this->weights[i], this->weights[i] + this->sizes[i] * this->sizes[i + 1], []{return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);});
	}
	return;
}

template<typename T> void NeuralNetwork<T>::large_weight_initializer()
{
	/*Initialize each weight using a Gaussian distribution with mean 0
	and standard deviation 1 over the square root of the number of
	weights connecting to the same neuron.  Initialize the biases
	using a Gaussian distribution with mean 0 and standard
	deviation 1.

	Note that the first layer is assumed to be an input layer, and
	by convention we won't set any biases for those neurons, since
	biases are only ever used in computing the outputs from later
	layers.
	*/
	this->biases  = new T*[this->num_layers - 1];  
	this->nabla_b = new T*[this->num_layers - 1]; 
	this->delta_nabla_b = new T*[this->num_layers - 1];

	this->weights = new T*[this->num_layers - 1];  
	this->nabla_w = new T*[this->num_layers - 1]; 
	this->delta_nabla_w = new T*[this->num_layers - 1];
	
	for (int i = 0; i < this->num_layers - 1; ++i)
	{
		this->biases[i] = new T[this->sizes[i + 1]]; 
		this->nabla_b[i] = new T[this->sizes[i + 1]]; 
		this->delta_nabla_b[i] = new T[this->sizes[i + 1]];
		this->weights[i] = new T[this->sizes[i] * this->sizes[i + 1]]; 
		this->nabla_w[i] = new T[this->sizes[i] * this->sizes[i + 1]]; 
		this->delta_nabla_w[i] = new T[this->sizes[i] * this->sizes[i + 1]];

		std::generate(biases[i], biases[i] + sizes[i + 1],[]{return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);});
		std::generate(weights[i], weights[i] + sizes[i] * sizes[i + 1], []{return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);});
	}
	return;
}

template<typename T> T* NeuralNetwork<T>::feedforward(T *data)
{
	/*Return the output of the network if ''a'' is input.*/
	T *a = new T[this->sizes[0]];
	std::memcpy(a, data, sizeof(T)*this->sizes[0]);
	for (int i = 0; i < this->num_layers - 1; ++i)
	{
		a = VecMat(a, this->weights[i], this->sizes[i], this->sizes[i + 1]);
		std::transform(a, a + this->sizes[i + 1], 
					   this->biases[i], a, 
					   [](const T &ai, const T &b)
					   {
					   	return (T)1. / ((T)1. + exp(-(ai + b)));
					   });
	}
	return a;
}

template<typename T> void NeuralNetwork<T>::SGD(const Patterns<T> &training, const int &epochs, const int &mini_batch_size, const T &eta, const Patterns<T> &evaluation_data, const T &lambda, bool convert, bool monitor_evaluation_cost, bool monitor_evaluation_accuracy, bool monitor_training_cost, bool monitor_training_accuracy)
{
	/*Train the neural network using mini-batch stochastic
	gradient descent.  The ``training_data`` is a list of tuples
	``(x, y)`` representing the training inputs and the desired
	outputs.  The other non-optional parameters are
	self-explanatory.  If ``test_data`` is provided then the
	network will be evaluated against the test data after each
	epoch, and partial progress printed out.  This is useful for
	tracking progress, but slows things down substantially.*/
	
	int mini_batch_number = (int)(training.Nrow / mini_batch_size) + ((training.Nrow % mini_batch_size) ? 1 : 0),
		**mini_batch = new int*[mini_batch_number], 
		*rand_idx = new int[training.Nrow];

	if (training.Nrow % mini_batch_size)
	{
		std::generate(mini_batch, mini_batch + mini_batch_number - 1, [&mini_batch_size]{return new int[mini_batch_size];});
		mini_batch[mini_batch_number - 1] = new int[training.Nrow % mini_batch_size];
	}
	else
		std::generate(mini_batch, mini_batch + mini_batch_number, [&mini_batch_size]{return new int[mini_batch_size];});

	std::iota(rand_idx, rand_idx + training.Nrow, 0);
	
	for (int j = 0; j < epochs; ++j)
	{
		std::random_shuffle(rand_idx, rand_idx + training.Nrow);
		for (int i = 0; i < training.Nrow; ++i)
			mini_batch[(int)(i / mini_batch_size)][i % mini_batch_size] = rand_idx[i];
		
		std::for_each(mini_batch, mini_batch + mini_batch_number,
					  [&training, &mini_batch_size, &eta, &lambda, this]
					  (int *batch)
					  {
					  	this->update_mini_batch(training, batch, mini_batch_size, eta, lambda);
					  });
		if (monitor_training_cost)
			std::cout << "Cost on training data : " << total_cost(training, convert, lambda) << std::endl;
		if (monitor_training_accuracy)
			std::cout << "Accuracy on training data : " << accuracy(training, convert) << " / " << training.Nrow << std::endl;
		if (monitor_evaluation_cost)
			std::cout << "Cost on evaluation data : " << total_cost(evaluation_data, convert, lambda) << std::endl;
		if (monitor_evaluation_accuracy)
			std::cout << "Accuracy on evaluation data : " << accuracy(evaluation_data, convert) << " / " << evaluation_data.Nrow << std::endl;
	}
	return;
}

template<typename T > void NeuralNetwork<T>::update_mini_batch(const Patterns<T> &training, int *mini_batch_i, const int &mini_batch_size, const T &eta, const T &lambda)
{
	/*Update the network's weights and biases by applying gradient
	descent using backpropagation to a single mini batch.  The
	``mini_batch`` is a list of tuples ``(x, y)``, ``eta`` is the
	learning rate.*/
	for(int i = 0; i < this->num_layers - 1; ++i)
	{
		std::memset(this->nabla_b[i], (T)0, sizeof(T)*this->sizes[i+1]);
		std::memset(this->nabla_w[i], (T)0, sizeof(T)*this->sizes[i]*this->sizes[i+1]);
	}

	for (int i = 0; i < mini_batch_size; ++i)
	{
		NeuralNetwork::backprop(training, mini_batch_i[i]);
		for (int j = 0; j < this->num_layers - 1; ++j)
		{
			std::transform(this->delta_nabla_b[j], this->delta_nabla_b[j] + this->sizes[j+1], 
						   this->nabla_b[j], this->nabla_b[j],
						   [&](const T &delta, const T &b)
						   {
						   	return b + delta;
						   });
			std::transform(this->delta_nabla_w[j], this->delta_nabla_w[j] + this->sizes[j]*this->sizes[j+1], 
						   this->nabla_w[j], this->nabla_w[j],
						   [&](const T &delta, const T &w)
						   {
						   	return w + delta;
						   });
		}
	}
	for (int j = 0; j < this->num_layers - 1; ++j)
	{
		std::transform(this->nabla_b[j], this->nabla_b[j] + this->sizes[j+1], 
					   this->biases[j], this->biases[j], 
					   [&eta, &mini_batch_size](const T &nabla, const T &bias)
					   {
					   	return bias - (eta/mini_batch_size)*nabla;
					   });
		std::transform(this->nabla_w[j], this->nabla_w[j] + this->sizes[j]*this->sizes[j+1], 
					   this->weights[j], this->weights[j], 
					   [&eta, &mini_batch_size, &lambda, &training](const T &nabla, const T &weight)
					   {
					   	return weight * (1 - eta * lambda / training.Nrow) - (eta / mini_batch_size) * nabla;
					   });
	}
	return;
}

template<typename T> void NeuralNetwork<T>::backprop(const Patterns<T> &training, const int &mini_batch_ii)
{
	/*Return a tuple ``(nabla_b, nabla_w)`` representing the
	gradient for the cost function C_x.  ``nabla_b`` and
	``nabla_w`` are layer-by-layer lists of numpy arrays, similar
	to ``self.biases`` and ``self.weights``.*/

	//feedforward
	T **activations = new T*[this->num_layers], **zs = new T*[this->num_layers - 1];
	activations[0] = new T[this->sizes[0]];
	std::memcpy(activations[0], training.input[mini_batch_ii], sizeof(T)*this->sizes[0]);

	for (int i = 1; i < this->num_layers; ++i)
	{
		activations[i] = new T[this->sizes[i]];
		zs[i - 1] = VecMat(activations[i - 1], this->weights[i - 1], this->sizes[i - 1], this->sizes[i]);
		std::transform(this->biases[i - 1], this->biases[i - 1] + this->sizes[i],
					   zs[i-1], zs[i-1],
					   [&](const T &bias, const T &z)
					   {
					   	return z + bias;
					   });
		std::transform(zs[i-1], zs[i-1] + this->sizes[i],
					   activations[i], 
					   [&](const T &z)
					   {
					   	return (T)1. / ((T)1. + exp(-z));
					   });
	}

	// backward pass
	T *delta = cost_delta(zs[this->num_layers - 2], activations[this->num_layers - 1], training.output[mini_batch_ii], this->sizes[this->num_layers - 1]);
	std::memcpy(this->delta_nabla_b[num_layers - 2], delta, sizeof(T)*this->sizes[this->num_layers - 1]);
	this->delta_nabla_w[this->num_layers - 2] = VecVec(activations[this->num_layers - 2], delta, this->sizes[this->num_layers - 2], this->sizes[this->num_layers - 1]);
	for (int l = 2; l < num_layers; ++l)
	{
		delta = MatVec(this->weights[this->num_layers - l], delta, this->sizes[this->num_layers - l], this->sizes[this->num_layers - l + 1]);
		
		std::transform(zs[this->num_layers - 1 - l], zs[this->num_layers - 1 - l] + this->sizes[this->num_layers - l],
					   delta, delta,
					   [&](const T &z, const T &d)
					   {
					   	return d * ( ((T)1./((T)1. + exp(-z))) * ((T)1. - (T)1./((T)1. + exp(-z))) );
					   });
		std::memcpy(this->delta_nabla_b[this->num_layers - 1 - l], delta, sizeof(T)*this->sizes[this->num_layers - l]);
		this->delta_nabla_w[this->num_layers - 1 - l] = VecVec(activations[this->num_layers - 1 - l], delta, this->sizes[this->num_layers - 1 - l], this->sizes[this->num_layers - l]);
	}
	delete[] delta;
	for (int i = 0; i < num_layers; ++i) delete[] activations[i];
	for (int i = 0; i < num_layers - 1; ++i) delete[] zs[i];
	delete[] activations;
	delete[] zs;
}

template<typename T> inline int NeuralNetwork<T>::accuracy(const Patterns<T> &pattern, const bool &convert)
{
	/*Return the number of inputs in ``data`` for which the neural
    network outputs the correct result. The neural network's
    output is assumed to be the index of whichever neuron in the
    final layer has the highest activation.

    The flag ``convert`` should be set to False if the data set is
    validation or test data (the usual case), and to True if the
    data set is the training data. The need for this flag arises
    due to differences in the way the results ``y`` are
    represented in the different data sets.  In particular, it
    flags whether we need to convert between the different
    representations.  It may seem strange to use different
    representations for the different data sets.  Why not use the
    same representation for all three data sets?  It's done for
    efficiency reasons -- the program usually evaluates the cost
    on the training data and the accuracy on other data sets.
    These are different types of computations, and using different
    representations speeds things up.  More details on the
    representations can be found in
    mnist_loader.load_data_wrapper.
	*/
	int acc;
	T *a = nullptr;
	acc = std::inner_product(pattern.input, pattern.input + pattern.Nrow, pattern.output, (T)0., 
								std::plus<int>(), 
								[&a, this](T* in, const int &out) mutable 
								{
									a = this->feedforward(in);
									return (std::distance(a, std::max_element(a, a + this->sizes[0])) == out) ? 1 : 0;
								});
	delete[] a;
	return acc;
}

template<typename T> inline T NeuralNetwork<T>::total_cost(const Patterns<T> &pattern, const bool &convert, const T &lambda)
{
	T cost_, *a = nullptr;

	cost_ = std::inner_product(pattern.input, pattern.input + pattern.Nrow, 
							   pattern.output, (T)0.,
							   std::plus<T>(),
							   [&a, this](T* in, const int &out) mutable
							   {
							   	a = this->feedforward(in);
							   	return this->fn(a, out, this->sizes[0]);
							   }) / pattern.Nrow;
	delete[] a;
	for (int i = 0; i < this->num_layers - 1; ++i)
		cost_ += (T).5 * (lambda / pattern.Nrow) * std::inner_product(this->weights[i], this->weights[i] + this->sizes[i]*this->sizes[i+1], this->weights[i], (T)0.);

	return cost_;
}

template<typename T> void NeuralNetwork<T>::save(const std::string &filename, const Patterns<T> &training, const int &num_layers, bool bin)
{
	std::ofstream os;
	if(bin)
	{
		// binary file
		os.open(filename, std::ios::out | std::ios::binary);
		// save training dimensions and network dimensions
		os.write( (const char *) &training.Nrow, sizeof(int));
		os.write( (const char *) &training.Ncol, sizeof(int));
		os.write( (const char *) &this->num_layers, sizeof(int));
		std::for_each(this->sizes, this->sizes + this->num_layers, 
					  [&os](const int &val)
					  {
					  	os.write( (const char*)&val, sizeof(int));
					  });
		// save weights matrix
		for (int i = 0; i < this->num_layers - 1; ++i)
			std::for_each(this->weights[i], this->weights[i] + this->sizes[i]*this->sizes[i+1], 
						  [&os](const T &val)
						  {
						  	os.write( (const char*)&val, sizeof(T));
						  });
		// save biases matrix
		for (int i = 0; i < this->num_layers - 1; ++i)
			std::for_each(this->biases[i], this->biases[i] + this->sizes[i] * this->sizes[i+1], 
						  [&os](const T &val)
						  {
						  	os.write( (const char*)&val, sizeof(T));
						  });
	}
	else
	{
		os.open(filename);
		//save training dimensions and network dimensions
		os << training.Nrow << "\t" << training.Ncol << "\t" << num_layers << std::endl;
		std::copy(sizes, sizes + num_layers - 1, std::ostream_iterator<int>(os, "\t"));
		os << sizes[num_layers - 1] << std::endl;
		//save weights matrix
		for (int i = 0; i < num_layers - 1; ++i)
		{
			std::copy(weights, weights + sizes[i] * sizes[i + 1] - 1, std::ostream_iterator<T>(os, "\t"));
			os << weights[i][sizes[i] * sizes[i + 1] - 1] << std::endl;
		}
		//save biases matrix
		for (int i = 0; i < num_layers - 1; ++i)
		{
			std::copy(biases, biases + sizes[i + 1] - 1, std::ostream_iterator<T>(os, "\t"));
			os << biases[i][sizes[i + 1] - 1] << std::endl;
		}
	}	
	os.close();
	return;
}
