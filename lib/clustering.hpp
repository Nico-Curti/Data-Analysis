#pragma once
#include "fstat.hpp"
#include "points.hpp"
#include "metrics.hpp"
#include <numeric>
#include <iterator>
#include <omp.h>

template<typename Dist, typename Cl_Center> class kmean : public Point, metrics
{
	int n_point, n_cluster, n_iteration;
	int *cluster, *point_per_cluster;
	float *w;
	Point point, centroid;
	inline int nearest(const int &, const Point &, const int &, float *, const float &, const float &, float pt_z = 0.f );
	void kpp(const Point &, Point &, const int &, const int &, int *, unsigned int seed = 0);
	void random_init(int *, const int &, unsigned int seed = 0);
public:
	kmean(const Point &, const int &, const int &, const int &, float *point_weight = nullptr);
	~kmean();
	void init_centroid(std::string mode = "k++", bool time = false);
	int* Kmean(Point &, bool time = false);
}

template<typename Dist> class densityclustering : public Point, metrics
{
	int n_point, *density;
	Point point;
public:
	densityclustering(const Point &point_, int pow_ = 2);
	~densityclustering();
	int* DC(Point &centroid, float control = 0.f, float ray = 0.f, bool time = false);
};	
