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

struct
{
	template<typename T> T** operator()(T **data, const int &Nrow, const int &Ncol, const int &Ncomp, bool axis = false)
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
} pca;

/*
struct
{
	template<typename T> T** operator()(T **data, const int &Nrow, const int &Ncol, const int &Ncomp, bool type = false, bool axis = false, const int &max_iter = 100, unsigned int seed = 0)
	{
		int it = 0;
		T 	G, 
			Gp, 
			delta = std::numeric_limits<T>::max(), 
			tol = (T)1e-6,
			norm;
		T ** W = new T[Ncomp], **Wlast = new T[Ncomp], **Sk = new T[Ncomp], **Zc = nullptr; // TO INIT ZC
		T *mean = new T[Nrow], tmp;
		T ** components = nullptr; // TO INIT
		std::generate(W, W + Ncomp, [&Nrow]{return new T[Nrow];});
		std::generate(Wlast, Wlast + Ncomp, [&Nrow]{return new T[Nrow];});
		std::generate(Sk, Sk + Ncomp, [&Ncol]{return new T[Ncol];});

		srand(seed);
		for(int i = 0; i < Ncomp; ++i)
		{
			std::generate(W[i], W[i] + Nrow, []{return static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX);});
			norm = sqrt(std::inner_product(W[i], W[i] + Nrow, (T)0.));
			std::transform(W[i], W[i] + Nrow, W[i], [&nrom](const T &w){return w /norm;});
		}
		std::transform(data, data + Nrow, 
						mean, [&Ncol](T *d)
						{
							return std::accumulate(d, d + Ncol, (T)0.)/Ncol;
						});

		while(delta > tol && it < max_iter)
		{
			++it;
			for(int i = 0; i < Ncomp; ++i)
			{
				std::memcpy(Wlast[i], W[i], sizeof(T)*Nrow);
				std::memset(W, (T)0., sizeof(T)*Nrow);
				int j = 0;
				Sk[i] = std::inner_product(Wlast[i], Wlast[i] + Nrow,
										   Zc, (T)0., std::plus<T>(),
										   [&j](const T &wl, T *zc) mutable
										   {
										   	return wl * zc[j++];
										   });
			}
			if(type) // Kurtosis
			{
				T Gp;
				for(int i = 0; i < Ncomp; ++i)
				{
					Gp = std::accumulate(Sk[i], Sk[i] + Ncol, (T)0., [](const T &res, const T &s){return 12*s*s;}) / Ncol;
					int j = 0;
					W[i] = std::accumulate(Zc, Zc + Nrow, (T)0.,
										   [&Sk, &i, &j](const T &res, T *zc) mutable
										   {
										   	return res + 12 * Sk[i][j] * Sk[i][j] * Sk[i][j] * zc[j++];
										   })
					std::transform(W[i], W[i] + Nrow, W[i], [&Ncol](const T &w){return w /Ncol;});
					std::transform(Wlast[i], Wlast[i] + Nrow, W[i], W[i], [&Gp](const T &wl, const T &w){return w - wl * Gp;});
					norm = sqrt(std::inner_product(W[i], W[i] + Nrow, (T)0.));
					std::transform(W[i], W[i] + Nrow, W[i], [&norm](const T &w){return w / norm;});
				}
			}
			else //Negentropy
			{
				T Gp;
				for(int i = 0; i < Ncomp; ++i)
				{
					Gp = std::accumulate(Sk[i], Sk[i] + Ncol, (T)0., [](const T &res, const T &s){return ((T)1. - s * s) * exp(-(T).5*s*s);}) / Ncol;
					int j = 0;
					W[i] = std::accumulate(Zc, Zc + Nrow, (T)0.,
										   [&Sk, &i, &j](const T &res, T *zc) mutable
										   {
										   	return res + Sk[i][j] * exp((T).5 * Sk[i][j] * Sk[i][j]) * zc[j++];
										   })
					std::transform(W[i], W[i] + Nrow, W[i], [&Ncol](const T &w){return w /Ncol;});
					std::transform(Wlast[i], Wlast[i] + Nrow, W[i], W[i], [&Gp](const T &wl, const T &w){return w - wl * Gp;});
					norm = sqrt(std::inner_product(W[i], W[i] + Nrow, (T)0.));
					std::transform(W[i], W[i] + Nrow, W[i], [&norm](const T &w){return w / norm;});
				}
			}

			delta = std::numeric_limits<T>::min();
			for(int i = 0; i < Ncomp; ++i)
			{
				tmp = std::inner_product(W[i], W[i] + Nrow, Wlast, (T)0.);
				delta = (tmp > delta) ? tmp : delta;
			}
		}
		//components = W * Zc;

		return components;
	}
} ica;
*/
