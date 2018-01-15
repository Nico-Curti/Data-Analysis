#pragma once
#include "fmath.hpp"
#include <cstring>
#include <vector>
#include <algorithm>
#include <fstream>

template<typename T> class coo_matrix;
template<typename T> class csr_matrix;
template<typename T> class sbcsr_matrix;

template<typename T> std::ostream& operator<<(std::ostream &, coo_matrix<T> &);
template<typename T> std::ostream& operator<<(std::ostream &, csr_matrix<T> &);
template<typename T> std::ostream& operator<<(std::ostream &, sbcsr_matrix<T> &);

template<typename T> class coo_matrix
{
public:
	using type = T;
	std::vector<T> * values;
	int Nrows, Ncols;
	std::vector<int> *index;
	coo_matrix();
	coo_matrix(const int &);
	coo_matrix(const int &, const int &);
	coo_matrix(const int &, const int &, int *, int *, T *, const int &);
	coo_matrix(const csr_matrix<T> &);
	coo_matrix(const coo_matrix &);
	~coo_matrix(void);
	coo_matrix<T>& operator=(const coo_matrix &);
	coo_matrix<T>& operator=(const csr_matrix<T> &);

	inline T operator()(const int &, const int &);
	inline void operator()(const int &i, const int &, const T &);

	inline bool operator==(const coo_matrix &);
	inline bool operator!=(const coo_matrix &);

	inline coo_matrix<T> transpose();
	inline void t();

	// operations with scalars
	template<typename F> inline void add(const F &);
	template<typename F> inline void diff(const F &);
	template<typename F> inline void mult(const F &);
	template<typename F> inline void div(const F &);
};

template<typename T> inline void print(const coo_matrix<T> &);
template<typename T> void save_gephi(const coo_matrix<T> &, std::string *, std::string filename = "net");

template<typename T> class csr_matrix
{
public:
	using type = T;

	int Nrows, Ncols;
	std::vector<T> *values;
	std::vector<int> *rows, *cols;

	csr_matrix();
	csr_matrix(const int &);
	csr_matrix(const int &, const int &);
	csr_matrix(const coo_matrix<T> &);
	csr_matrix(const int &, const int &, T *, int *, int *, const int &);
	~csr_matrix(void);

	csr_matrix(const csr_matrix &);
	csr_matrix<T>& operator=(const csr_matrix &);
	csr_matrix<T>& operator=(const coo_matrix<T> &);

	
	csr_matrix<T> operator*(const csr_matrix<T> &);
	template<typename F> csr_matrix<T> operator*(const F &); // TO DO
	template<typename F> T* operator*(F *);

	csr_matrix<T> operator+(const csr_matrix<T> &);
	template<typename F>csr_matrix<T> operator+(const F &);
	csr_matrix<T> operator-(const csr_matrix<T> &);
	template<typename F>csr_matrix<T> operator-(const F &);
	
	bool operator==(const csr_matrix &);
	bool operator!=(const csr_matrix &);
	T operator()(const int &, const int &);
	void operator()(const int &, const int &, const T &); // TO FIX

	csr_matrix<T> transpose(); // TO DO
	void t(); // TO DO
	
	// operations with scalars
	template<typename F> void add(const F &);
	template<typename F> void diff(const F &);
	template<typename F> void mult(const F &);
	template<typename F> void div(const F &);
};

template<typename T> inline void print(const csr_matrix<T> &);
template<typename T> void save_gephi(const csr_matrix<T> &, std::string *, std::string filename = "net");


template<typename T> class sbcsr_matrix
{
public:
	using type = T;

	int Nrows, Ncols, blc1, blc2;
	std::vector<T> *values;
	std::vector<int> *index, *block_ptr, *bcol_ind, *brow_ptr;
	int *perm;

	sbcsr_matrix();
	sbcsr_matrix(const int &);
	sbcsr_matrix(const int &, const int &);
	sbcsr_matrix(T *, int *, int *, const int &, const int &, const int &);
	sbcsr_matrix(const coo_matrix<T> &);
	~sbcsr_matrix(void);

	sbcsr_matrix(const sbcsr_matrix &);
	sbcsr_matrix<T>& operator=(const sbcsr_matrix &);

	inline T operator()(const int &, const int &); // TO DO
	inline void operator()(const int &, const int &, const T &); // TO DO
	inline bool operator==(const sbcsr_matrix &);
	inline bool operator!=(const sbcsr_matrix &);

	// operations with vectors
	template<typename F> T* operator*(F *);

	// operations with scalars
	template<typename F> inline void add(const F &);
	template<typename F> inline void diff(const F &);
	template<typename F> inline void mult(const F &);
	template<typename F> inline void div(const F &);	
};

template<typename T> inline void print(const sbcsr_matrix<T> &);
template<typename T> void save_gephi(const sbcsr_matrix<T> &, std::string *, std::string filename = "net");


#include "coo_matrix.hpp"
#include "csr_matrix.hpp"
#include "sbcsr_matrix.hpp"


template<typename T> std::ostream& operator<<(std::ostream &os, coo_matrix<T> &matrix)
{
	for(int i = 0; i < matrix.Nrows; ++i)
	{
		for(int j = 0; j < matrix.Ncols; ++j)
			os << matrix(i,j) << " ";
		os << std::endl;
	}
	return os;
}
template<typename T> std::ostream& operator<<(std::ostream &os, csr_matrix<T> &matrix)
{
	for (int i = 0; i < matrix.Nrows; ++i)
	{
		for (int j = 0; j < matrix.Ncols; ++j)
			os << matrix(i, j) << " ";
		os << std::endl;
	}
	return os;
}
template<typename T> std::ostream& operator<<(std::ostream &os, sbcsr_matrix<T> &matrix)
{
	for (int i = 0; i < matrix.Nrows; ++i)
	{
		for (int j = 0; j < matrix.Ncols; ++j)
			os << matrix(i, j) << " ";
		os << std::endl;
	}
	return os;
}