#pragma once
#include "os.hpp"
#include <chrono>
#include <random>
#include <iterator>
#include <cstring>
#ifndef M_PI
	#define M_PI 3.141592653589793238462643383279f
#endif

class Point
{
	void error(const std::string &, const int &);
public:
	float *x, *y, *z;
	int n, dim;
	Point();
	Point(const int &, int dim = 2);
	Point(const std::string &, std::string sep = "\t");
	Point& operator=(const Point &);
	Point(const Point &);

	void random_gen(const int &, int dim = 2, unsigned int seed = 0);
	void polar_gen(const int &, const float &, unsigned int seed = 0);
	void spherical_gen(const int &, const float &, unsigned int seed = 0);
	void binormal_gen(const int &, float *, float *, const int &, float *mean_y = nullptr, float *var_y = nullptr, unsigned int seed = 0);
};

void Point::error(const std::string &message, const int &n)
{
	if(n)
	{
		std::cerr << "Point error! " << message << std::endl;
		exit(n);
	}
	else exit(n);
}

Point::Point()
{
	this->n = 0;
	this->dim = 0;
	this->x = nullptr;
	this->y = nullptr;
	this->z = nullptr;
}

Point::Point(const int &n, int dim)
{
	this->n = n;
	this->x = new float[n];
	this->y = new float[n];
	if(dim == 3) this->z = new float[n];
}
Point::Point(const std::string &filename, std::string sep)
{
	std::ifstream file(filename);
	if( !file ) error("File not found. Given : " + filename, 1);
	std::string row; 
	std::vector<std::string> token;
	this->n = 1;
	std::getline(file, row);
	token = split(row, sep);
	this->dim = (int)token.size();
	file.unsetf(std::ios_base::skipws);
	//count the newlines with an algorithm specialized for counting:
	this->n += std::count(std::istream_iterator<char>(file),
	    				  std::istream_iterator<char>(), 
	    				  '\n');

	file.clear();
	file.seekg(0, std::ios::beg);
	file.setf(std::ios_base::skipws);
	this->x = new float[this->n]; this->y = new float[this->n];
	if(this->dim == 3) this->z = new float[this->n];
	for(int i = 0; i < this->n; ++i)
	{
		file >> this->x[i];
		file >> this->y[i];
		if(this->dim == 3) file >> this->z[i];
	}
	file.close();
}
	
Point& Point::operator=(const Point &t)
{
	if(this->n != 0)
	{
		delete[] this->x;
		delete[] this->y;
		if(this->dim == 3) delete[] this->z;
	}
	this->n = t.n;
	this->dim = t.dim;
	this->x = new float[t.n]; this->y = new float[t.n];
	if(this->dim == 3) this->z = new float[t.n];
	std::memcpy(this->x, t.x, sizeof(float)*t.n);
	std::memcpy(this->y, t.y, sizeof(float)*t.n);
	if(this->dim == 3) std::memcpy(t.z, this->z, sizeof(float)*t.n);
	return *this;
}

Point::Point(const Point &t)
{
	if(this->n != 0)
	{
		delete[] this->x;
		delete[] this->y;
		if(this->dim == 3) delete[] this->z;
	}
	this->n = t.n;
	this->dim = t.dim;
	this->x = new float[t.n]; this->y = new float[t.n];
	if(this->dim == 3) this->z = new float[t.n];
	std::memcpy(this->x, t.x, sizeof(float)*t.n);
	std::memcpy(this->y, t.y, sizeof(float)*t.n);
	if(this->dim == 3) std::memcpy(t.z, this->z, sizeof(float)*t.n);
}



void Point::polar_gen(const int &n_point, const float &radius, unsigned int seed)
{
	float theta, r;
	std::mt19937 engine{seed};
	std::uniform_real_distribution<float> r_dist{0.0f, 1.0f};
	if(this->n != 0)
	{
		delete[] this->x;
		delete[] this->y;
	}
	this->n = n_point;
	this->x = new float[n_point];
	this->y = new float[n_point];
	this->dim = 2;
	for(int k = 0; k < n_point; ++k)
	{
		theta = 2 * (float)M_PI * r_dist(engine);
		r = radius * r_dist(engine);
		this->x[k] = r * cos(theta);
		this->y[k] = r * sin(theta);
	}
	return;
}

void Point::spherical_gen(const int &n_point, const float &radius, unsigned int seed)
{
	float phi, theta, r;
	std::mt19937 engine{seed};
	std::uniform_real_distribution<float> r_dist{0.0f, 1.0f};
	
	if(this->n != 0)
	{
		delete[] this->x;
		delete[] this->y;
		delete[] this->z;
	}
	this->n = n_point;
	this->dim = 3;
	this->x = new float[n_point];
	this->y = new float[n_point];
	this->z = new float[n_point];
	for(int k = 0; k < n_point; ++k)
	{
		phi = (float)M_PI * r_dist(engine);
		theta = 2 * (float)M_PI * r_dist(engine);
		r = radius * r_dist(engine);
		this->x[k] = r * sin(phi) * cos(theta);
		this->y[k] = r * sin(phi) * sin(theta);
		this->z[k] = r * cos(phi);
	}
	return;
}

void Point::random_gen(const int &n_point, int dim, unsigned int seed)
{
	std::mt19937 engine{seed};
	std::uniform_real_distribution<float> r_dist{0.0f, 1.0f};
	if(this->n != 0)
	{
		delete[] this->x;
		delete[] this->y;
		if(this->dim == 3)
			delete[] this->z;
	}
	this->n = n_point;
	this->dim = dim;
	this->x = new float[n_point];
	this->y = new float[n_point];
	if(dim == 3)
		this->z = new float[n_point];
	for(int k = 0; k < n_point; ++k)
	{
		this->x[k] = r_dist(engine);
		this->y[k] = r_dist(engine);
		if(dim == 3)
			this->z[k] = r_dist(engine);
	}
	
	return;
}

void Point::binormal_gen(const int &n_point, float *mean_x, float *var_x, const int &N, float *mean_y, float *var_y, unsigned int seed)
{
	int index;
	std::mt19937 engine{seed};
	std::normal_distribution<float> *gauss_x = new std::normal_distribution<float>[N], 
									*gauss_y = new std::normal_distribution<float>[N];

	for (int i = 0; i < N; ++i)
	{
		gauss_x[i] = std::normal_distribution<float>( mean_x[i], var_x[i] );
		gauss_y[i] = std::normal_distribution<float>( mean_y[i], var_y[i] );
	}

	if(this->n != 0)
	{
		delete[] this->y;
		delete[] this->x;
	}
	this->n = n_point;
	this->dim = 2;
	this->x = new float[n_point];
	this->y = new float[n_point];

	for(int k = 0; k < n_point; ++k)
	{
		index = k % N;
		this->x[k] = gauss_x[index](engine);
		this->y[k] = gauss_y[index](engine);
	}
	delete[] gauss_x; delete[] gauss_y;
	return;
}
