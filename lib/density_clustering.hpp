

template<typename Dist> int* DC::operator()(const Point &point, Point &centroid, Dist dist, float control, float ray, bool time)
{
	auto start = std::chrono::steady_clock::now();
	bool bool_new;
	int *cluster = new int[point.n], idx_i, idx_j, best_centroid, *density = new int[point.n];
	float *delta = new float[point.n], distance, best_distance;
	std::vector<int> idx_cluster_center;

	std::fill_n(delta, point.n, std::numeric_limits<float>::max());
	std::memset(density, 0, sizeof(int)*point.n);
	
	for(int i = 0; i < point.n; ++i)
		for(int j = i+1; j < point.n; ++j)
			if( (distance = (point.dim == 3) ? 
							dist(point.x[i], point.x[j], point.y[i], point.y[j], point.z[i], point.z[j]) : 
							dist(point.x[i], point.x[j], point.y[i], point.y[j])
							) < control)
			{
				++density[j];
				++density[i];
			}
	cluster = argsort<false>(density, 0, point.n); // stable sort (high parallel sensitivity)
#pragma omp parallel for private(idx_i, idx_j, distance)
	for(int i = 0; i < point.n; ++i)
	{
		idx_i = cluster[i];
		for(int j = i+1; j < point.n; ++j)
		{
			idx_j = cluster[j];
			distance = (point.dim == 3) ? 
						dist(point.x[idx_i], point.x[idx_j], point.y[idx_i], point.y[idx_j], point.z[idx_i], point.z[idx_j]) :
						dist(point.x[idx_i], point.x[idx_j], point.y[idx_i], point.y[idx_j]);
			delta[idx_i] = (distance < delta[idx_i]) ? distance : delta[idx_i];
		}
		delta[idx_i] = -delta[idx_i]; //- for right indices to sort
	}
	cluster = argsort<true>(delta, 0, point.n);

	delete[] delta;
	idx_cluster_center.push_back(cluster[0]);

	for(int i = 1; i < point.n; ++i)
	{
		bool_new = true;
		idx_i = cluster[i];
		for(const auto &idx_j : idx_cluster_center)
			bool_new *= ( (point.dim == 3) ? 
							dist(point.x[idx_i], point.x[idx_j], point.y[idx_i], point.y[idx_j], point.z[idx_i], point.z[idx_j]) : 
							dist(point.x[idx_i], point.x[idx_j], point.y[idx_i], point.y[idx_j])
							 > ray);
		if(bool_new) idx_cluster_center.push_back(cluster[i]);
	}

	centroid = Point((int)idx_cluster_center.size());

	for(int i = 0; i < centroid.n; ++i)
	{
		centroid.x[i] = point.x[idx_cluster_center[i]]; 
		centroid.y[i] = point.y[idx_cluster_center[i]];
		if(point.dim == 3)
			centroid.z[i] = point.z[idx_cluster_center[i]];
	}

#pragma omp parallel for private(best_distance, best_centroid, distance)
	for(int k = 0; k < point.n; ++k)
	{
		best_distance = std::numeric_limits<float>::max();
		best_centroid = -1;
		for(int i = 0; i < centroid.n; ++i)
		{
			distance = (point.dim == 3) ? 
						dist(point.x[k], centroid.x[i], point.y[k], centroid.y[i], point.z[k], centroid.z[i]) : 
						dist(point.x[k], centroid.x[i], point.y[k], centroid.y[i]);
			if(distance < best_distance)
			{
				best_distance = distance;
				best_centroid = i;
			}
		}
		cluster[k] = best_centroid;
	}
	if(time)
		std::cout << "DensityClustering over " << point.n << " points, found " << centroid.n << " clusters in " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() << " sec" << std::endl;

	return cluster;
}

