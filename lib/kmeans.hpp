
kmean::kmean(const Point &point, const int &n_point, const int &n_cluster, const int &n_iteration, float *point_weight)
{
	this->n_point = n_point;
	this->n_cluster = n_cluster;
	this->n_iteration = n_iteration;
	this->point = point;

	this->centroid = Point(n_cluster);
	this->cluster = new int[n_point];
	this->point_per_cluster = new int[n_cluster];
	this->w = new float[n_point];
	if(point_weight != nullptr)
		std::memcpy(this->w, point_weight, sizeof(float)*this->n_point);
	else
		std::fill_n(w, this->n_point, 1.f);
}

kmean::~kmean()
{
	delete[] this->cluster;
	delete[] this->point_per_cluster;
	delete[] this->w;
}


template<typename Dist> inline int kmean::nearest(const int &lbl, const Point &centroid, const int &n_cluster, float *d2, Dist dist, const float &pt_x, const float &pt_y, float pt_z)
{
	int min_index = lbl;
	float d, min_dist = std::numeric_limits<float>::max();

	if(pt_z != 0.f)
	{
		for(int i = 0; i < n_cluster; ++i)
			if(min_dist > (d = dist(pt_x, centroid.x[i], pt_y, centroid.y[i], pt_z, centroid.z[i])))
				{ min_dist = d; min_index = i;}
	}
	else
	{
		for(int i = 0; i < n_cluster; ++i)
			if(min_dist > (d = dist(pt_x, centroid.x[i], pt_y, centroid.y[i])))
				{ min_dist = d; min_index = i;}
	}

	if(d2) *d2 = min_dist;
	return min_index;
}


template<typename Dist> void kmean::kpp_centroid(Dist dist, unsigned int seed, bool time)
{
	auto start = std::chrono::steady_clock::now();
	int cl, idx;
	float sum, *d = new float[n_point];
	std::mt19937 engine{seed};
	std::uniform_real_distribution<float> r_dist{0.f, 1.f};
	idx = (int)r_dist(engine)*n_point;
	this->centroid.x[0] = this->point.x[idx]; this->centroid.y[0] = this->point.y[idx];
	if(this->point.dim == 3) this->centroid.z[0] = this->point.z[idx];

	for(cl = 1; cl < this->n_cluster; ++cl)
	{
		sum = 0.f;
#pragma omp parallel for reduction(+:sum)
		for(int j = 0; j < this->n_point; ++j)
		{
			if(point.dim == 3)
				nearest(this->cluster[j], this->centroid, cl, d + j, dist, this->point.x[j], this->point.y[j], this->point.z[j]);
			else
				nearest(this->cluster[j], this->centroid, cl, d + j, dist, this->point.x[j], this->point.y[j]);
			sum += d[j];
		}
		
		sum *= r_dist(engine);

		for(int j = 0; j < n_point; ++j)
		{
			if((sum -= d[j]) > 0) continue;
			this->centroid.x[cl] = this->point.x[j];
			this->centroid.y[cl] = this->point.y[j];
			if(this->point.dim == 3)
				this->centroid.z[cl] = this->point.z[j];
			break;
		}
	}

#pragma omp parallel for
	for(int j = 0; j < this->n_point; ++j)
	{
		if(this->point.dim == 3)
			this->cluster[j] = nearest(this->cluster[j], this->centroid, cl, 0, dist, this->point.x[j], this->point.y[j], this->point.z[j]);
		else
			this->cluster[j] = nearest(this->cluster[j], this->centroid, cl, 0, dist, this->point.x[j], this->point.y[j]);
	}
	
	delete[] d;

#ifdef _DEBUG
	std::set<int> u_cluster;
	for(int i = 0; i < this->n_point; ++i)
		u_cluster.insert(this->cluster[i]);
	assert((int)u_cluster.size() == n_cluster);
	u_cluster.clear();
#endif

	if(time)
		std::cout << "Initialization of " << this->n_cluster << " kpp cluster took " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() << " sec" << std::endl;

	return;
}

void kmean::random_centroid(unsigned int seed, bool time)
{
	auto start = std::chrono::steady_clock::now();
	std::mt19937 engine{seed};
	std::uniform_int_distribution<int> i_dist{0, 1};
	std::generate(this->cluster, this->cluster + this->n_point, [&engine, &i_dist]{return i_dist(engine);});
	if(time)
		std::cout << "Initialization of " << this->n_cluster << " random cluster took " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() << " sec" << std::endl;

	return;
}


template<typename Dist, typename Cl_Center> int* kmean::Kmean(Point &centroid, Dist dist, Cl_Center cl, bool time)
{
	auto start = std::chrono::steady_clock::now();
	int iteration = 0, changed;

	while(true)
	{
		//Compute the centroid of the clusters
		cl(this->point, centroid, this->n_point, this->n_cluster, this->cluster, this->point_per_cluster, this->w);	
		
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
				float distance = (this->point.dim == 3) ? dist(this->point.x[k], centroid.x[i], this->point.y[k], centroid.y[i], this->point.z[k], centroid.z[i]) : dist(this->point.x[k], centroid.x[i], this->point.y[k], centroid.y[i]);
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






