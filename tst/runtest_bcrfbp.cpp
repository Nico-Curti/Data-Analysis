#include "ReplicatedFBP.hpp"
#include "score_coef.hpp"

int main(int argc, char *argv[])
{
	int hidden_layer = 3;
	std::string trainfile = "../tst/pattern/train_fbp.dat",
				testfile = "../tst/pattern/test_fbp.dat";

	Patterns<double> train(trainfile);
	int **weights = FBP::train<double, MagP64<double>>(train, hidden_layer, 135, 5);

	Patterns<double> test(testfile);
	int *label_predict = FBP::test<double>(test, weights, hidden_layer, test.Ncol);

	auto score_fbp = score::perfs(train.output, label_predict, train.Nout);

	std::cout << "Prediction FBP:" << std::endl;
	for(const auto &i : score_fbp)
		std::cout << i.first << " : " << i.second << std::endl;

	return 0;
}
