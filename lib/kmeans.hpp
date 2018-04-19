

template<typename Dist> inline int Kpp::nearest(const int &lbl, const Point &centroid, const int &n_cluster, float *d2, Dist dist, const float &pt_x, const float &pt_y, float pt_z)
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


template<typename Dist> int* Kpp::operator()(const Point &point, Point &centroid, Dist dist, const int &n_cluster, unsigned int seed, bool time)
{
    auto start = std::chrono::steady_clock::now();
    int cl, idx, *cluster = new int[point.n];
    float sum, *d = new float[point.n];
    std::mt19937 engine{seed};
    std::uniform_real_distribution<float> r_dist{0.f, 1.f};
    idx = (int)r_dist(engine)*point.n;
    centroid.x[0] = point.x[idx]; centroid.y[0] = point.y[idx];
    if(point.dim == 3) centroid.z[0] = point.z[idx];

    for(cl = 1; cl < n_cluster; ++cl)
    {
        sum = 0.f;
#pragma omp parallel for reduction(+:sum)
        for(int j = 0; j < point.n; ++j)
        {
            if(point.dim == 3)
                nearest(cluster[j], centroid, cl, d + j, dist, point.x[j], point.y[j], point.z[j]);
            else
                nearest(cluster[j], centroid, cl, d + j, dist, point.x[j], point.y[j]);
            sum += d[j];
        }
        
        sum *= r_dist(engine);

        for(int j = 0; j < point.n; ++j)
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
    for(int j = 0; j < point.n; ++j)
    {
        if(point.dim == 3)
            cluster[j] = nearest(cluster[j], centroid, cl, 0, dist, point.x[j], point.y[j], point.z[j]);
        else
            cluster[j] = nearest(cluster[j], centroid, cl, 0, dist, point.x[j], point.y[j]);
    }
    
    delete[] d;

#ifdef _DEBUG
    std::set<int> u_cluster;
    for(int i = 0; i < point.n; ++i)
        u_cluster.insert(cluster[i]);
    assert((int)u_cluster.size() == n_cluster);
    u_cluster.clear();
#endif

    if(time)
        std::cout << "Initialization of " << n_cluster << " kpp cluster took " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() << " sec" << std::endl;

    return cluster;
}

template<typename Dist> int* RNG::operator()(const Point &point, Point &centroid, Dist dist, const int &n_cluster, unsigned int seed, bool time)
{
    int *cluster = new int[point.n];
    auto start = std::chrono::steady_clock::now();
    std::mt19937 engine{seed};
    std::uniform_int_distribution<int> i_dist{0, 1};
    std::generate(cluster, cluster + point.n, [&engine, &i_dist]{return i_dist(engine);});
    if(time)
        std::cout << "Initialization of " << n_cluster << " random cluster took " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() << " sec" << std::endl;

    return cluster;
}

template<typename Dist, typename Cl_center, typename Init> 
int* Kmean::operator()(const Point &point, Point &centroid, Dist dist, Cl_center cl, Init init, const int &n_cluster, const int &n_iteration, float *point_weight, unsigned int seed, bool time)
{
    auto start = std::chrono::steady_clock::now();
    int iteration = 0, 
        changed,
        *cluster = init(point, centroid, dist, n_cluster, seed, time),
        *point_per_cluster = new int[n_cluster];
    if(point_weight == nullptr)
    {
        point_weight = new float[point.n];
        std::fill_n(point_weight, point.n, 1.f);
    }
    while(true)
    {
        //Compute the centroid of the clusters
        cl(point, centroid, point.n, n_cluster, cluster, point_per_cluster, point_weight);  
        
        // Exit once convergence is reached
        ++iteration;
        if(iteration > n_iteration)
            break;

        changed = 0; // stop when 99.9% of points are good 
        // Reassign points to clusters
#pragma omp parallel for reduction(+:changed)
        for(int k = 0; k < point.n; ++k)
        {
            float best_distance = std::numeric_limits<float>::max();
            int best_centroid = -1;
            for(int i = 0; i < n_cluster; ++i)
            {
                //float x = point.x[k] - centroid.x[i];
                //float y = point.y[k] - centroid.y[i];
                //float z = point.z[k] - centroid.z[i];
                float distance = (point.dim == 3) ? dist(point.x[k], centroid.x[i], point.y[k], centroid.y[i], point.z[k], centroid.z[i]) : dist(point.x[k], centroid.x[i], point.y[k], centroid.y[i]);
                if(distance < best_distance)
                {
                    best_distance = distance;
                    best_centroid = i;
                }
            }

    
            changed += (cluster[k] != best_centroid) ? 1 : 0;
            cluster[k] = best_centroid;
        }
        if(changed <= (point.n >> 10))
            break;
    }
    
    if(time)
        std::cout << "K-Means over " << point.n << " points and " << n_cluster << " clusters and " << n_iteration << " iterations takes " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() << " sec" << std::endl;

#ifdef _DEBUG
    std::set<int> u_cluster;
    for(int i = 0; i < point.n; ++i)
        u_cluster.insert(cluster[i]);
    assert((int)u_cluster.size() == n_cluster);
    u_cluster.clear();
#endif

    return cluster;
}







