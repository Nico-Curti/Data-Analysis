#include "fstat.hpp"
#include "fmath.hpp"
#include <numeric>

template<typename T> T** lanczos(T **mat, const int &Nrow, const int &Ncol, const int &Ncomp)
{
	int p = 0;
	T **eigen = new T*[Ncomp], *phi = new T[Nrow], err, proj, norm;
	std::generate(phi, phi + Nrow, []{return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);});
	while(p < Ncomp)
	{
		err = std::numeric_limits<T>::max();
		eigen[p] = new T[Nrow];
		while(err > (T)1e-6)
		{
			std::transform(mat, mat + Nrow, eigen[p], [&Nrow, &phi](T *c)
													 {
													 	return std::inner_product(c, c + Nrow, phi, (T)0.);
													 });
			if(p) // Gram-Schmidt
			{
				for(int i = 0; i < p; ++i)
				{
					proj = std::inner_product(eigen[p], eigen[p] + Nrow, eigen[i], (T)0., 
																					std::plus<T>(), 
																					[&](const T &v1, const T &v2)
																					{
																						return v1*v2;
																					});
					std::memcpy(phi, eigen[p], sizeof(T)*Nrow);
					std::transform(eigen[i], eigen[i] + Nrow, phi, phi, [&proj](const T &e, const T &p)
																		{
																			return p - proj*e;
																		});
				}
				norm = sqrt(std::inner_product(phi, phi + Nrow, phi, (T)0.));
				err = (T)0.;
				for(int i = 0; i < Nrow; ++i)
				{
					phi[i] /= norm;
					eigen[p][i] = phi[i];
					err += phi[i]*phi[i];
				}
			}
			else
			{
				norm = sqrt(std::inner_product(eigen[p], eigen[p] + Nrow, eigen[p], (T)0.));
				err = (T)0.;
				for (int i = 0; i < Nrow; ++i)
				{
					eigen[p][i] /= norm;
					phi[i] = eigen[p][i];
					err += phi[i] * phi[i];
				}
			}
			err = std::fabs(err - (T)1.);
		}
		++p;
	}
	delete[] phi;
	return eigen;
}

template<typename T> T** pca(T **data, const int &Nrow, const int &Ncol, const int &Ncomp, bool axis = false)
{
	T * mean = nullptr, **cov = nullptr, **eigen = nullptr, **components = new T*[Ncomp];
	if(axis) // pca of rows
	{
		mean = new T[Nrow];
		cov = cov_mat(data, Nrow, Ncol, axis, mean);
		eigen = lanczos(cov, Nrow, Nrow, Ncomp);
		std::generate(components, components + Ncomp, [&Ncol](){return new T[Ncol];});
		for(int i = 0; i < Ncomp; ++i)
			for(int j = 0; j < Ncol; ++j)
				components[i][j] = std::inner_product(data, data + Nrow, eigen[i],
													  (T)0., std::plus<T>(),
													  [mean, &j](T *d, const T &e) mutable
													  {
													  	return (d[j] - *mean++)*e;
													  });
		for(int i = 0; i < Nrow; ++i) delete[] cov[i];
	}
	else // pca of columns
	{
		mean = new T[Ncol];
		cov = cov_mat(data, Nrow, Ncol, axis, mean);
		eigen = lanczos(cov, Ncol, Ncol, Ncomp);
		std::generate(components, components + Ncomp, [&Nrow](){return new T[Nrow];});
		for(int i = 0; i < Ncomp; ++i)
			for(int j = 0; j < Nrow; ++j)
				components[i][j] = std::inner_product(data[j], data[j] + Ncol, eigen[i],
													  (T)0., std::plus<T>(),
													  [mean](const T &val, const T &e) mutable
													  {
													  	return (val - *mean++) * e;
													  });
		for(int i = 0; i < Ncol; ++i) delete[] cov[i];
	}
	
	for(int i = 0; i < Ncomp; ++i) delete[] eigen[i];
	delete[] eigen;
	delete[] mean;
	delete[] cov;
	return components;
}	

