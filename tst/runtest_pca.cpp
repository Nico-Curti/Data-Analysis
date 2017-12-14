#include "dimensionality_reduction.hpp"
#include "patterns.hpp"
#include <iterator>

int main(int argc, char *argv[])
{
	int Ncomp = 2;
	std::string patternsfile = "../tst/pattern/iris.dat";
	Patterns<float> data(patternsfile);

	float **components = pca(data.input, data.Ncol, data.Nrow, Ncomp);
	for(int i = 0; i < Ncomp; ++i)
	{
		std::copy(components[i], components[i] + data.Nrow, std::ostream_iterator<float>(std::cout, " "));
		std::cout << std::endl;
	}

	return 0;
}
