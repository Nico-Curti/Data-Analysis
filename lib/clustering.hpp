#pragma once
#include "fstat.hpp"
#include "points.hpp"
#include "metrics.hpp"
#include <numeric>
#include <iterator>
#include <omp.h>

class kmean : public Point
{
	int n_point, n_cluster, n_iteration;
	int *cluster, *point_per_cluster;
	float *w;
	Point point, centroid;
	template<typename Dist> inline int nearest(const int &, const Point &, const int &, float *, Dist, const float &, const float &, float pt_z = 0.f );
	template<typename Dist> void kpp(const Point &, Point &, const int &, const int &, int *, Dist, unsigned int seed = 0);
	void random_init(int *, const int &, unsigned int seed = 0);
public:
	kmean(const Point &, const int &, const int &, const int &, float *point_weight = nullptr);
	~kmean();
	template<typename Dist> void init_centroid(Dist, std::string mode = "k++", bool time = false);
	template<typename Dist, typename Cl_Center> int* Kmean(Point &, Dist dist, Cl_Center cl, bool time = false);
};

#include "kmeans.hpp"

class densityclustering : public Point
{
	int n_point, *density;
	Point point;
public:
	densityclustering(const Point &);
	~densityclustering();
	template<typename Dist> int* DensityClustering(Point &, Dist, float control = 1e-4f, float ray = 1e-1f, bool time = false);
};	

#include "density_clustering.hpp"
