#include "fmath.hpp"
#include "fstat.hpp"

template<typename T> void transpose_scalar_block(T *A, T *B, const int &N, const int &M, const int &block_size); 
template<typename T> void transpose_scalar_block(T **A, T **B, const int &N, const int &M, const int &block_size);

template<typename T> void matmultleaf(int mf, int ml, int nf, int nl, int pf, int pl, T **A, T **B, T **C);
template<typename T> void matmultleaf(int mf, int ml, int nf, int nl, int pf, int pl, T *A, T *B, T *C);
template<typename T> void matmultrec(int mf, int ml, int nf, int nl, int pf, int pl, T *A, T *B, T *C);
template<typename T> void matmultrec(int mf, int ml, int nf, int nl, int pf, int pl, T **A, T **B, T **C);
template<bool par, typename T, parallel<par, T> = nullptr> T* matmul(T *A, T *B, const int &Na, const int &Ma, const int &Nb, const int &Mb);
template<bool par, typename T, serial<par, T> = nullptr> T* matmul(T *A, T *B, const int &Na, const int &Ma, const int &Nb, const int &Mb);

template<bool par, typename T, parallel<par, T> = nullptr> T* mat_vec(T *A, T *b, const int &N, const int &M);
template<bool par, typename T, parallel<par, T> = nullptr> T* mat_vec(T **A, T *b, const int &N, const int &M);
template<bool par, typename T, serial<par, T> = nullptr> T* mat_vec(T *A, T *b, const int &N, const int &M);
template<bool par, typename T, serial<par, T> = nullptr> T* mat_vec(T **A, T *b, const int &N, const int &M);

template<bool par, typename T, parallel<par, T> = nullptr> T* tranpose(T *A, const int &N, const int &M); 
template<bool par, typename T, parallel<par, T> = nullptr> T** tranpose(T **A, const int &N, const int &M); 
template<bool par, typename T, serial<par, T> = nullptr> T* tranpose(T *A, const int &N, const int &M); 
template<bool par, typename T, serial<par, T> = nullptr> T** tranpose(T **A, const int &N, const int &M); 


//======================= eigen functions ======================================================================
template<bool par, typename T, parallel<par, T> = nullptr> void power_method(T **A, T *eigenvec, const int &N, const int &M);
template<bool par, typename T, parallel<par, T> = nullptr> void power_method(T *A, T *eigenvec, const int &N, const int &M);
template<bool par, typename T, serial<par, T> = nullptr> void power_method(T **A, T *eigenvec, const int &N, const int &M);
template<bool par, typename T, serial<par, T> = nullptr> void power_method(T *A, T *eigenvec, const int &N, const int &M);

//======================= system solver ========================================================================
template<typename T> T* gauss_seidel(T *A, T *b, const int &N, const int &M);
template<typename T> T* gauss_seidel(T **A, T *b, const int &N, const int &M);
template<typename T> T* thomas(T *b, T *a, T *c, T *d, const int &bsize, const int &csize);
template<typename T> T* thomas(T *b, T *c, T *d, const int &diag);

//====================== decomposition =========================================================================
template<typename T> T** cholesky(T **A, const int &N);

//==============================================================================================================

template<typename T> void transpose_scalar_block(T *A, T *B, const int &N, const int &M, const int &block_size) 
{
    for(int i = 0; i < block_size; ++i) 
        for(int j=0; j < block_size; ++j) 
            B[j * N + i] = A[i * M + j];
    return;
}
template<typename T> void transpose_scalar_block(T **A, T **B, const int &N, const int &M, const int &block_size) 
{
    
    return;
}

template<typename T> void matmultleaf(int mf, int ml, int nf, int nl, int pf, int pl, T **A, T **B, T **C)    
/*  
  subroutine that uses the simple triple loop to multiply  
  a submatrix from A with a submatrix from B and store the  
  result in a submatrix of C.   
  (We could use a tiled version,for better performance)  
*/  
// mf, ml; /* first and last+1 i index */  
// nf, nl; /* first and last+1 j index */  
// pf, pl; /* first and last+1 k index */  
{
    for (int i = mf; i < ml; i++)   
        for (int j = nf; j < nl; j++)   
            for (int k = pf; k < pl; k++)   
                C[i][j] += A[i][k]*B[k][j];   
    return;
}  

template<typename T> void matrix<T>::matmultleaf(int mf, int ml, int nf, int nl, int pf, int pl, T *A, T *B, T *C)   
{ 
    /*  
      subroutine that uses the simple triple loop to multiply  
      a submatrix from A with a submatrix from B and store the  
      result in a submatrix of C.   
      (We could use a tiled version,for better performance)  
    */  
    // mf, ml; /* first and last+1 i index */  
    // nf, nl; /* first and last+1 j index */  
    // pf, pl; /* first and last+1 k index */  
    for (int i = mf; i < ml; ++i)   
        for (int j = nf; j < nl; ++j)   
            for (int k = pf; k < pl; ++k)   
                C[i*nl + j] += A[i*pl + k]*B[k*nl + j];   
}

template<typename T> void matmultrec(int mf, int ml, int nf, int nl, int pf, int pl, T **A, T **B, T **C)
/*    
  recursive subroutine to compute the product of two  
  submatrices of A and B and store the result in C  
*/  
// mf, ml; /* first and last+1 i index */  
// nf, nl; /* first and last+1 j index */  
// pf, pl; /* first and last+1 k index */  
  
#define GRAIN  32768 /* product size below which matmultleaf is used */  
{     
        if ((ml-mf)*(nl-nf)*(pl-pf) < GRAIN)   
            matmultleaf(mf, ml, nf, nl, pf, pl, A, B, C);   
        else {   
    #pragma omp task
    {
       // C00 += A00 * B00
            matmultrec(mf, mf+(ml-mf)/2, nf, nf+(nl-nf)/2, pf, pf+(pl-pf)/2, A, B, C);   
       // C00 += A01 * B10
            matmultrec(mf, mf+(ml-mf)/2, nf, nf+(nl-nf)/2, pf+(pl-pf)/2, pl, A, B, C);   
    }
    #pragma omp task
    {
       // C01 += A00 * B01
            matmultrec(mf, mf+(ml-mf)/2, nf+(nl-nf)/2, nl, pf, pf+(pl-pf)/2, A, B, C);   
       // C01 += A01 * B11
            matmultrec(mf, mf+(ml-mf)/2, nf+(nl-nf)/2, nl, pf+(pl-pf)/2, pl, A, B, C);   
    }
    #pragma omp task
    {
       // C10 += A10 * B00
            matmultrec(mf+(ml-mf)/2, ml, nf, nf+(nl-nf)/2, pf, pf+(pl-pf)/2, A, B, C);   
       // C10 += A11 * B10
            matmultrec(mf+(ml-mf)/2, ml, nf, nf+(nl-nf)/2, pf+(pl-pf)/2, pl, A, B, C);   
    }
    #pragma omp task
    {
       // C11 += A10 * B01
            matmultrec(mf+(ml-mf)/2, ml, nf+(nl-nf)/2, nl, pf, pf+(pl-pf)/2, A, B, C);   
       // C11 += A11 * B11
            matmultrec(mf+(ml-mf)/2, ml, nf+(nl-nf)/2, nl, pf+(pl-pf)/2, pl, A, B, C);   
    }
    #pragma omp taskwait
    }   
    return;
} 

template<typename T> void matrix<T>::matmultrec(int mf, int ml, int nf, int nl, int pf, int pl, T *A, T *B, T *C)
/*    
  recursive subroutine to compute the product of two  
  submatrices of A and B and store the result in C  
*/  
// mf, ml; /* first and last+1 i index */  
// nf, nl; /* first and last+1 j index */  
// pf, pl; /* first and last+1 k index */  
  
#define GRAIN  32768 /* product size below which matmultleaf is used */  
{  
    if ((ml-mf)*(nl-nf)*(pl-pf) < GRAIN)   
        matmultleaf(mf, ml, nf, nl, pf, pl, A, B, C);   
    else 
    {   
#pragma omp task
        {
       // C00 += A00 * B00
            matmultrec(mf, mf+(ml-mf)/2, nf, nf+(nl-nf)/2, pf, pf+(pl-pf)/2, A, B, C);   
       // C00 += A01 * B10
            matmultrec(mf, mf+(ml-mf)/2, nf, nf+(nl-nf)/2, pf+(pl-pf)/2, pl, A, B, C);   
        }
#pragma omp task
        {
       // C01 += A00 * B01
            matmultrec(mf, mf+(ml-mf)/2, nf+(nl-nf)/2, nl, pf, pf+(pl-pf)/2, A, B, C);   
       // C01 += A01 * B11
            matmultrec(mf, mf+(ml-mf)/2, nf+(nl-nf)/2, nl, pf+(pl-pf)/2, pl, A, B, C);   
        }
#pragma omp task
        {
       // C10 += A10 * B00
            matmultrec(mf+(ml-mf)/2, ml, nf, nf+(nl-nf)/2, pf, pf+(pl-pf)/2, A, B, C);   
       // C10 += A11 * B10
            matmultrec(mf+(ml-mf)/2, ml, nf, nf+(nl-nf)/2, pf+(pl-pf)/2, pl, A, B, C);   
        }
#pragma omp task
        {
       // C11 += A10 * B01
            matmultrec(mf+(ml-mf)/2, ml, nf+(nl-nf)/2, nl, pf, pf+(pl-pf)/2, A, B, C);   
       // C11 += A11 * B11
            matmultrec(mf+(ml-mf)/2, ml, nf+(nl-nf)/2, nl, pf+(pl-pf)/2, pl, A, B, C);   
        }
#pragma omp taskwait
        }  
    return; 
}   


template<bool par, typename T, parallel<par, T>> T** matmul(T **A, T **B, const int &Na, const int &Ma, const int &Nb, const int &Mb)
{   
    assert(Ma == Nb);
    T **C = new T*[Na];
    for(int i = 0; i < Na; ++i) C[i] = new T[Mb];
#pragma omp parallel 
    {
      #pragma omp for
        for (int i = 0; i < Na; i++)   
            for (int j = 0; j < Mb; j++)   
                C[i][j] = 0;   

    #pragma omp  single
        matmultrec(0, Na, 0, Mb, 0, Ma, A, B, C);   

    } // parallel region
    return C;
}  

template<bool par, typename T, serial<par, T>> T* matmul(T *A, T *B, const int &Na, const int &Ma, const int &Nb, const int &Mb)
{   
    assert(Ma == Nb);
    T *C = new T[Na*Mb];
    std::memset(C, 0, sizeof(T)*Na*Mb);
    for (int i = 0; i < Na; ++i)   
        for (int j = 0; j < Mb; ++j)
            for (int k = 0; k < Ma; ++k)   
                C[i * Mb + j] += A[i*Ma + k] * B[k * Mb + j];   
    return C;
}  

template<bool par, typename T, parallel<par, T> = nullptr> T* mat_vec(T *A, T *b, const int &N, const int &M)
{
    T *x = new T[N];
#pragma omp parallel for
    for(int i = 0; i < N; ++i)
        x[i] = std::inner_product(A[i] + i*M, A[i] + (i+1)*M, b, b + M, (T)0.);
    return x;
}
template<bool par, typename T, parallel<par, T> = nullptr> T* mat_vec(T **A, T *b, const int &N, const int &M)
{
    T *x = new T[N];
#pragma omp parallel for
    for(int i = 0; i < N; ++i)
        x[i] = std::inner_product(A[i], A[i] + M, b, b + M, (T)0.);
    return x;
}

template<bool par, typename T, serial<par, T> = nullptr> T* mat_vec(T *A, T *b, const int &N, const int &M)
{
    T *x = new T[N];
    for(int i = 0; i < N; ++i)
        x[i] = std::inner_product(A[i] + i*M, A[i] + (i+1)*M, b, b + M, (T)0.);
    return x;
}
template<bool par, typename T, serial<par, T> = nullptr> T* mat_vec(T **A, T *b, const int &N, const int &M)
{
    T *x = new T[N];
    for(int i = 0; i < N; ++i)
        x[i] = std::inner_product(A[i] + M, A[i] + M, b, b + M, (T)0.);
    return x;
}


template<bool par, typename T, parallel<par, T>> T* tranpose(T *A, const int &N, const int &M)
{
    T *B = new T[N*M];
    int block_size = gcd(N, M);
#pragma omp parallel for
    for(int i = 0; i < N; i += block_size) 
        for(int j = 0; j < M; j += block_size) 
            transpose_scalar_block<par, T, parallel<par,T>>(A[i*M + j], B[j*N + i], block_size);
    return B;
}
template<bool par, typename T, parallel<par, T>> T** tranpose(T **A, const int &N, const int &M)
{
    T **B = new T*[M];
    for(int i = 0; i < M; ++i) B[i] = new T[N];

    return B;
}

template<bool par, typename T, serial<par, T>> T* tranpose(T *A, const int &N, const int &M)
{
    T *B = new T[N*M];
    int block_size = gcd(N, M);
    for(int i = 0; i < N; i += block_size) 
        for(int j = 0; j < M; j += block_size) 
            transpose_scalar_block<par, T, parallel<par,T>>(A[i*M + j], B[j*N + i], block_size);
    return B;
}
template<bool par, typename T, serial<par, T>> T** tranpose(T **A, const int &N, const int &M)
{
    T **B = new T*[M];
    for(int i = 0; i < M; ++i) B[i] = new T[N];

    return B;
}

//==================================================================================================

template<bool par, typename T, parallel<par, T>> T power_method(T **A, T *eigenvec, const int &N, const int &M)
{
    T eigenval = (T)0., tmp = (T)0., norm;
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 engine{seed};
    std::uniform_real_distribution<T> r_dist{(T)0., (T)1.};
    std::generate(eigenvec, eigenvec + N, [&engine, r_dist](){return r_dist(engine);});

    while(std::fabs(eigenval - tmp) > 1e-5f)
    {
        eigenvec = mat_vec<par, T, parallel>(A, eigenvec);
        tmp = eigenval;
        eigenval = max<par, T, parallel>(eigenvec);
        std::transform(eigenvec, eigenvec + N, eigenvec, [&eigenval](const T &val){return val/eigenval;});
    }
    norm = std::inner_product(eigenvec, eigenvec + N, eigenvec, eigenvec + N, (T)0.);
    std::transform(eigenvec, eigenvec + N, eigenvec, [&norm](const T &val){return val/norm;});
    return eigenval;
}
template<bool par, typename T, serial<par, T>> T power_method(T **A, T *eigenvec, const int &N, const int &M)
{
    T eigenval = (T)0., tmp = (T)0., norm;
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 engine{seed};
    std::uniform_real_distribution<T> r_dist{(T)0., (T)1.};
    std::generate(eigenvec, eigenvec + N, [&engine, r_dist](){return r_dist(engine);});

    while(std::fabs(eigenval - tmp) > 1e-5f)
    {
        eigenvec = mat_vec<par, T, serial>(A, eigenvec);
        tmp = eigenval;
        eigenval = max<par, T, serial>(eigenvec);
        std::transform(eigenvec, eigenvec + N, eigenvec, [&eigenval](const T &val){return val/eigenval;});
    }
    norm = std::inner_product(eigenvec, eigenvec + N, eigenvec, eigenvec + N, (T)0.);
    std::transform(eigenvec, eigenvec + N, eigenvec, [&norm](const T &val){return val/norm;});
    return eigenval;
}
template<bool par, typename T, parallel<par, T>> T power_method(T *A, T *eigenvec, const int &N, const int &M)
{
    T eigenval = (T)0., tmp = (T)0., norm;
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 engine{seed};
    std::uniform_real_distribution<T> r_dist{(T)0., (T)1.};
    std::generate(eigenvec, eigenvec + N, [&engine, r_dist](){return r_dist(engine);});
    
    while(std::fabs(eigenval - tmp) > 1e-5f)
    {
        eigenvec = mat_vec<par, T, parallel>(A, eigenvec);
        tmp = eigenval;
        eigenval = max<par, T, parallel>(eigenvec);
        std::transform(eigenvec, eigenvec + N, eigenvec, [&eigenval](const T &val){return val/eigenval;});
    }
    norm = std::inner_product(eigenvec, eigenvec + N, eigenvec, eigenvec + N, (T)0.);
    std::transform(eigenvec, eigenvec + N, eigenvec, [&norm](const T &val){return val/norm;});
    return eigenval;
}
template<bool par, typename T, serial<par, T>> T power_method(T *A, T *eigenvec, const int &N, const int &M)
{
    T eigenval = (T)0., tmp = (T)0., norm;
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 engine{seed};
    std::uniform_real_distribution<T> r_dist{(T)0., (T)1.};
    std::generate(eigenvec, eigenvec + N, [&engine, r_dist](){return r_dist(engine);});
    
    while(std::fabs(eigenval - tmp) > 1e-5f)
    {
        eigenvec = mat_vec<par, T, serial>(A, eigenvec);
        tmp = eigenval;
        eigenval = max<par, T, serial>(eigenvec);
        std::transform(eigenvec, eigenvec + N, eigenvec, [&eigenval](const T &val){return val/eigenval;});
    }
    norm = std::inner_product(eigenvec, eigenvec + N, eigenvec, eigenvec + N, (T)0.);
    std::transform(eigenvec, eigenvec + N, eigenvec, [&norm](const T &val){return val/norm;});
    return eigenval;
}

//==================================================================================================

template<typename T> T* gauss_seidel(T *A, T *b, const int &N, const int &M)
{
    int cnt, ITERATION_LIMIT = 1e20;
    T *x_new = new T[M], *res = new T[M], diff;
    for(int it = 0; it < ITERATION_LIMIT; ++it)
    {
        cnt = 0;
        for(int i = 0; i < A.Nrows; ++i)
        {
            diff = std::inner_product(A[i * M], A[i * M + i], x_new, (T)0.0) + std::inner_product(A[i * M + i + 1], A[(i+1) * M], x_new + i + 1, (T)0.0);
            x_new[i] = (b[i] - diff) / A[i * M + i];
            cnt += (std::fabs(res[i] - x_new[i]) > 1e-8) ? 1 : 0;
        }
        if (!cnt)
            break;
        std::memcpy(res, x_new, sizeof(T)*M);
    }
    delete[] x_new;
    return res;
}

template<typename T> T* gauss_seidel(T **A, T *b, const int &N, const int &M)
{
    int cnt, ITERATION_LIMIT = 1e20;
    T *x_new = new T[M], *res = new T[M], diff;
    for(int it = 0; it < ITERATION_LIMIT; ++it)
    {
        cnt = 0;
        for(int i = 0; i < A.Nrows; ++i)
        {
            diff = std::inner_product(A[i], A[i] + i, x_new, (T)0.0) + std::inner_product(A[i] + i + 1, A[i] + M, x_new + i + 1, (T)0.0);
            x_new[i] = (b[i] - diff) / A[i][i];
            cnt += (std::fabs(res[i] - x_new[i]) > 1e-8) ? 1 : 0;
        }
        if (!cnt)
            break;
        std::memcpy(res, x_new, sizeof(T)*M);
    }
    delete[] x_new;
    return res;
}

template<typename T> T* thomas(T *b, T *c, T *d, const int &bsize, const int &csize)
{
    T *x = new T[bsize], a = c[0], id, c_star = c[0], d_start = d[0];
    c[0] /= b[0];
    d[0] /= b[0];
    int i;
    for(i = 1; i < csize; ++i)
    {
        id = (T)1. / (b[i] - a * c_star);
        d[i] = (d[i] - a * d_start) * id;
        a = c[i];
        c[i] *= id;
        c_star = c[i];
        d_start = d[i];
    }
    d[i] = (d[i] - a*d_star) / (b[i] - a*c_star);
    x[bsize-1] = d[i];
    for (i = x.size() - 2; i != -1; --i)
        x[i] = d[i] - c[i] * x[i + 1];
    return x;
}

template<typename T> T* thomas(T *b, T *a, T *c, T *d, const int &bsize, const int &csize)
{
    T *x = new T[bsize], id, c_star = c[0], d_start = d[0];
    int i;
    c[0] /= b[0];
    d[0] /= b[0];
    for(i = 1; i < csize; ++i)
    {
        id = (T)1. / (b[i] - a[i - 1] * c_star);
        d[i] = (d[i] - a[i - 1] * d_star) * id;
        c[i] *= id;
        c_star = c[i];
        d_star = d[i];
    }
    d[i] = (d[i] - a[i - 1] * d_star) / (b[i] - a[i - 1] * c_star);
    x[bsize - 1] = d[i];
    for (i = x.size() - 2; i != -1; --i)
        x[i] = d[i] - c[i] * x[i + 1];
    return x;
}

//=========================================================================================================

template<typename T> T** cholesky(T **A, const int &N)
{
    T **L = new T*[N], s;
    std::generate(L, L + N, [&N]{return new T[N];});
    for(int i = 0; i < N; ++i)
        for(int j = 0; j < i+1; ++j)
        {
            s = std::inner_product(L[i], L[i] + j, L[j], (T)0.);
            L[i][j] = (i == j) ? sqrt(A[i][i] - s) : ((T)1. / L[j][j] * A[i][j] - s);
        }
    return L;
}
