
template<typename T> inline void print(const csr_matrix<T> &matrix)
{
    T element;
    for(int i = 0; i < matrix.Nrows; ++i)
        for(int j = 0; j < matrix.Ncols; ++j)
            if((element = matrix(i, j)))
                std::cout << "[" << i << ", " << j << "] " << element << std::endl;
    return;
}
template<typename T> void save_gephi(const csr_matrix<T> &matrix, const std::string *lbl, std::string filename)
{
    std::ofstream os(filename + "_" + std::to_string(matrix.Nrows) + "v_nodes.csv");
    os << "Id,Label\n";
    for(int i = 0; i < matrix.Nrows; ++i)
        os << i << "," << lbl[i] << std::endl;
    os.close();
    T element;
    os.open(filename + "_" + std::to_string(matrix.Nrows) + "v_edges.csv");
    os << "Source,Target,Weight" << std::endl;
    for(int i = 0; i < matrix.Nrows; ++i)
        for(int j = 0; j < matrix.Ncols; ++j)
            if((element = matrix(i, j)))
                std::cout << i << ", " << j << "," << element << std::endl;
    os.close();
    return;
}

template<typename T> csr_matrix<T>::csr_matrix()
{
    this->Nrows = 0; 
    this->Ncols = 0;
    this->values = nullptr;
    this->cols = nullptr;
    this->rows = nullptr;
}
template<typename T> csr_matrix<T>::csr_matrix(const int &n)
{
    this->Nrows = n; 
    this->Ncols = n;
    this->values = nullptr;
    this->cols = nullptr;
    this->rows = new std::vector<int>(n + 1);
    std::memset(&this->rows->at(0), 0, sizeof(this->rows->at(0))*this->rows->size());
}
template<typename T> csr_matrix<T>::csr_matrix(const int &n, const int &m)
{
    this->Nrows = n; 
    this->Ncols = m;
    this->values = nullptr;
    this->cols = nullptr;
    this->rows = new std::vector<int>(n + 1);
    std::memset(&this->rows->at(0), 0, sizeof(this->rows->at(0))*this->rows->size());
}
template<typename T> csr_matrix<T>::csr_matrix(const coo_matrix<T> &mat)
{
    int r, c;
    this->Nrows = mat.Nrows;
    this->Ncols = mat.Ncols;
    this->rows = new std::vector<int>(this->Nrows + 1);
    std::memset(&this->rows->at(0), 0, sizeof(this->rows->at(0))*this->rows->size());
    for(int i = 0; i < (int)mat.values->size(); ++i)
    {
        r = mat.index->at(i) / mat.Ncols;
        c = mat.index->at(i) % mat.Ncols;
        this->operator()(r, c, mat.values[i]);
    }
}

template<typename T> csr_matrix<T>::csr_matrix(const int &n, const int &m, T *val, int *irow, int *jcol, const int &nval)
{
    int r, c;
    this->Nrows = n;
    this->Ncols = m;
    this->rows = new std::vector<int>(this->Nrows + 1);
    std::memset(&this->rows->at(0), 0, sizeof(this->rows->at(0))*this->rows->size());
    for(int i = 0; i < nval; ++i)
        this->operator()(irow[i], jcol[i], val[i]);
}

template<typename T> csr_matrix<T>::~csr_matrix(void)
{
    if(this->values != nullptr)
    {
        delete this->values;
        delete this->cols;
    }
    delete this->rows;
}


template<typename T> csr_matrix<T> csr_matrix<T>::operator*(const csr_matrix<T> &mat)
{
    if(this->Ncols != mat.Nrows) { std::cerr << "Invalid dimensions! Product shapes (" << this->Nrows << "," << this->Ncols << ") and (" << mat.Nrows << "," << mat.Ncols << ") not aligned: " << this->Ncols << " (dim 1) != " << mat.Nrows << " (dim 0)!" << std::endl; exit(1);}
    csr_matrix<T> res(this->Nrows, mat.Ncols);
    
    std::cerr << "Not yet supported!" << std::endl; exit(1);
    return res;
}
template<typename T>template<typename F> T* csr_matrix<T>::operator*(F *vec)
{
    T *y = new T[this->Nrows];
    std::memset(y, 0, sizeof(T)*this->Nrows);
#pragma omp parallel for
    for(int i = 0; i < this->Nrows; ++i)
        for(int k = this->rows->at(i); k < this->rows->at(i+1)-1; ++k)
            y[i] += this->values->at(k)*vec[this->cols->at(k)];
    return y;
}

template<typename T> csr_matrix<T> csr_matrix<T>::operator+(const csr_matrix<T> &mat)
{
    csr_matrix<T> res(this->Nrows, this->Ncols);
    
    for(int i = 1; i < this->Nrows; ++i)
        for(int j = 1; j < this->Ncols; ++j)
            res(i, j, this->operator()(i, j));
    return res;
}

template<typename T> csr_matrix<T>::csr_matrix(const csr_matrix<T> &matrix)
{
    this->Nrows = matrix.Nrows;
    this->Ncols = matrix.Ncols;
    this->rows = new std::vector<int>(*(matrix.rows));
    if(matrix.values != nullptr)
    {
        this->cols = new std::vector<int>(*(matrix.cols));
        this->values = new std::vector<T>(*(matrix.values));
    }
}
template<typename T> csr_matrix<T>& csr_matrix<T>::operator=(const csr_matrix<T> &matrix)
{
    if( &matrix != this)
    {
        if(this->values != nullptr)
        {
            delete this->values;
            delete this->cols;
        }
        delete this->rows;
    }

    this->Nrows = matrix.Nrows;
    this->Ncols = matrix.Ncols;
    this->rows = new std::vector<int>(*(matrix.rows));
    if(matrix.values != nullptr)
    {
        this->cols = new std::vector<int>(*(matrix.cols));
        this->values = new std::vector<T>(*(matrix.values));
    }
    return *this;
}
template<typename T> csr_matrix<T>& csr_matrix<T>::operator=(const coo_matrix<T> &matrix)
{
    if(this->values != nullptr)
    {
        delete this->values;
        delete this->cols;
        delete this->rows;
        this->values = new std::vector<T>(matrix.values->size());
        this->cols = new std::vector<int>(matrix.index->size());
        this->rows = new std::vector<int>(matrix.Nrows);
    }
    this->Nrows = matrix.Nrows;
    this->Ncols = matrix.Ncols;
    int r, c;
    std::memset(&this->rows->at(0), 0, sizeof(this->rows->at(0))*this->rows->size());
    for(int i = 0; i < (int)matrix.values->size(); ++i)
    {
        r = matrix.index->at(i) / matrix.Ncols;
        c = matrix.index->at(i) % matrix.Ncols;
        this->operator()(r, c, matrix.values[i]);
    }
    return *this;
}
template<typename T> inline T csr_matrix<T>::operator()(const int &i , const int &j)
{
    int col = std::distance(this->cols->begin(), std::find(this->cols->begin() + this->rows->at(i), this->cols->begin() + this->rows->at(i+1), j));
    return (col < this->rows->at(i+1)) ? this->values->at(col) : T();
}
template<typename T> inline void csr_matrix<T>::operator()(const int &i, const int &j, const T &val)
{
    if(this->values == nullptr)
    {
        this->values = new std::vector<T>(1, val);
        this->cols = new std::vector<int>(1, j);
        for(int k = i+1; k <= this->Nrows; ++k)
            ++this->rows->at(k);
    }
    else
    {
        int col = std::distance(this->cols->begin(), std::find(this->cols->begin() + this->rows->at(i), this->cols->begin() + this->rows->at(i+1), j));
        if(col < this->rows->at(i+1))
        {
            if(std::fabs(val) > 1e-6)
                this->values->at(col) = val;
            else
            {
                for(int k = i+1; k <= this->Nrows; ++k)
                    --this->rows->at(k);
                this->cols->erase(this->cols->begin() + col);
                this->values->erase(this->values->begin() + col);
            }
        }
        else
        {
            if(std::fabs(val) > 1e-6)
            {
                for(int k = i+1; k <= this->Nrows; ++k)
                    ++this->rows->at(k);
                this->cols->insert(this->cols->begin() + (this->rows->at(i+1)-1), j);
                this->values->insert(this->values->begin() + (this->rows->at(i+1)-1), val);
            }
        }
    }
    return;
}

template<typename T> inline bool csr_matrix<T>::operator==(const csr_matrix<T> &m)
{
    return ((this->values == nullptr && m.values == nullptr)
                || (this->values != nullptr && m.values != nullptr && *(this->values) == *(m.values)))
            && ((this->cols == nullptr && m.cols == nullptr)
                || (this->cols != nullptr && m.cols != nullptr && *(this->cols) == *(m.cols)))
            && *(this->rows) == *(m.rows);
}
template<typename T> inline bool csr_matrix<T>::operator!=(const csr_matrix<T> &m)
{
    return !(*this == m);
}

template<typename T> csr_matrix<T> csr_matrix<T>::transpose()
{

}

template<typename T> void csr_matrix<T>::t()
{

}

template<typename T> template<typename F> inline void csr_matrix<T>::add(const F &a)
{
    std::transform(this->values->begin(), this->values->end(), this->values->begin(), [&a](const T &val){return val + a;});
    return;
}
template<typename T> template<typename F> inline void csr_matrix<T>::diff(const F &a)
{
    std::transform(this->values->begin(), this->values->end(), this->values->begin(), [&a](const T &val){return val - a;});
    return;
}
template<typename T> template<typename F> inline void csr_matrix<T>::mult(const F &a)
{
    std::transform(this->values->begin(), this->values->end(), this->values->begin(), [&a](const T &val){return val * a;});
    return;
}
template<typename T> template<typename F> inline void csr_matrix<T>::div(const F &a)
{
    std::transform(this->values->begin(), this->values->end(), this->values->begin(), [&a](const T &val){return val / a;});
    return;
}

