#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

template<class In> class CrossValidation
{
	In beg, end;
	int *whichFoldToGo;
public:
	CrossValidation() {};
	~CrossValidation();
	void KFold(const int &, const In &, const In &, bool shuff = true);
	void LeaveOneOut(const In &, const In &, bool shuff = true);
	template<class Out> void getFold(const int &, Out , Out , int &, int &);
	template<class Out> void getFold(const int &, Out , Out);
};

template<class In> CrossValidation<In>::~CrossValidation()
{ delete[] whichFoldToGo;}

template<class In> void CrossValidation<In>::KFold(const int &K, const In &beg, const In &end, bool shuff)
{
	this->beg = beg; this->end = end;
	if(K <= 0) {std::cerr << "KFold Error! K must be positive and not null" << std::endl; exit(1);}
	this->whichFoldToGo = new int[end - beg];
	int foldNo = 0, cnt = 0;
	for(In i = this->beg; i != this->end; ++i)
	{
		whichFoldToGo[cnt] = (++foldNo);
		if(foldNo == K) {foldNo = 0;}
		++cnt;
	}
	if(!K){std::cerr << "KFold Warning! With this value of K (" << K << ") equal division of the data is not possible" << std::endl; }
	if(shuff)
	{
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(whichFoldToGo, whichFoldToGo + (this->end - this->beg), g);
	}
	else
		std::random_shuffle(whichFoldToGo, whichFoldToGo + (this->end - this->beg));
	return;
}

template<class In> void CrossValidation<In>::LeaveOneOut(const In &beg, const In &end, bool shuff)
{
	this->beg = beg; this->end = end;
	this->whichFoldToGo = new int[end - beg];
	int foldNo = 0;
	for(In i = beg; i != end; ++i) whichFoldToGo[foldNo] = (++foldNo);
	if(shuff)
	{
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(whichFoldToGo, whichFoldToGo + (end - beg), g);
	}
	else
		std::random_shuffle(whichFoldToGo, whichFoldToGo + (end - beg));
	return;
}

template<class In>template<class Out> void CrossValidation<In>::getFold(const int &foldNo, Out training, Out test, int &ntrain, int &ntest)
{
	int k = 0;
	In i = this->beg;
	ntrain = 0; ntest = 0;
	while(i != this->end)
	{
		if(this->whichFoldToGo[k++] == foldNo) {*test++ = *i++; ++ntest;}
		else {*training++ = *i++; ++ntrain;}
	}
	return;
}
template<class In>template<class Out> void CrossValidation<In>::getFold(const int &foldNo, Out training, Out test)
{
	int k = 0;
	In i = this->beg;
	while(i != this->end)
	{
		if(this->whichFoldToGo[k++] == foldNo) *test++ = *i++;
		else *training++ = *i++;
	}
	return;
}
