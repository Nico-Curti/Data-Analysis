#pragma once

#include "os.hpp"
#include "parse_args.hpp"
#include "fmath.hpp"
#include "patterns.hpp"

#include <set>
#include <assert.h>
#include <climits>
#include <iomanip>
#include <chrono>
#include <cstring>
#include <numeric>



/**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-
**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-
*-* 																																				*-*
*-* 				::::::::::   ::::::::    ::::::::   :::    :::   ::::::::   :::::::::::  ::::    :::   ::::::::  								*-*
*-* 				:+:         :+:    :+:  :+:    :+:  :+:    :+:  :+:    :+:      :+:      :+:+:   :+:  :+:    :+: 								*-*
*-* 				+:+         +:+    +:+  +:+         +:+    +:+  +:+             +:+      :+:+:+  +:+  +:+        								*-*
*-* 				:#::+::#    +#+    +:+  +#+         +#+    +:+  +#++:++#++      +#+      +#+ +:+ +#+  :#:        								*-*
*-* 				+#+         +#+    +#+  +#+         +#+    +#+         +#+      +#+      +#+  +#+#+#  +#+   +#+# 								*-*	
*-* 				#+#         #+#    #+#  #+#    #+#  #+#    #+#  #+#    #+#      #+#      #+#   #+#+#  #+#    #+# 								*-*	
*-* 				###          ########    ########    ########    ########   ###########  ###    ####   ########  								*-*	
*-* 																																				*-*	
*-* 																																				*-*	
*-* 							:::::::::   ::::::::::  :::         :::::::::::  ::::::::::  :::::::::: 											*-*		
*-* 							:+:    :+:  :+:         :+:             :+:      :+:         :+:        											*-*	
*-* 							+:+    +:+  +:+         +:+             +:+      +:+         +:+        											*-*	
*-* 							+#++:++#+   +#++:++#    +#+             +#+      +#++:++#    :#::+::#   											*-*		
*-* 							+#+    +#+  +#+         +#+             +#+      +#+         +#+        											*-*				
*-* 							#+#    #+#  #+#         #+#             #+#      #+#         #+#        											*-*		
*-* 							#########   ##########  ##########  ###########  ##########  ###   													*-*			
*-* 																																				*-*		
*-* 																																				*-*					
*-* 	:::::::::   :::::::::    ::::::::   :::::::::       :::       ::::::::       :::      :::::::::::  :::::::::::   ::::::::   ::::    ::: 	*-*	
*-* 	:+:    :+:  :+:    :+:  :+:    :+:  :+:    :+:    :+: :+:    :+:    :+:    :+: :+:        :+:          :+:      :+:    :+:  :+:+:   :+: 	*-*	
*-* 	+:+    +:+  +:+    +:+  +:+    +:+  +:+    +:+   +:+   +:+   +:+          +:+   +:+       +:+          +:+      +:+    +:+  :+:+:+  +:+ 	*-*	
*-* 	+#++:++#+   +#++:++#:   +#+    +:+  +#++:++#+   +#++:++#++:  :#:         +#++:++#++:      +#+          +#+      +#+    +:+  +#+ +:+ +#+ 	*-*	
*-* 	+#+         +#+    +#+  +#+    +#+  +#+         +#+     +#+  +#+   +#+#  +#+     +#+      +#+          +#+      +#+    +#+  +#+  +#+#+# 	*-*	
*-* 	#+#         #+#    #+#  #+#    #+#  #+#         #+#     #+#  #+#    #+#  #+#     #+#      #+#          #+#      #+#    #+#  #+#   #+#+# 	*-*	
*-* 	###         ###    ###   ########   ###         ###     ###   ########   ###     ###      ###      ###########   ########   ###    #### 	*-*	
*-* 																																				*-*
*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**/

template<typename T> struct MagP64 
{
	T mag;
	MagP64() {};
	MagP64(const T &);
	~MagP64() = default;
	MagP64(const MagP64<T> &);

	static inline std::string magformat();
	MagP64<T>& operator=(const MagP64<T> &);

	inline MagP64<T> operator%(const MagP64<T> &);
	template<typename F> inline friend MagP64<F> operator-(const MagP64<F> &);
	inline MagP64<T> operator+(const MagP64<T> &);
	inline T operator+(const T &);
	inline MagP64<T>& operator+=(const MagP64<T> &);
	inline MagP64<T> operator/(const T &);
	template<typename F> inline friend MagP64<F> operator/(const F &, const MagP64<F> &);
	inline MagP64<T> operator/(const MagP64<T> &);
	inline MagP64<T> operator*(const MagP64<T> &);
	inline T operator*(const T &);
	template<typename F> inline friend F operator*(const F &, const MagP64<F> &);
	inline MagP64<T> operator-(const MagP64<T> &);
	inline MagP64<T> operator-(const T &);
	template<typename F> inline friend MagP64<F> operator-(const F &, const MagP64<F> &);
	inline bool operator==(const MagP64<T> &);
	inline bool operator!=(const MagP64<T> &);
};


namespace AtanhErf
{
	template<typename T> void getinp(const T&, const T&, T*&);
	template<typename T> inline T atanherf_largex(const T&);
	template<typename T> T atanherf_interp(const T&);
	template<typename T> inline T evalpoly(const T&);
	template<typename T> inline T atanherf(const T&);
}

template<typename T> struct MagT64 
{
	T mag;
	T mInf;
	MagT64() {};
	MagT64(const T &, T m = (T)30.);
	MagT64(const MagT64<T> &);
	~MagT64() = default;
	MagT64<T>& operator=(const MagT64<T> &);

	static inline std::string magformat();
	
	inline MagT64<T> operator%(const MagT64<T> &);
	template<typename F> inline friend MagT64<F> operator-(const MagT64<F> &);
	inline MagT64<T> operator+(const MagT64<T> &);
	inline T operator+(const T &);
	inline MagT64<T>& operator+=(const MagT64<T> &);
	inline MagT64<T> operator/(const T &);
	template<typename F> inline friend MagT64<F> operator/(const F &, const MagT64<F> &);
	inline MagT64<T> operator/(const MagT64<T> &);
	inline MagT64<T> operator*(const MagT64<T> &);
	inline T operator*(const T &);
	template<typename F> inline friend F operator*(const F &, const MagT64<F> &);
	inline MagT64<T> operator-(const MagT64<T> &);
	inline MagT64<T> operator-(const T &);
	template<typename F> inline friend MagT64<F> operator-(const F &, const MagT64<F> &);
	inline bool operator==(const MagT64<T> &);
	inline bool operator!=(const MagT64<T> &);
};

template<typename T, class Mag> using magP = typename std::enable_if<std::is_same<Mag, MagP64<T>>::value, Mag>::type*;
template<typename T, class Mag> using magT = typename std::enable_if<std::is_same<Mag, MagT64<T>>::value, Mag>::type*;

namespace Magnetization
{
	template<class Mag> inline int signbit(const Mag &);
	template<typename T, class Mag> inline Mag f2m(const T& );
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline Mag couple(const T &, const T &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline Mag couple(const T &, const T &);
	template<typename T, class Mag> inline void zeros(Mag *, const int &);
	template<typename T, class Mag> inline void zero(Mag &);
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline Mag damp(const Mag &, const Mag &, const T &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline Mag damp(const Mag &, const Mag &, const T &);
	template<typename T, class Mag> inline T abs(const Mag &);
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline Mag mtanh(const T &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline Mag mtanh(const T &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline Mag matanh(const Mag &);
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline Mag merf(const T &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline Mag merf(const T &);
	template<typename T> inline T clamp(const T &, const T &, const T &);
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline Mag bar(const Mag &, const Mag &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline Mag bar(const Mag &, const Mag &);
	template<typename T, class Mag> inline void copysign(Mag &, const T &);
	template<class Mag> inline void copysign(Mag &, const Mag &);	
	template<typename T, class Mag> inline T lr(const T &);
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline T log1pxy(const Mag &, const Mag &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline T log1pxy(const Mag &, const Mag &);
	template<typename T, class Mag> inline Mag arrow(const Mag &, const T &);
	template<typename T, typename std::enable_if<std::is_floating_point<T>::value, T>::type* = nullptr> inline int sign0(const T &x);
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline int sign0(const Mag &x);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline int sign0(const Mag &x);
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline T mcrossentropy(const Mag &, const Mag &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline T mcrossentropy(const Mag &, const Mag &);
	template<typename T> T logmag2pp(const MagP64<T> &);
	template<typename T> T logmag2pm(const MagP64<T> &);
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline T logZ(const Mag &, Mag *, const int &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline T logZ(const Mag &, Mag *, const int &);
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline Mag convert(const T &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline Mag convert(const T &);
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline T convert(const Mag &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline T convert(const Mag &);
	template<typename T> inline MagT64<T> auxmix(const MagT64<T> &, const T &, const T &);
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline Mag erfmix(const Mag &, const T &, const T &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline Mag erfmix(const Mag &, const T &, const T &);
	template<typename T, class Mag, magP<T, Mag> = nullptr> inline Mag exactmix(const Mag &, const Mag &, const Mag &);
	template<typename T, class Mag, magT<T, Mag> = nullptr> inline Mag exactmix(const Mag &, const Mag &, const Mag &);
	
}

					 using IVec = std::vector<int>;
template<typename T> using Vec = std::vector<T>;
template<typename T> using Vec2 = std::vector<std::vector<T>>;
template<class Mag>  using MagVec = Mag*;
template<class Mag>  using MagVec2 = MagVec<Mag>*;
template<class Mag>  using MagVec3 = MagVec2<Mag>*;


template<typename T, class Mag> class Cavity_Message : public MagP64<T>, public MagT64<T>
{
	static void error(const std::string &, const int &n);
public:
	int 			M,			// input sample size
					N, 			// input layers size
					K, 			// hidden layers size
					seed; 		// random seed
	MagVec3<Mag> 	weights; 	// uw
	MagVec2<Mag> 	m_star_j,	// ux
				 	m_j_star,	// mw
				 	m_in,		// mt1
				 	m_no,		// Ut1
				 	m_ni;		// ut1
	MagVec<Mag>  	m_on;		// mt2

	Cavity_Message();
	Cavity_Message(const int &, const int &, const int &, const T &, const int &);
	Cavity_Message(const Cavity_Message<T, Mag> &);
	Cavity_Message<T, Mag>& operator=(const Cavity_Message<T, Mag> &);
	~Cavity_Message();
	static Cavity_Message<T, Mag> read_messages(const std::string &);
};

int** read_weights(const std::string &, int &, int &, bool bin = false);
template<typename T, class Mag> void save_weights(const std::string &, const Cavity_Message<T, Mag> &, const int &, const int &, const int &, const int &, const T &, const T &, const std::string &, const std::string &);
void save_weights(const std::string &, int **, const int &, const int &);
template<typename T> int compute_output(T *,  int **, const int &, const int &);
template<typename T, class Mag> int compute_output(T *, MagVec2<Mag>, const int &, const int &);
template<typename T, class Mag> int** M2W(MagVec2<Mag> , const int &, const int &);
template<typename T, class Mag, magP<T, Mag> = nullptr> inline void set_outfields(const Cavity_Message<T, Mag> &, int *, const T &);
template<typename T, class Mag, magT<T, Mag> = nullptr> inline void set_outfields(const Cavity_Message<T, Mag> &, int *, const T &);
template<typename T> std::ostream& operator<<(std::ostream &, const MagP64<T> &);
template<typename T> std::ostream& operator<<(std::ostream &, const MagT64<T> &);



template<typename T, class Mag> class Params
{
public:
	bool 		quiet; 			// writing boolean
	int 		max_iters; 		// number of iterations
	Mag 		tan_gamma; 		// hyperbolic tangent of distance weight between replicas (gamma)
	T 			damping, 		// damping
				epsil, 			// error tollerance
	 			beta, 			// 1/kT
	 			r; 				// number of replicas -1
	std::string accuracy1, 		// updating accuracy of cavity probability (messages of hidden layers)
				accuracy2; 		// updating accuracy of cavity probability (messages of output node)

	Params(const T &d, const T &e, const T &b, const int &it, const std::string &ac1, const std::string &ac2, const T &R, const T &p, bool q = true);
	~Params() = default;
};


template<typename T> class FocusingProtocol
{
public:
	int Nrep;		// 
	T 	*gamma, 	// distance parameters 
	  	*n_rep, 	// (y) number of replicas
	  	*beta; 		// 1/kT

	FocusingProtocol(){};
	~FocusingProtocol();
	void StandardReinforcement(T *, const int &);
	void StandardReinforcement(const Vec<T> &);
	void StandardReinforcement(const T &);
	void Scoping(T *, const T &, const int &);
	void Scoping(const Vec<T> &, const T &);
	void PseudoReinforcement(T *, const int &, T x = (T).5);
	void PseudoReinforcement(const Vec<T> &, T x = (T).5);
	void PseudoReinforcement(const T &, T x = (T).5);
	void FreeScoping(T **, const int &);
	void FreeScoping(const Vec2<T> &);
};


static void menu_info();
template<typename T> void parse_training_args(const int &, char **, std::string &, std::string &, int &, int &, int &, T &, T &, std::string &, std::string &, char &, char &);
void parse_test_args(const int &, char **, std::string &, std::string &, std::string &);

template<typename T, class Mag, class Input> using patt = typename std::enable_if<std::is_same<Input, Patterns<T>>::value, Input>::type*;
template<typename T, class Mag, class Input> using file = typename std::enable_if<std::is_same<Input, std::string>::value, Input>::type*;


namespace FBP
{
	template<typename T, class Mag> T 	 theta_node_update_approx(MagVec<Mag>, Mag &, T *, MagVec<Mag>, Mag &, const Params<T, Mag> &, const int &, const int &);
	template<typename T, class Mag> T 	 theta_node_update_accurate(MagVec<Mag>, Mag &, T *, MagVec<Mag>, Mag &, const Params<T, Mag> &, const int &, const int &);
	template<typename T, class Mag> T 	 theta_node_update_exact(MagVec<Mag>, Mag &M, T *, MagVec<Mag>, Mag &, const Params<T, Mag> &, const int &, const int &);
	template<typename T, class Mag> T 	 free_energy_theta(MagVec<Mag>, const Mag &M, T *, MagVec<Mag>, const Mag &, const int &, const int &);
	template<typename T, class Mag> T 	 free_energy_theta_exact(MagVec<Mag> , const Mag &, T *, MagVec<Mag>, const Mag &, const int &);
	template<typename T, class Mag> T 	 m_star_update(Mag &, Mag &, const Params<T, Mag> &); //old entro_node_update
	template<typename T, class Mag> T 	 iterate(Cavity_Message<T, Mag> &, const Patterns<T> &, const Params<T, Mag> &);
	template<typename T, class Mag> bool converge( Cavity_Message<T, Mag> &, const Patterns<T> &, const Params<T, Mag> &);
	template<typename T, class Mag> int  nonbayes_test(const Cavity_Message<T, Mag> &, const Patterns<T> &);
	template<typename T, class Mag> T 	 free_energy(const Cavity_Message<T, Mag> &, const Patterns<T> &, const Params<T, Mag> &);
	template<typename T, class Mag> T 	 compute_S(const Cavity_Message<T, Mag> &, const Params<T, Mag> &);
	template<typename T, class Mag> Mag  compute_q_bar(const Cavity_Message<T, Mag> &, const Params<T, Mag> &);
	template<typename T, class Mag> T 	 compute_q(const Cavity_Message<T, Mag> &, const int &, const int &);
	template<typename T, class Mag> void mags_symmetry(const Cavity_Message<T, Mag> &, T *);

	template<typename T, class Mag> int** focusingBP(const int &, const Patterns<T> &, const int &, const int &, const int &, const T &, const bool &, const std::string &, const std::string &, const T &randfact, const FocusingProtocol<T> &, const T &, const std::string &, const bool &, const std::string &, std::string, std::string, const std::string &);

	template<typename T, class Mag, class Input, patt<T, Mag, Input> = nullptr> int** train(const Input &, int K = 3, int seed = 135, int max_iters = 1000, T randfact = (T).1, T damping = (T).5, std::string accuracy1 = "accurate", std::string accuracy2 = "accurate", std::string fprotocol = "standard_reinforcement", int protocol_size = 101, std::string output = "");
	template<typename T, class Mag, class Input, file<T, Mag, Input> = nullptr> int** train(const Input &, int K = 3, int seed = 135, int max_iters = 1000, T randfact = (T).1, T damping = (T).5, std::string accuracy1 = "accurate", std::string accuracy2 = "accurate", std::string fprotocol = "standard_reinforcement", int protocol_size = 101, std::string output = "");

	template<typename T> int* test(const Patterns<T> &, int**, const int &, const int &);
	template<typename T> int* test(const std::string &, const std::string &);
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

//==================================================================================================================================================//
//	___  ___  ___   _____  _   _  _____  _____  _____  ______  ___   _____  _____  _____  _   _        ___  ___ _____  _____  _   _  _____ ______   //
//	|  \/  | / _ \ |  __ \| \ | ||  ___||_   _||_   _||___  / / _ \ |_   _||_   _||  _  || \ | |       |  \/  ||  ___||_   _|| | | ||  _  ||  _  \  //
//	| .  . |/ /_\ \| |  \/|  \| || |__    | |    | |     / / / /_\ \  | |    | |  | | | ||  \| |       | .  . || |__    | |  | |_| || | | || | | |  //
//	| |\/| ||  _  || | __ | . ` ||  __|   | |    | |    / /  |  _  |  | |    | |  | | | || . ` |       | |\/| ||  __|   | |  |  _  || | | || | | |  //
//	| |  | || | | || |_\ \| |\  || |___   | |   _| |_ ./ /___| | | |  | |   _| |_ \ \_/ /| |\  |       | |  | || |___   | |  | | | |\ \_/ /| |/ /   //
//	\_|  |_/\_| |_/ \____/\_| \_/\____/   \_/   \___/ \_____/\_| |_/  \_/   \___/  \___/ \_| \_/       \_|  |_/\____/   \_/  \_| |_/ \___/ |___/    //
//	                                                                                                                                                //
//==================================================================================================================================================//

namespace Magnetization
{
	template<class Mag> inline int signbit(const Mag &m)
	{ return std::signbit(m.mag);}

	template<typename T, class Mag> inline Mag f2m(const T &x)
	{ return Mag(x);}

	template<typename T, class Mag, magP<T, Mag>> inline Mag couple(const T &x1, const T &x2)
	{ return Mag((x1 - x2) / (x1 + x2)); }
	template<typename T, class Mag, magT<T, Mag>> inline Mag couple(const T &x1, const T &x2)
	{ return Mag((std::log(x1) - std::log(x2)) / (T)2.); }

	template<typename T, class Mag> inline void zeros(MagVec<Mag> x, const int &n)
	{ std::memset(x, 0, sizeof(Mag)*n); return;}

	template<typename T, class Mag> inline void zero(Mag &m)
	{ m.mag = (T)0.; return;}

	template<typename T, class Mag, magP<T, Mag>> inline Mag damp(const Mag &newx, const Mag &oldx, const T &l)
	{ return newx.mag * ((T)1. - l) + oldx.mag * l; }
	template<typename T, class Mag, magT<T, Mag>> inline Mag damp(const Mag &newx, const Mag &oldx, const T &l)
	{ return Mag(std::atanh(newx.mag) * ((T)1. - l) + std::atanh(oldx.mag) * l); }

	template<typename T, class Mag> inline T abs(const Mag &a)
	{ return (T)std::fabs(a.mag);}

	template<typename T, class Mag, magP<T, Mag>> inline Mag mtanh(const T &x)
	{ return Mag(std::tanh(x));}
	template<typename T, class Mag, magT<T, Mag>> inline Mag mtanh(const T &x)
	{ return Mag(x);}

	template<typename T> inline MagT64<T> matanh(const MagT64<T> &m)
	{ return m.mag;}

	template<typename T, class Mag, magP<T, Mag>> inline Mag merf(const T &x)
	{ return Mag(std::erf(x));}
	template<typename T, class Mag, magT<T, Mag>> inline Mag merf(const T &x)
	{ return Mag(AtanhErf::atanherf(x));}

	template<typename T> inline T clamp(const T &x, const T &low, const T &high)
	{ return (low <= x && x <= high) ? x : (x < low) ? low : high; }

	template<typename T, class Mag, magP<T, Mag>> inline Mag bar(const Mag &m1, const Mag &m2)
	{ return ((m1.mag == m2.mag) ? Mag((T)0.) : Mag(clamp((m1.mag - m2.mag) / ((T)1. - m1.mag * m2.mag), (T)-1., (T)1.))); }
	template<typename T, class Mag, magT<T, Mag>> inline Mag bar(const Mag &m1, const Mag &m2)
	{ return ((m1.mag == m2.mag) ? Mag((T)0.) : Mag(std::atanh(m1.mag) -  std::atanh(m2.mag))); }

	template<typename T, class Mag> inline void copysign(Mag &x, const T &y)
	{ (signbit<Mag>(x) != signbit<Mag>(y)) ? -x : x; return;}

	template<class Mag> inline void copysign(Mag &x, const Mag &y)
	{ (signbit<Mag>(x) != signbit<Mag>(y)) ? -x : x; return;}

	template<typename T, class Mag> inline T lr(const T &x)
	{ return std::log1p(std::exp(-2 * std::fabs(x))); }

	template<typename T, class Mag, magP<T, Mag>> inline T log1pxy(const Mag &x, const Mag &y)
	{ return std::log(((T)1. + (x.mag * y.mag)) / (T)2.); }
	template<typename T, class Mag, magT<T, Mag>> inline T log1pxy(const Mag &x, const Mag &y)
	{
		T ax = std::atanh(x.mag), ay = std::atanh(y.mag);
		return !std::isinf(ax) && !std::isinf(ay) ? std::fabs(ax + ay) - std::fabs(ax) - std::fabs(ay) + lr<T, Mag>(ax + ay) - lr<T, Mag>( ax) - lr<T, Mag>( ay) :
			std::isinf(ax) && !std::isinf(ay) ? sign(ax)*ay - std::fabs(ay) - lr<T, Mag>( ay) :
			!std::isinf(ax) && std::isinf(ay) ? sign(ay)*ax - std::fabs(ax) - lr<T, Mag>( ax) :
			sign(ax) == sign(ay) ? (T)0. : - std::numeric_limits<T>::infinity();
	}

	template<typename T, class Mag> inline Mag arrow(const Mag &m, const T &x)
	{ return mtanh<T, Mag>(x * std::atanh(m.mag));}
	
	template<typename T, typename std::enable_if<std::is_floating_point<T>::value, T>::type*> inline int sign0(const T &x)
	{ return 1 - 2 * std::signbit(x);}
	template<typename T, class Mag, magP<T, Mag>> inline int sign0(const Mag &x)
	{ return 1 - 2 * signbit<Mag>(x);}
	template<typename T, class Mag, magT<T, Mag>> inline int sign0(const Mag &x)
	{ return 1 - 2 * signbit<Mag>(x);}

	template<typename T, class Mag, magP<T, Mag>> inline T mcrossentropy(const Mag &x, const Mag &y)
	{ return x.mag * std::atanh(y.mag) - std::log((T)1. - y.mag*y.mag) / (T)2. + std::log((T)2.);}
	template<typename T, class Mag, magT<T, Mag>> inline T mcrossentropy(const Mag &x, const Mag &y)
	{
		T tx = x.mag, ay = std::atanh(y.mag);
		return !std::isinf(ay) ? -std::fabs(ay)*(sign0(ay)*tx - 1) + lr<T, Mag>( ay) : (sign(tx) != sign(ay)) ? std::numeric_limits<T>::infinity() : (T)0.0;
	}

	template<typename T> inline T logmag2pp(const MagP64<T> &x) 
	{ return std::log(((T)1. + x.mag) / (T)2.); }


	template<typename T> inline T logmag2pm(const MagP64<T> &x) 
	{ return std::log(((T)1. - x.mag) / (T)2.); }

	template<typename T, class Mag, magP<T, Mag>> T logZ(const Mag &u0, Mag *u, const int &nu)
	{
		T zkip = std::log(((T)1. + u0.mag) / (T)2.), zkim = std::log(((T)1. - u0.mag) / (T)2.);
		for (int i = 0; i < nu; ++i)
		{
			zkip += std::log(((T)1. - u[i].mag) / (T)2.);
			zkim += std::log(((T)1. + u[i].mag) / (T)2.);
		}
		return std::log(std::exp(zkip) + std::exp(zkim));
	}
	template<typename T, class Mag, magT<T, Mag>> T logZ(const Mag &u0, Mag *u, const int &nu)
	{
		T a0 = std::atanh(u0.mag), s1, s2, s3, ai, hasinf;
		if (!std::isinf(a0))
		{
			s1 = a0;
			s2 = std::fabs(a0);
			s3 = lr<T, Mag>( a0);
			hasinf = (T)0.;
		}
		else
		{
			s1 = (T)0.0;
			s2 = (T)0.0;
			s3 = (T)0.0;
			hasinf = sign(0);
		}
		for (int i = 0; i < nu; ++i)
		{
			ai = std::atanh(u[i].mag);
			if (!std::isinf(ai))
			{
				s1 += ai;
				s2 += std::fabs(ai);
				s3 += lr<T, Mag>( ai);
			}
			else if (hasinf == 0) { hasinf = sign(ai); }
			else if (hasinf != sign(ai)) { return -std::numeric_limits<T>::infinity(); }
		}
		return std::fabs(s1) - s2 + lr<T, Mag>(s1) - s3;
	}

	template<typename T, class Mag, magP<T, Mag>> inline Mag convert(const T &x)
	{ return Mag(x);}
	template<typename T, class Mag, magT<T, Mag>> inline Mag convert(const T &x)
	{ return Mag(clamp(std::atanh(x), -(T)30., (T)30.));}

	template<typename T, class Mag, magP<T, Mag>> inline T convert(const Mag &x)
	{ return x.mag;}
	template<typename T, class Mag, magT<T, Mag>> inline T convert(const Mag &x)
	{ return std::atanh(x.mag);}

	template<typename T, class Mag, magP<T, Mag>> inline Mag erfmix(const Mag &H, const T &mp, const T &mm)
	{ return Mag(H.mag * (std::erf(mp) - std::erf(mm)) / ((T)2. + H.mag * (std::erf(mp) + std::erf(mm)))); }
	template<typename T, class Mag, magT<T, Mag>> inline Mag erfmix(const Mag &H, const T &mp, const T &mm)
	{ return auxmix(H, AtanhErf::atanherf(mp), AtanhErf::atanherf(mm)); }

	template<typename T, class Mag, magP<T, Mag>> inline Mag exactmix(const Mag &H, const Mag &pp, const Mag &pm)
	{ return Mag( (pp.mag - pm.mag) * H.mag / ((T)2. + (pp.mag + pm.mag) * H.mag) ); }
	template<typename T, class Mag, magT<T, Mag>> inline Mag exactmix(const Mag &H, const Mag &pp, const Mag &pm)
	{ return auxmix(H, pp.mag, pm.mag); }

	template<typename T> inline MagT64<T> auxmix(const MagT64<T> &H, const T &ap, const T &am)
	{
		T aH = std::atanh(H.mag), t1, t2;
		if (aH == 0) { return MagT64<T>((T)0.0); }
		else
		{
			T xH = aH + ap, xh = aH + am;
			if (std::isinf(H.mag))
			{
				if (!std::isinf(ap) && !std::isinf(am))
				{
					t1 = sign(aH) * (ap - am) - std::fabs(ap) + std::fabs(am);
					t2 = -lr<T, MagT64<T>>(ap) + lr<T, MagT64<T>>(am);
				}
				else if (std::isinf(ap) && !std::isinf(am))
				{
					if (sign(ap) == sign(am))
					{
						t1 = -sign(aH) * am + std::fabs(am);
						t2 = lr<T, MagT64<T>>(am);
					}
					else
					{
						t1 = -2 * H.mInf;
						t2 = (T)0.0;
					}
				}
				else if (!std::isinf(ap) && std::isinf(am))
				{				
					if (sign(am) == sign(aH))
					{
						t1 = -sign(aH) * ap - std::fabs(ap);
						t2 = -lr<T, MagT64<T>>(ap);
					}
					else
					{
						t1 = 2 * H.mInf;
						t2 = (T)0.0;
					}
				}
				else
				{
					if ((sign(ap) == sign(aH) && sign(ap) == sign(aH)) || ((sign(ap) != sign(aH) && sign(ap) != sign(aH))))
					{
						t1 = (T)0.0;
						t2 = (T)0.0;
					}
					else if (sign(ap) == sign(aH))
					{
						t1 = 2 * H.mInf;
						t2 = (T)0.0;
					}
					else
					{
						t1 = -2 * H.mInf;
						t2 = (T)0.0;
					}
				}
			}
			else
			{
				t1 = (T)0.0;
				t1 += std::isinf(ap) ? (T)0.0 : std::fabs(xH) - std::fabs(ap);
				t1 -= std::isinf(am) ? (T)0.0 : std::fabs(xh) - std::fabs(am);
				t2 = lr<T, MagT64<T>>(xH) - lr<T, MagT64<T>>(ap) - lr<T, MagT64<T>>(xh) + lr<T, MagT64<T>>(am);
			}
		}
		return MagT64<T>((t1 + t2) / 2.);
	}
} // end of namespace Magnetization

//======================================================================================================================================================================//
//	___  ___  ___   _____  _   _  _____  _____  _____  ______  ___   _____  _____  _____  _   _         _____ ______  _____ ______   ___   _____  _____ ______  _____   //
//  |  \/  | / _ \ |  __ \| \ | ||  ___||_   _||_   _||___  / / _ \ |_   _||_   _||  _  || \ | |       |  _  || ___ \|  ___|| ___ \ / _ \ |_   _||  _  || ___ \/  ___|  //
//  | .  . |/ /_\ \| |  \/|  \| || |__    | |    | |     / / / /_\ \  | |    | |  | | | ||  \| |       | | | || |_/ /| |__  | |_/ // /_\ \  | |  | | | || |_/ /\ `--.   //
//  | |\/| ||  _  || | __ | . ` ||  __|   | |    | |    / /  |  _  |  | |    | |  | | | || . ` |       | | | ||  __/ |  __| |    / |  _  |  | |  | | | ||    /  `--. \  //
//  | |  | || | | || |_\ \| |\  || |___   | |   _| |_ ./ /___| | | |  | |   _| |_ \ \_/ /| |\  |       \ \_/ /| |    | |___ | |\ \ | | | |  | |  \ \_/ /| |\ \ /\__/ /  //
//  \_|  |_/\_| |_/ \____/\_| \_/\____/   \_/   \___/ \_____/\_| |_/  \_/   \___/  \___/ \_| \_/        \___/ \_|    \____/ \_| \_|\_| |_/  \_/   \___/ \_| \_|\____/   //
//	                                                                                                                                                					//
//======================================================================================================================================================================//

template<typename T> MagP64<T>::MagP64(const T &x)
{ mag = x;}
template<typename T> MagT64<T>::MagT64(const T &x, T m) 
{ mag = (T)std::tanh(x); mInf = m; };

template<typename T> MagP64<T>::MagP64(const MagP64<T> &m) 
{ this->mag = m.mag; }
template<typename T> MagT64<T>::MagT64(const MagT64<T> &m) 
{ this->mag = m.mag; this->mInf = m.mInf;}

template<typename T> inline std::string MagP64<T>::magformat() 
{ return "plain"; };
template<typename T> inline std::string MagT64<T>::magformat() 
{ return "tanh"; };

template<typename T> inline MagP64<T> MagP64<T>::operator+(const MagP64<T> &m)
{ return MagP64<T>(this->mag + m.mag);}
template<typename T> MagT64<T> MagT64<T>::operator+(const MagT64<T> &m)
{ return MagT64<T>(this->mag + m.mag);}

template<typename T> inline T MagP64<T>::operator+(const T &x)
{ return x + this->mag;}
template<typename T> inline T MagT64<T>::operator+(const T &x)
{ return x + this->mag;}

template<typename T> inline MagP64<T> MagP64<T>::operator%(const MagP64<T> &m)
{ return MagP64<T>( Magnetization::clamp( (this->mag + m.mag) / ((T)1. + this->mag * m.mag), -(T)1., (T)1.)) ; }
template<typename T> inline MagT64<T> MagT64<T>::operator%(const MagT64<T> &m)
{
	T tmp = std::atanh(this->mag) + std::atanh(m.mag);
	return std::isnan(tmp) ? MagT64<T>(std::numeric_limits<T>::infinity()) : MagT64<T>(std::atanh(this->mag) + std::atanh(m.mag)); // temporary solution of nan problem (see atanherf)
}

template<typename T> inline MagP64<T> operator-(const MagP64<T> &m) 
{ return MagP64<T>(-m.mag); }
template<typename T> inline MagT64<T> operator-(const MagT64<T> &m) 
{ return MagT64<T>(-m.mag); }

template<typename T> std::ostream& operator<<(std::ostream& os, const MagP64<T> &m) 
{ os << m.mag << " "; return os; }
template<typename T> std::ostream& operator<<(std::ostream& os, const MagT64<T> &m) 
{ os << m.mag << " "; return os; }

template<typename T> inline MagP64<T>& MagP64<T>::operator=(const MagP64<T> &m) 
{ this->mag = m.mag; return *this; }
template<typename T> inline MagT64<T>& MagT64<T>::operator=(const MagT64<T> &m) 
{ this->mag = m.mag; this->mInf = m.mInf; return *this; }

template<typename T> inline MagP64<T>& MagP64<T>::operator+=(const MagP64<T> &m) 
{ this->mag += m.mag; return *this; }
template<typename T> inline MagT64<T>& MagT64<T>::operator+=(const MagT64<T> &m) 
{ this->mag += m.mag; return *this; }

template<typename T> inline MagP64<T> MagP64<T>::operator/(const T &x) 
{ return MagP64<T>(this->mag / x); }
template<typename T> inline MagT64<T> MagT64<T>::operator/(const T &x) 
{ return MagT64<T>(this->mag / x); }

template<typename T> inline MagP64<T> operator/(const T &x, const MagP64<T> &m) 
{ return MagP64<T>(x / m.mag); }
template<typename T> inline MagT64<T> operator/(const T &x, const MagT64<T> &m) 
{ return MagT64<T>(x / m.mag); }

template<typename T> inline MagP64<T> MagP64<T>::operator/(const MagP64<T> &m)
{ return MagP64<T>(this->mag / m.mag);}
template<typename T> inline MagT64<T> MagT64<T>::operator/(const MagT64<T> &m)
{ return MagT64<T>(this->mag / m.mag);}

template<typename T> inline MagP64<T> MagP64<T>::operator*(const MagP64<T> &m) 
{ return MagP64<T>(this->mag * m.mag); }
template<typename T> inline MagT64<T> MagT64<T>::operator*(const MagT64<T> &m) 
{
	T ax = std::atanh(this->mag), ay = std::atanh(m.mag), t1, t2;
	t1 = (ax >= ay && ax >= -ay) ? 2 * ay : (ax >= ay && ax < -ay) ? -2 * ax : (ax < ay && ax >= -ay) ? 2 * ax : -2 * ay;
	t2 = (std::isinf(ax) || std::isinf(ay)) ? (T)0.0 : Magnetization::lr<T, MagT64<T>>(ax + ay) - Magnetization::lr<T, MagT64<T>>(ax - ay);
	return MagT64<T>((t1 + t2) / 2);
}

template<typename T> inline T MagP64<T>::operator*(const T &x) 
{ return this->mag * x; }
template<typename T> inline T MagT64<T>::operator*(const T &x) 
{ return this->mag * x; }

template<typename T> inline T operator*(const T &x, const MagP64<T> &m) 
{ return m.mag * x; }
template<typename T> inline T operator*(const T &x, const MagT64<T> &m) 
{ return m.mag * std::atanh(x); }

template<typename T> inline MagP64<T> MagP64<T>::operator-(const MagP64<T> &m) 
{ return MagP64<T>(this->mag - m.mag); }
template<typename T> inline MagT64<T> MagT64<T>::operator-(const MagT64<T> &m) 
{ return MagT64<T>(this->mag - m.mag); }

template<typename T> inline MagP64<T> MagP64<T>::operator-(const T &x)
{ return MagP64<T>(this->mag - x);}
template<typename T> inline MagT64<T> MagT64<T>::operator-(const T &x)
{ return MagT64<T>(this->mag - x);}

template<typename T> inline MagP64<T> operator-(const T &x, const MagP64<T> &m)
{ return MagP64<T>(x - m.mag);}
template<typename T> inline MagT64<T> operator-(const T &x, const MagT64<T> &m)
{ return MagT64<T>(x - m.mag);}

template<typename T> inline bool MagP64<T>::operator==(const MagP64<T> &m) 
{ return this->mag == m.mag; }
template<typename T> bool MagT64<T>::operator==(const MagT64<T> &m) 
{ return this->mag == m.mag; }

template<typename T> inline bool MagP64<T>::operator!=(const MagP64<T> &m) 
{ return this->mag != m.mag; }
template<typename T> inline bool MagT64<T>::operator!=(const MagT64<T> &m) 
{ return this->mag != m.mag; }


//===============================================================================================================================//
//    ___   _____   ___   _   _  _   _  _____ ______ ______      ______  _   _  _   _  _____  _____  _____  _____  _   _  _____  //
//   / _ \ |_   _| / _ \ | \ | || | | ||  ___|| ___ \|  ___|     |  ___|| | | || \ | |/  __ \|_   _||_   _||  _  || \ | |/  ___| //
//  / /_\ \  | |  / /_\ \|  \| || |_| || |__  | |_/ /| |_        | |_   | | | ||  \| || /  \/  | |    | |  | | | ||  \| |\ `--.  //
//  |  _  |  | |  |  _  || . ` ||  _  ||  __| |    / |  _|       |  _|  | | | || . ` || |      | |    | |  | | | || . ` | `--. \ //
//  | | | |  | |  | | | || |\  || | | || |___ | |\ \ | |         | |    | |_| || |\  || \__/\  | |   _| |_ \ \_/ /| |\  |/\__/ / //
//  \_| |_/  \_/  \_| |_/\_| \_/\_| |_/\____/ \_| \_|\_|         \_|     \___/ \_| \_/ \____/  \_/   \___/  \___/ \_| \_/\____/  //
//                                                                                                                               //
//===============================================================================================================================//


namespace AtanhErf
{
	template<typename T> void getinp(const T &mm, const T &st, T *&inp)
	{
		std::string filename = "atanherf_interp.max_" + std::to_string(mm) + ".step_" + std::to_string(st) + ".txt";
		std::ifstream is(filename, std::ios::binary);
		int i = 0;
		if (is)
		{
			inp = new T[int((mm - 1) / st)];
			std::for_each(inp, inp + int((mm - 1) / st), [&is](const T &val){is.read((char*)&val, sizeof(T));});
			is.close();
			return;
		}
		else
		{
			// temporary solution without splines
			std::cout << "Computing atanh(erf(x)) table, this may take a while..." << std::endl;
			std::ofstream os(filename, std::ios::out | std::ios::binary);
			inp = new T[int((mm - 1) / st)];
			std::generate(inp, inp + int((mm - 1) / st), [&i, &st]{return std::atanh(std::erf((i++)*st + 1));});
			std::for_each(inp, inp + int((mm - 1) / st), [&os](const T &val){os.write( (const char *) &val, sizeof( T ));});
			os.close();
			return;
		}
	}

	template<typename T> T atanherf_interp(const T &x)
	{
		T *r = new T[int((16. - 1) / 1e-4)], *inp = nullptr, res = (T)0.;
		std::generate(r, r + int((16. - 1) / 1e-4), [&res]{return (res++)*1e-4 + 1;});
		AtanhErf::getinp((T)16., 1e-4, inp);
		res = inp[int((x - r[0]) / 1e-4 + 1)];
		delete[] r;
		delete[] inp;
		return res;
	}

	template<typename T> inline T evalpoly(const T &t)
	{
		T t2 = t * t, t3 = t * t * t, t6 = t3 * t3;
		return (T)1. - (T)1.25 * t + (T)3.0833333333333335 * t2 - (T)11.03125 * t3 + (T)51.0125 * t2 * t2 - (T)287.5260416666667 * t3 * t2 + (T)1906.689732142857 * t6 - (T)14527.3759765625 * t6 * t + (T)125008.12543402778 * t6 * t2 - (T)1.1990066259765625e6 * t6 * t3;
	}


	template<typename T> inline T atanherf_largex(const T &x)
	{
		T t = (T)1. / (x*x);
		return sign(x) * (2 * std::log(std::fabs(x)) + std::log(4 * M_PI) + 2 * x*x + t * AtanhErf::evalpoly(t)) / 4;
	}

	template<typename T> inline T atanherf(const T &x)
	{ return (std::fabs(x) <= 2) ? std::atanh(std::erf(x)) : (std::fabs(x) <= 15) ? sign(x) * AtanhErf::atanherf_interp(std::fabs(x)) : AtanhErf::atanherf_largex(x); }

} // end of namespace AtanhErf


//=============================================================================================================//
//    _____   ___   _   _  _____  _____ __   __     ___  ___ _____  _____  _____   ___   _____  _____  _____   // 
//   /  __ \ / _ \ | | | ||_   _||_   _|\ \ / /     |  \/  ||  ___|/  ___|/  ___| / _ \ |  __ \|  ___|/  ___|  // 
//   | /  \// /_\ \| | | |  | |    | |   \ V /      | .  . || |__  \ `--. \ `--. / /_\ \| |  \/| |__  \ `--.   // 
//   | |    |  _  || | | |  | |    | |    \ /       | |\/| ||  __|  `--. \ `--. \|  _  || | __ |  __|  `--. \  // 
//   | \__/\| | | |\ \_/ / _| |_   | |    | |       | |  | || |___ /\__/ //\__/ /| | | || |_\ \| |___ /\__/ /  // 
//    \____/\_| |_/ \___/  \___/   \_/    \_/       \_|  |_/\____/ \____/ \____/ \_| |_/ \____/\____/ \____/   // 
//                                                                                                             // 
//=============================================================================================================//

template<typename T, class Mag> void Cavity_Message<T, Mag>::error(const std::string &messages, const int &n)
{
	if( n )
	{
		std::cerr << "Cavity Messages error! " << messages << std::endl;
		exit(n);
	}
	else
		std::cerr << "Cavity_Message warning! " << messages << std::endl;
	return;
}

template<typename T, class Mag> Cavity_Message<T, Mag> Cavity_Message<T, Mag>::read_messages(const std::string &io)
{
	int n, m, k, seed;
	T x;
	std::string row;
	std::vector<std::string> l;
	std::ifstream is(io);
	if ( !is ) error("Messages file not found. Given: " + io, 1);
	std::getline(is, row);
	l = split(row, "\t");
	if (l.size() != 2 || l[0] == "fmt") error("Invalid messages file", 1);
	std::getline(is, row);
	l.resize(0);
	l = split(row, "\t");
	if (l.size() != 4 || l[0] != "N,M,K,x,seed:") error("Invalid messages file", 1);

	n = std::stoi(l[1].c_str());
	m = std::stoi(l[2].c_str());
	k = std::stoi(l[3].c_str());
	x = (T)std::stod(l[4].c_str());
	seed = std::stoi(l[5].c_str());

	Cavity_Message<T, Mag> message(n, m, k, x, seed);
	l.clear();
	return message;
}

template<typename T, class Mag> Cavity_Message<T, Mag>::Cavity_Message()
{
	this->N = 0;
	this->K = 0;
	this->M = 0;
}

template<typename T, class Mag> Cavity_Message<T, Mag>::Cavity_Message(const int &m, const int &n, const int &k, const T &x, const int &start)
{
	this->M = m;
	this->N = n;
	this->K = k;
	this->m_star_j = new Mag*[this->K];
	this->m_j_star = new Mag*[this->K];
	this->m_in = new Mag*[this->M];
	this->weights = new Mag**[this->M];
	this->m_no = new Mag*[this->M];
	this->m_on = new Mag[this->M];
	this->m_ni = new Mag*[this->M];

	std::generate(this->m_star_j, this->m_star_j + this->K, [&n]{return new Mag[n];});
	std::generate(this->m_j_star, this->m_j_star + this->K, [&n]{return new Mag[n];});
	std::generate(this->m_in, this->m_in + this->M, 		[&k]{return new Mag[k];});
	std::generate(this->m_no, this->m_no + this->M, 		[&k]{return new Mag[k];});
	std::generate(this->m_ni, this->m_ni + this->M, 		[&k]{return new Mag[k];});
	std::generate(this->weights, this->weights + this->M, 	[&k]{return new Mag*[k];});

	Magnetization::zeros<T>(this->m_on, this->M);
	
	std::mt19937 generator(start);
	std::uniform_real_distribution<T> distribution((T)0., (T)1.);

	for (int i = 0; i < this->K; ++i)
	{
		Magnetization::zeros<T>(this->m_star_j[i], this->N);
		Magnetization::zeros<T>(this->m_j_star[i], this->N);
	}

	for (int i = 0; i < this->M; ++i)
	{
		Magnetization::zeros<T>(this->m_no[i], this->K);
		Magnetization::zeros<T>(this->m_ni[i], this->K);
		Magnetization::zeros<T>(this->m_in[i], this->K);
		for (int ii = 0; ii < this->K; ++ii)
		{
			this->weights[i][ii] = new Mag[this->N];
			this->m_in[i][ii] = this->m_in[i][ii] % this->m_no[i][ii] % this->m_ni[i][ii];
			std::generate(this->weights[i][ii], this->weights[i][ii] + this->N, [&x, &distribution, &generator]{return Magnetization::f2m<T, Mag>(x * (2 * distribution(generator) - 1));});
		
		}
	}
// have to find better solution!! (functional programming)
	for (int i = 0; i < this->K; ++i)
	{
		for (int ii = 0; ii < this->N; ++ii)
		{
			for (int iii = 0; iii < this->M; ++iii)
				this->m_j_star[i][ii] = this->m_j_star[i][ii] % this->weights[iii][i][ii];
		}
	}
}


template<typename T, class Mag> Cavity_Message<T, Mag>::Cavity_Message(const Cavity_Message<T, Mag> &m)
{ 
	this->M = m.M; 
	this->N = m.N; 
	this->K = m.K;
	this->m_star_j 	= new Mag*[this->K];
	this->m_j_star 	= new Mag*[this->K];
	this->m_in 		= new Mag*[this->M];
	this->weights 	= new Mag**[this->M];
	this->m_no 		= new Mag*[this->M];
	this->m_on 		= new Mag [this->M];
	this->m_ni 		= new Mag*[this->M];

	std::generate(this->m_star_j, this->m_star_j + this->K, [this]{return new Mag[this->N];});
	std::generate(this->m_j_star, this->m_j_star + this->K, [this]{return new Mag[this->N];});
	std::generate(this->m_in, this->m_in + this->M, 		[this]{return new Mag[this->K];});
	std::generate(this->m_no, this->m_no + this->M, 		[this]{return new Mag[this->K];});
	std::generate(this->m_ni, this->m_ni + this->M, 		[this]{return new Mag[this->K];});
	std::generate(this->weights, this->weights + this->M, 	[this]{return new Mag*[this->K];});

	for (int i = 0; i < this->K; ++i)
	{
		std::memcpy(this->m_star_j[i], m.m_star_j[i], sizeof(Mag)*this->N);
		std::memcpy(this->m_j_star[i], m.m_j_star[i], sizeof(Mag)*this->N);
	}
	std::memcpy(this->m_on, m.m_on, sizeof(Mag)*this->M);
	for (int i = 0; i < this->M; ++i)
	{
		std::memcpy(this->m_in[i], m.m_in[i], sizeof(Mag)*this->K);
		std::memcpy(this->m_no[i], m.m_no[i], sizeof(Mag)*this->K);
		std::memcpy(this->m_ni[i], m.m_ni[i], sizeof(Mag)*this->K);
		for(int j = 0; j < this->K; ++j)
		{
			this->weights[i][j] = new Mag[this->N];
			std::memcpy(this->weights[i][j], m.weights[i][j], sizeof(Mag)*this->N);
		}
	} 
}

template<typename T, class Mag> Cavity_Message<T, Mag>& Cavity_Message<T, Mag>::operator=(const Cavity_Message<T, Mag> &m)
{
	if(this->K != 0)
	{
		for(int i = 0; i < this->K; ++i) 
		{
			delete[] this->m_j_star[i];
			delete[] this->m_star_j[i];
		}
		delete[] this->m_j_star;
		delete[] this->m_star_j;
	}
	if(this->M != 0)
	{
		for(int i = 0; i < this->M; ++i)
		{
			delete[] this->m_in[i];
			delete[] this->m_no[i];
			delete[] this->m_ni[i];
			for(int j = 0; j < this->K; ++j)
			{
				delete[] this->weights[i][j];
			}
			delete[] this->weights[i];
		}
		delete[] this->m_in;
		delete[] this->m_no;
		delete[] this->m_ni;
		delete[] this->weights;

		delete[] this->m_on;
	}
	this->M = m.M; 
	this->N = m.N; 
	this->K = m.K;
	this->m_star_j = new Mag*[this->K];
	this->m_j_star = new Mag*[this->K];
	this->m_in = new Mag*[this->M];
	this->weights = new Mag**[this->M];
	this->m_no = new Mag*[this->M];
	this->m_on = new Mag[this->M];
	this->m_ni = new Mag*[this->M];
	std::generate(this->m_star_j, this->m_star_j + this->K, [this]{return new Mag[this->N];});
	std::generate(this->m_j_star, this->m_j_star + this->K, [this]{return new Mag[this->N];});
	std::generate(this->m_in, this->m_in + this->M, 		[this]{return new Mag[this->K];});
	std::generate(this->m_no, this->m_no + this->M, 		[this]{return new Mag[this->K];});
	std::generate(this->m_ni, this->m_ni + this->M, 		[this]{return new Mag[this->K];});
	std::generate(this->weights, this->weights + this->M, 	[this]{return new Mag*[this->K];});
	for (int i = 0; i < this->K; ++i)
	{
		std::memcpy(this->m_star_j[i], m.m_star_j[i], sizeof(Mag)*this->N);
		std::memcpy(this->m_j_star[i], m.m_j_star[i], sizeof(Mag)*this->N);
	}
	std::memcpy(this->m_on, m.m_on, sizeof(Mag)*this->M);
	for (int i = 0; i < this->M; ++i)
	{
		std::memcpy(this->m_in[i], m.m_in[i], sizeof(Mag)*this->K);
		std::memcpy(this->m_no[i], m.m_no[i], sizeof(Mag)*this->K);
		std::memcpy(this->m_ni[i], m.m_ni[i], sizeof(Mag)*this->K);
		for(int j = 0; j < this->K; ++j)
		{
			this->weights[i][j] = new Mag[this->N];
			std::memcpy(this->weights[i][j], m.weights[i][j], sizeof(Mag)*this->N);
		}
	}
	return *this;
}

template<typename T, class Mag> Cavity_Message<T, Mag>::~Cavity_Message()
{
	if(this->K != 0)
	{
		for(int i = 0; i < this->K; ++i) 
		{
			delete[] this->m_j_star[i];
			delete[] this->m_star_j[i];
		}
		delete[] this->m_j_star;
		delete[] this->m_star_j;
	}
	if(this->M != 0)
	{
		for(int i = 0; i < this->M; ++i)
		{
			delete[] this->m_in[i];
			delete[] this->m_no[i];
			delete[] this->m_ni[i];
			for(int j = 0; j < this->K; ++j) delete[] this->weights[i][j];
			delete[] this->weights[i];
		}
		delete[] this->m_in;
		delete[] this->m_no;
		delete[] this->m_ni;
		delete[] this->weights;

		delete[] this->m_on;
	}
}

int** read_weights(const std::string &filename, int &K, int &N, bool bin)
{
	int **W = nullptr;
	std::ifstream is;
	if(bin)
	{
		is.open(filename, std::ios::binary);
		if( !is ) {std::cerr << "Weights file not found! Given : " << filename << std::endl; exit(1);}
		is.read((char*)&K, sizeof(int));
		is.read((char*)&N, sizeof(int));
		W = new int*[K];
		std::generate(W, W + K, [&N](){return new int[N];});
		for(int i = 0; i < K; ++i)
			std::for_each(W[i], W[i] + N, [&is](int &val){is.read((char*)&val, sizeof(int));});
		//TODO maybe a generate
		is.close();
	}
	else
	{
		std::string row;
		is.open(filename);
		if(!is){std::cerr << "Weights file not found! Given : " << filename << std::endl; exit(1);}
		std::getline(is, row); // fmt:
		is >> row;
		is >> K;
		is >> N;
		W = new int*[K];
		std::generate(W, W + K, [&N](){return new int[N];});
		for(int i = 0; i < K; ++i)
			std::for_each(W[i], W[i] + N, [&is](int &val){ is >> val; });
		is.close();
	}
	return W;
}

template<typename T, class Mag> void save_weights(const std::string &filename, const Cavity_Message<T, Mag> &message, const int &seed, const int &max_iters, const T &randfact, const T &damping, const std::string &accuracy1, const std::string &accuracy2)
{
	std::ofstream os(filename);
	os << "fmt: " << Mag::magformat() << ", seed: " << seed << ", max_iters: " << max_iters << ", randfact: " << randfact << ", damping: " << damping << ", accuracy1: " << accuracy1 << ", accuracy2: " << accuracy2 << std::endl <<
		  "K,N:\t" << message.K << "\t" << message.N << std::endl;
	for (int i = 0; i < message.K; ++i)
	{
		std::for_each(message.m_j_star[i], message.m_j_star[i] + message.N, [&os](const Mag &m){os << Magnetization::sign0<T, Mag>(m) << "\t";});
		os << std::endl;
	}
	os << "END" << std::endl;
	os.close();
	return;
}

void save_weights(const std::string &filename, int **W, const int &K, const int &N)
{
	std::ofstream os(filename, std::ios::out | std::ios::binary);
	os.write( (const char *) &K, sizeof( int ));
	os.write( (const char *) &N, sizeof( int ));
	
	//TODO maybe a generate	
	for(int i = 0; i < K; ++i)
		for(int j = 0; j < N; ++j)
			os.write( (const char *) &W[i][j], sizeof( int ));
	os.close();
}

template<typename T> int compute_output(T *input, int **W, const int &K, const int &N)
{
	return sign(std::accumulate(	W, W + K, 0, 
							[&input, &N] (const int &out, int *Wi)
							{
								return out + sign( std::inner_product( Wi, Wi + N, input, (T)0.) );
							})
				);
}

template<typename T, class Mag> int compute_output(T *input, MagVec2<Mag> W, const int &K, const int &N)
{
	return sign(std::accumulate(	W, W + K, 0, 
							[&input, &N] (const int &out, MagVec<Mag> Wi)
							{
								return out + sign( std::inner_product( Wi, Wi + N, input, (T)0.) );
							})
				);
}


template<typename T, class Mag> int** M2W(MagVec2<Mag> M, const int &K, const int &N)
{
	int **W = new int*[K];
	std::generate(W, W + K, [&N](){return new int[N];});
	
	for(int i = 0; i < K; ++i)
		std::transform(M[i], M[i] + N, W[i],
						[](const Mag &Mij)
						{
							return Magnetization::sign0<T, Mag>(Mij);
						});
	return W;
};


template<typename T, class Mag, magP<T, Mag>> inline void set_outfields(const Cavity_Message<T, Mag> &message, int *output, const T &beta)
{
	//assert(Nout == message.M);
	T t = (T)tanh(beta / 2);
	std::transform(output, output + message.M, message.m_on, [&t](const int o){return Mag(o * t);});
	return;
}

template<typename T, class Mag, magT<T, Mag>> inline void set_outfields(const Cavity_Message<T, Mag> &message, int *output, const T &beta)
{
	//assert(Nout == message.M);
	T t = (T)tanh(beta / 2);
	std::transform(output, output + message.M, message.m_on, [&t](const int o){return Mag(std::atanh(o * t));});
	return;
}

template<typename T, class Mag> std::ostream& operator<<(std::ostream &os, const Cavity_Message<T, Mag> &message)
{
	for (int k = 0; k < message.K; ++k)
		for (int i = 0; i < message.N; ++i)
			os << k << " " << i << " " << message.m_j_star[k][i].mag << std::endl;
	return os;
}



//======================================================================================================//
//   ______   ___  ______   ___  ___  ___ _____     ___  ___ _____  _____  _   _  _____ ______  _____ 	//
//   | ___ \ / _ \ | ___ \ / _ \ |  \/  |/  ___|    |  \/  ||  ___||_   _|| | | ||  _  ||  _  \/  ___|	//
//   | |_/ // /_\ \| |_/ // /_\ \| .  . |\ `--.     | .  . || |__    | |  | |_| || | | || | | |\ `--. 	//
//   |  __/ |  _  ||    / |  _  || |\/| | `--. \    | |\/| ||  __|   | |  |  _  || | | || | | | `--. \	//
//   | |    | | | || |\ \ | | | || |  | |/\__/ /    | |  | || |___   | |  | | | |\ \_/ /| |/ / /\__/ /	//
//   \_|    \_| |_/\_| \_|\_| |_/\_|  |_/\____/     \_|  |_/\____/   \_/  \_| |_/ \___/ |___/  \____/ 	//
//                                                                                                    	//
//======================================================================================================//


template<typename T, class Mag> Params<T, Mag>::Params(const T &d, const T &e, const T &b, const int &it, const std::string &ac1, const std::string &ac2, const T &R, const T &p, bool q)
{
	this->damping = d;
	this->epsil = e;
	this->beta = b;
	this->max_iters = it;
	this->accuracy1 = ac1;
	this->accuracy2 = ac2;
	this->r = R;
	this->tan_gamma = Mag(p);
	this->quiet = q;
};


//==================================================================================================================================//
//  ______  _____  _____  _   _  _____  _____  _   _  _____      ______ ______  _____  _____  _____  _____  _____  _      _____ 	//
//  |  ___||  _  |/  __ \| | | |/  ___||_   _|| \ | ||  __ \     | ___ \| ___ \|  _  ||_   _||  _  |/  __ \|  _  || |    /  ___|	//
//  | |_   | | | || /  \/| | | |\ `--.   | |  |  \| || |  \/     | |_/ /| |_/ /| | | |  | |  | | | || /  \/| | | || |    \ `--. 	//
//  |  _|  | | | || |    | | | | `--. \  | |  | . ` || | __      |  __/ |    / | | | |  | |  | | | || |    | | | || |     `--. \	//
//  | |    \ \_/ /| \__/\| |_| |/\__/ / _| |_ | |\  || |_\ \     | |    | |\ \ \ \_/ /  | |  \ \_/ /| \__/\\ \_/ /| |____/\__/ /	//
//  \_|     \___/  \____/ \___/ \____/  \___/ \_| \_/ \____/     \_|    \_| \_| \___/   \_/   \___/  \____/ \___/ \_____/\____/ 	//
//                                                                                                                              	//
//==================================================================================================================================//


template<typename T> FocusingProtocol<T>::~FocusingProtocol()
{
	if(this->Nrep)
	{
		delete[] this->gamma;
		delete[] this->n_rep;
		delete[] this->beta;
	}
}

template<typename T> void FocusingProtocol<T>::StandardReinforcement(T *rho, const int &Nrho)
{
	this->Nrep = Nrho;
	this->gamma = new T[Nrho];
	this->n_rep = new T[Nrho];
	this->beta = new T[Nrho];
	std::fill_n(this->gamma, Nrho, std::numeric_limits<T>::infinity());
	std::transform(rho, rho + Nrho, this->n_rep, [](const T &i){ return (T)1. / ((T)1. - i);});
	std::fill_n(this->beta, Nrho, std::numeric_limits<T>::infinity());
	return;
}

template<typename T> void FocusingProtocol<T>::StandardReinforcement(const Vec<T> &rho)
{
	this->Nrep = (int)rho.size();
	this->gamma = new T[this->Nrep];
	this->n_rep = new T[this->Nrep];
	this->beta = new  T[this->Nrep];
	std::fill_n(this->gamma, this->Nrep, std::numeric_limits<T>::infinity());
	std::transform(rho.begin(), rho.end(), this->n_rep, [](const T &i){ return (T)1. / ((T)1. - i);});
	std::fill_n(this->beta, this->Nrep, std::numeric_limits<T>::infinity());
	return;
}

template<typename T> void FocusingProtocol<T>::StandardReinforcement(const T &drho)
{
	this->Nrep = int((1 - drho) / drho);
	T step = (T)0.;
	this->gamma = new T[this->Nrep];
	this->beta = new  T[this->Nrep];
	this->n_rep = new T[this->Nrep];
	std::fill_n(this->gamma, this->Nrep, std::numeric_limits<T>::infinity());
	std::generate(this->n_rep, this->n_rep + this->Nrep, [&drho, &step]{return (T)1. / ((T)1. - (drho * step++));});
	std::fill_n(this->beta, this->Nrep, std::numeric_limits<T>::infinity());
	return;
}

template<typename T> void FocusingProtocol<T>::Scoping(T *gr, const T &x, const int &ngr)
{
	this->Nrep = ngr;
	this->gamma = new T[this->Nrep];
	this->beta = new  T[this->Nrep];
	this->n_rep = new T[this->Nrep];
	std::memcpy(this->gamma, gr, sizeof(T)*ngr);
	std::fill_n(this->n_rep, ngr, x);
	std::fill_n(this->beta, ngr, std::numeric_limits<T>::infinity());
	return;
}

template<typename T> void FocusingProtocol<T>::Scoping(const Vec<T> &gr, const T &x)
{
	this->Nrep = (int)gr.size();
	this->gamma = new T[this->Nrep];
	this->beta = new  T[this->Nrep];
	this->n_rep = new T[this->Nrep];
	std::memcpy(this->gamma, &gr[0], sizeof(T)*this->Nrep);
	std::fill_n(this->n_rep, this->Nrep, x);
	std::fill_n(this->beta, this->Nrep, std::numeric_limits<T>::infinity());
	return;
}

template<typename T> void FocusingProtocol<T>::PseudoReinforcement(T *rho, const int &nrho, T x)
{
	this->Nrep = nrho;
	this->gamma = new T[this->Nrep];
	this->beta = new  T[this->Nrep];
	this->n_rep = new T[this->Nrep];
	std::transform(rho, rho + nrho, this->gamma, [&x](const T &i){return (T)std::atanh(std::pow(i, x));});
	std::transform(rho, rho + nrho, this->n_rep, [&x](const T &i){return (T)1. + (T)std::pow(i, (T)1. - (T)2. * x) / ((T)1. - i);});
	std::fill_n(this->beta, this->Nrep, std::numeric_limits<T>::infinity());
	return;
}

template<typename T> void FocusingProtocol<T>::PseudoReinforcement(const Vec<T> &rho, T x)
{
	this->Nrep = (int)rho.size();
	this->gamma = new T[this->Nrep];
	this->beta = new  T[this->Nrep];
	this->n_rep = new T[this->Nrep];
	std::transform(rho.begin(), rho.end(), this->gamma, [&x](const T &i){return (T)std::atanh(std::pow(i, x));});
	std::transform(rho.begin(), rho.end(), this->n_rep, [&x](const T &i){return (T)1. + (T)std::pow(i, (T)1. - (T)2. * x) / ((T)1. - i);});
	std::fill_n(this->beta, this->Nrep, std::numeric_limits<T>::infinity());
	return;
}
	
template<typename T> void FocusingProtocol<T>::PseudoReinforcement(const T &drho, T x)
{
	this->Nrep = int((1 - drho) / drho);
	this->gamma = new T[this->Nrep];
	this->n_rep = new T[this->Nrep];
	this->beta = new  T[this->Nrep];

	T step = 0;
	std::generate(this->gamma, this->gamma + this->Nrep, [&drho, &x, &step]{return (T)std::atanh(std::pow(drho * step++, x));});
	step = 0;
	std::generate(this->n_rep, this->n_rep + this->Nrep, [&drho, &x, &step]{return (T)1. + (T)std::pow(drho * step, (T)1. - (T)2. * x) / ((T)1. - drho * step++);});
	std::fill_n(this->beta, this->Nrep, std::numeric_limits<T>::infinity());
	return;		
}

template<typename T> void FocusingProtocol<T>::FreeScoping(T **list, const int &nlist)
{
	this->Nrep = nlist;
	this->gamma = new T[this->Nrep];
	this->n_rep = new T[this->Nrep];
	this->beta = new  T[this->Nrep];
	for (int i = 0; i < nlist; ++i)
	{
		this->gamma[i] = list[i][0];
		this->n_rep[i] = list[i][1];
		this->beta[i] = list[i][2];
	}
	return;
}

template<typename T> void FocusingProtocol<T>::FreeScoping(const Vec2<T> &list)
{
	this->Nrep = (int)list.size();
	this->gamma = new T[this->Nrep];
	this->n_rep = new T[this->Nrep];
	this->beta = new  T[this->Nrep];
	for (size_t i = 0; i < list.size(); ++i)
	{
		this->gamma[i] = list[i][0];
		this->n_rep[i] = list[i][1];
		this->beta[i] = list[i][2];
	}
	return;
}

//======================================//
//			______ ______ ______ 		//
//			|  ___|| ___ \| ___ \		//
//			| |_   | |_/ /| |_/ /		//
//			|  _|  | ___ \|  __/ 		//
//			| |    | |_/ /| |    		//
//			\_|    \____/ \_|    		//
//			                      		//
//======================================//



namespace FBP
{

	template<typename T, class Mag> T theta_node_update_approx(MagVec<Mag> m, Mag &M, T *xi, MagVec<Mag> u, Mag &U, const Params<T, Mag> &params, const int &nxi, const int &nm)
	{
		T maxdiff = (T)0., *vh = new T[nm], vH, mu = (T)0., sigma2 = (T)0., dsigma2, g, p0, pmu, psigma;
		MagVec<Mag> h = new Mag[nm];

		Mag old_m_on((T)0.), new_u((T)0.), new_m((T)0.);
		old_m_on = Magnetization::bar<T, Mag>(M, U);
		vH = old_m_on.mag;

		for (int i = 0; i < nm; ++i)
		{
			h[i] = Magnetization::bar<T, Mag>(m[i], u[i]);
			vh[i] = h[i].mag;
			mu += vh[i] * xi[i];
		}
		sigma2 = std::inner_product(vh, vh + nxi, xi, (T)0., std::plus<T>(), [](const T &i, const T &j){return (1 - i * i) * (j * j);});

		dsigma2 = 2 * sigma2;
		new_u = Magnetization::merf<T, Mag>(mu / std::sqrt(dsigma2));
		maxdiff = Magnetization::abs<T, Mag>(U - new_u);
		U = Magnetization::damp(new_u, U, params.damping);
		new_m = old_m_on % U;
		M = new_m;

		g = std::exp(-(mu*mu) / dsigma2) / std::sqrt(M_PI*dsigma2);

		p0 = 2 * vH * g / (1 + vH*U);
		pmu = p0 * (p0 + mu / sigma2);
		psigma = p0*(1 - mu / sigma2 - mu*p0) / dsigma2;

		for (int i = 0; i < nm; ++i)
		{
			new_u = Magnetization::convert<T, Mag>(Magnetization::clamp(xi[i]*(p0+xi[i]*(vh[i]*pmu+xi[i]*(1-vh[i]*vh[i])*psigma)), -1+std::numeric_limits<T>::epsilon(), 1- std::numeric_limits<T>::epsilon()));
			maxdiff = (T)std::fmax(maxdiff, Magnetization::abs<T, Mag>(new_u - u[i]));
			u[i] = Magnetization::damp(new_u, u[i], params.damping);
			m[i] = h[i] % u[i];
		}
		delete[]vh;
		delete[]h;
		return maxdiff;
	}

	template<typename T, class Mag> T theta_node_update_accurate(MagVec<Mag> m, Mag &M, T *xi, MagVec<Mag> u, Mag &U, const Params<T, Mag> &params, const int &nxi, const int &nm)
	{
		T maxdiff = (T)0., *vh = new T[nm], mu = (T)0., sigma2, tmp;
		MagVec<Mag> h = new Mag[nm];
		Mag old_m_on((T)0.), new_u((T)0.);
		old_m_on = Magnetization::bar<T, Mag>(M, U);

		for (int i = 0; i < nm; ++i)
		{
			h[i] = Magnetization::bar<T, Mag>(m[i], u[i]);
			vh[i] = h[i].mag;
			mu += vh[i] * xi[i];
		}
		sigma2 = std::inner_product(vh, vh + nxi, 
									xi, (T)0., 
									std::plus<T>(), 
									[](const T &i, const T &j)
									{
										return ((T)1. - i * i) * (j * j);
									});
		new_u = Magnetization::merf<T, Mag>(mu / std::sqrt(2*sigma2));
		U = Magnetization::damp(new_u, U, params.damping);
		M = old_m_on % U;

		for (int i = 0; i < nm; ++i)
		{
			tmp = std::sqrt(2 * (sigma2 - (1 - vh[i] * vh[i]) * xi[i] * xi[i]));
			new_u = Magnetization::erfmix(old_m_on, ((mu - xi[i] * vh[i]) + xi[i]) / tmp , ((mu - xi[i] * vh[i]) - xi[i]) / tmp);
			maxdiff = (T)std::fmax(maxdiff, Magnetization::abs<T, Mag>(new_u - u[i]));
			u[i] = Magnetization::damp(new_u, u[i], params.damping);
			m[i] = h[i] % u[i];
		}
		delete[]vh;
		delete[]h;
		return maxdiff;
	}

	template<typename T, class Mag> T theta_node_update_exact(MagVec<Mag> m, Mag &M, T *xi, MagVec<Mag> u, Mag &U, const Params<T, Mag> &params, const int &nxi, const int &nm)
	{
		T maxdiff = (T)0., *vh = new T[nm], **leftC = new T*[nm], **rightC = new T*[nm], z, vH, pm = (T)0., pp = (T)0., pz, p;
		MagVec<Mag> h = new Mag[nm];
		Vec<T> lC0, rC0;
		
		Mag old_m_on((T)0.), new_u((T)0.), mp((T)0.), mm((T)0.);
		old_m_on = Magnetization::bar<T, Mag>(M, U);
		vH = old_m_on.mag;
		for (int i = 0; i < nm; ++i)
		{
			h[i] = Magnetization::bar<T, Mag>(m[i], u[i]);
			vh[i] = h[i].mag;
			leftC[i] = new T[i + 2];
			rightC[i] = new T[nm - i + 1];
		}
		leftC[0][0] = ((T)1. - xi[0] * vh[0]) / (T)2.;
		leftC[0][1] = ((T)1. + xi[0] * vh[0]) / (T)2.;
		for (int i = 1; i < nm; ++i)
		{
			lC0.resize(0);
			for (int j = 0; j < i + 1; ++j) 
				lC0.push_back(leftC[i - 1][j]); 

			leftC[i][0] = lC0[0] * ((T)1. - (xi[i] * vh[i])) / (T)2.;
			
			for (int j = 1; j < i + 1; ++j) 
				leftC[i][j] = lC0[j - 1] * ((T)1. + (xi[i] * vh[i])) / (T)2. + lC0[j] * ((T)1. - (xi[i] * vh[i])) / (T)2.;

			leftC[i][i+1] = lC0[lC0.size() - 1] * ((T)1. + (xi[i] * vh[i])) / (T)2.;
		}

		rightC[nm-1][0] = ((T)1. - xi[nxi - 1] * vh[nm - 1]) / (T)2.;
		rightC[nm-1][1] = ((T)1. + xi[nxi - 1] * vh[nm - 1]) / (T)2.;
		for (int i = nm - 2; i >= 0; --i)
		{
			rC0.resize(0);
			for (int j = 0; j < nm - i; ++j) 
				rC0.push_back(rightC[i + 1][j]); 

			rightC[i][0] = rC0[0] * ((T)1. - (xi[i] * vh[i])) / (T)2.;
			
			for (int j = 1; j < nm - i + 1; ++j) 
				rightC[i][j] = rC0[j - 1] * ((T)1. + (xi[i] * vh[i])) / (T)2. + rC0[j] * ((T)1. - (xi[i] * vh[i])) / (T)2.;

			rightC[i][nm - i] = rC0[rC0.size() - 1] * ((T)1. + (xi[i] * vh[i])) / (T)2.;
		}

		assert(nm % 2);
		z = (nm + 1) / 2;
		for (int i = 0; i < nm + 1; ++i)
		{
			pm += (i < z) ? rightC[0][i] : (T)0.;
			pp += (i >= z) ? rightC[0][i] : (T)0.;
		}
		new_u = Magnetization::couple<T, Mag>(pp, pm);
		assert(!std::isinf(new_u.mag));
		U = Magnetization::damp(new_u, U, params.damping);
		M = old_m_on % U;
		assert(!std::isinf(M.mag));
		Vec<T> lC, rC;
		for (int i = 0; i < nm; ++i)
		{
			assert(xi[i] * xi[i] == 1);
			if (i > 0) 
			{ 
				lC.resize(0); 
				for (int j = 0; j < i+1; ++j) 
					lC.push_back(leftC[i - 1][j]); 
			}
			else 
			{ 
				lC.resize(0); 
				lC.push_back((T)1.); 
			}
			if (i < nm - 1)
			{ 
				rC.resize(0); 
				for (int j = 0; j < nm-i; ++j) 
					rC.push_back(rightC[i + 1][j]); 
			}
			else 
			{ 
				rC.resize(0); 
				rC.push_back((T)1.); 
			}
			pm = (T)0.0; pz = (T)0.0; pp = (T)0.0;
			for (int j = 0; j < nm; ++j)
			{
				p = (T)0.0;
				for (int k = std::max(0, j + i - nm + 1); k < std::min(j, i) + 1; ++k) 
					p += lC[k] * rC[j - k];  

				if (j < z-1) pm += p;
				else if (j == z-1) pz = p;
				else pp += p;
			}

			mp = Magnetization::convert<T, Mag>(Magnetization::clamp(pp + xi[i] * pz - pm, (T)-1.0, (T)1.0));
			mm = Magnetization::convert<T, Mag>(Magnetization::clamp(pp - xi[i] * pz - pm, (T)-1.0, (T)1.0));
			new_u = Magnetization::exactmix<T, Mag>(old_m_on, mp, mm);
			maxdiff = std::max(maxdiff, Magnetization::abs<T, Mag>(new_u - u[i]));
			u[i] = Magnetization::damp(new_u, u[i], params.damping);
			m[i] = h[i] % u[i];
			assert(!std::isinf(u[i].mag));
		}

		delete[]vh;
		delete[]h;
		for(int i = 0; i < nm; ++i) delete[] rightC[i];
		delete[] rightC;
		for (int i = 0; i < nm; ++i) delete[]leftC[i];
		delete[]leftC;
		return maxdiff;
	}

	template<typename T, class Mag> T free_energy_theta(MagVec<Mag> m, const Mag &M, T *xi, MagVec<Mag> u, const Mag &U, const int &nxi, const int &nm)
	{
		MagVec<Mag> h = new Mag[nm];
		T *vh = new T[nm], mu = (T)0., sigma, f;

		Mag old_m_on((T)0.), new_u((T)0.), b((T)0.);
		old_m_on = Magnetization::bar<T, Mag>(M, U);

		for (int i = 0; i < nm; ++i)
		{
			h[i] = Magnetization::bar<T, Mag>(m[i], u[i]);
			vh[i] = h[i].mag;
			mu += vh[i] * xi[i];
		}
		sigma = std::sqrt(2 * std::inner_product(vh, vh + nxi, 
											 	 xi, (T)0., 
											 	 std::plus<T>(), 
											 	 [](const T &v, const T &x)
											 	 {
											 	 	return ((T)1. - v * v) * (x * x);
											 	 }));
		b = Magnetization::merf<T, Mag>(mu/sigma);

		f = -Magnetization::log1pxy<T, Mag>(old_m_on, b);
		assert(!std::isinf(f));

		f = std::inner_product(h, h + nm,
							   u, f, 
							   std::plus<T>(), 
							   [](const Mag &h_, const Mag &u_)
							   {
							   	return Magnetization::log1pxy<T, Mag>(h_, u_);
							   });
		delete[] vh;
		delete[]h;
		return f;
	}

	// have to find better solution
	template<typename T, class Mag> T free_energy_theta_exact(MagVec<Mag> m, const Mag &M, T *xi, MagVec<Mag> u, const Mag &U, const int &nm)
	{
		T *vh = new T[nm], **leftC = new T*[nm], z, vH, pm = (T)0., pp = (T)0., f = (T)0.;
		MagVec<Mag> h = new Mag[nm];
		Vec<T> lC0, rC0, lC, rC, u1(nm);

		Mag old_m_on((T)0.), new_u((T)0.), mp((T)0.), mm((T)0.), b((T)0.);
		old_m_on = Magnetization::bar<T, Mag>(M, U);
		vH = old_m_on.mag;
		for (int i = 0; i < nm; ++i)
		{
			h[i] = Magnetization::bar<T, Mag>(m[i], u[i]);
			vh[i] = h[i].mag;
			leftC[i] = new T[nm + 1];
		}

		leftC[0][0] = (1 - xi[0] * vh[0]) / 2;
		leftC[0][1] = (1 + xi[0] * vh[0]) / 2;

		for (int i = 1; i < nm; ++i)
		{
			lC0.resize(0);
			for (int j = 0; j < i + 1; ++j) 
				lC0.push_back(leftC[i - 1][j]); 
			leftC[i][0] = lC0[0] * (1 - (xi[i] * vh[i])) / 2;
			for (int j = 1; j < i + 1; ++j) 
				leftC[i][j] = lC0[j - 1] * (1 + (xi[i] * vh[i])) / 2 + lC0[j] * (1 - (xi[i] * vh[i])) / 2; 
			leftC[i][i + 1] = lC0[lC0.size() - 1] * (1 + (xi[i] * vh[i])) / 2;
		}

		assert(nm % 2);
		z = int((nm + 1) / 2);
		for (int i = 0; i < nm + 1; ++i)
		{
			pm += (i < z) ? leftC[nm - 1][i] : 0.;
			pp += (i >= z) ? leftC[nm - 1][i] : 0.;
		}
		
		b = Magnetization::couple<T, Mag>(pp, pm);
		f -= Magnetization::log1pxy<T, Mag>(old_m_on, b);
		assert(!std::isinf(f));

		f = std::inner_product(h, h + nm, u, f, std::plus<T>(), [](const Mag &h, const Mag &u){return Magnetization::log1pxy<T, Mag>(h, u);});
		delete[]vh;
		delete[]h;
		for (int i = 0; i < nm; ++i)
			delete[]leftC[i];
		delete[]leftC;
		return f;
	}

	template<typename T, class Mag> T m_star_update(Mag &m_j_star, Mag &m_star_j, const Params<T, Mag> &params) //old entro_node_update
	{
		T diff;
		Mag old_m_j_star = Magnetization::bar<T, Mag>(m_j_star, m_star_j), new_m_star_j((T)0.), new_m_j_star((T)0.);
		if (params.r == (T)0. || params.tan_gamma.mag == (T)0.) { Magnetization::zero<T, Mag>(new_m_star_j);  }
		else if (params.r == std::numeric_limits<T>::infinity()) { (old_m_j_star == (T)0.) ? Magnetization::zero<T, Mag>(new_m_star_j) : Magnetization::copysign<Mag>(params.tan_gamma, old_m_j_star); }
		else { new_m_star_j = Magnetization::arrow((old_m_j_star * params.tan_gamma), params.r) * params.tan_gamma; }

		diff = Magnetization::abs<T, Mag>(new_m_star_j - m_star_j);
		new_m_star_j = Magnetization::damp(new_m_star_j, m_star_j, params.damping);
		new_m_j_star = old_m_j_star % new_m_star_j;
		m_j_star = new_m_j_star;
		m_star_j = new_m_star_j;
		return diff; 
	}


	template<typename T, class Mag> T iterate(Cavity_Message<T, Mag> &messages, const Patterns<T> &patterns, const Params<T, Mag> &params)
	{
		int i, j, k;
		T maxdiff = (T)0., *ones = new T[messages.K];
		T(*tnu1)(MagVec<Mag>, Mag &, T*, MagVec<Mag>, Mag &, const Params<T, Mag> &, const int &, const int &) = nullptr;
		T(*tnu2)(MagVec<Mag>, Mag &, T*, MagVec<Mag>, Mag &, const Params<T, Mag> &, const int &, const int &) = nullptr;
		Mag z((T)0.);
		if (params.accuracy1 == "accurate") { tnu1 = &theta_node_update_accurate; }
		else if (params.accuracy1 == "exact") { tnu1 = &theta_node_update_exact; }
		else if (params.accuracy1 == "none") { tnu1 = &theta_node_update_approx; }
		else { std::cerr << "accuracy must be one of 'exact', 'accurate', 'none'; was given " << params.accuracy1 << std::endl; }

		if (params.accuracy2 == "accurate") { tnu2 = &theta_node_update_accurate; }
		else if (params.accuracy2 == "exact") { tnu2 = &theta_node_update_exact; }
		else if (params.accuracy2 == "none") { tnu2 = &theta_node_update_approx; }
		else { std::cerr << "accuracy must be one of 'exact', 'accurate', 'none'; was given " << params.accuracy2 << std::endl; }

		int size = messages.M + messages.N*messages.K, *randperm = new int[size];
		std::iota(randperm, randperm + size, 0);
		std::random_shuffle(randperm, randperm + size);
		std::fill_n(ones, messages.K, (T)1.);
		for (int a = 0; a < size; ++a)
		{
			if (randperm[a] < messages.M)
			{
				z.mag = (T)0.;
				for (k = 0; k < messages.K; ++k)
					maxdiff = std::fmax(maxdiff, tnu1(messages.m_j_star[k], messages.m_in[randperm[a]][k], patterns.input[randperm[a]] , messages.weights[randperm[a]][k], messages.m_no[randperm[a]][k], params, patterns.Ncol, messages.N));

				maxdiff = std::fmax(maxdiff, tnu2(messages.m_in[randperm[a]], messages.m_on[randperm[a]], ones, messages.m_ni[randperm[a]], z, params, messages.K, messages.K));
			}

			else
				if (!(params.r == 0 || params.tan_gamma.mag == 0)) 
				{
					j = int(randperm[a] - messages.M);
					k = int((T)j / messages.N);
					i = int((j) % messages.N);
					maxdiff = std::fmax(maxdiff, m_star_update(messages.m_j_star[k][i], messages.m_star_j[k][i], params));
				}
		}
		return maxdiff;
	}

	template<typename T, class Mag> bool converge( Cavity_Message<T, Mag> &messages, const Patterns<T> &patterns, const Params<T, Mag> &params)
	{
		bool ok = false;
		T diff;
		
		auto start_time = std::chrono::system_clock::now();
		for (int it = 0; it < params.max_iters; ++it)
		{
			diff = iterate(messages, patterns, params);
			if (!params.quiet) std::cout << "\r" << "[it=" << it << " Delta=" << diff << " lambda=" << params.damping << "]" << "\r";
			if (diff < params.epsil)
			{
				ok = true;
				if (!params.quiet) std::cout << std::endl << "ok" << std::endl;
				break;
			}
		}
		auto run_time = std::chrono::system_clock::now();
		if(!params.quiet)
		{
			if (!ok)  std::cout << std::endl << "failed" << std::endl; 
			std::cout << "elapsed time = " << std::chrono::duration_cast<std::chrono::milliseconds>(run_time - start_time).count() << " milliseconds" << std::endl;	
		}
		return ok;
	}

	template<typename T, class Mag> int nonbayes_test(const Cavity_Message<T, Mag> &messages, const Patterns<T> &patterns) 
	{
		T **sign_m_j_star = new T*[messages.K], s, s2, trsf0;
		int t = 0;
		for (int k = 0; k < messages.K; ++k)
		{
			sign_m_j_star[k] = new T[messages.N];
			std::transform(messages.m_j_star[k], messages.m_j_star[k] + messages.N,
						   sign_m_j_star[k], [](const Mag &mj)
						   {
						   	return Magnetization::sign0<T, Mag>(mj);
						   });
		}

		for (int i = 0; i < patterns.Nrow; ++i)
		{
			s = std::accumulate(sign_m_j_star, sign_m_j_star + messages.K, (T)0.,
								[&trsf0, &s2, &patterns, &i](const T &val, T *mjs_i)
								{
									trsf0 = (T)0.;
									std::inner_product(mjs_i, mjs_i + patterns.Ncol,
													   patterns.input[i], (T)0.,
													   std::plus<T>(), [&trsf0](const T &mjs, const T &in)
													   {
													   	trsf0 += mjs * in;
													   	return (1 - mjs * mjs) * (in * in);
													   });
									return std::erf( trsf0 / std::sqrt(s2) );
								});
			t += ((int)sign<T>(s) != patterns.output[i]);
		}
		for (int i = 0; i < messages.K; ++i)
			delete[]sign_m_j_star[i];
		delete[]sign_m_j_star;
		return t;
	}

	template<typename T, class Mag> T free_energy(const Cavity_Message<T, Mag> &messages, const Patterns<T> &patterns, const Params<T, Mag> &params) 
	{
		T f = (T)0., *ones = new T[messages.K];
		MagVec<Mag> v = new Mag[messages.M];
		Mag z((T)0.), old_m_j_star((T)0.);

		std::memset(ones, 1, messages.K*sizeof(T));
		for (int a = 0; a < messages.M; ++a)
		{
			for (int k = 0; k < messages.K; ++k)
				f += free_energy_theta(messages.m_j_star[k], messages.m_in[a][k], patterns.input[a], messages.weights[a][k], messages.m_no[a][k], patterns.Ncol, messages.N);
			f += free_energy_theta_exact(messages.m_in[a], messages.m_on[a], ones, messages.m_ni[a], z, messages.K); 
		}

		for (int k = 0; k < messages.K; ++k)
			for (int i = 0; i < messages.N; ++i)
			{
				for (int a = 0; a < messages.M; ++a)
					v[a] = Mag(messages.weights[a][k][i].mag); 
				
				f -= Magnetization::logZ<T, Mag>(messages.m_star_j[k][i], v, messages.M);

				f -= log2_over_2;
				f += Magnetization::log1pxy<T, Mag>(params.tan_gamma, -params.tan_gamma) / 2;
				old_m_j_star = Magnetization::bar<T, Mag>(messages.m_j_star[k][i], messages.m_star_j[k][i]);
				f += Magnetization::log1pxy<T, Mag>(old_m_j_star, messages.m_star_j[k][i]);
				f += Magnetization::mcrossentropy<T, Mag>(Magnetization::arrow(old_m_j_star*params.tan_gamma, params.r + 1), old_m_j_star*params.tan_gamma);
			}
		delete[]ones;
		delete[]v;
		return f / (messages.N * messages.K);
	}

	template<typename T, class Mag> T compute_S(const Cavity_Message<T, Mag> &messages, const Params<T, Mag> &params)
	{
		T S = (T)0.;
		Mag old_m_j_star((T)0.);
		for (int k = 0; k < messages.K; ++k)
			S = std::inner_product(messages.m_star_j[k], messages.m_star_j[k] + messages.N,
							   		messages.m_j_star[k], S,
							   		std::plus<T>(),
							   		[&params, &old_m_j_star](const Mag &msj, const Mag &mjs)
							   		{
							   			old_m_j_star = Magnetization::bar<T, Mag>(msj, mjs);
							   			return ( ( old_m_j_star * Magnetization::arrow((old_m_j_star * params.tan_gamma), params.r) ) % params.tan_gamma ).mag;
							   		});
		return S / (messages.N * messages.K);
	}

	template<typename T, class Mag> Mag compute_q_bar(const Cavity_Message<T, Mag> &messages, const Params<T, Mag> &params)
	{
		Mag q_bar((T)0.), old_m_j_star((T)0.), mx((T)0.);

		for (int k = 0; k < messages.K; ++k)
			for (int i = 0; i < messages.N; ++i)
			{
				old_m_j_star = Magnetization::bar<T, Mag>(messages.m_j_star[k][i], messages.m_star_j[k][i]);
				mx = Magnetization::arrow((old_m_j_star * params.tan_gamma), (params.r + 1));
				q_bar += mx*mx;
			}
		return q_bar / (messages.N * messages.K);
	}


	template<typename T, class Mag> T compute_q(const Cavity_Message<T, Mag> &messages, const int &nm_j_star, const int &nm_j_star_col)
	{
		T q = (T)0.;
		for (int i = 0; i < nm_j_star; ++i)
			q = std::inner_product(messages.m_j_star[i], messages.m_j_star[i] + nm_j_star_col,
								   messages.m_j_star[i], q,
								   std::plus<T>(),
								   [](Mag &j, const Mag &k)
								   {
								   	return (j * k).mag;
								   });		
		return q / (messages.N * messages.K);
	}


	template<typename T, class Mag> void mags_symmetry(const Cavity_Message<T, Mag> &messages, T *overlaps)
	{
		T s = (T)0., *qs = new T[messages.K];
		std::transform(messages.m_j_star, messages.m_j_star + messages.K,
					   qs, [&messages](Mag *m)
					   {
					   	return std::sqrt(std::inner_product(m, m + messages.N, 
					   								   m, (T)0., 
					   								   std::plus<T>(), 
					   								   [](const Mag &a, const Mag &b)
					   								   {
					   								   	return a.mag * b.mag;
					   								   }));
					   });

		for (int k1 = 0; k1 < messages.K; ++k1)
		{
			overlaps[k1 * messages.K + k1] = (T)1.;
			for (int k2 = k1 + 1; k2 < messages.K; ++k2)
			{
				s = std::inner_product(messages.m_j_star[k1], messages.m_j_star[k1] + messages.N, messages.m_j_star[k2], (T)0., std::plus<T>(), [](const Mag &a, const Mag &b){return a.mag * b.mag;}) / (qs[k1] * qs[k2]);
				overlaps[k1*messages.K + k2] = s;
				overlaps[k2*messages.K + k1] = s;
			}
		}

		delete[]qs;
		return; /* qs/N; */
	}


	template<typename T, class Mag> int** focusingBP(
										const int &K,
										const Patterns<T> &patterns,
										const int &max_iters,
										const int &max_steps,
										const int &seed,
										const T &damping,
										const bool &quiet,
										const std::string &accuracy1,
										const std::string &accuracy2,
										const T &randfact,
										const FocusingProtocol<T> &fprotocol,
										const T &epsil,
										const std::string &initmess,
										const bool &outatzero,
										const std::string &writeoutfile,
										std::string outfile,
										std::string outmessfiletmpl,
										const std::string &output
										)
	{
		bool ok;
		int 	it = 1, 
				errs, 
				M = patterns.Nrow, 
				N = patterns.Ncol;
		T 	S, 
			q, 
			betaF, 
			Sint, 
			*mags = new T[K * K];
		std::ofstream os;
		Cavity_Message<T, Mag> messages;
		Params<T, Mag> params(damping, epsil, T(NAN), max_iters, accuracy1, accuracy2, (T)0., (T)0., quiet);
		Mag pol   ((T)0.), 
			q_bar ((T)0.);

		if (N <= 0) { std::cerr << "N must be positive; given: " << N << std::endl; exit(1);}
		if (K <= 0) { std::cerr << "K must be positive; given: " << K << std::endl; exit(1);}
		if (writeoutfile != "auto" && writeoutfile != "always" && writeoutfile != "none") { std::cerr << "invalid writeoutfile, expected one of 'auto', 'always', 'none'; given: " << writeoutfile << std::endl; exit(1);}
		if (max_iters < 0) { std::cerr << "max_iters must be non-negative; given: " << max_iters << std::endl; exit(1);}
		if (max_steps < 0) { std::cerr << "max_steps must be non-negative; given: " << max_steps << std::endl; exit(1);}
		if (damping < 0 || damping >= 1) { std::cerr << "damping must be in [0,1); given: " << damping << std::endl; exit(1);}
		if (randfact < 0 || randfact >= 1) { std::cerr << "randfact must be in [0,1); given: " << randfact << std::endl; exit(1);}
		if (accuracy1 != "exact" && accuracy1 != "accurate" && accuracy1 != "none") { std::cerr << "accuracy1 must be one of 'exact', 'accurate' or 'none'; given: " << accuracy1 << std::endl; exit(1);}
		if (accuracy2 != "exact" && accuracy2 != "accurate" && accuracy2 != "none") { std::cerr << "accuracy2 must be one of 'exact', 'accurate' or 'none'; given: " << accuracy2 << std::endl; exit(1);}
		
		if (accuracy1 == "exact" && !(N % 2)) { std::cerr << "when accuracy1 == 'exact', N must be odd; given: " << N << std::endl; exit(1); }
		if (accuracy2 == "exact" && !(K % 2)) { std::cerr << "when accuracy2 == 'exact', K must be odd; given: " << K << std::endl; exit(1); }
		
		messages = (initmess == "nothing") ? Cavity_Message<T, Mag>(M, N, K, randfact, seed) : Cavity_Message<T, Mag>::read_messages(initmess);

		if (messages.N != N) { std::cerr << "wrong messages size, expected N=" << N << "; given: " << messages.N << std::endl; exit(1); }
		if (messages.K != K) { std::cerr << "wrong messages size, expected K=" << K << "; given: " << messages.K << std::endl; exit(1); }
		if (messages.M != M) { std::cerr << "wrong messages size, expected M=" << M << "; given: " << messages.M << std::endl; exit(1); }
		
		if (outfile == "")  outfile = "results_BPCR_N" + std::to_string(N) + "_K" + std::to_string(K) + "_M" + std::to_string(M) + "_s" + std::to_string(seed) + ".txt";
		if(outmessfiletmpl == "")  outmessfiletmpl = "messages_BPCR_N" + std::to_string(N) + "_K" + std::to_string(K) + "_M" + std::to_string(M) + "_gamma_s" + std::to_string(seed) + ".txt";
		if (outfile != "nothing" && (writeoutfile == "always" || writeoutfile == "auto"))
		{
			if (!quiet) std::cout << "writing outfile '" << outfile << "'" << std::endl; 
			os.open(outfile);
			os << "#1=pol 2=y 3=beta 4=S 5=q 6=q_bar 7=betaF 8=S_int 9=E" << std::endl;
			os.close();
		}
		if (initmess != "nothing")
		{
			errs = nonbayes_test(messages, patterns);
			if (!quiet) std::cout << "initial errors = " << errs << std::endl;
			if (outatzero && errs == 0) {delete[] mags; return M2W<T, Mag>(messages.m_j_star, K, N);}
		}
		if (!quiet && K > 1) 
		{ 
			std::cout << "mags overlaps =" << std::endl;
			mags_symmetry(messages, mags);
			for (int i = 0; i < K; ++i)
			{
				for(int j = 0; j < K; ++j) 
					std::cout << std::setprecision(6) << std::fixed << mags[i * K + j] << " ";
				std::cout << std::endl;
			}
			std::cout << std::endl; 
		}

		for (int i = 0; i < fprotocol.Nrep; ++i)
		{
			if (!std::isinf(fprotocol.beta[i])) { std::cerr << "finite beta not yet supported; given: " << fprotocol.beta[i] << std::endl; exit(1);}
			pol = Magnetization::mtanh<T, Mag>(fprotocol.gamma[i]);
			params.tan_gamma = pol;
			params.r = fprotocol.n_rep[i] - 1;
			params.beta = fprotocol.beta[i];
			
			set_outfields(messages, patterns.output, fprotocol.beta[i]); 
			
			ok = converge(messages, patterns, params);
			
			if (!quiet && K > 1)
			{
				std::cout << "mags overlaps =" << std::endl;
				mags_symmetry(messages, mags);
				for (int i = 0; i < K; ++i)
				{
					for(int j = 0; j < K; ++j) 
						std::cout << std::setprecision(6) << std::fixed << mags[i * K + j] << " ";
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
			errs = nonbayes_test(messages, patterns);
			if (writeoutfile == "always" || (writeoutfile == "auto" && !outatzero))
			{
				S = compute_S(messages, params);
				q = compute_q(messages, K, N);
				q_bar = compute_q_bar(messages, params);
				betaF = free_energy(messages, patterns, params);
				Sint = - betaF - fprotocol.gamma[i] * S;

				if (!quiet) std::cout << "it=" << it << " pol=" << pol << " y=" << fprotocol.n_rep[i] << " beta=" << fprotocol.beta[i] << " (ok=" << ok << ") S=" << S << " betaF=" << betaF << " S_int=" << Sint << " q=" << q << " q_bar=" << q_bar << " E=" << errs << std::endl;
				if ((ok || writeoutfile == "always") && outfile != "nothing")
				{
					os.open(outfile, std::ios_base::app);
					os << pol << " " << fprotocol.n_rep[i] << " " << fprotocol.beta[i] << " " << S << " " << q << " " << q_bar << " " << betaF << " " << Sint << " " << errs << std::endl;
					os.close();
				}
				if (outmessfiletmpl != "nothing") 
				{
					os.open(outmessfiletmpl, std::ios_base::app);
					os << "fmt: " << Mag::magformat() << ", seed: " << seed << ", max_iters: " << max_iters << ", randfact: " << randfact << ", damping: " << damping << ", accuracy1: " << accuracy1 << ", accuracy2: " << accuracy2 << "\nK,N:\t" << messages.K << "\t" << messages.N << std::endl;
					os.close();
				}
			}
			else
			{
				if(!quiet) std::cout << "it=" << it << " pol=" << pol << " y=" << fprotocol.n_rep[i] << " beta=" << fprotocol.beta[i] << " (ok=" << ok << ") E=" << errs << std::endl;
				if (errs == 0) 
				{
					if(output != "") save_weights<T, Mag>(output, messages, seed, max_iters, randfact, damping, accuracy1, accuracy2);
					delete[] mags;
					return M2W<T, Mag>(messages.m_j_star, K, N); 
				}
			}
			++it;
			if (it >= max_steps)  break;
		}

		delete[] mags;
		return M2W<T, Mag>(messages.m_j_star, K, N);
	}

	//====================================================================================================//
	//		______  _   _  _   _       ______  _   _  _   _  _____  _____  _____  _____  _   _  _____ 	  //
	//		| ___ \| | | || \ | |      |  ___|| | | || \ | |/  __ \|_   _||_   _||  _  || \ | |/  ___|	  //
	//		| |_/ /| | | ||  \| |      | |_   | | | ||  \| || /  \/  | |    | |  | | | ||  \| |\ `--. 	  //
	//		|    / | | | || . ` |      |  _|  | | | || . ` || |      | |    | |  | | | || . ` | `--. \	  //
	//		| |\ \ | |_| || |\  |      | |    | |_| || |\  || \__/\  | |   _| |_ \ \_/ /| |\  |/\__/ /	  //
	//		\_| \_| \___/ \_| \_/      \_|     \___/ \_| \_/ \____/  \_/   \___/  \___/ \_| \_/\____/ 	  //
	//		                                                                                         	  //
	//====================================================================================================//

	template<typename T, class Mag, class Input, file<T, Mag, Input>> int** train(const Input &patternsfile, int K, int seed, int max_iters, T randfact, T damping, std::string accuracy1, std::string accuracy2, std::string fprotocol, int protocol_size, std::string output)
	{
		Patterns<T> patterns(patternsfile);
		check_binary(patterns);
		return train(patterns, K, seed, max_iters, randfact, damping, accuracy1, accuracy2, fprotocol, protocol_size, output);
	}

	template<typename T, class Mag, class Input, patt<T, Mag, Input>> int** train(const Input &patterns, int K, int seed, int max_iters, T randfact, T damping, std::string accuracy1, std::string accuracy2, std::string fprotocol, int protocol_size, std::string output)
	{
		FocusingProtocol<T> fps;
		int p = (fprotocol == "scoping") ? 0 : (fprotocol == "pseudo_reinforcement") ? 1 : (fprotocol == "free_scoping") ? 2 : (fprotocol == "standard_reinforcement") ? 3 : 4, i = 0;
		switch (p)
		{
		case 0:
			{
				T *scop = new T[protocol_size];
				std::generate(scop, scop + protocol_size, [&i]{return (i++)*(T).1;});
				fps.Scoping(scop, 21, protocol_size);
				delete[] scop;
				break;
			}
		case 1:
			{
				T *pseudo = new T[protocol_size];
				std::generate(pseudo, pseudo + protocol_size, [&i]{return (i++)*(T).01;});
				fps.PseudoReinforcement(pseudo, protocol_size);
				delete[] pseudo;
				break;
			}
		case 2:
			{
				T **freeS = new T*[protocol_size];
				std::generate(freeS, freeS + protocol_size, []{return new T[3];});
				for (int i = 0; i < protocol_size; ++i)
				{
					freeS[i][0] = (T)std::atanh(std::sqrt(i*(T).001));
					freeS[i][1] = ((T)2. - i*(T).001) / ((T)1. - i*(T).001);
					freeS[i][2] = std::numeric_limits<T>::infinity();
				}
				fps.FreeScoping(freeS, protocol_size);
				for(int i = 0; i < protocol_size; ++i) delete[] freeS[i];
				delete[] freeS;
				break;
			}
		case 3:
			{
				fps.StandardReinforcement((T)1e-2);
				break;
			}
		case 4:
			{
				std::cerr << "Invalid protocol parameter. Given: " << fprotocol << ". Possible values are 'scoping', 'pseudo_reinforcement', 'free_scoping' and 'standard_reinforcement'" << std::endl;
				exit(1);
			}
		}
		return focusingBP<T, Mag>(K, patterns, max_iters, INT_MAX, seed, damping, false, accuracy1, accuracy2, randfact, fps, (T)1e-3, "nothing", true, "none", "nothing", "nothing", output);
	}

	template<typename T> int* test(const Patterns<T> &patterns, int **weights, const int &K, const int &N)
	{
		int *results = new int[patterns.Nrow]; 
		std::transform(	patterns.input, patterns.input + patterns.Nrow,
						results, [&weights, &K, &N](const T *input)
						{
							return compute_output(input, weights, K, N);
						});
		return results;
	}

	template<typename T> int* test(const std::string &ifile, const std::string &wfile)
	{
		Patterns<T> patterns(ifile);
		check_binary(patterns);		

		int 	K, 
				N,
				**weights = read_weights(wfile, K, N),
				*results = new int[patterns.Nrow];
		std::transform(	patterns.input, patterns.input + patterns.Nrow,
						results, [&weights, &K, &N](const T *input)
						{
							return compute_output(input, weights, K, N);
						});

		return results;
	}


} // end of namespace BinaryCommitteeMachineFBP

//======================================================================//
//	______   ___  ______  _____  _____       ___  ______  _____  _____ 	//
//	| ___ \ / _ \ | ___ \/  ___||  ___|     / _ \ | ___ \|  __ \/  ___|	//
//	| |_/ // /_\ \| |_/ /\ `--. | |__      / /_\ \| |_/ /| |  \/\ `--. 	//
//	|  __/ |  _  ||    /  `--. \|  __|     |  _  ||    / | | __  `--. \	//
//	| |    | | | || |\ \ /\__/ /| |___     | | | || |\ \ | |_\ \/\__/ /	//
//	\_|    \_| |_/\_| \_|\____/ \____/     \_| |_/\_| \_| \____/\____/ 	//
//	                                                                   	//
//======================================================================//


static void menu_FBP()
{
	std::cout << std::endl;
	std::cout << " BinaryCommitteeMachineFBP Algorithm" << std::endl;
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

template<typename T> void parse_training_args(int argc, char *argv[], std::string &patternsfile, std::string &output, int &K, int &seed, int &max_iters, T &randfact, T &damping, std::string &accuracy1, std::string &accuracy2, std::string &fprotocol, char &mag)
{
	ArgumentParser argparse("Training BeliefPropagation 3.0");
	argparse.add_argument<std::string>("fArg", "f", "file", "Pattern Filename (with extension)", true, "");
	argparse.add_argument<std::string>("oArg", "o", "output", "Output Filename (with extension)", true, "");
	argparse.add_argument<int>("kArg", "k", "hidden", "Number of Hidden Layers", false, 3);
	argparse.add_argument<int>("rArg", "r", "seed", "Seed random generator", false, 135);
	argparse.add_argument<int>("iArg", "i", "iteration", "Max Number of Iterations", false, 1000);
	argparse.add_argument<T>("gArg", "g", "randfact", "Seed random generator of Cavity Messages", false, (T).1);
	argparse.add_argument<T>("dArg", "d", "damping", "Damping parameter", false, (T).5);
	argparse.add_argument<std::string>("aArg", "a", "accuracy", "Accuracy of the messages computation at the hidden units level (choose between 'exact', 'accurate', 'approx', 'none')", true, "");
	argparse.add_argument<std::string>("pArg", "p", "protocol", "Specify protocol : (0) Scooping, (1) PseudoReinforcement, (2) FreeScoping, (3) StandardReinforcement", true);
	argparse.add_argument<char>("mArg", "m", "mag", "Specify Magnetization: (0) MagnetizationP (MagP64), (1) MagnetizationT (MagT64)", true);

	argparse.parse_args(argc, argv);

	argparse.get<std::string>("fArg", patternsfile);
	if(!os::file_exists(patternsfile)){std::cerr << "Pattern file not found. Given : " << patternsfile << std::endl;}
	argparse.get<std::string>("oArg", output);
	argparse.get<int>("kArg", K);
	argparse.get<int>("rArg", seed);
	argparse.get<int>("iArg", max_iters);
	argparse.get<T>("gArg", randfact);
	argparse.get<T>("dArg", damping);
	
	std::vector<std::string> accuracy;
	argparse.get<std::string>("aArg", accuracy);
	if((int)accuracy.size() > 2) {std::cerr << "Too many accuracy variables given. Needed two." << std::endl; exit(1);}
	for(auto &ac : accuracy) if(ac != "exact" && ac != "accurate" && ac != "approx" && ac != "none") {std::cerr << "Invalid accuracy variable given. Given : " << ac << std::endl; exit(1);}
	if((int)accuracy.size() == 1){accuracy1 = accuracy[0]; accuracy2 = "accurate";}
	else{accuracy1 = accuracy[0]; accuracy2 = accuracy[1];}
	accuracy.clear();

	argparse.get<std::string>("pArg", fprotocol);
	if (fprotocol != "scoping" && fprotocol != "pseudo_reinforcement" && fprotocol != "free_scoping" && fprotocol != "standard_reinforcement"){std::cerr << "Invalid focusing protocol found. Given : " << fprotocol << std::endl; exit(1);}

	argparse.get<char>("mArg", mag);
	if(mag != '0' && mag != '1'){std::cerr << "Invalid magnetization found. Given : " << mag << std::endl; exit(1);};

	return;
}

void parse_test_args(int argc, char *argv[], std::string &test_file, std::string &weight_file, std::string &output_file)
{
	ArgumentParser argparse("Test BeliefPropagation 3.0");
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



