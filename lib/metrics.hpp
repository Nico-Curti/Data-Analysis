#pragma once
#include "fmath.hpp"
#include "points.hpp"
#include <map>
#include <vector>

struct
{
	template<typename T> inline T operator()(const T &x1, const T &x2, const T &y1, const T &y2, T z1 = (T)0., T z2 = (T)0.)
	{
		return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2);
	}
} euclidean;

struct 
{
	template<typename T> inline T operator()(const T &x1, const T &x2, const T &y1, const T &y2, T z1 = (T)0., T z2 = (T)0.)
	{
		return std::fabs(x1 - x2) + std::fabs(y1 - y2) + std::fabs(z1 - z2); 
	}
} manhattan;

struct 
{
	template<typename T> inline T operator()(const T &x1, const T &x2, const T &y1, const T &y2, T z1 = (T)0., T z2 = (T)0., T POW = (T)1.)
	{
		return pow(std::abs(x1 - x2), POW) + pow(std::abs(y1 - y2), POW) + pow(std::abs(z1 - z2), POW);
	}
} minkowsy;

struct
{
	template<typename T> void operator()(const Point &point, Point &centroid, const int &n_point, const int &n_cluster, int *cluster, int *point_per_cluster, T *weight)
	{
		T *sum_w = new T[n_cluster];
		std::memset(centroid.x, (T)0., sizeof(T)*n_cluster);
		std::memset(centroid.y, (T)0., sizeof(T)*n_cluster);
		if(point.dim == 3) std::memset(centroid.z, (T)0., sizeof(T)*n_cluster);
		std::memset(point_per_cluster, 0, sizeof(int)*n_cluster);
		std::memset(sum_w, (T)0., sizeof(T)*n_cluster);

	#if defined _OPENMP
		T *sum_x = nullptr, *sum_y = nullptr, *sum_z = nullptr;
		int *sum_point = nullptr; T *sum_weight = nullptr;
	#pragma omp parallel
		{
			const int n_thread = omp_get_max_threads();
	#pragma omp single
			{
				sum_x = new T[n_thread * n_cluster];
				sum_y = new T[n_thread * n_cluster];
				if(point.dim == 3)
					sum_z = new T[n_thread * n_cluster];
				sum_point = new int[n_thread * n_cluster];
				sum_weight = new T[n_thread * n_cluster];
				std::memset(sum_x, (T)0., n_thread*n_cluster*sizeof(T));
				std::memset(sum_y, (T)0., n_thread*n_cluster*sizeof(T));
				if(point.dim == 3)
					std::memset(sum_z, (T)0., n_thread*n_cluster*sizeof(T));
				std::memset(sum_point, 0, n_thread*n_cluster*sizeof(int));
				std::memset(sum_weight, (T)0., n_thread*n_cluster*sizeof(T));
			}
	#pragma omp for
			for(int k = 0; k < n_point; ++k)
			{
				int i = cluster[k], id = omp_get_thread_num(), idx = id * n_cluster + i;
				sum_x[idx] += point.x[k];
				sum_y[idx] += point.y[k];
				if(point.dim == 3)
					sum_z[idx] += point.z[k];
				++sum_point[idx];
				sum_weight[idx] += w[k];
			}
	#pragma omp for
			for(int i = 0; i < n_cluster; ++i)
				for(int k = 0; k < n_thread; ++k)
				{
					int idx = k * n_cluster + i;
					centroid.x[i] += sum_x[idx];
					centroid.y[i] += sum_y[idx];
					if(point.dim == 3)
						centroid.z[i] += sum_z[idx];
					point_per_cluster[i] += sum_point[idx];
					sum_w[i] += sum_weight[idx];
				}
		}
		delete[] sum_x; delete[] sum_y; delete[] sum_point; delete[] sum_weight;
	#else
		for(int k = 0; k < n_point; ++k)
		{
			int i = cluster[k];
			centroid.x[i] += point.x[k];
			centroid.y[i] += point.y[k];
			if(point.dim == 3) 
				centroid.z[i] += point.z[k];
			++point_per_cluster[i];
			sum_w[i] += w[k];
		}
	#endif

		for(int i = 0; i < n_cluster; ++i)
		{
			int n_point_cluster = point_per_cluster[i];
			centroid.x[i] /= sum_w[i];//n_point_cluster;
			centroid.y[i] /= sum_w[i];//n_point_cluster;
			if(point.dim == 3)
				centroid.z[i] /= sum_w[i];//n_point_cluster;
		}
		return;
	}
} middle_point;

struct
{
	template<typename T> void operator()(const Point &point, Point &centroid, const int &n_point, const int &n_cluster, int *cluster, int * point_per_cluster, T *weight)
	{
		std::map<int, std::vector<T>> table_x, table_y, table_z;

	#pragma omp parallel for
		for(int i = 0; i < n_point; ++i)
		{
			int idx_cl = cluster[i];
			for(int j = 0; j < w[i]; ++j)
			{
				table_x[idx_cl].push_back(point.x[i]);
				table_y[idx_cl].push_back(point.y[i]);
				if(point.dim == 3)
					table_z[idx_cl].push_back(point.z[i]);
			}
		}
	#ifdef _DEBUG
		assert(n_cluster == (int)table_x.size() && n_cluster == (int)table_y.size() && n_cluster == (int)table_z.size());
	#endif

		auto iter_x = table_x.begin(); auto iter_y = table_y.begin();// auto iter_z = table_z.begin();
		for(; iter_x != table_x.end(); ++iter_x, ++iter_y/*, ++iter_z*/)
		{
			int size = (int)iter_x->second.size(), idx_cl = iter_x->first;
			if(iter_x->second.size() % 2)
			{
				std::nth_element(iter_x->second.begin(), iter_x->second.begin() + size/2, iter_x->second.end());
				std::nth_element(iter_y->second.begin(), iter_y->second.begin() + size/2, iter_y->second.end());
				//std::nth_element(iter_z->second.begin(), iter_z->second.begin() + size/2, iter_z->second.end());
				centroid.x[idx_cl] = iter_x->second[size/2];
				centroid.y[idx_cl] = iter_y->second[size/2];
				//centroid.z[idx_cl] = iter_z->second[size/2];
			}
			else
			{
				std::nth_element(iter_x->second.begin(), iter_x->second.begin() + size/2, iter_x->second.end());
				std::nth_element(iter_y->second.begin(), iter_y->second.begin() + size/2, iter_y->second.end());
				//std::nth_element(iter_z->second.begin(), iter_z->second.begin() + size/2, iter_z->second.end());
				T arg1x = iter_x->second[size/2], arg1y = iter_y->second[size/2]/*, arg1z = iter_z->second[size/2]*/;
				std::nth_element(iter_x->second.begin(), iter_x->second.begin() + size/2-1, iter_x->second.end());
				std::nth_element(iter_y->second.begin(), iter_y->second.begin() + size/2-1, iter_y->second.end());
				//std::nth_element(iter_z->second.begin(), iter_z->second.begin() + size/2-1, iter_z->second.end());
				T arg2x = iter_x->second[size/2-1], arg2y = iter_y->second[size/2-1]/*, arg2z = iter_z->second[size/2-1]*/;

				centroid.x[idx_cl] = (arg1x + arg2x)/2;
				centroid.y[idx_cl] = (arg1y + arg2y)/2;
				//centroid.z[idx_cl] = (arg1z + arg2z)/2;
			}
			point_per_cluster[idx_cl] = size;
		}

		return;
	}
} median_point;
