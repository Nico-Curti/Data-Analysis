#include "classifier.hpp"
#include "score_coef.hpp"

int main(int argc, char *argv[])
{
	int hidden_layer = 3,
		y = 3,
		batch = 2,
		max_epochs = 4;

	double eta = 2.0,
		   lambda = 0.75,
		   gamma = 0.05,
		   eta_factor = 1.0,
		   lambda_factor = 1.0,
		   gamma_step = 0.001;
/*
	std::string formula = "simple",
				trainfile = "tst/pattern/train_sgd.dat",
				testfile = "tst/pattern/test_sgd.dat",
				output = "tst/pattern/results.dat";

	Patterns<double> train(trainfile);
	Patterns<double> test(testfile);
	

	ReplicatedSGD<double> sgd;
	sgd.train<MagP64<double>>(	input, 
								hidden_layer, 
								y, 
								batch, 
								max_epochs, 
								eta, 
								lambda, 
								gamma, 
								eta_factor, 
								lambda_factor, 
								gamma_step,
								formula);

	int *label_predict = sgd.test(test);
	auto score_sgd = score::perfs(test.output, label_predict, test.Nout);

	std::cout << "Prediction SGD:" << std::endl;
	for(const auto &i : score_sgd)
		std::cout << i.first << " : " << i.second << std::endl;

	if(os::dir_exists("tst/res"))
	{
		std::ofstream os("tst/res/rsgd.deep");
		os << "true_label\tpredict_label" << std::endl; // header
		for(int i = 0; i < test.Nrow; ++i) os << test.output[i] << "\t" << label_predict[i] << std::endl;
		os.close();
	}
-------------------------------------*/
	return 0;
}


