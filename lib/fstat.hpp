#pragma once
#include "fmath.hpp"
#include <iostream>
#include <climits>
#include <numeric>
#include <algorithm>
#include <cstring>
#include <functional>
#include <type_traits>
#include <omp.h>

template<bool c, typename T> using parallel = typename std::enable_if<c>::type*;
template<bool c, typename T> using serial = typename std::enable_if<!c>::type*;
// utility struct for index functions (ex. idxmax, idxmin, argsort)
template<typename T> struct pair
{
	T value;
	int idx;
	pair(){};
	pair& operator=(const pair &);
	pair(const pair &);
};
//==================== min and max functions =================================
template<bool par, typename T, parallel<par, T> = nullptr> inline T max(T *x, const int &N);
template<bool par, typename T, serial<par, T> = nullptr> inline T max(T *x, const int &N);
template<bool par, typename T, parallel<par, T> = nullptr> inline T idxmax(T *x, const int &N);
template<bool par, typename T, serial<par, T> = nullptr> inline T idxmax(T *x, const int &N);
template<bool par, typename T, parallel<par, T> = nullptr> inline T min(T *x, const int &N);
template<bool par, typename T, serial<par, T> = nullptr> inline T min(T *x, const int &N);
template<bool par, typename T, parallel<par, T> = nullptr> inline T idxmin(T *x, const int &N);
template<bool par, typename T, serial<par, T> = nullptr> inline T idxmin(T *x, const int &N);
//======================= sort functions =====================================
template<typename T> int inline binary_search(T *x, const T &item, const int &low, const int &high);
template<typename T> void inline insertion_sort(T *x, const int &N);
#define SMALL 1000 // minimum size of array for serial sort
template<typename T, typename Ord = std::less<T>> void inline mergesort_serial(T *a, const int &start, const int &end, Ord ord = Ord());
template<typename T, typename Ord = std::less<T>> void inline mergesort_parallel_omp (T *a, const int &start, const int &end, const int &threads, Ord ord = Ord());
template<bool par, typename T, typename Ord = std::less<T>, parallel<par, T> = nullptr> void inline sort(T *a, const int &start, const int &end, Ord ord = Ord());
template<bool par, typename T, typename Ord = std::less<T>, serial<par, T> = nullptr> void inline sort(T *a, const int &start, const int &end, Ord ord = Ord());
template<typename T, typename Ord = std::less<pair<T>>> void inline mergeargsort_serial(pair<T> *arr, T *a, const int &start, const int &end, Ord ord = Ord());
template<typename T, typename Ord = std::less<pair<T>>> void inline mergeargsort_parallel_omp (pair<T> *arr, T *a, const int &start, const int &end, const int &threads, Ord ord = Ord());
template<bool par, typename T, typename Ord = std::less<pair<T>>, parallel<par, T> = nullptr> inline int* argsort(T *a, const int &start, const int &end, Ord ord = Ord());
template<bool par, typename T, typename Ord = std::less<pair<T>>, serial<par, T> = nullptr> inline int* argsort(T *a, const int &start, const int &end, Ord ord = Ord());
//====================== variance functions ==================================
template<typename T> inline T online_variance(T *x, const int &beg, const int &end, T &mean);
template<typename T> inline T online_covariance(T *x, T *y, const int &N);
template<typename T> inline T combine_variance(T &avg_a, const int &count_a, T &var_a, T &avg_b, const int &count_b, T &var_b, T &mean_x);
template<typename T> inline T weighted_var(T *x, const int &N, T *w);
template<typename T> inline void moments(T *x, const int &N, T &mean, T &var, T &skew, T &kurt);
template<typename T> inline T combine_variance(T *vars, T *means, const int &n_vars, const int &N);
template<typename T> inline void variance_omp(T *x, T *var, T *mean, const int &beg, const int &end, const int &nth, int &TID);
template<bool par, typename T, parallel<par, T> = nullptr> inline T variance(T *x, const int &beg, const int &end);
template<bool par, typename T, serial<par, T> = nullptr> inline T variance(T *x, const int &beg, const int &end);
template<typename T> T** cov_mat(T **data, const int &Nrow, const int &Ncol, bool axis = false, T *mean = nullptr);
//====================== median functions ====================================
template<bool par, typename T, parallel<par, T> = nullptr> inline T median(T *data, const int &N);
template<bool par, typename T, serial<par, T> = nullptr> inline T median(T *data, const int &N);
template<typename T> T* median(T **data, const int &Nrows, const int &Ncols);
//====================== correlation functions ===============================
template<bool par, typename T, parallel<par, T> = nullptr> T* autocorrelation(T *signal, const int &n_sample);
template<bool par, typename T, serial<par, T> = nullptr> T* autocorrelation(T *signal, const int &n_sample);




//===============================================================================================================
template<typename T> pair<T>& pair<T>::operator=(const pair<T> &a) {this->value = a.value; this->idx = a.idx; return *this;}
template<typename T> pair<T>::pair(const pair<T> &a){this->value = a.value; this->idx = a.idx;}
template<typename T> bool operator<(const pair<T> &a, const pair<T> &b){return a.value < b.value;};
template<typename T> bool operator>(const pair<T> &a, const pair<T> &b){return a.value > b.value;};
// ==============================================================================================================

template<bool par, typename T, parallel<par, T>> inline T max(T *x, const int &N)
{
	T max_val = std::numeric_limits<T>::min();
#pragma omp parallel for reduction (max : max_val)
	for(int i = 0; i < N; ++i) max_val = (x[i] > max_val) ? x[i] : max_val;
	return max_val;
}
template<bool par, typename T, serial<par, T>> inline T max(T *x, const int &N)
{
	return *std::max_element(x, x + N);
}

template<bool par, typename T, parallel<par, T>> T idxmax(T *x, const int &N)
{
#pragma omp declare reduction(maximum : struct pair<T> : omp_out = omp_in.value > omp_out.value ? omp_in : omp_out) initializer(omp_priv = omp_orig)
	pair<T> max;
	max.value = std::numeric_limits<T>::min();
	max.idx = 0;
#pragma omp parallel for reduction(maximum : max)
	for(int i = 0; i < N; ++i)
		if(x[i] > max.value)
		{
			max.value = x[i];
			max.idx = i;
		}
	return max.idx;
}
template<bool par, typename T, serial<par, T>> inline T idxmax(T *x, const int &N)
{
	return std::distance(x, std::max_element(x, x + N));
}

template<bool par, typename T, parallel<par, T>> inline T min(T *x, const int &N)
{
	T min_val = std::numeric_limits<T>::max();
#pragma omp parallel for reduction (min: min_val)
	for(int i = 0; i < N; ++i)  min_val = (x[i] < min_val) ? x[i] : min_val;
	return min_val;
}
template<bool par, typename T, serial<par, T>> inline T min(T *x, const int &N)
{
	return *std::min_element(x, x + N);
}

template<bool par, typename T, parallel<par, T>> inline T idxmin(T *x, const int &N)
{
#pragma omp declare reduction(minimum : struct pair<T> : omp_out = omp_in.value < omp_out.value ? omp_in : omp_out) initializer(omp_priv = omp_orig)
	pair<T> min;
	min.value = std::numeric_limits<T>::max();
	min.idx = 0;
#pragma omp parallel for reduction(minimum : min)
	for(int i = 0; i < N; ++i)
		if(x[i] < min.value)
		{
			min.value = x[i];
			min.idx = i;
		}
	return min.idx;
}
template<bool par, typename T, serial<par, T>> inline T idxmin(T *x, const int &N)
{
	return std::distance(x, std::min_element(x, x + N));
}


//=========================================================================


template<typename T> int inline binary_search(T *x, const T &item, const int &low, const int &high)
{
	if(high <= low) return (item > x[low]) ? low + 1 : low;
	int mid = (low + high) >> 1;
	if(item == x[mid]) return mid + 1;
	if(item > x[mid]) return binary_search(x, item, mid + 1, high);
	return binary_search(x, item, low, mid - 1);
}

template<typename T> void inline insertion_sort(T *x, const int &N)
{
	int loc, j;
	T selected;
	for(int i = 1; i < N; ++i)
	{
		j = i - 1;
		selected = x[i];
		loc = binary_search(x, selected, 0, j);
		while(j >= loc)
		{
			x[j+1] = x[j];
			j--;
		}
		x[j+1] = selected;
	}
	return;
}

template<typename T, typename Ord> void inline mergesort_serial(T *a, const int &start, const int &end, Ord ord)
{
	if((end-start) == 2) 
	{
		if(ord(a[start] , a[end-1])) return;
		else
		{
			std::swap(a[start], a[end-1]);
			return;
		}
	}
	int pivot = start + ((end-start) >> 1);
	if((end-start) < SMALL) {std::sort(a + start, a + end, ord); return;}
	else
	{
		mergesort_serial(a, start, pivot, ord);
		mergesort_serial(a, pivot, end, ord);
	}

	std::inplace_merge(a + start, a + pivot, a + end, ord);

	return;
}

template<typename T, typename Ord> void inline mergesort_parallel_omp (T *a, const int &start, const int &end, const int &threads, Ord ord)
{
	int pivot = start + ((end-start) >> 1);
	if(threads <= 1) {mergesort_serial(a, start, end, ord); return;}
	else
	{
#pragma omp task shared(start, end, threads)
		{
			mergesort_parallel_omp(a, start, pivot, threads >> 1, ord);
		}
#pragma omp task shared(start, end, threads)
		{
			mergesort_parallel_omp(a, pivot, end, threads - (threads >> 1), ord);
		}
#pragma omp taskwait
	}
	std::inplace_merge(a + start, a + pivot, a + end, ord);
	return;
}

template<bool par, typename T, typename Ord, parallel<par, T>> void inline sort(T *a, const int &start, const int &end, Ord ord)
{
	int nth, size, diff;
#pragma omp parallel
	{
#pragma omp single
		{
			nth = (omp_get_num_threads() % 2) ? omp_get_num_threads()-1 : omp_get_num_threads();
			size = ((diff = end % nth)) ? end - diff : end;
			mergesort_parallel_omp(a, start, size, nth, ord);
		}
	}
	if(diff)
	{
		std::sort(a + size, a + end, ord);
		std::inplace_merge(a + start, a + size, a + end, ord);
	}
	return;
}

template<bool par, typename T, typename Ord, serial<par, T>> void inline sort(T *a, const int &start, const int &end, Ord ord)
{
	mergesort_serial(a, start, end, ord);
	return;
}


template<typename T, typename Ord> void inline mergeargsort_serial(pair<T> *arr, T *a, const int &start, const int &end, Ord ord)
{
	if((end-start) == 2) 
	{
		if(ord(arr[start], arr[end-1])) return;
		else
		{
			std::swap(arr[start], arr[end-1]);
			return;
		}
	}
	int pivot = start + ((end-start) >> 1);
	if((end-start) < SMALL) std::sort(arr + start, arr + end, ord);
	else
	{
		mergeargsort_serial(arr, a, start, pivot, ord);
		mergeargsort_serial(arr, a, pivot, end, ord);
	}

	std::inplace_merge(arr + start, arr + pivot, arr + end, ord);

	return;
}

template<typename T, typename Ord> void inline mergeargsort_parallel_omp (pair<T> *arr, T *a, const int &start, const int &end, const int &threads, Ord ord)
{
	int pivot = start + ((end-start) >> 1);
	if(threads <= 1) {mergeargsort_serial(arr, a, start, end, ord); return;}
	else
	{
#pragma omp task shared(start, end, threads)
		{
			mergeargsort_parallel_omp(arr, a, start, pivot, threads >> 1, ord);
		}
#pragma omp task shared(start, end, threads)
		{
			mergeargsort_parallel_omp(arr, a, pivot, end, threads - (threads >> 1), ord);
		}
#pragma omp taskwait
	}

	std::inplace_merge(arr + start, arr + pivot, arr + end, ord);
	return;
}

template<bool par, typename T, typename Ord, parallel<par, T>> inline int* argsort(T *a, const int &start, const int &end, Ord ord)
{
	int nth, size, diff;
	pair<T> *arr = new pair<T>[end-start];
	for(int i = 0; i < end - start; ++i)
	{
		arr[i].value = a[i+start];
		arr[i].idx = i+start;
	}

#pragma omp parallel
	{
#pragma omp single
		{
			nth = (omp_get_num_threads() % 2) ? omp_get_num_threads()-1 : omp_get_num_threads();
			size = ((diff = end % nth)) ? end - diff : end;
			mergeargsort_parallel_omp(arr, a, start, size, nth, ord);
		}
	}
	if(diff)
	{
		std::sort(arr + size, arr + end, ord);
		std::inplace_merge(arr + start, arr + size, arr + end, ord);
	}
	int *idx = new int[end - start];
	for(int i = 0; i < end - start; ++i) idx[i] = arr[i].idx;
	delete[] arr;
	return idx;
}

template<bool par, typename T, typename Ord, serial<par, T>> inline int* argsort(T *a, const int &start, const int &end, Ord ord)
{
	pair<T> *arr = new pair<T>[end-start];
	for(int i = 0; i < end - start; ++i)
	{
		arr[i].value = a[i+start];
		arr[i].idx = i+start;
	}
	mergeargsort_serial(arr, a, start, end, ord);
	int *idx = new int[end - start];
	for(int i = 0; i < end - start; ++i) idx[i] = arr[i].idx;
	delete[] arr;
	return idx;
}


//=====================================================================================================================
template<typename T> inline T online_variance(T *x, const int &beg, const int &end, T &mean)
{
	T var = (T)0., delta, delta2;
	mean = (T)0.;
	int n = 0;
	for(int i = beg; i < end; ++i)
	{
		++n;
		delta = x[i] - mean;
		mean += delta / n;
		delta2 = x[i] - mean;
		var += delta * delta2;
	}
	return var / n;
}

template<typename T> inline T online_covariance(T *x, T *y, const int &N)
{
	T meanx = (T)0., meany = (T)0., cov = (T)0., deltax, deltay;
	for(int i = 0; i < N; ++i)
	{
		deltax = (x[i] - meanx)/(i+1);
		meanx += deltax;
		deltay = (y[i] - meany)/(i+1);
		meany += deltay;
		cov += i * deltax * deltay - cov / (i+1);
	}
	return cov * N / (N - 1);
}

template<typename T> inline T combine_variance(T &avg_a, const int &count_a, T &var_a, T &avg_b, const int &count_b, T &var_b, T &mean_x)
{
	T delta = avg_b - avg_a, m_a = var_a * (count_a - 1), m_b = var_b * (count_b - 1), M2;
	
	mean_x = (std::abs(count_b - count_a) < 10 && (count_b > 500 && count_a > 500)) ? (count_a * avg_a + count_b * avg_b) / (count_a + count_b) : avg_a + delta * count_b /(count_b + count_a);

	M2 = m_a + m_b + delta * delta * count_a * count_b / (count_a + count_b);
	return M2 / (count_a + count_b - 1);
}

template<typename T> inline T weighted_var(T *x, const int &N, T *w)
{
	T wSum = (T)0.0, mean = (T)0.0, var = (T)0.0, meanOld;
	for(int i = 0; i < N; ++i)
	{
		wSum += w[i];
		meanOld = mean;
		mean = meanOld + (w[i] / wSum) * (x[i] - meanOld);
		var += w[i] * (x[i] - meanOld) * (x[i] - mean);
	}
	return var;
}

template<typename T> inline void moments(T *x, const int &N, T &mean, T &var, T &skew, T &kurt)
{
	mean = (T)0., var = (T)0., skew = (T)0., kurt = (T)0.;
	int n = 0, n1;
	T delta, delta_n, delta_n2, term1;
	for(int i = 0; i < N; ++i)
	{
		n1 = n;
		++n;
		delta = x[i] - mean;
		delta_n = delta / n;
		delta_n2 = delta_n * delta_n;
		term1 = delta * delta_n * n1;
		mean += delta_n;
		kurt += term1 * delta_n2 * (n * n - 3 * n + 3) + 6 * delta_n2 * var - 4 * delta_n * skew;
		skew += term1 * delta_n * (n - 2) - 3 * delta_n * var;
		var += term1;
	}

	kurt = (n * kurt) / (var * var) - 3;
	skew *= sqrt(n) / (pow(var, 3./2));
	var /= (n-1);
	return;
}

template<typename T> inline T combine_variance(T *vars, T *means, const int &n_vars, const int &N)
{
	T frac = 1./n_vars, tot_var, tmp = (T)0.;
	tot_var = std::inner_product(vars, vars + n_vars, means, (T)0., std::plus<T>(), 
								[&frac, &tmp](const T &v, const T &m)
								{
									T res = v + m*m*(1 - frac) - 2*tmp*frac*m;
									tmp += m;
									return res;
								});
	return tot_var * frac;
}

template<typename T> inline void variance_omp(T *x, T *var, T *mean, const int &beg, const int &end, const int &nth, int &TID)
{
	if(nth == 1) 
	{   
		#pragma omp critical
		{
			var[TID] = online_variance(x, beg, end, mean[TID]);
			++TID;
		}
	}
	else
	{
		int pivot = beg + ((end-beg) >> 1);
		#pragma omp task shared(beg, pivot, nth, mean, var, TID) untied
		{
			variance_omp(x, var, mean, beg, pivot, nth>>1, TID);
		}
		#pragma omp task shared(pivot, end, nth, mean, var, TID) untied
		{           
			variance_omp(x, var, mean, pivot, end, nth>>1, TID);
		}
		#pragma omp taskwait
	}
	return;
}

template<bool par, typename T, parallel<par, T>> inline T variance(T *x, const int &beg, const int &end)
{
	// not right for big array and slow cause (unavoidable) critical section in variance_omp!
	int nth, size, diff, TID = 0;
	T *var = nullptr, *mean = nullptr, tot_var, tmp = (T)0., frac;
#pragma omp parallel reduction (+:TID)
	{
	#pragma omp single nowait
		{
			nth = omp_get_num_threads();
			nth = (nth % 2) ? nth-1 : nth;
			var = new T[nth]; mean = new T[nth];
			size = ((diff = end % nth)) ? end - diff : end;
			frac = (T)1./nth;
			variance_omp(x, var, mean, beg, size, nth, TID);
		}
	}
	tot_var = std::inner_product(var, var + nth, mean, (T)0., std::plus<T>(), 
				[&frac, &tmp](const T &v, const T &m)
				{
					T res = v + m*m*(1 - frac) - 2*tmp*frac*m;
					tmp += m;
					return res;
				}) * frac;
	if(diff)
	{
		T tmp_mean, tmp_var = online_variance(x, end-diff, end, tmp_mean);
		tot_var = ( (T(size)/(end-beg))*( tot_var + (tmp/nth)*(tmp/nth)*((T)1. - T(size)/(end-beg)) ) + 
					(T(diff)/(end-beg))*( tmp_var + tmp_mean * tmp_mean*((T)1. - T(diff)/(end-beg))   - 
											2*(tmp/nth)*tmp_mean*(T(size)/(end-beg)) )
											 );
	}
	delete[] var;
	delete[] mean;
	return tot_var;
}


template<bool par, typename T, serial<par, T>> inline T variance(T *x, const int &beg, const int &end)
{
	T mean;
	return online_variance(x, beg, end, mean);
}

template<typename T> T** cov_mat(T **data, const int &Nrow, const int &Ncol, bool axis, T *mean)
{
	bool del = false;
	T **cov = nullptr;
	if(axis) // covariance of rows
	{
		if(mean == nullptr) {mean = new T[Nrow]; del = true;}
		cov = new T*[Nrow];
		std::generate(cov, cov + Nrow, [&Nrow](){return new T[Nrow];});
		std::transform(data, data + Nrow, mean, [&Ncol](T* d){return std::accumulate(d, d + Ncol, (T)0.)/Ncol;});
		for (int i = 0; i < Nrow; ++i)
			for (int j = 0; j < Nrow; ++j)
				cov[i][j] = std::inner_product(data[i], data[i] + Ncol, data[j], (T)0.,
												std::plus<T>(),
												[&i, &j, &mean](const T &di, const T &dj)
												{
													return (di - mean[i]) * (dj - mean[j]);
												}) / (Ncol - 1);
	}
	else // covariance of columns
	{
		if(mean == nullptr) {mean = new T[Ncol]; del = true;}
		cov = new T*[Ncol];
		std::generate(cov, cov + Ncol, [&Ncol](){return new T[Ncol];});
		std::memset(mean, 0, sizeof(T)*Ncol);

		std::for_each(data, data + Nrow, [&mean, &Ncol](T *di)
						{
							return std::transform(di, di + Ncol, mean, mean,
												  [&](const T &d, const T &m)
												  {
												  	return m + d;
												  });
						});
		std::transform(mean, mean + Ncol, mean, 
					   [&Nrow](const T &val)
					   {
				   			return val / Nrow;
					   });
		for(int i = 0; i < Ncol; ++i)
			for(int j = 0; j < Ncol; ++j)
				cov[i][j] = std::accumulate(data, data + Nrow, (T)0.,
											[&i, &j, &mean](const T &val, T *d)
											{
												return val + (d[i] - mean[i]) * (d[j] - mean[j]);
											}) / (Nrow - 1);
	}

	if(del) delete[] mean;
	return cov;
}

//=====================================================================================================================================

template<bool par, typename T, parallel<par, T>> inline T median(T *data, const int &N)
{
	int token_size = 2, n_token, i, mid;
	T median, *x = new T[N], *tmp_median = nullptr;
	std::memcpy(x, data, sizeof(T)*N);
	while(true)
	{
		if(N % token_size)
			++token_size;
		else
		{
			token_size = N / token_size;
			break;
		}
	}
	n_token = N / token_size;
	tmp_median = new T[n_token];
	if(token_size % 2)
	{
		mid = token_size/2;
#pragma omp parallel for
		for(int j = 0; j < n_token; ++j)
		{
			i = j*token_size;
			std::nth_element(x + i, x + i + mid , x + i + token_size);
			tmp_median[j] = x[i + mid];
		}
		mid = n_token/2;
		std::nth_element(tmp_median, tmp_median + mid, tmp_median + n_token);
		median = tmp_median[mid];
	}
	else
	{
		T val1, val2;
		mid = token_size/2;
#pragma omp parallel for
		for(int j = 0; j < n_token; ++j)
		{
			i = j*token_size;
			std::nth_element(x + i, x + i + mid , x + i + token_size);
			val1 = x[i + mid];
			std::nth_element(x + i, x + i + mid -1, x + i + token_size);
			val2 = x[i + mid - 1];
			tmp_median[j] = (val1 + val2)/2;
		}
		mid = n_token/2;
		std::nth_element(tmp_median, tmp_median + mid, tmp_median + n_token);
		val1 = tmp_median[mid];
		std::nth_element(tmp_median, tmp_median + mid - 1, tmp_median + n_token);
		val2 = tmp_median[mid - 1];
		median = (val1 + val2)/2;
	}
	delete[] tmp_median;
	delete[] x;
	return median;
}

template<bool par, typename T, serial<par, T>> T median(T *data, const int &N)
{
	T median, *x = new T[N], arg1, arg2;
	std::memcpy(x, data, sizeof(T)*N);
	if(N % 2)
	{
		std::nth_element(x, x + N/2, x + N);
		median = x[N/2];
	}
	else
	{
		std::nth_element(x, x + N/2, x + N);
		arg1 = x[N/2];
		std::nth_element(x, x + N/2 - 1, x + N);
		arg2 = x[N/2 - 1];
		median = (arg1 + arg2) / 2;
	}
	delete[] x;
	return median;
}

template<typename T> T* median(T **data, const int &Nrows, const int &Ncols)
{
	int non_zeros, num_zeros;
	T eps = (T)1e-5, sum_dist, norm, norm_inv, M, m, euclidean, 
	 *med = new T[Ncols], *distance = new T[Nrows], *tmp_median = new T[Ncols], *tmp = new T[Ncols];
	
	for(int i = 0; i < Nrows; ++i)
		for(int j = 0; j < Ncols; ++j)
			med[j] += data[i][j];
	std::transform(med, med + Ncols, med, [&Nrows](const T &val){return val /Nrows;});
	
	while(true)
	{
		sum_dist = (T)0.;
		non_zeros = 0;
		for(int i = 0; i < Nrows; ++i)
		{
			distance[i] = sqrt(std::inner_product(data[i], data[i] + Ncols, med, med + Ncols, (T)0., std::plus<T>(), [](const T &d, const T &m){return (d-m)*(d-m);}));
			if(distance[i] != (T)0.)
			{
				sum_dist += (T)1. / distance[i];
				++non_zeros;
			}
		}
		std::memset(tmp, 0, sizeof(T)*Ncols);

		for(int i = 0; i < Nrows; ++i)
		{
			distance[i] = (distance[i] == (T)0.) ? (T)0. : (T)1. / (distance[i] * sum_dist); 
			for(int j = 0; j < Ncols; ++j)
				tmp[j] += distance[i]*data[i][j];
		}

		num_zeros = Nrows - non_zeros;

		if(num_zeros == 0)
			tmp_median = tmp;
		else if(num_zeros == Nrows)
		{
			delete[] distance;
			delete[] tmp_median;
			delete[] tmp;
			return med;
		}
		else
		{
			norm = sqrt(std::inner_product(tmp, tmp + Ncols, med, med + Ncols, (T)0., std::plus<T>(), [&sum_dist](const T &t, const T &m){return (t-m)*sum_dist * (t-m)*sum_dist;}));
			norm_inv = (norm == (T)0.) ? (T)0. : (T)num_zeros/norm;
			M = std::max((T)0., (T)1. - norm_inv);
			m = std::min((T)1., norm_inv);
			std::transform(tmp, tmp + Ncols, med, tmp_median, [&M, &m](const T &t, const T &med_){return M*t + m*med_;});
		}
		euclidean = std::inner_product(med, med + Ncols, tmp_median, tmp_median + Ncols, (T)0., std::plus<T>(), [](const T &m, const T &t){return (m-t)*(m-t);});
		std::memcpy(med, tmp_median, sizeof(T)*Ncols);
		if(sqrt(euclidean) < eps)
		{
			delete[] distance;
			delete[] med;
			delete[] tmp;
			return tmp_median;
		}
	}
}
//=====================================================================================================================================

template<bool par, typename T, parallel<par, T>> T* autocorrelation(T *signal, const int &n_sample)
{
	T *auto_corr = new T[n_sample], maxC = std::numeric_limits<T>::min();
#pragma omp parallel for reduction(max : maxC)
	for(int m = 0; m < n_sample - 1; ++m)
	{
		auto_corr[m] = std::inner_product(signal, signal + n_sample - m - 1, signal + m, (T)0., std::plus<T>(), [](const T &i, const T &j){return i * j;}) / (n_sample - m);
		maxC = (maxC < auto_corr[m]) ? auto_corr[m] : maxC;
	}
	std::transform(auto_corr, auto_corr + n_sample, auto_corr, [&maxC](const T &val){return val / maxC;});
	return auto_corr;
}
template<bool par, typename T, serial<par, T>> T* autocorrelation(T *signal, const int &n_sample)
{
	T *auto_corr = new T[n_sample], maxC = std::numeric_limits<T>::min();
	for(int m = 0; m < n_sample - 1; ++m)
	{
		auto_corr[m] = std::inner_product(signal, signal + n_sample - m - 1, signal + m, (T)0., std::plus<T>(), [](const T &i, const T &j){return i * j;}) / (n_sample - m);
		maxC = (maxC < auto_corr[m]) ? auto_corr[m] : maxC;
	}
	std::transform(auto_corr, auto_corr + n_sample, auto_corr, [&maxC](const T &val){return val / maxC;});
	return auto_corr;
}


