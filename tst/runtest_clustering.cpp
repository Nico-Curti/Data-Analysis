#include "clustering.hpp"



int main(int argc, char *argv[])
{
	int n_point = 150, //1474560, 
		n_cluster = 10, //1024,
		n_iteration = 10,
		n_dim = 2,
		*index_km = nullptr,
		*index_dc = nullptr;
	float 	radius = 10.f, 
			control = .1f,
			ray = .5f;

	Point 	points,
			centroid(n_cluster, n_dim);
	points.polar_gen(n_point, radius, 123);

	// Kmeans clustering

	kmean KM(points, n_point, n_cluster, n_iteration);
	KM.kpp_centroid(euclidean, 123, true);	
	index_km = KM.Kmean(centroid, euclidean, middle_point, true);

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

	densityclustering DC(points);
	index_dc = DC.DensityClustering(centroid, euclidean, control, ray, true);
	
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

