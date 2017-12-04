#pragma once
#include <cmath>
#include <cstdint>
#ifndef M_PI
#define M_PI  3.14159265359
#endif
#define erf1 0.254829592
#define erf2 -0.284496736
#define erf3 1.421413741
#define erf4 -1.453152027
#define erf5 1.061405429
#define erfp 0.3275911
#define int_e 1076754516
#define int_10 1092616192
#define log2_over_2 0.34657359027997264311
//constexpr double magic_number = std::pow(2, 23) * (127 - 0.043035);
#define magic_number 1064992212.25472 // std::pow(2, 23) * (127 - 0.043035);
#define ln10 2.3025850929940459

template<typename T> inline T erf(const T &);
static inline float pow2 (const float &p);
template<typename T> inline T exp(const T &p);
template<typename T> inline T log2 (const T &x);
template<typename T> inline T log(const T &x);
template<typename T> inline T pow(const T &a, const T &b);
template<typename T> inline T log10(const T &x);
template<typename T> inline T atanh(const T &x);
template<typename T> inline T tanh(const T &x);
template<typename T> inline T sqrt(const T &);
template<typename T> inline int sign(const T &);
inline int gcd(const int &a, const int &b); // greatest common divisor
inline int gd(const int &a); // greatest divisor



//===============================================================================//
//	        _    _  _        __                      _    _                      //
//	       | |  (_)| |      / _|                    | |  (_)                     //
//	 _   _ | |_  _ | |     | |_  _   _  _ __    ___ | |_  _   ___   _ __   ___   //
//	| | | || __|| || |     |  _|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|  //
//	| |_| || |_ | || |     | |  | |_| || | | || (__ | |_ | || (_) || | | |\__ \  //
//	 \__,_| \__||_||_|     |_|   \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/  //
//	                                                                             //
//===============================================================================//


template<typename T> inline T erf(const T &x)
{
#ifdef FAST_MATH
    // Save the sign of x
    int sign = (x < 0) ? -1 : 1;
    T x_tmp = (T)std::fabs(x);
    // A&S formula 7.1.26
    T t = 1.f / (1.f + erfp * x_tmp);
    T y = 1.f - (((((erf5 * t + erf4) * t) + erf3) * t + erf2) * t + erf1) * t * exp(-x_tmp * x_tmp);
    return sign*y;
#else
    return std::erf(x);
#endif
}

static inline float pow2 (const float &p)
{
#ifdef FAST_MATH
	float offset = (p < 0) ? 1.0f : 0.0f;
	float clipp = (p < -126) ? -126.0f : p;
	int w = clipp;
	float z = clipp - w + offset;
	union { uint32_t i; float f; } v = { (1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) };

	return v.f;
#else
	return 0.f;
#endif
}

template<typename T> inline T exp(const T &p)
{  
#ifdef FAST_MATH
	return pow2(1.442695040f * p); 
#else
	return std::exp(p);
#endif
}

template<typename T> inline T log2 (const T &x)
{
#ifdef FAST_MATH
	union { float f; uint32_t i; } vx = { (float)x };
	union { uint32_t i; float f; } mx = { (vx.i & 0x007FFFFF) | 0x3f000000 };
	float y = vx.i;
	y *= 1.1920928955078125e-7f;
	return y - 124.22551499f - 1.498030302f * mx.f - 1.72587999f / (0.3520887068f + mx.f);
#else
	return std::log2(x);
#endif
}

template<typename T> inline T log(const T &x)
{
#ifdef FAST_MATH
	int Ix = *(int *)&(x);
	float y = (Ix - magic_number) / (int_e - magic_number);

	y -= (exp(y) - x)/( exp(y));
	y -= (exp(y) - x)/( exp(y));
	return y;
#else
	return std::log(x);
#endif
}

template<typename T> inline T pow(const T &a, const T &b)
{  
#ifdef FAST_MATH
	return pow2(b * log2(a)); 
#else
	return std::pow(a, b);
#endif
}

template<typename T> inline T log10(const T &x)
{
#ifdef FAST_MATH
	int Ix = *(int *)&(x);
	float y = (Ix - magic_number) / (int_10 - magic_number);

	y -= (pow(10, y) - x)/( ln10*pow(10, y));
	y -= (pow(10, y) - x)/( ln10*pow(10, y));
	return y;
#else
	return std::log10(x);
#endif
}

template<typename T> inline T atanh(const T &x)
{
#ifdef FAST_MATH
	return (T)0.5 * log(((T)1. + x)/((T)1. - x));
#else
	return std::atanh(x);
#endif
}

template<typename T> inline T tanh(const T &x)
{
#ifdef FAST_MATH
	T e = exp(-2*x);
	return ((T)1. - e) / ((T)1. + e);
#else
	return std::tanh(x);
#endif
}

template<typename T> inline T hardtanh(const T &x)
{ 
	return (-(T)1. <= x && x <= (T)1.) ? x : (x < -(T)1.) ? -(T)1. : (T)1.; 
}

template<typename T> inline T sqrt(const T &x)
{
#ifdef FAST_MATH
	const float xhalf = 0.5f * x;

	union // get bits for floating value
	{
		float x;
		int i;
	} u;
	u.x = x;
	u.i = 0x5f3759df - (u.i >> 1);  // gives initial guess y0
	return x * u.x * (1.5f - xhalf * u.x * u.x);// Newton step, repeating increases accuracy 
#else
	return std::sqrt(x);
#endif
}

template<typename T> inline int sign(const T &x)
{ 
	return (x > 0) ? 1 : ((x < 0) ? -1 : 0); 
}

inline int gcd(const int &a, const int &b) // greatest commond divisor
{
	return b == 0 ? a : gcd(b, a % b);
}

inline int gd(const int &a) // greatest divisor
{
	if(!(a % 2))
		return a/2;
	for(int i = 3; i < a; i += 2)
		if(!(a % i))
	  		return a / i;
	return a;     
}
