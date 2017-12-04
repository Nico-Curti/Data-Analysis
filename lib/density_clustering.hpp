#include "clustering.hpp"

densityclustering::densityclustering(const Point &point, std::string metric, int pow)
{
	this->n_point = point.n;
	this->point = point;
	this->met.pow = pow;
	if(metric != "euclidean" && metric != "manhattan" && metric != "minkowsy") {std::cerr << "Invalid metric! Given : " << metric << ". Possible values are 'euclidean', 'manhattan', 'minkowsy'." << std::endl; exit(1);}
	this->dist = (metric == "euclidean") ? &metrics::euclidean : (metric == "manhattan") ? &metrics::manhattan : &metrics::manhattan;
	this->density = new int[n_point];
}
densityclustering::~densityclustering()
{
	delete[] this->density;
	this->dist = nullptr;
}

int* densityclustering::DC(Point &centroid, float control, float ray, bool time)
{
	auto start = std::chrono::steady_clock::now();
	bool bool_new;
	int *cluster = new int[this->n_point], idx_i, idx_j, best_centroid;
	float *delta = new float[this->n_point], distance, best_distance;
	std::vector<int> idx_cluster_center;
	control = (control == 0.f) ? 1e-4f : control;
	ray = (ray == 0.f) ? 1e-1f : ray;

	std::memset(density, 0, sizeof(int)*this->n_point);
	std::iota(cluster, cluster + this->n_point, 0);
	
#pragma omp parallel for
	for(int i = 0; i < this->n_point; ++i)
		for(int j = i+1; j < this->n_point; ++j)
		{
			distance = (this->point.dim == 3) ? (met.*dist)(this->point.x[i], this->point.x[j], this->point.y[i], this->point.y[j], this->point.z[i], this->point.z[j]) : (met.*dist)(this->point.x[i], this->point.x[j], this->point.y[i], this->point.y[j]);
			if(distance < control)
			{
				this->density[j] += 1;
				this->density[i] += 1;
			}
		}

	argsort(cluster, this->density, this->n_point);

	delete[] this->density;
#pragma omp parallel for
	for(int i = 0; i < this->n_point; ++i)
	{
		idx_i = cluster[i];
		cluster[i] = i;
		delta[idx_i] = std::numeric_limits<float>::max();
		for(int j = i+1; j < n_point; ++j)
		{
			idx_j = cluster[j];
			distance = (this->point.dim == 3) ? (met.*dist)(this->point.x[idx_i], this->point.x[idx_j], this->point.y[idx_i], this->point.y[idx_j], this->point.z[idx_i], this->point.z[idx_j]) : (met.*dist)(this->point.x[idx_i], this->point.x[idx_j], this->point.y[idx_i], this->point.y[idx_j]);
			delta[idx_i] = (distance < delta[idx_i]) ? distance : delta[idx_i];
		}
		delta[idx_i] = -delta[idx_i]; //- per avere i valori giusti da sortare dopo
		delta[idx_i] = (i == n_point) ? INT_MIN : delta[idx_i];
	}

	argsort(cluster, delta, this->n_point);

	delete[] delta;
	idx_cluster_center.push_back(cluster[0]);

//#pragma omp parallel for
	for(int i = 1; i < n_point; ++i)
	{
		bool_new = true;
		idx_i = cluster[i], n_cluster = (int)idx_cluster_center.size();
		for(int j = 0; j < n_cluster; ++j)
		{
			idx_j = idx_cluster_center[j];
			distance = (this->point.dim == 3) ? (met.*dist)(this->point.x[idx_i], this->point.x[idx_j], this->point.y[idx_i], this->point.y[idx_j], this->point.z[idx_i], this->point.z[idx_j]) : (met.*dist)(this->point.x[idx_i], this->point.x[idx_j], this->point.y[idx_i], this->point.y[idx_j]);
			bool_new *= distance > ray;  
		}
		if(bool_new)
			idx_cluster_center.push_back(cluster[i]);
	}

	centroid = Point((int)idx_cluster_center.size());

	for(int i = 0; i < centroid.n; ++i)
	{
		std::cout << idx_cluster_center[i] << std::endl;
		centroid.x[i] = this->point.x[idx_cluster_center[i]]; 
		centroid.y[i] = this->point.y[idx_cluster_center[i]];
		if(this->point.dim == 3)
			centroid.z[i] = this->point.z[idx_cluster_center[i]];
	}

#pragma omp parallel for
	for(int k = 0; k < n_point; ++k)
	{
		best_distance = std::numeric_limits<float>::max();
		best_centroid = -1;
		for(int i = 0; i < centroid.n; ++i)
		{
			//float x = this->point.x[k] - centroid.x[i];
			//float y = this->point.y[k] - centroid.y[i];
			//float z = this->point.z[k] - centroid.z[i];
			distance = (point.dim == 3) ? (met.*dist)(this->point.x[k], centroid.x[i], this->point.y[k], centroid.y[i], this->point.z[k], centroid.z[i]) : (met.*dist)(this->point.x[k], centroid.x[i], this->point.y[k], centroid.y[i]);
			if(distance < best_distance)
			{
				best_distance = distance;
				best_centroid = i;
			}
		}
		cluster[k] = best_centroid;
	}
	if(time)
		std::cout << "DensityClustering over " << this->n_point << " points, found " << centroid.n << " clusters in " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() << " sec" << std::endl;

	return cluster;
}

