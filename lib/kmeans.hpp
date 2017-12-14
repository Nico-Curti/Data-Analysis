#pragma once
#include "clustering.hpp"

kmean::kmean(const Point &point, const int &n_point, const int n_cluster&, const int n_iteration&, std::string metric, float *point_weight, int pow)
{
	this->n_point = n_point;
	this->n_cluster = n_cluster;
	this->n_iteration = n_iteration;
	this->point = point;
	this->met.pow = pow;
	if(metric != "euclidean" && metric != "manhattan" && metric != "minkowsy") {std::cerr << "Invalid metric! Given : " << metric << ". Possible values are 'euclidean', 'manhattan', 'minkowsy'." << std::endl; exit(1);}
	this->dist = (metric == "euclidean") ? &metrics::euclidean : (metric == "manhattan") ? &metrics::manhattan : &metrics::manhattan;

	this->centroid = Point(n_cluster);
	this->cluster = new int[n_point];
	this->point_per_cluster = new int[n_cluster];
	this->w = new float[n_point];
	if(point_weight)
		std::memcpy(this->w, this->point_weight, sizeof(float)*this->n_point);
	else
		std::memset(w, 1, sizeof(float)*this->n_point);
}

kmean::~kmean()
{
	delete[] this->cluster;
	delete[] this->point_per_cluster;
	this->dist = nullptr;
}


inline int kmean::nearest(const int &lbl, const Point &centroid, const int &n_cluster, float *d2, const float &pt_x, const float &pt_y, float pt_z = 0.f)
{
	int min_index = lbl;
	float d, min_dist = std::numeric_limits<float>::max();

	if(pt_z != 0.f)
	{
		for(int i = 0; i < n_cluster; ++i)
			if(min_dist > (d = (this->met.*dist)(pt_x, centroid.x[i], pt_y, centroid.y[i], pt_z, centrodi.z[i])))
				{ min_dist = d; min_index = i;}
	}
	else
	{
		for(int i = 0; i < n_cluster; ++i)
			if(min_dist > (d = (this->met.*dist)(pt_x, centroid.x[i], pt_y, centroid.y[i])))
				{ min_dist = d; min_index = i;}
	}

	if(d2) *d2 = min_dist;
	return min_index;
}


void kmean::kpp(const Point &point, Point &centroid, const int &n_point, const int &n_cluster, int *cluster)
{
	int cl;
	float sum, *d = new float[n_point];
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 engine{seed};
	std::uniform_real_distribution<float> r_dist{0.0f, 1.0f};
	int idx = (int)r_dist(engine)*n_point;
	centroid.x[0] = point.x[idx]; centroid.y[0] = point.y[idx];
	if(point.dim == 3) centroid.z = point.z[idx];

	for(cl = 1; cl < n_cluster; ++cl)
	{
		sum = 0.0f;
#pragma omp parallel for reduction(+:sum)
		for(int j = 0; j < n_point; ++j)
		{
			if(point.dim == 3)
				nearest(cluster[j], centroid, cl, d + j, point.x[j], point.y[j], point.z[j]);
			else
				nearest(cluster[j], centroid, cl, d + j, point.x[j], point.y[j]);
			sum += d[j];
		}
		
		sum *= r_dist(engine);

		for(int j = 0; j < n_point; ++j)
		{
			if((sum -= d[j]) > 0) continue;
			centroid.x[cl] = point.x[j];
			centroid.y[cl] = point.y[j];
			if(point.dim == 3)
				centroid.z[cl] = point.z[j];
			break;
		}
	}

#pragma omp parallel for
	for(int j = 0; j < n_point; ++j)
	{
		if(point.dim == 3)
			cluster[j] = nearest(cluster[j], centroid, cl, 0, point.x[j], point.y[j], point.z[j]);
		else
			cluster[j] = nearest(cluster[j], centroid, cl, 0, point.x[j], point.y[j]);
	}
	
	delete[] d;

#ifdef _DEBUG
	std::set<int> u_cluster;
	for(int i = 0; i < n_point; ++i)
		u_cluster.insert(cluster[i]);
	assert((int)u_cluster.size() == n_cluster);
	u_cluster.clear();
#endif

	return;
}

void kmean::random_init(int *cluster, const int &n_point)
{
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 engine{seed};
	std::uniform_int_distribution<int> i_dist{0, 1};
	std::generate(cluster, cluster + n_point, [&engine, &i_dist]{return i_dist(engine);});
	return;
}

void kmean::init_centroid(std::string mode, bool time)
{
	auto start = std::chrono::steady_clock::now();
	if(mode == "k++") kpp(this->point, this->centroid, this->n_point, this->n_cluster, this->cluster);
	else if(mode == "random") random_init(this->cluster, this->n_point);
	else {std::cerr << "Invalid Initialization mode. Given : " << mode << ". Possible values are 'k++', 'random'." << std::endl; exit(1);}
	if(time)
		std::cout << "Initialization of " << this->n_cluster << " took " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() << " sec" << std::endl;
}


int* kmean::Kmean(Point &centroid, bool time)
{
	auto start = std::chrono::steady_clock::now();
	int iteration = 0, changed;

	while(true)
	{
		//Compute the centroid of the clusters
		(this->met.*cl_center)(this->point, centroid, this->n_point, this->n_cluster, this->cluster, this->point_per_cluster, this->w);	
		
		// Exit once convergence is reached
		++iteration;
		if(iteration > this->n_iteration)
			break;

		changed = 0; // stop when 99.9% of points are good 
		// Reassign points to clusters
#pragma omp parallel for reduction(+:changed)
		for(int k = 0; k < this->n_point; ++k)
		{
			float best_distance = std::numeric_limits<float>::max();
			int best_centroid = -1;
			for(int i = 0; i < this->n_cluster; ++i)
			{
				//float x = point.x[k] - centroid.x[i];
				//float y = point.y[k] - centroid.y[i];
				//float z = point.z[k] - centroid.z[i];
				float distance = (this->point.dim == 3) ? (this->met.*dist)(this->point.x[k], centroid.x[i], this->point.y[k], centroid.z[k], this->point.z[k], centroid.z[i]) : (this->met.*dist)(this->point.x[k], centroid.x[i], this->point.y[k], centroid.z[k]);
				if(distance < best_distance)
				{
					best_distance = distance;
					best_centroid = i;
				}
			}
			changed += (cluster[k] != best_centroid) ? 1 : 0;
			this->cluster[k] = best_centroid;
		}
		if(changed <= (this->n_point >> 10))
			break;
	}
	
	if(time)
		std::cout << "K-Means over " << this->n_point << " points and " << this->n_cluster << " clusters and " << this->n_iteration << " iterations takes " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() << " sec" << std::endl;

#ifdef _DEBUG
	std::set<int> u_cluster;
	for(int i = 0; i < n_point; ++i)
		u_cluster.insert(cluster[i]);
	assert((int)u_cluster.size() == n_cluster);
	u_cluster.clear();
#endif

	std::memcpy(centroid.x, this->centroid.x, n_cluster*sizeof(float));
	std::memcpy(centroid.y, this->centroid.y, n_cluster*sizeof(float));
	if(point.dim == 3)
		std::memcpy(centroid.z, this->centroid.z, n_cluster*sizeof(float));

	return cluster;
}






