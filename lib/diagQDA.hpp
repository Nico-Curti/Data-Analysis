
template<typename T> diagQDA<T>::diagQDA()
{
	this->mean = nullptr;
	this->var = nullptr;
	this->prior = nullptr;
	this->n_label = nullptr;
	this->n_class = 0;
}

template<typename T> diagQDA<T>::~diagQDA()
{
	if(this->n_class != 0)
	{
		for(int i = 0; i < this->n_class; ++i)
		{
			delete[] this->mean[i];
			delete[] this->var[i];
		}
		delete[] this->mean;
		delete[] this->var;
		delete[] this->prior;
		delete[] this->n_label;
	}
}


template<typename T> void diagQDA<T>::train(const Patterns<T> &pattern, T *prior)
{
	if(this->n_class == 0)
	{
		std::unordered_set<int> lbl(pattern.output, pattern.output + pattern.Nout, pattern.Nout*sizeof(int));
		this->n_class = (int)lbl.size();
	}
	if(this->prior != nullptr)
	{
		this->prior = new T[this->n_class];
		std::memcpy(this->prior, prior, sizeof(T)*this->n_class);
	}
	else
		this->prior = nullptr;
	this->mean = new T*[this->n_class];
	this->var = new T*[this->n_class];
	this->n_label = new int[this->n_class];
	std::generate(this->mean, this->mean + this->n_class, [&pattern]{return new T[pattern.Ncol];});
	std::for_each(this->mean, this->mean + this->n_class, [&pattern](T *mu){std::memset(mu, 0, sizeof(T)*pattern.Ncol);});
	std::generate(this->var, this->var + this->n_class, [&pattern]{return new T[pattern.Ncol];});
	std::for_each(this->var, this->var + this->n_class, [&pattern](T *v){std::memset(v, 0, sizeof(T)*pattern.Ncol);});
	std::memset(this->n_label, 0, sizeof(int)*this->n_class);
	T delta;
	int o;
	for(int i = 0; i < pattern.Nrow; ++i)
	{
		o = pattern.output[i];
		++this->n_label[o];
		for(int j = 0; j < pattern.Ncol; ++j)
		{
			delta = pattern.input[i][j] - this->mean[o][j];
			this->mean[o][j] += delta / this->n_label[o];
			this->var[o][j]  += delta * (pattern.input[i][j] - this->mean[o][j]);
		}
	}
	std::transform(this->n_label, this->n_label + this->n_class,
				   this->var, this->var,
				   [&pattern](const int &n, T *v)
				   {
				   		std::transform(v, v + pattern.Ncol, 
									   v,
									   [&n](const T &val)
									   {
									   	return val/n;
									   });
				   		return v;
				   });

}

template<typename T> int* diagQDA<T>::test(const Patterns<T> &pattern)
{
	int *score = new int[pattern.Nout];
	T max, discr, det;
	for(int i = 0; i < pattern.Nrow; ++i)
	{
		max = -std::numeric_limits<T>::infinity();
		for(int c = 0; c < this->n_class; ++c)
		{
			det = (T)0.;
			discr = (T)0.;
			for(int j = 0; j < pattern.Ncol; ++j)
			{
				det -= log(this->var[c][j]);
				discr += (pattern.input[i][j] - this->mean[c][j]) * (pattern.input[i][j] - this->mean[c][j]) / this->var[c][j];
			}
			discr = -(T).5 * discr -(T).5 * det;
			discr += (this->prior != nullptr) ? log(this->prior[c]) : (T)0.;
			if(max < discr)
			{
				max = discr;
				score[i] = c;
			}
		}
	}
	return score;
}
