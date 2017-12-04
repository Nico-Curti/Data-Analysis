
template<typename T> inline void print(const coo_matrix<T> &matrix)
{
	int r, c;
	for(int i = 0; i < (int)matrix.index->size(); ++i)
	{
		r = matrix.index->at(i) / matrix.Ncols;
		c = matrix.index->at(i) % matrix.Ncols;
		std::cout << "[" << r << "," << c << "] " << matrix.values->at(i) << std::endl;
	}
	std::cout << std::endl;
	return;
}

template<typename T> void save_gephi(const coo_matrix<T> &matrix, std::string *lbl, std::string filename)
{
	std::ofstream os(filename + "_" + std::to_string(matrix.Nrows) + "v_nodes.csv");
	os << "Id,Label\n";
	for(int i = 0; i < matrix.Nrows; ++i)
		os << i << "," << lbl[i] << std::endl;
	os.close();
	int r, c;
	os.open(filename + "_" + std::to_string(matrix.Nrows) + "v_edges.csv");
	os << "Source,Target,Weight" << std::endl;
	for(int i = 0; i < (int)matrix.index->size(); ++i)
	{
		r = matrix.index->at(i) / matrix.Ncols;
		c = matrix.index->at(i) % matrix.Ncols;
		os << r << ", " << c << "," << matrix.values->at(i) << std::endl;
	}
	os.close();
	return;
}

template<typename T> coo_matrix<T>::coo_matrix()
{
	this->Nrows = 0;
	this->Ncols = 0;
	this->index = nullptr;
	this->values = nullptr;
}
template<typename T> coo_matrix<T>::coo_matrix(const int &n)
{
	this->Nrows = n;
	this->Ncols = n;
	this->index = nullptr;
	this->values = nullptr;
}
template<typename T> coo_matrix<T>::coo_matrix(const int &n, const int &m)
{
	this->Nrows = n;
	this->Ncols = m;
	this->index = nullptr;
	this->values = nullptr;
}

template<typename T> coo_matrix<T>::coo_matrix(const int &n, const int &m, int *row, int *col, T *val, const int &n_val)
{
	this->Nrows = n;
	this->Ncols = m;
	this->index = new std::vector<int>(n_val);
	this->values = new std::vector<T>(n_val);
	for(int i = 0; i < n_val; ++i)
	{
		this->index->at(i) = row[i] * this->Ncols + col[i];
		this->values->at(i) = val[i];
	}
}

template<typename T> coo_matrix<T>::coo_matrix(const csr_matrix<T> &mat)
{
	this->Nrows = mat.Nrows;
	this->Ncols = mat.Ncols;
	this->index = new std::vector<int>(mat.values->size());
	this->values = new std::vector<T>(mat.values->size());
	int start, pos;
	for(int i = 0; i < (int)mat.rows->size(); ++i)
		if(mat.rows->at(i) != 0)
		{
			start = i;
			break;
		}
	pos = mat.rows->at(start);
	for(int i = 0; i < (int)mat.values->size(); ++i)
	{
		if(pos)
			--pos;
		else
		{
			++start;
			pos = mat.rows->at(start) - mat.rows->at(start-1) - 1;
		}
		this->values->at(i) = mat.values->at(i);
		this->index->at(i) = (start-1) * this->Ncols + mat.cols->at(i);
	}
}

template<typename T> coo_matrix<T>::~coo_matrix(void)
{
	if(this->values != nullptr)
	{
		delete this->values;
		delete this->index;
	}
}
template<typename T> coo_matrix<T>::coo_matrix(const coo_matrix<T> &matrix)
{
	this->Nrows = matrix.Nrows;
	this->Ncols = matrix.Ncols;
	if(matrix.values != nullptr)
	{
		this->index = new std::vector<int>(*(matrix.index));
		this->values = new std::vector<T>(*(matrix.values));
	}
}
template<typename T> coo_matrix<T>& coo_matrix<T>::operator=(const coo_matrix<T> &matrix)
{
	if( &matrix != this)
	{
		if(this->values != nullptr)
		{
			delete this->values;
			delete this->index;
		}
	}

	this->Nrows = matrix.Nrows;
	this->Ncols = matrix.Ncols;
	if(matrix.values != nullptr)
	{
		this->index = new std::vector<int>(*(matrix.index));
		this->values = new std::vector<T>(*(matrix.values));
	}
	return *this;
}

template<typename T> coo_matrix<T>& coo_matrix<T>::operator=(const csr_matrix<T> &matrix)
{
	int start, pos;
	if(this->values->size() != matrix.values->size())
	{
		delete this->values;
		delete this->index;
		this->values = new std::vector<T>(matrix.values->size());
		this->index = new std::vector<int>(matrix.values->size());
	}
	this->Nrows = matrix.Nrows;
	this->Ncols = matrix.Ncols;
	for(int i = 0; i < (int)matrix.rows->size(); ++i)
		if(matrix.rows->at(i) != 0)
		{
			start = i;
			break;
		}
	pos = matrix.rows->at(start);
	for(int i = 0; i < (int)matrix.values->size(); ++i)
	{
		if(pos)
			--pos;
		else
		{
			++start;
			pos = matrix.rows->at(start) - matrix.rows->at(start-1) - 1;
		}
		this->values->at(i) = matrix.values->at(i);
		this->index->at(i) = (start-1) * this->Ncols + matrix.cols->at(i);
	}

	return *this;
}

template<typename T> inline T coo_matrix<T>::operator()(const int &i, const int &j)
{
	int idx = std::distance(this->index->begin(), std::find(this->index->begin(), this->index->end(), i*this->Ncols + j));
	return (idx != (int)this->index->size()) ? this->values->at(idx) : T();
}

template<typename T> inline void coo_matrix<T>::operator()(const int &i, const int &j, const T &val)
{
	if(values == nullptr)
	{
		values = new std::vector<T>(1, val);
		index = new std::vector<int>(1, i*this->Ncols + j);
	}
	else
	{
		values->push_back(val);
		index->push_back(i*this->Ncols + j);
	}
	return;
}

template<typename T> inline bool coo_matrix<T>::operator==(const coo_matrix<T> &m)
{
	return ((this->values == nullptr && m.values == nullptr)
				|| (this->values != nullptr && m.values != nullptr && *(this->values) == *(m.values)))
			&& ((this->index == nullptr && m.index == nullptr)
				|| (this->index != nullptr && m.index != nullptr && *(this->index) == *(m.index)));
}

template<typename T> inline bool coo_matrix<T>::operator!=(const coo_matrix<T> &m)
{
	return !(*this != m);	
}

template<typename T> inline coo_matrix<T> coo_matrix<T>::transpose()
{
	coo_matrix<T> t_mat = *this;
	std::transform(this->index->begin(), this->index->end(), t_mat.index->begin(), [&this->Ncols, &this->Nrows](const int &idx){return (idx % this->Ncols) * this->Nrows + idx / this->Ncols;});
	return t_mat;
}

template<typename T> inline void coo_matrix<T>::t()
{
	std::transform(this->index->begin(), this->index->end(), this->index->begin(), [&this->Ncols, &this->Nrows](const int &idx){return (idx % this->Ncols) * this->Nrows + idx / this->Ncols;});
	return;
}


template<typename T> template<typename F> inline void coo_matrix<T>::add(const F &a)
{
	std::transform(this->values->begin(), this->values->end(), this->values->begin(), [&a](const T &val){return val + a;});
	return;
}
template<typename T> template<typename F> inline void coo_matrix<T>::diff(const F &a)
{
	std::transform(this->values->begin(), this->values->end(), this->values->begin(), [&a](const T &val){return val - a;});
	return;
}
template<typename T> template<typename F> inline void coo_matrix<T>::mult(const F &a)
{
	std::transform(this->values->begin(), this->values->end(), this->values->begin(), [&a](const T &val){return val * a;});
	return;
}
template<typename T> template<typename F> inline void coo_matrix<T>::div(const F &a)
{
	std::transform(this->values->begin(), this->values->end(), this->values->begin(), [&a](const T &val){return val / a;});
	return;
}

