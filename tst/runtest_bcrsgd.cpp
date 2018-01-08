#include <omp.h>
#include "classifier.hpp"


int main(int argc, char *argv[])
{
	int hidden_layer = 3;
	std::string patternsfile = "tst/pattern/fbp_train.dat";
	Patterns<float> data(patternfile);

	ReplicatedSGD rsgd;

	rsgd.train(data, hidden_layer);
	int *predict_bcrsgd = rsgd.test(test);
	//if(os::dir_exists("tst/res"))
	//{
	//	std::ofstream os("tst/res/bcrsgd.class");
	//	for(int i = 0; i < data.Ncol; ++i) os << "feature" << i << "\t"; os << "label" << std::endl; //header
	//	for(int i = 0; i < data.Nrow; ++i)
	//	{
	//		std::copy(data.input[i], data.input[i] + data.Ncol, std::ostream_iterator<float>(os, "\t"));
	//		os << predict_bcrsgd[i] << std::endl;
	//	}
	//	os.close();
	//}

	return 0;
}
