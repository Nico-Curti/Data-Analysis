#pragma once
#include "fstat.hpp"
#include "points.hpp"
#include "metrics.hpp"
#include <numeric>
#include <omp.h>

class kmean : public Point, metrics
{
	int n_point, n_cluster, n_iteration;
	int *cluster, *point_per_cluster;
	float *w;
	Point point, centroid;
	metrics met;
	float(metrics::*dist)(const float &, const float &, const float &, const float &, float &, float &);
	void(metrics::*cl_center)(const Point &, Point &, const int &, const int &, int *, int *, float *);

	inline int nearest(const int &, const Point &, const int &, float *, const float &, const float &, float pt_z = 0.f );
	void kpp(const Point &, Point &, const int &, const int &, int *);
	void random_init(int *, const int &);
public:
	kmean(const Point &, const int &, const int &, const int &, std::string metric = "euclidean", float *point_weight = nullptr, int pow = 2);
	~kmean();
	void init_centroid(std::string mode = "k++", bool time = false);
	int* Kmean(Point &, bool time = false);
}

class densityclustering : public Point, metrics
{
	int n_point, *density;
	Point point;
	metrics met;
	float(metrics::*dist)(const float &, const float &, const float &, const float &, float &, float &);

public:
	densityclustering(const Point &point_, std::string metric_ = "euclidean", int pow_ = 2);
	~densityclustering();
	int* DC(Point &centroid, float control = 0.f, float ray = 0.f, bool time = false);
};	
