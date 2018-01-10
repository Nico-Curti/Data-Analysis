#include "classifier.hpp"
#include "score_coef.hpp"

int main(int argc, char *argv[])
{
	int hidden_layer = 3,
		max_iters = 1000,
		protocol_size = 101,
		seed = 123;
	double  randfact = .1,
			damping = .5;
	std::string trainfile = "tst/pattern/train_fbp.dat",
				testfile = "tst/pattern/test_fbp.dat",
				accuracy = "accurate",
				fprotocol = "standard_reinforcement";

	Patterns<double> train(trainfile);
	Patterns<double> test(testfile);
	/*-------------------------------------*/

	ReplicatedFBP<double> fbp;
	fbp.train<MagP64<double>>(	train, 
								hidden_layer, 
								max_iters, 
								randfact, 
								damping, 
								accuracy,
								accuracy,
								fprotocol,
								protocol_size,
								seed);
	int *label_predict = fbp.test(test);
	auto score_fbp = score::perfs(test.output, label_predict, test.Nout);

	std::cout << "Prediction FBP:" << std::endl;
	for(const auto &i : score_fbp)
		std::cout << i.first << " : " << i.second << std::endl;

	if(os::dir_exists("tst/res"))
	{
		std::ofstream os("tst/res/rfbp.deep");
		os << "true_label\tpredict_label" << std::endl; // header
		for(int i = 0; i < test.Nrow; ++i) os << test.output[i] << "\t" << label_predict[i] << std::endl;
		os.close();
	}

	return 0;
}
