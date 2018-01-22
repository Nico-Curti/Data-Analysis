#include "classifier.hpp"

int main(int argc, char *argv[])
{
	int hidden_layers = 0,
		*hidden_sizes = new int[hidden_layers],
		epochs = 50,
		mini_batch_size = 2;
		hidden_sizes[0] = 0;
	float eta = .8f, lambda = .0f;
	std::string pattern = "tst/pattern/train_nn.dat",
				testfile = "tst/pattern/test_nn.dat",
				wfile = "tst/pattern/nn_weights.dat";
	Patterns<float> training(pattern);
	Patterns<float> test(testfile);
	/*--------------------------------*/
	std::cout << "Perceptron classifier" << std::endl << std::endl;

	Perceptron<float> neuron;
	neuron.train<false>(training);
	int *lbl_predict_perceptron = neuron.test<false>(test);

	std::cout << "input vector\ttrue\tpredict" << std::endl;
	for(int i = 0; i < test.Nout; ++i)
	{
		std::cout << "test of (";
		for(int j = 0; j < test.Ncol - 1; ++j)
			std::cout << test.input[i][j] << ",";
		std::cout<< test.input[i][test.Ncol - 1] << ")" << "\t" << test.output[i] << "\t" << lbl_predict_perceptron[i] << std::endl;
	}

	std::cout << "Performances : " << 
				float( std::count_if(test.output, test.output + test.Nout,
									[lbl_predict_perceptron](const int &t) mutable 
									{ 
										return (t == *lbl_predict_perceptron++);
									}) * 100 ) / test.Nout << " %" << std::endl;

	if(os::dir_exists("tst/res"))
	{
		std::ofstream os("tst/res/perceptron.class");
		for(int i = 0; i < test.Ncol; ++i) os << "feature" << i << "\t"; os << "label" << std::endl; //header
		for(int i = 0; i < test.Nrow; ++i)
		{
			std::copy(test.input[i], test.input[i] + test.Ncol, std::ostream_iterator<float>(os, "\t"));
			os << lbl_predict_perceptron[i] << std::endl;
		}
		os.close();
	}
	
	/*--------------------------------*/
	std::cout << std::endl << std::endl << "NeuralNetwork classifier" << std::endl << std::endl;

	NeuralNetwork<float> nn;
	nn.train(training, hidden_sizes, hidden_layers, CrossEntropyCost, epochs, mini_batch_size, eta, lambda, 123);
	nn.save(wfile);
	int * lbl_predict_nn = nn.test(test);

	std::cout << "input vector\ttrue\tpredict" << std::endl;
	for(int i = 0; i < test.Nout; ++i)
	{
		std::cout << "test of (";
		for(int j = 0; j < test.Ncol - 1; ++j)
			std::cout << test.input[i][j] << ",";
		std::cout<< test.input[i][test.Ncol - 1] << ")" << "\t" << test.output[i] << "\t" << lbl_predict_perceptron[i] << std::endl;
	}

	std::cout << "Performances : " << 
				float( std::count_if(test.output, test.output + test.Nout,
									[lbl_predict_nn](const int &t) mutable 
									{ 
										return (t == *lbl_predict_nn++);
									}) * 100 ) / test.Nout << " %" << std::endl;
	
	if(os::dir_exists("tst/res"))
	{
		std::ofstream os("tst/res/neuralnetwork.class");
		for(int i = 0; i < test.Ncol; ++i) os << "feature" << i << "\t"; os << "label" << std::endl; //header
		for(int i = 0; i < test.Nrow; ++i)
		{
			std::copy(test.input[i], test.input[i] + test.Ncol, std::ostream_iterator<float>(os, "\t"));
			os << lbl_predict_nn[i] << std::endl;
		}
		os.close();
	}
	return 0;
}
