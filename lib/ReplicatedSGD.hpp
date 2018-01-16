#pragma once

#include "os.hpp"
#include "parse_args.hpp"
#include "fmath.hpp"
#include "patterns.hpp"

#include <iomanip>
#include <climits>
#include <iterator>
#include <numeric>
#include <assert.h>

template<typename T> struct NetNK
{
	using type = T;
	int N, K, size;
	T *H, *DeltaH;
	int *J, *oldJ;
	NetNK();
	NetNK(const int &, const int &);
	~NetNK();
	NetNK(const NetNK &);
	NetNK& operator=(const NetNK &);
	inline int operator-(const NetNK<T> &);
};

template<typename T> struct parameters
{
	using type = T;
	int y;
	T eta, lambda, gamma, eta_factor, lambda_factor, gamma_step;
	parameters(const int &, const T &, const T &, const T &, const T &, const T &, const T &);
	~parameters() = default;
	parameters<T>& operator=(const parameters<T> &);
	parameters(const parameters<T> &);
};

template<class C,typename std::enable_if<std::is_same<C, NetNK<typename C::type>>::value, C>::type* = nullptr> inline void update(C &obj);
template<class C,typename std::enable_if<std::is_same<C, parameters<typename C::type>>::value, C>::type* = nullptr> inline void update(C &obj);


struct PatternPermutation
{
	int a, M, batch, *MM;
	PatternPermutation();
	PatternPermutation(const int &, const int &, const int &);
	~PatternPermutation();
	PatternPermutation(const PatternPermutation &);
	PatternPermutation& operator=(const PatternPermutation &);
	int getbatch(int *& );
};

static void menu_info();
template<typename T> void parse_training_args(const int &, char **, std::string &, int &, int &, int &, int &, T &, T &, T &, T &, T &, T &, bool &, bool &, bool &, std::string &, std::string &);
template<typename T> void parse_test_args(const int &, char **, std::string &, std::string &, std::string &);

namespace RSGD
{
	template<typename T> NetNK<T> mean_net(NetNK<T> *, const int &, const int &, const int &);
	template<typename T> int compute_errs(const NetNK<T> &net, const Patterns<T> &pattern);
	template<typename T> void subepoch(NetNK<T> &, const Patterns<T> &, PatternPermutation &, const parameters<T> &);
	template<typename T> inline T compute_kick(const std::string &, const T &);
	template<typename T> void kickboth(NetNK<T> &, NetNK<T> &, const parameters<T> &);
	template<typename T> void kickboth_traced(NetNK<T> &, NetNK<T> &, const parameters<T> &, const std::string &);
	template<typename T> void kickboth_traced_continuous(NetNK<T> &, NetNK<T> &, const parameters<T> &);

	int* read_weights(const std::string &, int &, int &, bool bin = false);
	template<typename T> void save_weights(const std::string &, const NetNK<T> &, const int &, const int &, const int &, const int &, const std::string, const T &, const T &, const T &, const T &, const T &, const T &);
	void save_weights(const std::string &, int *, const int &, const int &);

	template<typename T> int* RSGD(const Patterns<T> &, const int &, const int &, parameters<T> &, const int &, const std::string &, const int &, const int &, const bool &, const bool &, const bool &, std::string outfile = "", bool quiet = false);	

	template<typename T> int* train(const std::string &, int K = 1, int y = 1, int batch = 5, int max_epochs = 1000, T eta = (T)2., T lambda = (T).1, T gamma = std::numeric_limits<T>::infinity(), T eta_factor = (T)1., T lambda_factor = (T)1., T gamma_step = (T).01, std::string formula = "simple", bool waitcenter = false, bool equal = true, bool center = false, int seed = 202, std::string outfile = "");
	template<typename T> int* train(const Patterns<T> &, int K = 1, int y = 1, int batch = 5, int max_epochs = 1000, T eta = (T)2., T lambda = (T).1, T gamma = std::numeric_limits<T>::infinity(), T eta_factor = (T)1., T lambda_factor = (T)1., T gamma_step = (T).01, std::string formula = "simple", bool waitcenter = false, bool equal = true, bool center = false, int seed = 202, std::string outfile = "");

	template<typename T> int* test(const Patterns<T> &, int *, const int &, const int &);
	template<typename T> int* test(std::string &, std::string &);

	template<typename T> int compute_output(T *,  int *, const int &, const int &);
}



/**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-***_**
-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**--**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**_**
*_**																																														    *_**	
*_**    :::::::::::  ::::    ::::   :::::::::   :::         ::::::::::  ::::    ::::   ::::::::::  ::::    :::  :::::::::::      :::      :::::::::::  :::::::::::   ::::::::   ::::    :::		*_**	 
*_**        :+:      +:+:+: :+:+:+  :+:    :+:  :+:         :+:         +:+:+: :+:+:+  :+:         :+:+:   :+:      :+:        :+: :+:        :+:          :+:      :+:    :+:  :+:+:   :+:		*_**	 
*_**        +:+      +:+ +:+:+ +:+  +:+    +:+  +:+         +:+         +:+ +:+:+ +:+  +:+         :+:+:+  +:+      +:+       +:+   +:+       +:+          +:+      +:+    +:+  :+:+:+  +:+		*_**	 
*_**        +#+      +#+  +:+  +#+  +#++:++#+   +#+         +#++:++#    +#+  +:+  +#+  +#++:++#    +#+ +:+ +#+      +#+      +#++:++#++:      +#+          +#+      +#+    +:+  +#+ +:+ +#+		*_**	 
*_**        +#+      +#+       +#+  +#+         +#+         +#+         +#+       +#+  +#+         +#+  +#+#+#      +#+      +#+     +#+      +#+          +#+      +#+    +#+  +#+  +#+#+#		*_**	 
*_**        #+#      #+#       #+#  #+#         #+#         #+#         #+#       #+#  #+#         #+#   #+#+#      #+#      #+#     #+#      #+#          #+#      #+#    #+#  #+#   #+#+#		*_**	 
*_**    ###########  ###       ###  ###         ##########  ##########  ###       ###  ##########  ###    ####      ###      ###     ###      ###      ###########   ########   ###    ####		*_**	 
*_**																																															*_**			
-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**--**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**_**
-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**--**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**_**/


//======================================//
//  _   _          _     _   _   _  __  //
// | \ | |   ___  | |_  | \ | | | |/ /  //
// |  \| |  / _ \ | __| |  \| | | ' /   //
// | |\  | |  __/ | |_  | |\  | | . \   //
// |_| \_|  \___|  \__| |_| \_| |_|\_\  //
//======================================//



template<typename T> NetNK<T>::NetNK()
{
	this->N = 0; this->K = 0; this->size = 0;
	this->H = nullptr; this->DeltaH = nullptr; this->J = nullptr; this->oldJ = nullptr;
}

template<typename T> NetNK<T>::NetNK(const int &N, const int &K)
{
	this->N = N; this->K = K;
	this->size = this->N * this->K;
	this->H = new T[this->size];
	this->DeltaH = new T[this->size];
	this->J = new int[this->size];
	this->oldJ = new int[this->size];
}

template<typename T> NetNK<T>::~NetNK()
{
	if(this->N != 0)
	{
		delete[] this->H; 
		delete[] this->DeltaH; 
		delete[] this->J; 
		delete[] this->oldJ;
	}
}

template<typename T> NetNK<T>::NetNK(const NetNK &net)
{
	this->N = net.N; this->K = net.K;
	this->size = net.size;
	this->H = new T[this->size];
	this->DeltaH = new T[this->size];
	this->J = new int[this->size];
	this->oldJ = new int[this->size];
	std::memcpy(this->H, net.H, sizeof(T)*this->size);
	std::memcpy(this->DeltaH, net.DeltaH, sizeof(T)*this->size);
	std::memcpy(this->J, net.J, sizeof(int)*this->size);
	std::memcpy(this->oldJ, net.oldJ, sizeof(int)*this->size);
}

template<typename T> NetNK<T>& NetNK<T>::operator=(const NetNK<T> &net)
{
	if(this->N != 0)
	{
		delete[] H;
		delete[] DeltaH;
		delete[] J;
		delete[] oldJ;
	}
	this->N = net.N; this->K = net.K; this->size = net.size;
	this->H = new T[this->size];
	this->DeltaH = new T[this->size];
	this->J = new int[this->size];
	this->oldJ = new int[this->size];
	std::memcpy(this->H, net.H, sizeof(T)*this->size);
	std::memcpy(this->DeltaH, net.DeltaH, sizeof(T)*this->size);
	std::memcpy(this->J, net.J, sizeof(int)*this->size);
	std::memcpy(this->oldJ, net.oldJ, sizeof(int)*this->size);
	return *this;
}


template<typename T> inline int NetNK<T>::operator-(const NetNK<T> &net)
{
	int dot, sum, dist = 0;
	for(int i = 0; i < this->N; ++i)
	{
		dot = 0; sum = 0;
		for(int j = 0; j < this->K; ++j)
		{
			dot += this->J[i*this->K + j] * net.J[i*net.K + j];
			sum += this->J[i*this->K + j] + net.J[i*net.K + j];
		}
		dist += -2 * dot + sum;
	}
	return dist;
}

//=================================================================================//
//    ____                                              _                          //
//   |  _ \    __ _   _ __    __ _   _ __ ___     ___  | |_    ___   _ __   ___    //
//   | |_) |  / _` | | '__|  / _` | | '_ ` _ \   / _ \ | __|  / _ \ | '__| / __|   //
//   |  __/  | (_| | | |    | (_| | | | | | | | |  __/ | |_  |  __/ | |    \__ \   //
//   |_|      \__,_| |_|     \__,_| |_| |_| |_|  \___|  \__|  \___| |_|    |___/   //
//=================================================================================//


template<typename T> parameters<T>::parameters(const int &y, const T &eta, const T &lambda, const T &gamma, const T &eta_factor, const T &lambda_factor, const T &gamma_step)
{
	this->y = y;
	this->eta = eta;
	this->lambda = lambda;
	this->gamma = gamma;
	this->eta_factor = eta_factor;
	this->lambda_factor = lambda_factor;
	this->gamma_step = gamma_step;
}

template<typename T> parameters<T>& parameters<T>::operator=(const parameters<T> &p)
{
	this->y = p.y;
	this->eta = p.eta;
	this->lambda = p.lambda;
	this->gamma = p.gamma;
	this->eta_factor = p.eta_factor;
	this->lambda_factor = p.lambda_factor;
	this->gamma_step = p.gamma_step;
}

template<typename T> parameters<T>::parameters(const parameters<T> &p)
{
	this->y = p.y;
	this->eta = p.eta;
	this->lambda = p.lambda;
	this->gamma = p.gamma;
	this->eta_factor = p.eta_factor;
	this->lambda_factor = p.lambda_factor;
	this->gamma_step = p.gamma_step;
}

//=====================================================================================================================//
//                                                              __                          _     _                    //
//     ___    ___    _ __ ___    _ __ ___     ___    _ __      / _|  _   _   _ __     ___  | |_  (_)   ___    _ __     //
//    / __|  / _ \  | '_ ` _ \  | '_ ` _ \   / _ \  | '_ \    | |_  | | | | | '_ \   / __| | __| | |  / _ \  | '_ \    //
//   | (__  | (_) | | | | | | | | | | | | | | (_) | | | | |   |  _| | |_| | | | | | | (__  | |_  | | | (_) | | | | |   //
//    \___|  \___/  |_| |_| |_| |_| |_| |_|  \___/  |_| |_|   |_|    \__,_| |_| |_|  \___|  \__| |_|  \___/  |_| |_|   //
//                                                                                                                     //
//=====================================================================================================================//

template<class C,typename std::enable_if<std::is_same<C, NetNK<typename C::type>>::value, C>::type* = nullptr> inline void update(C &obj)
{
	using T = typename C::type;
	#pragma omp parallel for
	for(int i = 0; i < obj.size; ++i)
	{
		obj.H[i] += obj.DeltaH[i];
		obj.J[i] = (obj.H[i] > (T)0.) ? 1 : 0;
	}
	return;
}
template<class C,typename std::enable_if<std::is_same<C, parameters<typename C::type>>::value, C>::type* = nullptr> inline void update(C &obj)
{
	obj.eta *= obj.eta_factor;
	obj.lambda *= obj.lambda_factor;
	obj.gamma += obj.gamma_step;
	return;
}


//===========================================================================================//
//                    _     _                                                                //
//    _ __     __ _  | |_  | |_    ___   _ __   _ __      _ __     ___   _ __   _ __ ___     //
//   | '_ \   / _` | | __| | __|  / _ \ | '__| | '_ \    | '_ \   / _ \ | '__| | '_ ` _ \    //
//   | |_) | | (_| | | |_  | |_  |  __/ | |    | | | |   | |_) | |  __/ | |    | | | | | |   //
//   | .__/   \__,_|  \__|  \__|  \___| |_|    |_| |_|   | .__/   \___| |_|    |_| |_| |_|   //
//   |_|                                                 |_|                                 //
//===========================================================================================//

PatternPermutation::PatternPermutation()
{ this->MM = nullptr; this->a = 0; this->M = 0; this->batch = 0;}

PatternPermutation::PatternPermutation(const int &a, const int &M, const int &batch)
{
	this->a = a; this->M = M; this->batch = batch;
	this->MM = new int[this->M];
	std::iota(this->MM, this->MM + this->M, 0);
	std::random_shuffle(this->MM, this->MM + this->M);
}

PatternPermutation::~PatternPermutation()
{ delete[] this->MM; }

PatternPermutation::PatternPermutation(const PatternPermutation &patt)
{
	this->a = patt.a; this->M = patt.M; this->batch = patt.batch;
	this->MM = new int[this->M];
	std::memcpy(this->MM, patt.MM, this->M*sizeof(int));
}

PatternPermutation& PatternPermutation::operator=(const PatternPermutation &patt)
{
	if(this->M != 0) delete[] this->MM;
	this->a = patt.a; this->M = patt.M; this->batch = patt.batch;
	this->MM = new int[this->M];
	std::memcpy(this->MM, patt.MM, this->M*sizeof(int));
	return *this;
}

int PatternPermutation::getbatch(int *&perm )
{
	int b = std::min(this->a + this->batch - 1, this->M);
	if(b == this->M)
	{
		std::random_shuffle(this->MM, this->MM + this->M);
		this->a = 1;
	}
	else
		this->a = b + 1;
	perm = new int[b - a + 1];
	std::copy(this->MM + this->a - 1, this->MM + b, perm);
	return b - a + 1;
}


namespace RSGD
{
	template<typename T> NetNK<T> mean_net(NetNK<T> *nets, const int &n_nets, const int &N, const int &K)
	{
		NetNK<T> netmean(N, K);
		for(int i = 0; i < netmean.size; ++i)
		{
			netmean.H[i] = 2 * std::accumulate(nets, nets + n_nets, 0, [&i](const int &a, const NetNK<T> &b){return a + b.J[i];});
			netmean.DeltaH[i] = (T)0.;
			//netmean.J[i] = 0;
			//netmean.oldJ[i] = 0;
			//for(int j = 0; j < n_nets; ++j)
			//	netmean.H[i] += nets[j].J[i];
			netmean.H[i] /= /*2 * netmean.H[i] /*/ n_nets - (T)1.;
			netmean.J[i] = (netmean.H[i] > (T)0.) ? 1 : 0;
			netmean.oldJ[i] = netmean.J[i];
		}
		return netmean;
	}

	template<typename T> int compute_errs(const NetNK<T> &net, const Patterns<T> &pattern)
	{
		int errs = 0, dot, sum, h_out, t_out;
		for(int i = 0; i < pattern.Nrow; ++i)
		{
			h_out = 0;
			for(int j = 0; j < net.K; ++j)
			{
				dot = 0; sum = 0;
				for(int k = 0; k < net.N; ++k)
				{
					dot += pattern.input[i][k] * net.J[j*net.N + k];
					sum += pattern.input[i][k] + net.J[j*net.N + k];
				}

				h_out += ((4 * dot - 2 * sum + net.N) > 0) ? 1 : -1;
			}
			t_out = (h_out > 0) ? 1 : -1;
			errs += (t_out != pattern.output[i]);
		}
		return errs;
	}

	template<typename T> void subepoch(NetNK<T> &net, const Patterns<T> &pattern, PatternPermutation &patt_perm, const parameters<T> &params)
	{
		int *Mbatch = nullptr, size_mb = patt_perm.getbatch(Mbatch), dot, sum, *h = new int[net.K], t_out, h_out, tofix, tmp;
		std::vector<int> wrongh, indh;

		std::memset(net.DeltaH, 0, sizeof(T)*net.size);
		for(int i = 0; i < size_mb; ++i)
		{
			h_out = 0;
			for(int j = 0; j < net.K; ++j)
			{
				dot = 0; sum = 0;
				for(int k = 0; k < net.N; ++k)
				{
					dot += pattern.input[Mbatch[i]][k] * net.J[j*net.N + k];
					sum += pattern.input[Mbatch[i]][k] + net.J[j*net.N + k];
				}
				h[i] = 4 * dot - 2 * sum + net.N;
				h_out += (h[i] > 0) ? 1 : -1;
			}
			t_out = (h_out > 0) ? 1 : -1;
			if(t_out != pattern.output[Mbatch[i]])
			{
				// compute_gd
				tofix = (int)(T(-pattern.output[Mbatch[i]] * h_out + 1) / 2), tmp;
				for(int j = 0; j < net.K; ++j)
				{
					tmp = h[i] * pattern.output[Mbatch[i]];
					if(tmp > 0) continue;
					wrongh.push_back(-tmp);
					indh.push_back(i);
				}
				std::sort(indh.begin(), indh.end(), [&wrongh](int a, int b) {return wrongh[a] < wrongh[b];});
				for(int i = 0; i < tofix; ++i)
					for(int j = 0; j < net.N; ++j)
						net.DeltaH[ indh[i] ] += params.eta * pattern.output[Mbatch[i]] * ( 2 * pattern.input[Mbatch[i]][j] - 1 );
				wrongh.clear();
				indh.clear();
			}
		}
		update(net);

		delete[] Mbatch;
		delete[] h;
		return;
	}


	template<typename T> inline T compute_kick(const std::string &val, const T &x)
	{
		return (val == "hard") ? hardtanh(x) : tanh(x);
	}

	template<typename T> void kickboth(NetNK<T> &net1, NetNK<T> &net2, const parameters<T> &params)
	{
		assert(net1.size == net2.size);
		for(int i = 0; i < net1.size; ++i)
		{
			net1.H[i] += params.lambda*(net2.J[i] - net1.J[i]);
			net2.H[i] -= params.lambda*(net2.J[i] - net1.J[i]);

			net1.J[i] = (net1.H[i] > 0) ? 1 : 0;
			net2.J[i] = (net2.H[i] > 0) ? 1 : 0;
		}
		return;
	}

	template<typename T> void kickboth_traced(NetNK<T> &net1, NetNK<T> &net2, const parameters<T> &params, const std::string &val)
	{
		assert(net1.size == net2.size);
		T correction = (val == "corrected") ? tanh(params.gamma * params.y) : (T)1.;
		for(int i = 0; i < net1.size; ++i)
		{
			net1.H[i] += params.lambda * (compute_kick(val, params.gamma * params.y * net2.H[i]) - correction * (2 * net1.J[i] - 1));
			net1.J[i] = (net1.H[i] > 0) ?  1 : 0;
			
			net2.H[i] += 2 * T(T(net1.J[i]) - T(net1.oldJ[i])) / params.y;
			net2.J[i] = (net2.H[i] > 0) ? 1 : 0;
		}
		return;
	}

	template<typename T> void kickboth_traced_continuous(NetNK<T> &net1, NetNK<T> &net2, const parameters<T> &params)
	{
		assert(net1.size == net2.size);
		for(int i = 0; i < net1.size; ++i)
		{
			net1.H[i] += params.lambda*(net2.H[i] - 2 * net1.J[i] + 1);
			net1.J[i] = (net1.H[i] > 0) ? 1 : 0;

			net2.H[i] += 2 * T(T(net1.J[i]) - T(net1.oldJ[i])) / params.y;
			net2.J[i] = (net2.H[i] > 0) ? 1 : 0;
		}
		return;
	}

	int* read_weights(const std::string &filename, int &K, int &N, bool bin)
	{
		int *W = nullptr;
		std::ifstream is;
		if(bin)
		{
			is.open(filename, std::ios::binary);
			if( !is ) {std::cerr << "Weights file not found! Given : " << filename << std::endl; exit(1);}
			is.read((char*)&K, sizeof(int));
			is.read((char*)&N, sizeof(int));
			W = new int[K * N];
			std::for_each(W, W + K*N, [&is](int &val){is.read((char*)&val, sizeof(int));});
			
			is.close();
		}
		else
		{
			std::string row;
			is.open(filename);
			if(!is){std::cerr << "Weights file not found! Given : " << filename << std::endl; exit(1);}

			is >> K;
			is >> N;
			W = new int[K * N];

			std::for_each(W, W + K*N, [&is](int &val){ is >> val; });
			is.close();
		}
		return W;
	}

	template<typename T> void save_weights(const std::string &filename, const NetNK<T> &net, const int &seed, const int &y, const int &batch, const int &max_epochs, const std::string formula, const T &eta, const T &lambda, const T &gamma, const T &eta_factor, const T &lambda_factor, const T &gamma_step)
	{
		std::ofstream os(filename);
		os << "seed: " << seed <<  ", nrep: " << y << ", batch: " << batch << ", max_epochs: " << max_epochs <<  ", formula:" << formula << std::endl;
		os << "eta: " << eta << ", lambda: " << lambda << ", gamma:" << gamma << ", eta_factor:" << eta_factor << ", lambda_factor: " << lambda_factor << ", gamma_step: "<< gamma_step << std::endl;
		os << "K: " << net.K << ", N: " << net.N << std::endl;
		std::for_each(net.J, net.J + (net.N)*(net.K), [&os](const int &j){os << j << "\t";});
			os << std::endl;
		
		os << "END" << std::endl;
		os.close();
		return;
	}

	void save_weights(const std::string &filename, int *W, const int &K, const int &N)
	{
		std::ofstream os(filename, std::ios::out | std::ios::binary);
		os.write( (const char *) &K, sizeof( int ));
		os.write( (const char *) &N, sizeof( int ));
		

		for(int i = 0; i < K; ++i)
			for(int j = 0; j < N; ++j)
				os.write( (const char *) &W[i * N + j], sizeof( int ));
		os.close();
	}

	template<typename T> int* RSGD(const Patterns<T> &pattern, const int &K, parameters<T> &params, const int &batch, const std::string &formula, const int &seed, const int &max_epochs, const bool &init_equal, const bool &waitcenter, const bool &center, std::string outfile, bool quiet)
	{
		bool ok = false;
		int N = pattern.Ncol, M = pattern.Nrow, errc, sub_epochs, epoch, *shuffle_idx = new int[params.y], minerr, m_errs = std::numeric_limits<int>::max(), *errs = new int[params.y], *dist = new int[params.y], *minerrs = new int[params.y], minerrc;
		T mean_errs = (T)0., mean_dist = (T)0.;
		PatternPermutation *p_perm = new PatternPermutation[params.y];
		NetNK<T> *nets = new NetNK<T>[params.y], netc(N, K);
		std::ofstream os;

		const char* valid_formula[] = { "simple", "hard", "corrected", "continuous" };

		if (K < 1 || K % 2 == 0) { std::cerr << "K must be positive and odd!" << std::endl; exit(1); }
		if (params.y < 1){	std::cerr << "y must be positive!" << std::endl; exit(1);}
		if (batch < 1){	std::cerr << "batch must be positive!" << std::endl; exit(1);}
		if (formula != "simple" && formula != "hard" && formula != "corrected" && formula != "continuous") {std::cerr << "Invalid Iteration Scheme! Given : " << formula << std::endl; exit(1); }
		if (params.lambda == 0 && waitcenter){	std::cout << "Warning : lambda = " << params.lambda << ", waitcenter = \n" << waitcenter << std::endl; exit(1);}
		if (init_equal && batch >= M){std::cout << "Warning : batch = " << batch << ", M = " << M << ", init_equal = " << init_equal << std::endl; exit(1);}
		
		std::random_device rd;
		std::mt19937 eng(rd());
		std::bernoulli_distribution binary_dist;

		if(center || init_equal)
		{
			std::generate(netc.H, netc.H + netc.size, [&binary_dist, &eng]{return (binary_dist(eng)) ? (T)1. : -(T)1.;});
			//netc.H[0]=-1;
			//netc.H[1]=1;
			//netc.H[2]=1;
			//netc.H[3]=-1;
			//netc.H[4]=-1;
			//netc.H[5]=1;
			//netc.H[6]=-1;
			//netc.H[7]=1;
			//netc.H[8]=-1;
			//netc.H[9]=1;
			//netc.H[10]=1;
			//netc.H[11]=1;
			//netc.H[12]=1;
			//netc.H[13]=-1;
			//netc.H[14]=-1;
			//netc.H[15]=1;
			//netc.H[16]=1;
			//netc.H[17]=1;
			//netc.H[18]=-1;
			//netc.H[19]=-1;
			//netc.H[20]=-1;
			//netc.H[21]=1;
			//netc.H[22]=1;
			//netc.H[23]=-1;
			//netc.H[24]=-1;
			//netc.H[25]=-1;
			//netc.H[26]=1;

			std::memset(netc.DeltaH, 0, sizeof(T)*netc.size);
			std::transform(netc.H, netc.H + netc.size, netc.J, [](const T &i){return (i > 0) ? 1 : 0;});
			std::memcpy(netc.oldJ, netc.J, sizeof(int)*netc.size);
		}

		for(int i = 0; i < params.y; ++i) 
		{
			if(init_equal) nets[i] = netc;
			else
			{
				std::generate(nets[i].H, nets[i].H + nets[i].size, [&binary_dist, &eng]{return (binary_dist(eng)) ? (T)1. : -(T)1.;});
				std::memset(nets[i].DeltaH, 0, sizeof(T)*nets[i].size);
				std::transform(nets[i].H, nets[i].H + nets[i].size, nets[i].J, [](const T &n){return (n > 0) ? 1 : 0;});
				std::memcpy(nets[i].oldJ, nets[i].J, sizeof(int)*nets[i].size);
			}
			//for(int j = 0; j < netc.size; ++j)
			//{
			//	nets[i].H[j] = (binary_dist(eng)) ? (T)1. : -(T)1.;
			//	nets[i].DeltaH[i] = (T)0.;
			//	nets[i].J[j] = (nets[i].H[j] > 0) ? 1 : 0;
			//	nets[i].oldJ[j] = nets[i].J[j];
			//}
		}

		if(!center) netc = mean_net(nets, params.y, N, K);

		errc = compute_errs(netc, pattern);
		for(int i = 0; i < params.y; ++i)
		{
			shuffle_idx[i] = i;
			errs[i] = compute_errs(nets[i], pattern);
			dist[i] = netc - nets[i];
			p_perm[i] = PatternPermutation(1, M, batch);
			std::random_shuffle(p_perm[i].MM, p_perm[i].MM + p_perm[i].M);
			minerrs[i] = errs[i];
			minerr = (errc < errs[i]) ? errc : errs[i];
			m_errs = (errs[i] < m_errs) ? errs[i] : m_errs;
			mean_errs += errs[i];
			mean_dist += dist[i];
		}

		if(outfile != "" && !os::file_exists(outfile))
		{
			os.open(outfile);
			if (outfile != "" )
			{
				os << "#epoch\terr(Wc)\terr(best)\t|\t";
				for (int i = 0; i < params.y; ++i)	os << "err(W" << i << ")" << "\t";
				os << "\t|\tlambda\tgamma\t|\t";
				for (int i = 0; i < params.y; ++i) os << "d(W" << i << ")\t";
				os << std::endl;
			}
			os << epoch << "\t" << errc << "\t" << std::min(errc, m_errs) << "\t";
			std::for_each(errs, errs + params.y - 1, [&os](const int &i){os << i << "\t";}); os << errs[params.y-1];
			os << std::setprecision(3) << params.lambda << "\t" << std::setprecision(3) << params.gamma << "\t";
			std::for_each(dist, dist + params.y - 1, [&os](const int &i){os << i << "\t";}); os << dist[params.y-1];
		}
		if (!quiet)
		{
			std::cout << "ep: " << epoch << " lambda: " << std::setprecision(3) << params.lambda << " gamma: " << std::setprecision(3) << params.gamma << " eta: " << params.eta << std::endl;
			std::cout << "\terrc " << errc << " [" << errc << "]" << std::endl;
			std::cout << "\terrs " << m_errs << " [";
			std::for_each(errs, errs + params.y - 1, [](const int &i){std::cout << i << ",";}); std::cout << errs[params.y-1];
			std::cout << "] (mean = " << std::setprecision(3) << mean_errs / params.y << ")" << std::endl;
			std::cout << "\tdist = [";
			std::for_each(dist, dist + params.y - 1, [](const int &i){std::cout << i << ",";}); std::cout << dist[params.y-1];
			std::cout << "] (mean = " << std::setprecision(3) << mean_dist / params.y << ")" << std::endl;
		}
		sub_epochs = (int)(T(M + batch -1) / batch);
		if( ok = (errc == 0) || (!waitcenter && (minerr == 0))) epoch = 0;

		while(!ok && epoch < max_epochs)
		{
			++epoch;
			for(int i = 0; i < sub_epochs; ++i)
			{

				std::random_shuffle(shuffle_idx, shuffle_idx + params.y);

				for(int j = 0; j < params.y; ++j)
				{
					std::memcpy(nets[shuffle_idx[j]].oldJ, nets[shuffle_idx[j]].J, sizeof(int)*nets[shuffle_idx[j]].size);
					subepoch(nets[shuffle_idx[j]], pattern, p_perm[shuffle_idx[j]], params);
					if(!center)
						(formula == "continuous") ? kickboth_traced_continuous(nets[shuffle_idx[j]], netc, params) : kickboth_traced(nets[shuffle_idx[j]], netc, params, formula);
					else if(params.lambda > 0)
						kickboth(nets[shuffle_idx[j]], netc, params);
				}
			}
			ok = ((errc = compute_errs(netc, pattern)) == 0) ? true : false;
			minerrc = (minerrc < errc) ? minerrc : errc;
			minerr = std::numeric_limits<int>::max();
			m_errs = std::numeric_limits<int>::max();
			mean_errs = (T)0.;
			mean_dist = (T)0.;
			for(int i = 0; i < params.y; ++i)
			{
				errs[i] = compute_errs(nets[i], pattern);
				minerrs[i] = (minerrs[i] < errs[i]) ? minerrs[i] : errs[i];
				minerr = (minerrc < minerrs[i]) ? minerrc : minerrs[i];
				m_errs = (minerrs[i] < m_errs) ? minerrs[i] : m_errs;
				mean_errs += errs[i];
				mean_dist += dist[i];
				ok = (errs[i] == 0 && waitcenter == false) ? true : ok;
				dist[i] = netc - nets[i];
			}
			if(outfile != "")
			{
				os << epoch << "\t" << errc << "\t" << std::min(errc, m_errs) << "\t";
				std::for_each(errs, errs + params.y - 1, [&os](const int &i){os << i << "\t";}); os << errs[params.y-1];
				os << std::setprecision(3) << params.lambda << "\t" << std::setprecision(3) << params.gamma << "\t";
				std::for_each(dist, dist + params.y - 1, [&os](const int &i){os << i << "\t";}); os << dist[params.y-1];
			}
			if (!quiet)
			{
				std::cout << "ep: " << epoch << " lambda: " << std::setprecision(3) << params.lambda << " gamma: " << std::setprecision(3) << params.gamma << " eta: " << params.eta << std::endl;
				std::cout << "\terrc: " << errc << " [" << errc << "]" << std::endl;
				std::cout << "\terrs: " << m_errs << " [";
				std::for_each(errs, errs + params.y - 1, [](const int &i){std::cout << i << ",";}); std::cout << errs[params.y-1];
				std::cout << "] (mean = " << std::setprecision(3) << mean_errs / params.y << ")" << std::endl;
				std::cout << "\tdist = [";
				std::for_each(dist, dist + params.y - 1, [](const int &i){std::cout << i << ",";}); std::cout << dist[params.y-1];
				std::cout << "] (mean = " << std::setprecision(3) << mean_dist / params.y << ")" << std::endl;
			}
			update(params);
		}
		if(outfile != "") os.close();
		if (ok)
		{
			std::cout << "SOLVED\n";
			if (outfile != "") 	save_weights(outfile, netc.J, netc.K, netc.N);
		}
		else
			std::cout<< "FAILED\n";

		delete[] errs;
		delete[] dist;
		delete[] minerrs;
		delete[] shuffle_idx;
		delete[] p_perm;
		delete[] nets;

		return netc.J;
	}


	template<typename T> int* train(const std::string &patternsfile, int K, int y, int batch, int max_epochs, T eta, T lambda, T gamma, T eta_factor, T lambda_factor, T gamma_step, std::string formula, bool waitcenter, bool init_equal, bool center, int seed, std::string outfile)
	{
		Patterns<T> pattern(patternsfile);
		check_binary(pattern);
		parameters<T> params(y, eta, lambda, gamma, eta_factor, lambda_factor, gamma_step);
		return RSGD(pattern, K, params, batch, formula, seed, max_epochs, init_equal, waitcenter, center, outfile, false);
	}

	template<typename T> int* train(const Patterns<T> &pattern, int K, int y, int batch, int max_epochs, T eta, T lambda, T gamma, T eta_factor, T lambda_factor, T gamma_step, std::string formula, bool waitcenter, bool init_equal, bool center, int seed, std::string outfile)
	{
		parameters<T> params(y, eta, lambda, gamma, eta_factor, lambda_factor, gamma_step);
		return RSGD(pattern, K, params, batch, formula, seed, max_epochs, init_equal, waitcenter, center, outfile, false);
	}

	template<typename T> int* test(const Patterns<T> &pattern, int *weights, const int &K, const int &N)
	{
		int dot, sum, h_out, *results = new int[pattern.Nrow];

		for(int i = 0; i < pattern.Nrow; ++i)
		{
			h_out = 0;
			for(int j = 0; j < K; ++j)
			{
				dot = 0; sum = 0;
				for(int k = 0; k < N; ++k)
				{
					dot += pattern.input[i][k] * weights[j*N + k];
					sum += pattern.input[i][k] + weights[j*N + k];
				}

				h_out += ((4 * dot - 2 * sum + N) > 0) ? 1 : -1;
			}
			results[i] = (h_out > 0) ? 1 : -1;
		}
		return results;
	}

	template<typename T> int* test(const std::string &ifile, const std::string &wfile)
	{
		Patterns<T> pattern(ifile);
		check_binary(pattern);		

		int 	K, 
				N,
				*weights = read_weights(wfile, K, N);

		return test(pattern, weights, K, N);
	}

	template<typename T> int compute_output(T *input,  int *weights, const int &K, const int &N)
	{
		int dot = 0, sum = 0, h_out = 0;

		for(int j = 0; j < K; ++j)
		{
			for(int k = 0; k < N; ++k)
			{
				dot += input[k] * weights[j*N + k];
				sum += input[k] + weights[j*N + k];
			}

			h_out += ((4 * dot - 2 * sum + N) > 0) ? 1 : -1;
		}
		return (h_out > 0) ? 1 : -1;
	}

} // end of namespace RSGD

static void menu_RSGD()
{
	std::cout << std::endl;
	std::cout << " BinaryCommitteeMachineRSGD Algorithm" << std::endl;
	std::cout << " ====================================" << std::endl;
	std::cout << " *                                  *" << std::endl;
	std::cout << " *       University of Bologna      *" << std::endl;
	std::cout << " *                                  *" << std::endl;
	std::cout << " *    Rachele Luzi  &  Nico Curti   *" << std::endl;
	std::cout << " *                                  *" << std::endl;
	std::cout << " *      rachele.luzi2@unibo.it      *" << std::endl;
	std::cout << " *       nico.curti2@unibo.it       *" << std::endl;
	std::cout << " *                                  *" << std::endl;
	std::cout << " ====================================" << std::endl;
	std::cout << "              Library OP             " << std::endl << std::endl;
}

template<typename T> void parse_training_args(const int &argc, char **argv, std::string &patternsfile, int &K, int &y, int &batch, int &max_epochs, T &eta, T &lambda, T &gamma, T &eta_factor, T &lambda_factor, T &gamma_step, bool &waitcenter, bool &init_equal, bool &center, std::string &output, std::string &formula)
{
	ArgumentParser argparse("Training ReplicatedSGD 3.0");
	argparse.add_argument<std::string>("fArg", "f", "file", "Pattern Filename (with extension)", true, "");
	argparse.add_argument<std::string>("oArg", "o", "output", "Output Filename (with extension)", true, "");
	argparse.add_argument<int>("kArg", "k", "hidden", "Number of Hidden Layers", false, 3);
	argparse.add_argument<int>("yArg", "y", "replica", "Number of replicas", false, 1);
	argparse.add_argument<int>("bArg", "b", "batch", "Minibatch size", false, 5);
	argparse.add_argument<int>("iArg", "i", "epochs", "Max number of epochs", false, 1000);
	argparse.add_argument<T>("eArg", "e", "eta", "Initial value of the step for the energy (loss) term gradient", false, (T)2.);
	argparse.add_argument<T>("lArg", "l", "lambda", "Initial value of the step for the interaction gradient ('nu' in paper)", false, (T).1);
	argparse.add_argument<T>("gArg", "g", "gamma", "Initial value of the interaction strength", false, std::numeric_limits<T>::infinity());
	argparse.add_argument<bool>("wArg", "w", "waitcenter", "Whether to only exit successfully if the center replica has solved the problem", false, false);
	argparse.add_argument<bool>("qArg", "q", "equal", "Whether to initialize all replicated networks equally", false, true);
	argparse.add_argument<bool>("cArg", "c", "center", "Whether to explicity use a central replica (if 'false', it is traced out)", false, false);
	argparse.add_argument<std::string>("zArg", "z", "formula", "Choose Iteration Update Scheme:\t\t\t(Each option implements a different update rule for the Weight Matrix)\n\n\t\t\t\t1. Simple (uses the simplest traced-out center formula eq. (C7))\n\t\t\t\t2. Corrected (applies the correction eq. (C9) to the formula eq. (C7))\n\t\t\t\t3. Continuous (the center is continuous and traced-out)\n\t\t\t\t4. Hard (uses the simplest traced-out center formula eq. (C7) with improved performaces due to the hard tanh)", true, "simple");

	argparse.add_argument<T>("xArg", "x", "factor", "Factors used to update 'eta', 'lambda' and 'gamma' respectively after each epoch", false);

	argparse.parse_args(argc, argv);

	argparse.get<std::string>("fArg", patternsfile);
	if(!os::file_exists(patternsfile)){std::cerr << "Pattern file not found. Given : " << patternsfile << std::endl;}
	argparse.get<std::string>("oArg", output);
	argparse.get<int>("kArg", K);
	argparse.get<int>("yArg", y);
	argparse.get<int>("bArg", batch);
	argparse.get<int>("iArg", max_epochs);
	argparse.get<T>("eArg", eta);
	argparse.get<T>("lArg", lambda);
	argparse.get<T>("gArg", gamma);

	std::vector<T> factors;
	argparse.get<T>("xArg", factors);
	if(factors.size() == 0){ eta_factor = (T)1.; lambda_factor = (T)1.; gamma_step = (T).01; }
	else if(factors.size() == 1) { eta_factor = factors[0]; lambda_factor = (T)1.; gamma_step = (T).01; }
	else if(factors.size() == 2) { eta_factor = factors[0]; lambda_factor = lambda_factor[1]; gamma_step = (T).01; }
	else if(factors.size() == 3) { eta_factor = factors[0]; lambda_factor = lambda_factor[1]; gamma_step = factors[2]; }
	else {std::cerr << "Invalid number of factors! Required values are associated to 'eta', 'lambda' and 'gamma', respectively. Given " << factors.size() << " inputs" << std::endl; exit(1);}

	argparse.get<bool>("wArg", waitcenter);
	argparse.get<bool>("qArg", init_equal);
	argparse.get<bool>("cArg", center);
	argparse.get<std::string>("zArg", formula);
	if (formula != "simple" && formula != "hard" && formula != "corrected" && formula != "continuous") { std::cerr << "Invalid Iteration Scheme! Given : " << formula << std::endl; exit(1); }

	return;
}

template<typename T> void parse_test_args(int argc, char *argv[], std::string &test_file, std::string &weight_file, std::string &output_file)
{
	ArgumentParser argparse("Test ReplicatedSGD 3.0");
	argparse.add_argument<std::string>("fArg", "f", "test_file", "Test Filename (with extension)", true, "");
	argparse.add_argument<std::string>("wArg", "w", "weights", "Weights Matrix Filename (with extension)", true, "");
	argparse.add_argument<std::string>("oArg", "o", "output", "Output Filename (with extension)", false, "output.txt");
	
	argparse.parse_args(argc, argv);

	argparse.get<std::string>("fArg", test_file);
	if(!os::file_exists(test_file)) {std::cerr << "Test file not found! Given : " << test_file << std::endl; exit(1);}
	argparse.get<std::string>("wArg", weight_file);
	if(!os::file_exists(weight_file)) {std::cerr << "Weights Matrix file not found! Given : " << weight_file << std::endl; exit(1);}
	argparse.get<std::string>("oArg", output_file);
	return;
}


