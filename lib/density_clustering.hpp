
densityclustering::densityclustering(const Point &point)
{
	this->n_point = point.n;
	this->point = point;
	this->density = new int[n_point];
}
densityclustering::~densityclustering()
{
	delete[] this->density;
}

// TO FIX
template<typename Dist> int* densityclustering::DensityClustering(Point &centroid, Dist dist, float control, float ray, bool time)
{
	auto start = std::chrono::steady_clock::now();
	bool bool_new;
	int *cluster = new int[this->n_point], idx_i, idx_j, best_centroid, n_cluster;
	float *delta = new float[this->n_point], distance, best_distance;
	std::vector<int> idx_cluster_center;

	std::memset(density, 0, sizeof(int)*this->n_point);
	std::iota(cluster, cluster + this->n_point, 0);
	
	for(int i = 0; i < this->n_point; ++i)
		for(int j = i+1; j < this->n_point; ++j)
		{
			distance = (this->point.dim == 3) ? dist(this->point.x[i], this->point.x[j], this->point.y[i], this->point.y[j], this->point.z[i], this->point.z[j]) : dist(this->point.x[i], this->point.x[j], this->point.y[i], this->point.y[j]);
			if(distance < control)
			{
				this->density[j] += 1;
				this->density[i] += 1;
			}
		}

	cluster = argsort<false>(this->density, 0, this->n_point);

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
			distance = (this->point.dim == 3) ? dist(this->point.x[idx_i], this->point.x[idx_j], this->point.y[idx_i], this->point.y[idx_j], this->point.z[idx_i], this->point.z[idx_j]) : dist(this->point.x[idx_i], this->point.x[idx_j], this->point.y[idx_i], this->point.y[idx_j]);
			delta[idx_i] = (distance < delta[idx_i]) ? distance : delta[idx_i];
		}
		delta[idx_i] = -delta[idx_i]; //- for right indices to sort
		delta[idx_i] = (i == n_point) ? INT_MIN : delta[idx_i];
	}

	cluster = argsort<false>(delta, 0, this->n_point);

	delete[] delta;
	idx_cluster_center.push_back(cluster[0]);

	for(int i = 1; i < n_point; ++i)
	{
		bool_new = true;
		idx_i = cluster[i];
		n_cluster = (int)idx_cluster_center.size();
		for(int j = 0; j < n_cluster; ++j)
		{
			idx_j = idx_cluster_center[j];
			distance = (this->point.dim == 3) ? dist(this->point.x[idx_i], this->point.x[idx_j], this->point.y[idx_i], this->point.y[idx_j], this->point.z[idx_i], this->point.z[idx_j]) : dist(this->point.x[idx_i], this->point.x[idx_j], this->point.y[idx_i], this->point.y[idx_j]);
			bool_new *= distance > ray;  
		}
		if(bool_new)
			idx_cluster_center.push_back(cluster[i]);
	}

	centroid = Point((int)idx_cluster_center.size());

	for(int i = 0; i < centroid.n; ++i)
	{
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
			distance = (point.dim == 3) ? dist(this->point.x[k], centroid.x[i], this->point.y[k], centroid.y[i], this->point.z[k], centroid.z[i]) : dist(this->point.x[k], centroid.x[i], this->point.y[k], centroid.y[i]);
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

