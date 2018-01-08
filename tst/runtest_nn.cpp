#include "classifier.hpp"

int main(int argc, char *argv[])
{
	int num_layers = 3,
		*sizes = new int[num_layers],
		epochs = 100,
		mini_batch_size = 2;
	sizes[0] = 3; sizes[1] = 4; sizes[2] = 2;
	float eta = .8f, lambda = .0f;
	std::string pattern = "tst/pattern/nand_train.dat",
				testfile = "tst/pattern/nand_test.dat",
				wfile = "tst/pattern/nn_weights_nand.dat";
	Patterns<float> training(pattern);
	Patterns<float> test(testfile);
	/*--------------------------------*/
	std::cout << "Perceptron classifier" << std::endl << std::endl;

	Perceptron<float> neuron;
	neuron.train<false>(training);
	int *lbl_predict_perceptron = neuron.test<false>(test);

	std::cout << "input vector\ttrue\tpredict" << std::endl;
	for(int i = 0; i < test.Nout; ++i)
		std::cout << "test of (" << test.input[i][0] << "," << test.input[i][1] << "," << test.input[i][2] << ")"
				  << "\t" << test.output[i] << "\t" << lbl_predict_perceptron[i] << std::endl;

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
	nn.train(training, sizes, num_layers, CrossEntropyCost, epochs, mini_batch_size, eta, lambda, 123);
	//nn.save(wfile);
	int * lbl_predict_nn = nn.test(test);

	std::cout << "input vector\ttrue\tpredict" << std::endl;
	for(int i = 0; i < test.Nout; ++i)
		std::cout << "test of (" << test.input[i][0] << "," << test.input[i][1] << "," << test.input[i][2] << ")"
				  << "\t" << test.output[i] << "\t" << lbl_predict_nn[i] << std::endl;

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
