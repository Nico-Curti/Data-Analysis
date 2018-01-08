#include "ReplicatedSGD.hpp"
#include "score_coef.hpp"

int main(int argc, char *argv[])
{
	int hidden_layer = 3,
	    n_rep = 7;
	std::string trainfile = "tst/pattern/fbp_train.dat",
				testfile = "tst/pattern/fb_test.dat";

	Patterns<float> train(trainfile);
	float *weights = RSGD::train(train, hidden_layer, n_rep);

	Patterns<float> test(testfile);
	int *label_predict = RSGD::test(test, weights, hidden_layer, test.Ncol);

	auto score_rsgd = score::perfs(train.output, label_predict, train.Nout);

	std::cout << "Prediction RSGD:" << std::endl;
	for(const auto &i : score_rsgd)
		std::cout << i.first << " : " << i.second << std::endl;

	return 0;
}

