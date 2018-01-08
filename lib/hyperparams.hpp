#include <functional>
#include <tuple>
#define COUT_WNAME(os, name, sep) do{(os) << name << sep;}while(false)
#define COUTNAME(os, a, sep) COUT_WNAME((os), #a, (sep))
#define COUTVAL(os, a, sep)	 COUT_WNAME((os), (a), (sep))

// Perceptron
template<class Cls> 
struct hyperparams<Cls, typename std::enable_if<std::is_same<Cls, Perceptron<typename Cls::type>>::value>::type>
{
	using T = typename Cls::type;
	T eta;
	int max_iter;
	unsigned int seed;
	std::tuple<	std::function<T()>, 
				std::function<unsigned int()>> func;
	hyperparams()
	{
		this->func = std::make_tuple([&](){return static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX);},
									 [&](){return static_cast<unsigned int>(std::rand());});
	}
	hyperparams(const T &eta, const int &max_iter, const unsigned int &seed)
	{ this->eta = eta; this->seed = seed; this->max_iter = max_iter; }
	hyperparams& operator=(const hyperparams &p)
	{ this->eta = p.eta; this->seed = p.seed; this->max_iter = p.max_iter; return *this;}
	~hyperparams() = default;
	inline void operator()(const unsigned int &s) // rn generator
	{
		std::srand(s);
		this->eta = std::get<0>(this->func)();
		this->seed = std::get<1>(this->func)();
		this->max_iter = 100;
	}
	inline void operator!(void) // mutation operator
	{
		int pos = static_cast<int>(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * std::tuple_size<decltype(this->func)>::value);
		switch(pos)
		{
			case 0: this->eta = std::get<0>(this->func)();
			break;
			case 1: this->seed = std::get<1>(this->func)();
			break;
		}
		return;
	}
	inline hyperparams operator+(const hyperparams &p) // cross over operator
	{
		hyperparams res;
		int pos = static_cast<int>(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * std::tuple_size<decltype(this->func)>::value > 0.5f);
		switch(pos)
		{
			case 0: res = hyperparams(p.eta, 100, this->seed);
			break;
			case 1: res = hyperparams(this->eta, 100, p.seed);
			break;
		}
		return res;
	}
	inline void get_name(std::ostream &os, char sep = '\t')
	{
		COUTNAME(os, eta, sep);
		COUTNAME(os, max_iter, sep);
		COUTNAME(os, seed, sep);
		os << std::endl;
		return;
	}
	friend std::ostream& operator<<(std::ostream &os, const hyperparams &p)
	{
		COUTVAL(os, p.eta, '\t');
		COUTVAL(os, p.max_iter, '\t');
		COUTVAL(os, p.seed, '\t');
		return os;
	}
};

// NeuralNetwork
template<class Cls> 
struct hyperparams<Cls, typename std::enable_if<std::is_same<Cls, NeuralNetwork<typename Cls::type>>::value>::type>
{
	using T = typename Cls::type;
	std::tuple<	std::function<int()>, // num_layers
				std::function<int()>, // cost
				std::function<int()>, // epochs
				std::function<int()>, // mini_batch_size
				std::function<T()>, // eta
				std::function<T()>, // lambda
				std::function<unsigned int()> // seed
				> func;
	int *sizes = nullptr, num_layers, epochs, mini_batch_size, cost;
	std::tuple<Qcost, Ecost> cost_func;
	T eta, lambda;
	unsigned int seed;
	hyperparams()
	{
		this->sizes = nullptr;
		this->func = std::make_tuple(
									 [&](){return static_cast<int>(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 10) + 1;}, // num_layers
									 [&](){return static_cast<int>(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (std::tuple_size<decltype(cost_func)>::value - 1));}, // cost
									 [&](){return 1000;}, // epochs
/*FIX*/								 [&](){return 2;}, // mini_batch_size
									 [&](){return static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX);}, // eta
/*FIX*/								 [&](){return (T)2.;}, // lambda
									 [&](){return static_cast<unsigned int>(std::rand());} // seed
									 );
	}
	hyperparams(int *sizes, const int &num_layers, const int &cost, const int &epochs, const int &mini_batch_size, const T &eta, const T &lambda, const unsigned int &seed)
	{ 
		this->num_layers = num_layers;
		this->sizes = new int[this->num_layers];
		this->cost = cost;
		std::memcpy(this->sizes, sizes, this->num_layers*sizeof(int));
		this->epochs = epochs;
		this->mini_batch_size = mini_batch_size;
		this->eta = eta;
		this->lambda; 
		this->seed = seed;
	}
	hyperparams& operator=(const hyperparams &p)
	{ 
		this->num_layers = p.num_layers;
		if(this->sizes != nullptr) delete[]this->sizes;
		this->sizes = new int[p.num_layers];
		std::memcpy(this->sizes, p.sizes, this->num_layers*sizeof(int));
		this->cost = p.cost;
		this->epochs = p.epochs;
		this->mini_batch_size = p.mini_batch_size;
		this->eta = p.eta; 
		this->lambda = p.lambda;
		this->seed = p.seed; 
		return *this;
	}
	~hyperparams()
	{ if(this->sizes != nullptr) delete[] this->sizes;}
	inline void operator()(const unsigned int &s) // rn generator
	{
		srand(s);
		if(this->sizes != nullptr) delete[]this->sizes;
		this->num_layers = std::get<0>(this->func)();
		this->cost = std::get<1>(this->func)();
		this->epochs = std::get<2>(this->func)();
		this->mini_batch_size = std::get<3>(this->func)();
		this->eta = std::get<4>(this->func)();
		this->lambda = std::get<5>(this->func)();
		this->seed = std::get<6>(this->func)();
		this->sizes = new int[this->num_layers];

		std::generate(this->sizes, this->sizes + this->num_layers, []{return static_cast<int>(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 100);});
		return;
	}
	inline void operator!(void) // mutation operator
	{
		int pos = static_cast<int>(static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * std::tuple_size<decltype(this->func)>::value);
		switch(pos)
		{
			case 0:
			{
				if(this->sizes == nullptr) this->sizes = new int[this->num_layers];
				std::generate(this->sizes, this->sizes + this->num_layers, []{return static_cast<int>(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 100);});
			}break;
			case 1: this->num_layers = std::get<0>(this->func)();
			break;
			case 2: this->cost = std::get<1>(this->func)();
			break;
			case 3: this->epochs = std::get<2>(this->func)();
			break;
			case 4: this->mini_batch_size = std::get<3>(this->func)();
			break;
			case 5: this->eta = std::get<4>(this->func)();
			break;
			case 6: this->lambda = std::get<5>(this->func)();
			break;
			case 7: this->seed = std::get<6>(this->func)();
			break;
		}
		return;
	}
	inline hyperparams operator+(const hyperparams &p) // cross over operator
	{
		hyperparams res;
		int pos = static_cast<int>(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (std::tuple_size<decltype(this->func)>::value + 1));
		switch(pos)
		{
			case 0: res = hyperparams(p.sizes, p.num_layers, p.cost, p.epochs, p.mini_batch_size, p.eta, p.lambda, p.seed);
			break;
			case 1: res = hyperparams(this->sizes, p.num_layers, p.cost, p.epochs, p.mini_batch_size, p.eta, p.lambda, p.seed);
			break;
			case 2: res = hyperparams(this->sizes, this->num_layers, p.cost, p.epochs, p.mini_batch_size, p.eta, p.lambda, p.seed);
			break;
			case 3: res = hyperparams(this->sizes, this->num_layers, this->cost, p.epochs, p.mini_batch_size, p.eta, p.lambda, p.seed);
			break;
			case 4: res = hyperparams(this->sizes, this->num_layers, this->cost, this->epochs, p.mini_batch_size, p.eta, p.lambda, p.seed);
			break;
			case 5: res = hyperparams(this->sizes, this->num_layers, this->cost, this->epochs, this->mini_batch_size, p.eta, p.lambda, p.seed);
			break;
			case 6: res = hyperparams(this->sizes, this->num_layers, this->cost, this->epochs, this->mini_batch_size, this->eta, p.lambda, p.seed);
			break;
			case 7: res = hyperparams(this->sizes, this->num_layers, this->cost, this->epochs, this->mini_batch_size, this->eta, this->lambda, p.seed);
			break;
			case 8: res = hyperparams(this->sizes, this->num_layers, this->cost, this->epochs, this->mini_batch_size, this->eta, this->lambda, this->seed);
			break;
		}		
		return res;
	}
	inline void get_name(std::ostream &os, char sep = '\t')
	{
		os 	<< "sizes(";
		COUTNAME(os, num_layers, ')');
		COUTNAME(os, sizes, sep);
		os << sep << "QuadraticCost(0)/CrossEntropyCost(1)" << sep;
		COUTNAME(os, epochs, sep); 
		COUTNAME(os, mini_batch_size, sep);
		COUTNAME(os, eta, sep); 
		COUTNAME(os, lambda, sep); 
		COUTNAME(os, seed, sep);
		return;
	}
	friend std::ostream& operator<<(std::ostream &os, const hyperparams &p)
	{
		std::copy(p.sizes, p.sizes + p.num_layers, std::ostream_iterator<int>(os, "\t"));
		COUTVAL(os, p.num_layers, '\t'); 
		COUTVAL(os, p.cost, '\t');
		COUTVAL(os, p.epochs, '\t'); 
		COUTVAL(os, p.mini_batch_size, '\t'); 
		COUTVAL(os, p.eta, '\t'); 
		COUTVAL(os, p.lambda, '\t'); 
		COUTVAL(os, p.seed, '\t');
		return os;
	}
};

// diagQDA && QDA
template<class Cls> 
struct hyperparams<Cls, typename std::enable_if<std::is_same<Cls, diagQDA<typename Cls::type>>::value || std::is_same<Cls, QDA<typename Cls::type>>::value>::type>
{
	using T = typename Cls::type;
	T *prior;
	std::tuple<	std::function<void(T*)>> func;
	hyperparams()
	{
		this->prior = nullptr;
		this->func = std::make_tuple([&](T *p)
									 {
									 	std::generate(p, p + 2, []{return static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX);});
										T sum = std::accumulate(p, p + 2, (T)0.);
										std::transform(p, p + 2, p, [&sum](const T &p){return p / sum;});
									 }
									);
	}
	hyperparams(T *prior)
	{
		if(this->prior != nullptr) delete[] this->prior;
		this->prior = new T[2]; 
		std::memcpy(this->prior, prior, 2*sizeof(T));
	}
	hyperparams& operator=(const hyperparams &p)
	{
		this->prior = new T[2];
		std::memcpy(this->prior, p.prior, sizeof(T)*2);
		return *this;
	}
	~hyperparams()
	{ if(this->prior != nullptr) delete[] this->prior;}
	void operator()(const unsigned int &s) // rn generator
	{
		std::srand(s);
		if(this->prior != nullptr) delete[] this->prior;
		this->prior = new T[2];
		std::get<0>(this->func)(this->prior)();
		return;
	}
	void operator!(void) // mutation operator
	{
		T 	r = static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX), 
			diff = r - this->prior[0];
		this->prior[0] = r; // set of the new variable with random value
		this->prior[1] -= diff; // set of another old value with the diff (binding sum 1)
		return;
	}
	hyperparams operator+(const hyperparams &p) // cross over operator
	{
		hyperparams res;
		res.prior = new T[2];
		res.prior[0] = this->prior[0];
		res.prior[1] = p.prior[1];
		T sum = std::accumulate(res.prior, res.prior + 2, (T)0.);
		std::transform(res.prior, res.prior + 2, res.prior, [&sum](const T &p){return p / sum;});
		return res;
	}
	void get_name(std::ostream &os, char sep = '\t')
	{
		COUTNAME(os, this->prior, sep);
		return;
	}
	friend std::ostream& operator<<(std::ostream &os, const hyperparams &p)
	{ 
		std::copy(p.prior, p.prior + 2, std::ostream_iterator<T>(os, "\t")); 
		return os;
	}
};
