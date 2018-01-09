#include "genetic.hpp"
#include <numeric> // std::inner_product

const std::string TARGET = "I'm a genetic algorithm";
const int LENGTH = TARGET.size();

auto randchar = []() -> char {
	        const char charset[] =
	        "0123456789"
	        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	        "abcdefghijklmnopqrstuvwxyz";
	        const size_t max_index = (sizeof(charset) - 1);
	        return charset[ rand() % max_index ];
	    };

struct mystring
{
	std::string str;
	mystring(){};
	mystring(const std::string &str) {this->str = str;}
	mystring(const mystring &a) {this->str = a.str;}
	~mystring(){this->str.clear();}
	mystring& operator=(const mystring &a){this->str = a.str; return *this;}
	// random generation
	mystring(const unsigned int &seed)
	{
		srand(seed);
	    this->str.resize(LENGTH);
	    std::generate_n( this->str.begin(), LENGTH, randchar );
	}
	// crossover function
	mystring operator+(const mystring &s)
	{
		int pos = rand() % LENGTH;
		return this->str.substr(0, pos) + s.str.substr(pos, LENGTH - pos);
	}
	// mutation function
	void operator!(void)
	{
		int ipos = rand() % LENGTH;
		int delta = (rand() % 120-32) + 32;
		this->str[ipos] = ((this->str[ipos] + delta) % 120);
	}
};

std::ostream& operator<<(std::ostream &os, const mystring &s)
{	os << s.str;	return os; }

struct
{
	int operator()(const mystring &a, const mystring &b)
	{
		return std::inner_product(a.str.begin(), a.str.end(), b.str.begin(), 0, 
								  std::plus<int>(), [](const char &i, const char &j)
								  {
								  	return ((int)i - (int)j) * ((int)i - (int)j);
								  });	
	}
} fitness;

int main(int argc, char *argv[])
{
	mystring target(TARGET);
	genetic(target, 
			2048, // number of dna
			16384, // max number of iterations
			fitness, // fitness function as anonymous struct operator
			.1f, // percentage of population to conserve
			.3f, // probability of mutation
			123, // random seed
			true); // output file ON

	return 0;
}
