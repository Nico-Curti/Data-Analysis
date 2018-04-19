
template<typename T> void print(const sbcsr_matrix<T> &matrix)
{
    T element;
    for(int i = 0; i < matrix.Nrows; ++i)
        for(int j = 0; j < matrix.Ncols; ++j)
            if((element = matrix(i, j)))
                std::cout << "[" << i << ", " << j << "] " << element << std::endl;
    return;
}
template<typename T> void save_gephi(const sbcsr_matrix<T> &matrix, std::string *lbl, std::string filename)
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


template<typename T> sbcsr_matrix<T>::sbcsr_matrix()
{
    this->Nrows = 0;
    this->Ncols = 0;
    this->blc1 = 0;
    this->blc2 = 0;
    this->values = nullptr;
    this->index = nullptr;
    this->block_ptr = nullptr;
    this->bcol_ind = nullptr;
    this->brow_ptr = new std::vector<int>(1,0);
    this->perm = nullptr;
}
template<typename T> sbcsr_matrix<T>::sbcsr_matrix(const int &n)
{
    this->Nrows = n;
    this->Ncols = n;
    this->blc1 = gd(n);
    this->blc2 = this->blc1;
    this->values = nullptr;
    this->index = nullptr;
    this->block_ptr = new std::vector<int>(1, 0);
    this->bcol_ind = nullptr;
    this->brow_ptr = new std::vector<int>(this->Nrows/this->blc1 + 1);
    std::memset(&this->brow_ptr->at(0), 0, sizeof(this->brow_ptr->at(0)) * this->brow_ptr->size());
    this->perm = nullptr;
}
template<typename T> sbcsr_matrix<T>::sbcsr_matrix(const int &n, const int &m)
{
    this->Nrows = n;
    this->Ncols = m;
    this->blc1 = gd(n);
    this->blc2 = (this->Nrows != this->Ncols) ? gd(m) : this->blc1;
    this->values = nullptr;
    this->index = nullptr;
    this->block_ptr = new std::vector<int>(1, 0);
    this->bcol_ind = nullptr;
    this->brow_ptr = new std::vector<int>(this->Nrows/this->blc1 + 1);
    std::memset(&this->brow_ptr->at(0), 0, sizeof(this->brow_ptr->at(0)) * this->brow_ptr->size());
    this->perm = nullptr;
}
template<typename T> sbcsr_matrix<T>::sbcsr_matrix(T *v, int *irow, int *jcol, const int &nval, const int &n, const int &m)
{
    this->Nrows = n;
    this->Ncols = m;
    this->blc1 = gd(n);
    this->blc2 = (this->Nrows != this->Ncols) ? gd(m) : this->blc1;
    this->values = nullptr;
    this->index = nullptr;
    this->brow_ptr = new std::vector<int>(this->Nrows/this->blc1 + 1);
    std::memset(&this->brow_ptr->at(0), 0, sizeof(this->brow_ptr->at(0)) * this->brow_ptr->size());
    this->block_ptr = new std::vector<int>(1, 0);
    this->bcol_ind = nullptr;
    this->perm = nullptr;//BFS(irow, jcol, nval, n);

    for(int i = 0; i < nval; ++i)
        this->operator()(irow[i], jcol[i], v[i]);
}
template<typename T> sbcsr_matrix<T>::sbcsr_matrix(const coo_matrix<T> &mat)
{
    this->Nrows = mat.Nrows;
    this->Ncols = mat.Ncols;
    this->blc1 = gd(mat.Nrows);
    this->blc2 = (this->Nrows != this->Ncols) ? gd(mat.Ncols) : this->blc1;
    this->values = nullptr;
    this->index = nullptr;
    this->brow_ptr = new std::vector<int>(this->Nrows/this->blc1 + 1);
    std::memset(&this->brow_ptr->at(0), 0, sizeof(this->brow_ptr->at(0)) * this->brow_ptr->size());
    this->block_ptr = new std::vector<int>(1, 0);
    this->bcol_ind = nullptr;
    this->perm = nullptr;//BFS(mat.index, mat.index->size(), mat.Nrows, mat.Ncols);

    int irow, jcol;
    for(int i = 0; i < (int)mat.values->size(); ++i)
    {
        irow = mat.index[i] / mat.Ncols;
        jcol = mat.index[i] % mat.Ncols;
        this->operator()(irow, jcol, mat.values[i]);
    }
}
template<typename T> sbcsr_matrix<T>::~sbcsr_matrix(void)
{
    if(this->values != nullptr)
    {
        delete values;
        delete index;
        delete block_ptr;
        delete bcol_ind;
        delete[] perm;
    }
    delete brow_ptr;
}
template<typename T> sbcsr_matrix<T>::sbcsr_matrix(const sbcsr_matrix<T> &matrix)
{
    this->Nrows = matrix.Nrows;
    this->Ncols = matrix.Ncols;
    this->block_ptr = new std::vector<int>(*(matrix.block_ptr));
    this->brow_ptr = new std::vector<int>(*(matrix.brow_ptr));
    if(matrix.values != nullptr)
    {
        this->bcol_ind = new std::vector<int>(*(matrix.bcol_ind));
        this->index = new std::vector<int>(*(matrix.index));
        this->values = new std::vector<T>(*(matrix.values));
        if(matrix.perm != nullptr)
        {
            this->perm = new int[this->Nrows];
            std::memcpy(this->perm, matrix.perm, sizeof(int)*this->Nrows);
        }
        else
            this->perm = nullptr;
    }
}
template<typename T> sbcsr_matrix<T>& sbcsr_matrix<T>::operator=(const sbcsr_matrix<T> &matrix)
{
    if( &matrix != this)
    {
        if(this->values != nullptr)
        {
            delete this->values;
            delete this->index;
            delete this->bcol_ind;
            delete[] this->perm;
        }
        delete this->bcol_ind;
        delete this->brow_ptr;
    }
    this->Nrows = matrix.Nrows;
    this->Ncols = matrix.Ncols;
    this->block_ptr = new std::vector<int>(*(matrix.block_ptr));
    this->brow_ptr = new std::vector<int>(*(matrix.brow_ptr));
    if(matrix.values != nullptr)
    {
        this->bcol_ind = new std::vector<int>(*(matrix.bcol_ind));
        this->index = new std::vector<int>(*(matrix.index));
        this->values = new std::vector<T>(*(matrix.values));
        if(matrix.perm != nullptr)
        {
            this->perm = new int[this->Nrows];
            std::memcpy(this->perm, matrix.perm, sizeof(int)*this->Nrows);
        }
        else
            this->perm = nullptr;
    }
    return *this;
}


template<typename T> inline T sbcsr_matrix<T>::operator()(const int &i, const int &j)
{
    // WRONG!!
    int idx = i / this->blc1 + 1;
    int pos = std::distance(this->index->begin(), std::find(this->index->begin() + this->block_ptr->at(this->brow_ptr->at(idx-1)), this->index->begin() + this->block_ptr->at(this->brow_ptr->at(idx)), (i % this->blc1) * this->blc2 + j % this->blc2));
    if(pos < this->block_ptr->at(this->brow_ptr->at(idx)))
        return this->values->at(pos);
    else
        return T();
}
template<typename T> inline void sbcsr_matrix<T>::operator()(const int &irow, const int &jcol, const T &val)
{
    if(this->values == nullptr)
    {
        this->values = new std::vector<T>(1);
        this->index = new std::vector<int>(1);
        int idx = irow / this->blc1 + 1, col, pos;
        this->values->at(0) = val;
        this->index->at(0) = (irow % this->blc1) * this->blc2 + jcol % this->blc2;
        this->bcol_ind = new std::vector<int>(1, jcol / this->blc2);
        this->block_ptr->insert(this->block_ptr->begin() + 1, 1);
        std::transform(this->brow_ptr->begin() + idx, this->brow_ptr->end(), this->brow_ptr->begin() + idx, [](int &i){return ++i;});
    }
    else
    {
        int idx = irow / this->blc1 + 1;
        int col = std::distance(this->bcol_ind->begin(), std::find(this->bcol_ind->begin() + this->brow_ptr->at(idx-1), this->bcol_ind->begin() + this->brow_ptr->at(idx), jcol/this->blc2));
        int pos;

        if(col < this->brow_ptr->at(idx))
        {
            if(std::fabs(val) > 1e-6)
            {

                col = 0;
                for(int k = this->brow_ptr->at(idx-1); k < this->brow_ptr->at(idx); ++k)
                    if (this->bcol_ind->at(k) < (jcol / this->blc2))
                        ++col;
                    else
                        break;
                pos = 0;
                for(int k = this->block_ptr->at(this->brow_ptr->at(idx-1)  + col); k < this->block_ptr->at(this->brow_ptr->at(idx-1) + col + 1); ++k)
                    if (this->index->at(k) < ((irow % this->blc1) * this->blc2 + jcol % this->blc2))
                        ++pos;
                    else
                        break;
                for(int k = this->brow_ptr->at(idx-1) + col + 1; k < (int)this->block_ptr->size(); ++k)
                    ++this->block_ptr->at(k);
                this->values->insert(this->values->begin() + this->block_ptr->at(this->brow_ptr->at(idx-1)) + pos, val);
                this->index->insert(this->index->begin() + this->block_ptr->at(this->brow_ptr->at(idx-1)) + pos, (irow % this->blc1) * this->blc2 + jcol % this->blc2);
            }
            /*else
            {
                for(int k = idx; k < (int)this->brow_ptr->size(); ++k)
                    --this->brow_ptr->at(k);
                this->bcol_ind->erase(this->bcol_ind->begin() + this->block_ptr->at(col+1));
                this->block_ptr->erase(this->block_ptr->begin() + this->block_ptr->at(col+1));
                for(int k = col+1; k < (int)this->block_ptr->size(); ++k)
                    --this->block_ptr->at(k);
                this->values->erase(this->values->begin() + this->block_ptr->at(col+1));
                this->index->erase(this->index->begin() + this->block_ptr->at(col+1)); 
            }*/
        }
        else
        {
            if(std::fabs(val) > 1e-6)
            {
                col = 0;
                for(int k = this->brow_ptr->at(idx-1); k < this->brow_ptr->at(idx); ++k)
                    if (this->bcol_ind->at(k) < jcol/this->blc2)
                        ++col;
                    else
                        break;
                
                for(int k = idx; k < (int)this->brow_ptr->size(); ++k)
                    ++this->brow_ptr->at(k);
                
                (this->brow_ptr->at(idx-1) + col + 1 >= this->block_ptr->size()) ? this->block_ptr->push_back(this->block_ptr->at(this->block_ptr->size()-1)) : this->block_ptr->insert(this->block_ptr->begin() + this->brow_ptr->at(idx-1) + col + 1, this->block_ptr->at(this->brow_ptr->at(idx-1) + col));

                this->bcol_ind->insert(this->bcol_ind->begin() + this->brow_ptr->at(idx-1) + col, jcol / this->blc2);

                for(int k = this->brow_ptr->at(idx-1) + col + 1; k < (int)this->block_ptr->size(); ++k)
                    ++this->block_ptr->at(k);

                this->values->insert(this->values->begin() + this->block_ptr->at(this->brow_ptr->at(idx-1)+col), val);
                this->index->insert(this->index->begin() + this->block_ptr->at(this->brow_ptr->at(idx-1)+col), (irow % this->blc1) * this->blc2 + jcol % this->blc2);
            }
        }
    }
    return;
}

template<typename T> inline bool sbcsr_matrix<T>::operator==(const sbcsr_matrix<T> &m)
{
    return ((this->values == nullptr && m.values == nullptr)
                || (this->values != nullptr && m.values != nullptr && *(this->values) == *(m.values)))
            && ((this->index == nullptr && m.index == nullptr)
                || (this->index != nullptr && m.index != nullptr && *(this->index) == *(m.index)))
            && ((this->bcol_ind == nullptr && m.bcol_ind == nullptr)
                || (this->bcol_ind != nullptr && m.bcol_ind != nullptr && *(this->bcol_ind) == *(m.bcol_ind)))
            && *(this->block_ptr) == *(m.block_ptr)
            && *(this->brow_ptr) == *(m.brow_ptr);
}
template<typename T> inline bool sbcsr_matrix<T>::operator!=(const sbcsr_matrix<T> &m)
{
    return !(this == m);
}

template<typename T>template<typename F> T* sbcsr_matrix<T>::operator*(F *vec)
{
    T *y = new T[this->Nrows];
#pragma omp parallel for
    for(int b = 0; b < this->brow_ptr->at(this->brow_ptr->size()-1); ++b)
    {
        y[b] = (T)0.;
        for(int j = this->brow_ptr->at(b); j < this->brow_ptr->at(b+1)-1; ++j)
            for(int z = this->block_ptr->at(j); z < this->block_ptr->at(j+1)-1; ++z)
                y[this->blc1*b + this->index->at(z)/this->Ncols -1] += this->values->at(z)*vec[this->blc1*(this->bcol_ind->at(j)-1) + this->index->at(z)%this->Ncols -1];
    }
    return y;
}

template<typename T> template<typename F> inline void sbcsr_matrix<T>::add(const F &a)
{
    std::transform(this->values->begin(), this->values->end(), this->values->begin(), [&a](const T &val){return val + a;});
    return;
}
template<typename T> template<typename F> inline void sbcsr_matrix<T>::diff(const F &a)
{
    std::transform(this->values->begin(), this->values->end(), this->values->begin(), [&a](const T &val){return val - a;});
    return;
}
template<typename T> template<typename F> inline void sbcsr_matrix<T>::mult(const F &a)
{
    std::transform(this->values->begin(), this->values->end(), this->values->begin(), [&a](const T &val){return val * a;});
    return;
}
template<typename T> template<typename F> inline void sbcsr_matrix<T>::div(const F &a)
{
    std::transform(this->values->begin(), this->values->end(), this->values->begin(), [&a](const T &val){return val / a;});
    return;
}