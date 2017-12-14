#include "cross_validation.hpp"
#include <iterator>
#include <numeric>

int main(int argc, char **argv)
{
	std::vector<int> v(10);
	std::iota(v.begin(), v.end(), 0);
	for(auto &i : v) std::cout << i << " ";	std::cout << std::endl;

	int K = 6, ntrain, ntest, ntrain2, ntest2;

	CrossValidation<std::vector<int>::const_iterator> cv;
	std::vector<int> test, train;

	std::cout << "==============   std::vector    =============" << std::endl;
	std::cout << "========= Leave One Out Cross Validation =========" << std::endl;
	cv.LeaveOneOut(v.begin(), v.end(), false);
	for(int i = 0; i < (int)v.size(); ++i)
	{
		cv.getFold(i+1, std::back_inserter(train), std::back_inserter(test));
		std::cout << "Fold " << i + 1 << " training data" << std::endl;
		for(auto &x : train) std::cout << x << " "; std::cout << std::endl;
		std::cout << "Fold " << i + 1 << " testing data" << std::endl;
		for(auto &x : test) std::cout << x << " "; std::cout << std::endl;
		train.clear();
		test.clear();
	}

	std::cout << "============ K-Fold Cross Validation ============" << std::endl;
	cv.KFold(K, v.begin(), v.end(), false);
	for (int i = 0; i < K; ++i)
	{
		cv.getFold(i + 1, std::back_inserter(train), std::back_inserter(test), ntrain, ntest);
		std::cout << "Fold " << i + 1 << " Training Data" << std::endl;
		for (auto &x : train) std::cout << x << " "; std::cout << std::endl;
		std::cout << "Fold " << i + 1 << " Testing Data" << std::endl;
		for (auto &x : test) std::cout << x << " ";	std::cout << std::endl;
		
		train.clear();
		test.clear();
	}



	int *v2 = new int[10];
	std::iota(v2, v2 + 10, 0);
	CrossValidation<int*> cv2;
	int *test2 = new int[10], *train2 = new int[10];
	std::cout << "==============   pointer    =============" << std::endl;
	std::cout << "========= Leave One Out Cross Validation =========" << std::endl;
	cv2.LeaveOneOut(v2, v2 + 10, false);
	for(int i = 0; i < 10; ++i)
	{
		cv2.getFold(i+1, train2, test2);
		std::cout << "Fold " << i + 1 << " training data" << std::endl;
		for(int j = 0; j < 9; ++j) std::cout << train2[j] << " "; std::cout << std::endl;
		std::cout << "Fold " << i + 1 << " testing data" << std::endl;
		for(int j = 0; j < 1; ++j) std::cout << test2[j] << " "; std::cout << std::endl;
	}

	std::cout << "============ K-Fold Cross Validation ============" << std::endl;
	cv2.KFold(K, v2, v2 + 10, false);
	for (int i = 0; i < K; ++i)
	{
		cv2.getFold(i + 1, train2, test2, ntrain2, ntest2);
		std::cout << "Fold " << i + 1 << " Training Data" << std::endl;
		for(int j = 0; j < ntrain2; ++j) std::cout << train2[j] << " "; std::cout << std::endl;
		std::cout << "Fold " << i + 1 << " testing data" << std::endl;
		for(int j = 0; j < ntest2; ++j) std::cout << test2[j] << " "; std::cout << std::endl;
	}
	std::cout << std::endl;
	return 0;
}
