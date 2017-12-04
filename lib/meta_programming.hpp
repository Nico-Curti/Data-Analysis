#pragma once
#include <map>

// factorial of a number 
template< int i > struct FACTORIAL
{	enum {RESULT = i * FACTORIAL<i-1>::RESULT};	};
template<> struct FACTORIAL< 1 >
{	enum {RESULT = 1}; };

// integer power of a number
template< int P, typename T> struct POW
{	static T result(const T &base){return base * POW<P-1, T>::result(base);	}};
template< typename T> struct POW<1, T>
{	static T result(const T &base){return base;	}};
template< int P, typename T> inline T Pow(const T &base )
{	return POW<P, T>::result(base);	}

// sum of range (0, i)
template< int i > struct SUM
{	enum {RESULT = i + SUM<i-1>::RESULT};	};
template<> struct SUM< 0 >
{	enum {RESULT = 0};	};

// mean of array
template< int DIM, typename T> struct MEAN
{	static T result(T *elm){return *elm + MEAN<DIM-1, T>::result(elm+1);}};
template< typename T> struct MEAN<1, T>
{	static T result(T *elm){return *elm;}};
template< int DIM, typename T> inline T Mean(T *v)
{	return MEAN<DIM, T>::result(v) / DIM;}

// greatest common divider
template< int X, int Y> struct GCD 
{	enum {RESULT = GCD<Y, X % Y>::RESULT};	};
template< int X > struct GCD<X, 0>
{	enum {RESULT = X};	};

// greatest divider of a number
template<int N, int i, bool res> struct GD
{ enum {RESULT = GD<N, i+1, (bool)(N % (i+1))>::RESULT};};
template<int N, int i> struct GD<N, i, false>
{ enum {RESULT = N / i};};
template<int N> inline int gd()
{ return GD<N, 1, true>::RESULT; }

// count element equal to val
template< int DIM, typename T> struct COUNT
{	static int result(T *elm, const T &val){return ((*elm == val) ? 1 : 0) + COUNT<DIM-1, T>::result(elm + 1, val);	}};
template< typename T> struct COUNT<0, T>
{	static int result(T *elm, const T &val){return ((*elm == val) ? 1 : 0);}};
template< int DIM, typename T> inline int count_obj(T *a, const T &val)
{	return COUNT<DIM, T>::result(a, val);	}

// inner product
template<int DIM, typename T> struct DOT
{ static T result(T *a, T *b){return *a * *b + DOT<DIM-1, T>::result(a+1, b+1);}};
template<typename T> struct DOT<1, T>
{ static T result(T *a, T *b){return *a * *b;}};
template<int DIM, typename T> inline T dot(T *a, T *b)
{ return DOT<DIM, T>::result(a, b);}

// print vector
template<int DIM, typename T> struct PRINT
{ static void result(T *v){std::cout << *v << " "; PRINT<DIM-1, T>::result(v+1); return;}};
template<typename T> struct PRINT<0, T>
{ static void result(T *v){std::cout << std::endl; return;}};
template<int DIM, typename T> void print(T *v)
{	return PRINT<DIM, T>::result(v);}

// variance of array
template<int DIM, typename T> struct VARIANCE
{ static T result(T *v, const T &mean){return power<2>(*v - mean) + VARIANCE<DIM-1, T>::result(v+1, mean);}};
template<typename T> struct VARIANCE<1, T>
{ static T result(T *v, const T &mean){return power<2>(*v - mean);}};
template<int DIM, typename T> inline T variance(T *v)
{ return VARIANCE<DIM, T>::result(v, MEAN<DIM, T>::result(v) / DIM) /DIM;}

// add element to each value array
template<int BEG, int END, typename T> struct ADD
{ static void result(T *v, T val){*v += val; ADD<BEG+1, END, T>::result(v+1, val);}};
template<int END, typename T> struct ADD<END, END, T>
{ static void result(T *v, T val){return;}};
template<int BEG, int END, typename T> inline void add(T *a, T b)
{ return ADD<BEG, END, T>::result(a + BEG, b);}

// subtract element to each value array
template<int BEG, int END, typename T> struct SUB
{ static void result(T *v, T val){*v -= val; SUB<BEG+1, END, T>::result(v+1, val);}};
template<int END, typename T> struct SUB<END, END, T>
{ static void result(T *v, T val){return;}};
template<int BEG, int END, typename T> inline void sott(T *a, T b)
{ return SUB<BEG, END, T>::result(a + BEG, b);}

// multiply element to each value array
template<int BEG, int END, typename T> struct MULT
{ static void result(T *v, T val){*v *= val; MULT<BEG+1, END, T>::result(v+1, val);}};
template<int END, typename T> struct MULT<END, END, T>
{ static void result(T *v, T val){return;}};
template<int BEG, int END, typename T> inline void mult(T *a, T b)
{ return MULT<BEG, END, T>::result(a + BEG, b);}

// divide element to each value array
template<int BEG, int END, typename T> struct DIV
{ static void result(T *v, T val){*v /= val; DIV<BEG+1, END, T>::result(v+1, val);}};
template<int END, typename T> struct DIV<END, END, T>
{ static void result(T *v, T val){return;}};
template<int BEG, int END, typename T> inline void div(T *a, T b)
{ return DIV<BEG, END, T>::result(a + BEG, b);}

// find value and index of element in array
template<int BEG, int END, typename T> struct FIND_IDX
{ static int result(T *v, T val){ return (*v == val) ? BEG : FIND_IDX<BEG+1, END, T>::result(v+1, val);}};
template<int END, typename T> struct FIND_IDX<END, END, T>
{ static int result(T *v, T val){ return END;}};
template<int BEG, int END, typename T> struct FIND_VAL
{ static bool result(T *v, T val){ return (*v == val) ? true : FIND_VAL<BEG+1, END, T>::result(v+1, val);}};
template<int END, typename T> struct FIND_VAL<END, END, T>
{ static bool result(T *v, T val){ return false;}};
template<int BEG, int END, typename T> inline int find_index(T *a, T b)
{ return FIND_IDX<BEG, END, T>::result(a + BEG, b);}
template<int BEG, int END, typename T> inline bool find_value(T *a, T b)
{ return FIND_VAL<BEG, END, T>::result(a + BEG, b);}

// fill an array with equal values
template<int BEG, int END, typename T> struct FILL
{ static void result(T *v, T val){ *v = val; FILL<BEG+1, END, T>::result; return;}};
template<int END, typename T> struct FILL<END, END, T>
{ static void result(T *v, T val){ *v = val; return;}};
template<int BEG, int END, typename T> inline void fill(T *a, const T &b)
{ return FILL<BEG, END, T>::result(a, b);}
template<int BEG, int END, typename T> inline void zeros(T *a)
{ return FILL<BEG, END, T>::result(a, (T)0.);}
template<int DIM, typename T> inline void fill(T *a, const T&b)
{ return FILL<0, DIM, T>::result(a, b);}
template<int DIM, typename T> inline void zeros(T *a)
{ return FILL<0, DIM, T>::result(a, (T)0.);}

// print a matrix
template<int N, int M, int i, int j, typename T> struct PRINT_MAT
{ static void result(T** a){ std::cout << *(*(a + i) + j) << " "; PRINT_MAT<N, M, i, j+1, T>::result(a);return;}};
template<int N, int M, int i, typename T> struct PRINT_MAT<N, M, i, M, T>
{ static void result(T** a){ std::cout << *(*(a + i) + M) << std::endl; PRINT_MAT<N, M, i+1, 0, T>::result(a);return;}};
template<int N, int M, int j, typename T> struct PRINT_MAT<N, M, N, j, T>
{ static void result(T** a){ std::cout << *(*(a + N) + j) << " "; PRINT_MAT<N, M, N, j+1, T>::result(a);return;}};
template<int N, int M, typename T> struct PRINT_MAT<N, M, N, M, T>
{ static void result(T** a){std::cout << *(*(a + N) + M) << std::endl; return;}};
template<int N, int M, typename T> inline void print(T** a)
{ return PRINT_MAT<N-1, M-1, 0, 0, T>::result(a);}

// matrix-vector product
template<int N, int M, int i, int j, typename T> struct MAT_VEC
{ static void result(T **a, T *x, T *b){ *(b + i) += *(*(a + i) + j) * *(x + j); MAT_VEC<N, M, i, j + 1, T>::result(a, x, b); return;}};
template<int N, int M, int i, typename T> struct MAT_VEC<N, M, i, M, T>
{ static void result(T **a, T *x, T *b){ *(b + i) += *(*(a + i) + M) * *(x + M); MAT_VEC<N, M, i + 1, 0, T>::result(a, x, b); return;}};
template<int N, int M, int j, typename T> struct MAT_VEC<N, M, N, j, T>
{ static void result(T **a, T *x, T *b){ *(b + N) += *(*(a + N) + j) * *(x + j); MAT_VEC<N, M, N, j + 1, T>::result(a, x, b); return;}};
template<int N, int M, typename T> struct MAT_VEC<N, M, N, M, T>
{ static void result(T **a, T *x, T *b){ *(b + N) += *(*(a + N) + M) * *(x + M); return;}};
template<int N, int M, typename T> inline T* mat_vec(T **a, T *x)
{
	T *b = new T[N];
	zeros<N>(b);
	MAT_VEC<N-1, M-1, 0, 0, T>::result(a, x, b);
	return b; 
}

// vector-matrix product
template<int N, int M, int i, int j, typename T> struct VEC_MAT
{ static void result(T **a, T *x, T *b){ *(b + j) += *(*(a + i) + j) * *(x + i); VEC_MAT<N, M, i, j + 1, T>::result(a, x, b); return;}};
template<int N, int M, int i, typename T> struct VEC_MAT<N, M, i, M, T>
{ static void result(T **a, T *x, T *b){ *(b + M) += *(*(a + i) + M) * *(x + i); VEC_MAT<N, M, i + 1, 0, T>::result(a, x, b); return;}};
template<int N, int M, int j, typename T> struct VEC_MAT<N, M, N, j, T>
{ static void result(T **a, T *x, T *b){ *(b + j) += *(*(a + N) + j) * *(x + N); VEC_MAT<N, M, N, j + 1, T>::result(a, x, b); return;}};
template<int N, int M, typename T> struct VEC_MAT<N, M, N, M, T>
{ static void result(T **a, T *x, T *b){ *(b + M) += *(*(a + N) + M) * *(x + N); return;}};
template<int N, int M, typename T> inline T* vec_mat(T *x, T **a)
{
	T *b = new T[N];
	zeros<N>(b);
	VEC_MAT<N-1, M-1, 0, 0, T>::result(a, x, b);
	return b; 
}

// memset 0 to matrix
template<int N, int M, int i, typename T> struct INIT
{ static void result(T **a){a[i] = new T[M]; INIT<N, M, i+1, T>::result(a); return;}};
template<int N, int M, typename T> struct INIT<N, M, N, T>
{ static void result(T **a){a[N] = new T[M]; return;}};
template<int N, int M, typename T> inline T** init()
{ T **a = new T*[N]; INIT<N-1, M, 0, T>::result(a); return a;}

// matrix-matrix product
template<int N, int M, int K, typename T> struct MAT_MAT
{ static void result(T **A, T **B, T **C){VEC_MAT<M-1, K-1, 0, 0, T>::result(B, *(A + N-1), *(C + N-1)); MAT_MAT<N-1, M, K, T>::result(A, B, C); return;}};
template<int M, int K, typename T> struct MAT_MAT<0, M, K, T> 
{ static void result(T **A, T **B, T **C){return;}};
template<int N, int M, int K, typename T> inline T** dot(T **A, T **B)
{
	T **C = init<N, K, T>();
	for(int i = 0; i < N; ++i) zeros<K>(C[i]);
	MAT_MAT<N, M, K, T>::result(A, B, C);
	return C;
}

// nested map
template<int N, typename Key, typename Value> struct MAP 
{
    typedef std::map<Key, typename MAP<N - 1, Key, Value>::type> type;
    void print();
};
template<typename Key, typename Value> struct MAP<1, Key, Value> {
    typedef std::map<Key, Value> type;
};




