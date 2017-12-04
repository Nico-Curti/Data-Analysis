#include "classifier.hpp"

int main()
{
	std::string patternsfile = "../tst/pattern/iris.dat";
	Patterns<float> data(patternsfile);

	// Bayesian diag-quadratic discriminant analysis classifier
	diagQDA<float> cls_1;
	cls_1.train(data);
	int *predict_diagQDA = cls_1.test(data);

	std::map<int, int> score_diagQDA;
	for(int i = 0; i < data.Nout; ++i)
		if(predict_diagQDA[i] == data.output[i])
			++score_diagQDA[data.output[i]];
	std::cout << "Prediction diagQDA:" << std::endl;
	for(const auto &i : score_diagQDA)
		std::cout << i.first << " : " << i.second << std::endl;

	// Bayesian quadratic discriminant analysis classifier
	QDA<float> cls_2;
	cls_2.train(data);
	int *predict_QDA = cls_2.test(data);

	std::map<int, int> score_QDA;
	for(int i = 0; i < data.Nout; ++i)
		if(predict_QDA[i] == data.output[i])
			++score_QDA[data.output[i]];
	std::cout << "Prediction QDA:" << std::endl;
	for(const auto &i : score_QDA)
		std::cout << i.first << " : " << i.second << std::endl;

	return 0;
}
