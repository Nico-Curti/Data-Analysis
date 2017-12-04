#include "dimensionality_reduction.hpp"
#include "patterns.hpp"

int main(int argc, char *argv[])
{
	int Ncomp = 2;
	std::string patternsfile = "../tst/pattern/iris.dat";
	Patterns<float> data(patternsfile);

	float **components = pca(data.input, data.Ncol, data.Nrow, Ncomp);

	return 0;
}
