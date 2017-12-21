
template<typename T> QDA<T>::QDA()
{
	this->mean = nullptr;
	this->cov_inv = nullptr;
	this->prior = nullptr;
	this->n_label = nullptr;
	this->n_class = 0;
	this->N = 0;
	this->det = nullptr;
}

template<typename T> QDA<T>::~QDA()
{
	if(this->n_class != 0)
	{
		for(int i = 0; i < this->n_class; ++i)
		{
			delete[] this->mean[i];
			for(int j = 0; j < this->N; ++j)
				delete[] this->cov_inv[i][j];
			delete[] this->cov_inv[i];
		}
		delete[] this->mean;
		delete[] this->cov_inv;
		delete[] this->prior;
		delete[] this->n_label;
		delete[] this->det;
	}
}

template<typename T> void QDA<T>::train(const Patterns<T> &pattern, T *prior)
{
	if(this->n_class == 0)
	{
		std::unordered_set<int> lbl(pattern.output, pattern.output + pattern.Nout, pattern.Nout*sizeof(int));
		this->n_class = (int)lbl.size();
		this->n_label = new int[this->n_class];
		for(const auto &u : lbl) this->n_label[u] = std::count(pattern.output, pattern.output + pattern.Nout, u);
	}
	if(this->prior != nullptr)
	{
		this->prior = new T[this->n_class];
		std::memcpy(this->prior, prior, sizeof(T)*this->n_class);
	}
	else
		this->prior = nullptr;
	this->mean = new T*[this->n_class];
	this->det = new T[this->n_class];
	this->cov_inv = new T**[this->n_class];
	T ***mean_sq = new T**[this->n_class];
	this->N = pattern.Ncol;
	std::generate(this->mean, this->mean + this->n_class, [this]{return new T[this->N];});
	std::for_each(this->mean, this->mean + this->n_class, [this](T *mu){std::memset(mu, 0, sizeof(T)*this->N);});
	for(int i = 0; i < this->n_class; ++i)
	{
		mean_sq[i] = new T*[this->N];
		this->cov_inv[i] = new T*[this->N];
		for(int j = 0; j < this->N; ++j)
		{
			mean_sq[i][j] = new T[this->N];
			std::memset(mean_sq[i][j], 0, sizeof(T)*this->N);
			this->cov_inv[i][j] = new T[this->N];
		}
	}
	std::memset(this->det, 0, sizeof(int)*this->n_class);
	int o;
	for(int i = 0; i < pattern.Nrow; ++i)
	{
		o = pattern.output[i];
		for(int j = 0; j < this->N; ++j)
			for(int k = j; k < this->N; ++k)
			{
				this->mean[o][k] += (j == 0) ? pattern.input[i][k] : (T)0.;
				mean_sq[o][j][k] += pattern.input[i][j] * pattern.input[i][k];
				this->cov_inv[o][j][k] = mean_sq[o][j][k] - this->mean[o][k] * this->mean[o][j] / this->n_label[o];
				this->cov_inv[o][j][k] /= (this->n_label[o] - 1);
				this->cov_inv[o][k][j] = this->cov_inv[o][j][k];
			}
	}
	// inversion of covariance matrix through Cholesky decomposition
	T sum, *p = new T[this->N];
	for(int i = 0; i < this->n_class; ++i)
	{
		// Cholesky decomposition
		for(int j = 0; j < this->N; ++j)
			for(int k = j; k < this->N; ++k)
			{
				// have to see cholesky upper triangular!!!
				//sum = std::inner_product(this->cov_inv[i][j], this->cov_inv[i][j] + j - 1, 
				//						 this->cov_inv[i][k], this->cov_inv[i][j][k],
				//						 std::minus<T>(),
				//						 [](const T &c1, const T &c2)
				//						 {
				//						 	return c1 * c2;
				//						 });
				sum = this->cov_inv[i][j][k];
				for(int l = j - 1; l >= 0; --l)
					sum -= this->cov_inv[i][j][l] * this->cov_inv[i][k][l];
				if(j == k)
				{
					if(sum <= (T)0.)
					{
						std::cerr << "Cholesky Error! Matrix is not positive defined!" << std::endl;
						exit(1);
					}
					p[j] = sqrt(sum);
					this->det[i] += log(p[j]);
				}
				else
					this->cov_inv[i][k][j] = sum / p[j];
			}
		this->det[i] *= (T)2.;
		//std::cout << this->det[i] << std::endl;
		// Cholesky inversion
		for(int j = 0; j < this->N; ++j)
		{
			this->cov_inv[i][j][j] = (T)1. / p[j];
			for(int k = j + 1; k < this->N; ++k)
			{
				sum = (T)0.;
				for(int l = j; l < k; ++l)
					sum -= this->cov_inv[i][k][l] * this->cov_inv[i][l][j];
				this->cov_inv[i][k][j] = sum / p[k];
			}
		}
		//===============================
		for(int j = 0; j < this->N; ++j)
			for(int k = j + 1; k < this->N; ++k)
				this->cov_inv[i][j][k] = (T)0.;

		//==============================
		// miss step of null upper triangular
		for(int j = 0; j < this->N; ++j)
		{
			//this->cov_inv[i][j][j] = std::accumulate(this->cov_inv[i], this->cov_inv[i] + j + 1,
			//										 this->cov_inv[i][j][j]*this->cov_inv[i][j][j],
			//										 [&j](const T &val, T *c)
			//										 {
			//										 	return val + c[j]*c[j];
			//										 });
			this->cov_inv[i][j][j] *= this->cov_inv[i][j][j];
			for(int k = j + 1; k < this->N; ++k)
				this->cov_inv[i][j][j] += this->cov_inv[i][k][j] * this->cov_inv[i][k][j];
			for(int k = j + 1; k < this->N; ++k)
				for(int l = k; l < this->N; ++l)
					this->cov_inv[i][j][k] += this->cov_inv[i][l][j] * this->cov_inv[i][l][k];
		}
		for(int j = 0; j < this->N; ++j)
		{
			for(int k = 0; k < j; ++k)
				this->cov_inv[i][j][k] = this->cov_inv[i][k][j];
			delete[] mean_sq[i][j];
		}
		delete[] mean_sq[i];

		//TODO=========================
		//for(int j = 0; j < this->N; ++j)
		//{
		//	for(int k = 0; k < this->N; ++k)
		//		std::cout << this->cov_inv[i][j][k] << " ";
		//	std::cout << std::endl;
		//}
		//std::cin.get();

		//=============================

		// CHECK COV_INV DONE!!!
		// WRONG DETERMINANT COMPUTING
		// SEE ALSO CHOLESKY DECOMPOSITION UPPER TRIANGULAR FOR INNER PRODUCT STD

	} // end class loop
	delete[] mean_sq;
	delete[] p;
	return;
}

template<typename T> int* QDA<T>::test(const Patterns<T> &pattern)
{
	int *score = new int[pattern.Nout];
	T max, discr, m1;
	for(int i = 0; i < pattern.Nrow; ++i)
	{
		max = -std::numeric_limits<T>::infinity();
		for(int c = 0; c < this->n_class; ++c)
		{
			discr = (T)0.;
			for(int j = 0; j < pattern.Ncol; ++j)
			{
				m1 = pattern.input[i][j] - this->mean[c][j] / this->n_label[c];
				for (int k = 0; k < pattern.Ncol; ++k)
					discr += (pattern.input[i][k] - this->mean[c][k] / this->n_label[c]) * this->cov_inv[c][j][k] * m1;
			}
			discr = -(T).5 * discr -(T).5 * this->det[c];
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
