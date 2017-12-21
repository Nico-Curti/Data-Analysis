#include "dimensionality_reduction.hpp"
#include "patterns.hpp"
#include <iterator>

int main(int argc, char *argv[])
{
	int Ncomp = 2;
	std::string patternsfile = "tst/pattern/iris.dat";
	Patterns<float> data(patternsfile);

	float **components = pca(data.input, data.Nrow, data.Ncol, Ncomp);
	if(os::dir_exists("tst/res"));
	{
		std::ofstream os("tst/res/pca.class");
		for(int i = 0; i < Ncomp; ++i) os << "component" << i << "\t"; os << "label" << std::endl; //header
		for(int i = 0; i < data.Nrow; ++i)
		{
			for(int j = 0; j < Ncomp; ++j)
				os << components[j][i] << "\t";
			os << data.output[i] << std::endl;
		}
		os.close();
	}

	return 0;
}
