#include "grid_search.hpp"

int main(int argc, char *argv[])
{
	int num_layers = 3,
		*sizes = new int[num_layers],
		epochs = 1000,
		mini_batch_size = 2,
		cost = 0, // 0 -> QuadraticCost -- 1 -> CrossEntropyCost
		K = 10,
		n_population = 10,//2048,
		max_iter = 3;//2000;
	unsigned int seed = 123;
	float 	eta = .8f, 
			lambda = .3f,
			elit_rate = .3f, // percentage of population to conserve
			mutation_rate = .3f; // probability of mutation

	std::string patternsfile = "tst/pattern/iris.dat",
				genetic_out = "res/GANeuralNet_n" + std::to_string(n_population) +
											 "_i" + std::to_string(max_iter) +
											 "_e" + std::to_string(elit_rate) +
											 "_m" + std::to_string(mutation_rate) +
											 "_s" + std::to_string(seed) +
											 ".log";
	Patterns<float> data(patternsfile);
	std::map<int, int> n_lbl;
	for(int i = 0; i < data.Nout; ++i) ++n_lbl[data.output[i]];
	std::cout << "Label to predict : " << std::endl;
	for(const auto &i : n_lbl) std::cout << i.first << " : " << i.second << std::endl;

	sizes[0] = (int)n_lbl.size(); sizes[1] = 7; sizes[2] = 3;
	hyperparams<NeuralNetwork<float>> params(sizes,
											 num_layers,
											 cost,
											 epochs,
											 mini_batch_size,
											 eta,
											 lambda,
											 seed);

	std::cout << "Stratified " << K << "-Fold CV with Neural Network" << std::endl;
	auto score = cross_val_score<false, NeuralNetwork<float>>(	data,
																std::bind(	&CrossValidation::StratifiedKFold, 
																 			std::placeholders::_1, 
																 			data.output, K, 0, data.Nout, true, seed),
																score::perfs,
																params);
	std::cout << "Performances : " << std::endl;
	for(const auto &i : score) std::cout << i.first << " : " << float(i.second)/n_lbl[i.first] * 100 << std::endl;

	std::cout << "Genetic GridSearch of optimal parameters for the Neural Network with Stratified " << K << "-Fold CV" << std::endl;
	auto best_params = grid_search::genetic<NeuralNetwork<float>>(	data, 
																	n_population,
																	max_iter,
																	elit_rate,
																	mutation_rate,
																	K,
																	seed,
																	8, // number of thread to use for parallel GA
																	false,
																	genetic_out);

	delete[] sizes;
	return 0;
}
