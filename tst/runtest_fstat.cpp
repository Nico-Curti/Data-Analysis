#include "timing_func.hpp"
#include "fstat.hpp"
#include <random>
#include <assert.h>

auto seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 engine{(unsigned int)seed};
std::uniform_real_distribution<float> r_dist{0.0f, 1.0f};


int main(int argc, char *argv[])
{
	int N = 100000,
		*idx_sort_serial = nullptr, *idx_sort_par = nullptr;
	float 	*x = new float[N], *y = new float[N],
			min_par, min_serial,
			max_par, max_serial,
			idx_min_par, idx_min_serial,
			idx_max_par, idx_max_serial,
			var_x, var_y,
			med_x, med_y,
			*auto_corr1 = nullptr, *auto_corr2 = nullptr;

	std::generate(x, x + N, []{return r_dist(engine);});
	std::memcpy(y, x, N*sizeof(float));

	std::cout << std::endl << "(*) max of array" << std::endl;
	compare_function<>::exec([&x, &N, &max_par](){max_par = max<true>(x, N);}, [&x, &N, &max_serial](){max_serial = max<false>(x, N);});
	assert(max_serial == max_par);
	std::cout << std::endl << "(*) min of array" << std::endl;
	compare_function<>::exec([&x, &N, &min_par](){min_par = min<true>(x, N);}, [&x, &N, &min_serial](){min_serial = min<false>(x, N);});
	assert(min_serial == min_par);
	std::cout << std::endl << "(*) index of max of array" << std::endl;
	compare_function<>::exec([&x, &N, &idx_max_par](){idx_max_par = idxmax<true>(x, N);}, [&y, &N, &idx_max_serial](){idx_max_serial = idxmax<false>(y, N);});
	assert(idx_max_par == idx_max_serial);
	std::cout << std::endl << "(*) index of min of array" << std::endl;
	compare_function<>::exec([&x, &N, &idx_min_par](){idx_min_par = idxmin<true>(x, N);}, [&y, &N, &idx_min_serial](){idx_min_serial = idxmin<false>(y, N);});
	assert(idx_min_par == idx_min_serial);

	std::cout << std::endl << "(*) sort array" << std::endl;
	compare_function<>::exec([&x, &N](){sort<true>(x, 0, N);}, [&y, &N](){sort<false>(y, 0, N);});
	check(x, N, y);

	std::random_shuffle(x, x + N);
	std::random_shuffle(y, y + N);

	std::cout << std::endl << "(*) argsort array" << std::endl;
	compare_function<>::exec([&x, &N, &idx_sort_par](){idx_sort_par = argsort<true>(x, 0, N);}, [&x, &N, &idx_sort_serial](){idx_sort_serial = argsort<false>(x, 0, N);});
	check(idx_sort_par, N, idx_sort_serial);
	delete[] idx_sort_serial;
	delete[] idx_sort_par;

	std::cout << std::endl << "(*) variance of array" << std::endl;
	compare_function<>::exec([&x, &N, &var_x](){var_x = variance<true>(x, 0, N);}, [&x, &N, &var_y](){var_y = variance<false>(x, 0, N);});
	std::cout << ((var_x - var_y) < 1e-3 ? "GOOD" : "BAD...") << std::endl;

	std::cout << std::endl << "(*) median of array" << std::endl;
	compare_function<>::exec([&x, &N, &med_x](){med_x = median<true>(x, N);}, [&y, &N, &med_y](){med_y = median<false>(y, N);});
	std::cout << ((med_x - med_y) < 1e-3 ? "GOOD" : "BAD...") << std::endl;

	std::cout << std::endl << "(*) autocorrelation of array" << std::endl;
	compare_function<>::exec([&x, &N, &auto_corr1](){auto_corr1 = autocorrelation<true>(x, N);}, [&x, &N, &auto_corr2](){auto_corr2 = autocorrelation<false>(x, N);});
	check(auto_corr1, N, auto_corr2);
	delete[] auto_corr1;
	delete[] auto_corr2;

	return 0;
}
