#include "cross_validation.hpp"


int main(int argc, char *argv[])
{
	int N = 13, 
		K = 3,
		*labels = new int[N];
		
	std::vector<int> v(N);
	std::iota(v.begin(), v.end(), 0);
	std::cout << "indices : ";
	std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;

	CrossValidation cv;
	std::vector<int> test, train;

	std::cout << "========== Leave One Out Cross Validation ============" << std::endl;
	cv.LeaveOneOut(0, N);
	for(int i = 0; i < N; ++i)
	{
		cv.getFold(i, train, test);
		std::cout << "Fold " << i + 1 << " training data" << std::endl;
		std::copy(train.begin(), train.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
		std::cout << "Fold " << i + 1 << " testing data" << std::endl;
		std::copy(test.begin(), test.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
	}

	std::cout << "============== K-Fold Cross Validation ===============" << std::endl;
	cv.KFold(K, 0, N, false);
	for (int i = 0; i < K; ++i)
	{
		cv.getFold(i, train, test);
		std::cout << "Fold " << i + 1 << " Training Data" << std::endl;
		std::copy(train.begin(), train.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
		std::cout << "Fold " << i + 1 << " Testing Data" << std::endl;
		std::copy(test.begin(), test.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
	}

	std::fill_n(labels, 6, 0);
	std::fill_n(labels + 6, 4, 1);
	std::fill_n(labels + 10, 3, 2);
	std::random_shuffle(labels, labels + N);
	std::cout << "labels : ";
	std::copy(labels, labels + N, std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;

	std::cout << "========= Stratified K-Fold Cross Validation =========" << std::endl;
	cv.StratifiedKFold(labels, K, 0, N, false);
	for (int i = 0; i < K; ++i)
	{
		cv.getFold(i, train, test);
		std::cout << "Fold " << i + 1 << " Training Data" << std::endl;
		std::copy(train.begin(), train.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
		std::cout << "Fold " << i + 1 << " Testing Data" << std::endl;
		std::copy(test.begin(), test.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
	}

	delete[] labels;

	return 0;
}