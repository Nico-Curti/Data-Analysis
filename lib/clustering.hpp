#pragma once
#include "fstat.hpp"
#include "points.hpp"
#include "metrics.hpp"
#include <numeric>
#include <iterator>
#include <omp.h>

struct
{
	template<typename Dist> inline int nearest(const int &, const Point &, const int &, float *, Dist, const float &, const float &, float pt_z = 0.f );
	template<typename Dist> int* operator()(const Point &, Point &, Dist, const int &, unsigned int seed = 0, bool time = false);
} kpp;
typedef decltype(kpp) Kpp;

struct
{
	template<typename Dist> int* operator()(const Point &, Point &, Dist, const int &, unsigned int seed = 0, bool time = false);
} rng_centroid;
typedef decltype(rng_centroid) RNG;

struct
{
	template<typename Dist, typename Cl_center, typename Init> int *operator()(const Point &, Point &, Dist, Cl_center, Init, const int &, const int &, float *point_weight = nullptr, unsigned int seed = 0, bool time = false);
} kmean;
typedef decltype(kmean) Kmean;

#include "kmeans.hpp"

struct
{
	template<typename Dist> int* operator()(const Point &, Point &, Dist, float control = 1e-4f, float ray = 1e-1f, bool time = false);
} densityclustering;
typedef decltype(densityclustering) DC;

#include "density_clustering.hpp"
