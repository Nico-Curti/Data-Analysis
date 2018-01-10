#include "ReplicatedSGD.hpp" //#include "classifier.hpp"
#include "score_coef.hpp"

int main(int argc, char *argv[])
{
	int hidden_layer = 3,
	    n_rep = 7,
	    batch = 5,
	    max_epochs = 1000;
	float 	eta = 2.f,
			lambda = .1f,
			gamma = std::numeric_limits<float>::infinity(),
			step_eta = 1.f,
			step_lambda = 1.f,
			step_gamma = 1.f;
	std::string trainfile = "tst/pattern/train_fbp.dat",
				testfile = "tst/pattern/test_fbp.dat",
				formula = "simple";

	Patterns<float> train(trainfile);
	Patterns<float> test(testfile);
	/*-------------------------------------*/

	//ReplicatedSGD<double> rsgd;
	//rsgd.train(	train,
	//			);
	//int *label_predict = rsgd.test(test);

	float *weights = RSGD::train(train, hidden_layer, n_rep);
	int *label_predict = RSGD::test(test, weights, hidden_layer, test.Ncol);
	auto score_rsgd = score::perfs(test.output, label_predict, test.Nout);

	std::cout << "Prediction RSGD:" << std::endl;
	for(const auto &i : score_rsgd)
		std::cout << i.first << " : " << i.second << std::endl;

	if(os::dir_exists("tst/res"))
	{
		std::ofstream os("tst/res/rsgd.deep");
		os << "true_label\tpredict_label" << std::endl; // header
		for(int i = 0; i < test.Nrow; ++i) os << test.output[i] << "\t" << label_predict[i] << std::endl;
		os.close();
	}

	return 0;
}

