#include "clustering.hpp"



int main(int argc, char *argv[])
{
	int n_point = 150, 
		n_cluster = 10,
		n_iteration = 10,
		n_dim = 2,
		*index_km = nullptr,
		*index_dc = nullptr;
	float 	radius = 10.f, 
			control = 1.5f,
			ray = 20.5f;

	Point 	points,
			centroid(n_cluster, n_dim);
	points.polar_gen(n_point, radius, 123);

	// Kmeans clustering

	index_km = kmean(points, centroid, euclidean, middle_point, kpp, n_cluster, n_iteration, nullptr, 123, true);

	std::map<int, int> point_per_cluster;
	for(int i = 0; i < n_point; ++i) ++point_per_cluster[index_km[i]];

	std::cout << "KMeans clustering:" << std::endl;
	for(const auto &i : point_per_cluster) std::cout << "cluster " << i.first << " : " << i.second << " points" << std::endl;
	std::cout << std::endl;

	if(os::dir_exists("tst/res"))
	{
		std::ofstream os("tst/res/kmean.cluster");
		for(int i = 0; i < n_dim; ++i) os << "dim" << i << "\t"; os << "label" << std::endl; //header
		for(int i = 0; i < n_point; ++i)
		{
			os << points.x[i] << "\t" << points.y[i] << "\t";
			os << index_km[i] << std::endl;
		}
		os.close();
	}

	// Density Clustering

	index_dc = densityclustering(points, centroid, euclidean, control, ray, true);
	
	point_per_cluster.clear();
	for(int i = 0; i < n_point; ++i) ++point_per_cluster[index_dc[i]];
		
	std::cout << "Density clustering:" << std::endl;
	for(const auto &i : point_per_cluster) std::cout << "cluster " << i.first << " : " << i.second << " points" << std::endl;
	std::cout << std::endl;
	
	if(os::dir_exists("tst/res"))
	{
		std::ofstream os("tst/res/densityclustering.cluster");
		for(int i = 0; i < n_dim; ++i) os << "dim" << i << "\t"; os << "label" << std::endl; //header
		for(int i = 0; i < n_point; ++i)
		{
			os << points.x[i] << "\t" << points.y[i] << "\t";
			os << index_dc[i] << std::endl;
		}
		os.close();
	}

	return 0;
}

