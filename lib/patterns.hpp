#pragma once

#include "os.hpp"
#include <cstring>
#include <random>


template<typename T> class Patterns
{
    static void error(const std::string &, const int &);
    int *lbl2num(const std::vector<std::string> &);
public:
    int Nrow,           // number of input vectors (rows of X)
        Ncol,           // cols of X
        Nout,           // size of output
        *output;        // output vector
    T   **input;        // input matrix

    Patterns(const std::string &, bool bin = false);
    Patterns(const int &, const int &);
    Patterns& operator=(const Patterns &);
    Patterns(T **, const int &, const int &, int *, const int &);
    Patterns();
    ~Patterns();
};

template<typename T> inline void check_binary(const Patterns<T> &);

//==================================================================================================================//
//  ______   ___   _____  _____  _____ ______  _   _  _____     ___  ___ _____  _____  _   _  _____ ______  _____   //
//  | ___ \ / _ \ |_   _||_   _||  ___|| ___ \| \ | |/  ___|    |  \/  ||  ___||_   _|| | | ||  _  ||  _  \/  ___|  //
//  | |_/ // /_\ \  | |    | |  | |__  | |_/ /|  \| |\ `--.     | .  . || |__    | |  | |_| || | | || | | |\ `--.   //
//  |  __/ |  _  |  | |    | |  |  __| |    / | . ` | `--. \    | |\/| ||  __|   | |  |  _  || | | || | | | `--. \  //
//  | |    | | | |  | |    | |  | |___ | |\ \ | |\  |/\__/ /    | |  | || |___   | |  | | | |\ \_/ /| |/ / /\__/ /  //
//  \_|    \_| |_/  \_/    \_/  \____/ \_| \_|\_| \_/\____/     \_|  |_/\____/   \_/  \_| |_/ \___/ |___/  \____/   //
//                                                                                                                  //
//==================================================================================================================//


template<typename T> inline void check_binary(const Patterns<T> &pattern)
{
    if(pattern.Nout != pattern.Nrow) Patterns<T>::error("Error check dimensions. Incompatible lenghts of inputs and outputs: " + std::to_string(pattern.Nrow) + " vs " + std::to_string(pattern.Nout), 1);
    int count = 0;
    for(int i = 0; i < pattern.Nrow; ++i)   count += std::count_if(pattern.input[i], pattern.input[i] + pattern.Ncol, [](const T &val){return std::abs(val) == 1;});
    if(count != pattern.Nrow * pattern.Ncol) Patterns<T>::error("Error check values. Inputs must be in {-1, 1}", 1);
    count = std::count_if(pattern.output, pattern.output + pattern.Nout, [](const int &val){return std::abs(val) != 1;});
    if(count) Patterns<T>::error("Error check values. Outputs must be in {-1, 1}", 1);
    return;
}

template<typename T> void Patterns<T>::error(const std::string &message, const int &n)
{
    if(n)
    {
        std::cerr << "Patterns error! " << message << std::endl;
        exit(n);
    }
    else std::cerr << message << std::endl;
}

template<typename T> int* Patterns<T>::lbl2num(const std::vector<std::string> &lbl)
{
    int *num = new int[(int)lbl.size()];
    std::unordered_set<std::string> str_lbl(lbl.begin(), lbl.end(), lbl.size()*sizeof(std::string));
    std::transform(lbl.begin(), lbl.end(), num, 
                   [&str_lbl](const std::string &l)
                   {
                    return std::distance(str_lbl.begin(), str_lbl.find(l));
                   });
    return num;
}

template<typename T> Patterns<T>::Patterns(const std::string &patternsfile, bool bin)
{
    std::ifstream is;
    if(bin)
    {
        is.open(patternsfile, std::ios::binary);
        if( !is ) this->error("File not found. Given " + patternsfile, 1);
        is.read((char*)&this->Nrow, sizeof(int));
        is.read((char*)&this->Ncol, sizeof(int));
        this->Nout = this->Nrow;
        this->input = new T*[this->Nrow];
        this->output = new int[this->Nout];
        for(int i = 0; i < this->Nout; ++i) is.read((char*) &this->output[i], sizeof(int));
        for(int i = 0; i < this->Nrow; ++i)
        {
            this->input[i] = new T[this->Ncol];
            std::for_each(this->input[i], this->input[i] + this->Ncol, [&is](const T &val){is.read((char*) &val, sizeof(T));});
        }
        is.close();
    }
    else
    {
        std::string row;
        std::vector<std::string> token;
        is.open(patternsfile);
        if( !is ) this->error("File not found. Given " + patternsfile, 1);
        std::getline(is, row);
        token = split(row, "\t");
        this->Nout = (int)token.size();
        if(std::isdigit(token[0][0]) || (token[0].size() > 1 && (token[0][0] == '-' || token[0][0] == '+'))) 
        {
            this->output = new int[this->Nout];
            std::transform(token.begin(), token.end(), output, [](const std::string &val){return std::stoi(val);});
        }
        else
            this->output = lbl2num(token);

        // read the first pattern line
        std::getline(is, row);
        token = split(row, "\t");
        this->Nrow = 1;
        this->Ncol = (int)token.size();
        while(std::getline(is, row))
        { 
            token = split(row, "\t");
            if(this->Ncol != (int)token.size()) this->error("Error dimensions. PatternsFile must be a matrix or a vector", 1);
            ++this->Nrow;
        }
        if(this->Nrow != this->Nout) this->error("Error dimensions. Input and output must have the same dimension", 1);
        this->input = new T*[this->Nrow];
        
        is.clear();
        is.seekg(0, std::ios::beg);
        
        std::getline(is, row);
        for (int i = 0; i < this->Nrow; ++i)
        {
            this->input[i] = new T[this->Ncol];
            std::getline(is, row);
            token = split(row, "\t");
            std::transform(token.begin(), token.end(), input[i], [](const std::string &val){return (T)std::stod(val);});
        }
        is.close();
    }
};

template<typename T> Patterns<T>::Patterns(const int &n, const int &m)
{
    this->Nrow = m;
    this->input = new T*[this->Nrow];
    this->output = new int[this->Nrow];
    this->Ncol = n;
    this->Nout = this->Nrow;

    std::default_random_engine generator;
    std::bernoulli_distribution distribution(0.5);
    std::memset(this->output, 1, sizeof(int)*this->Nout);
    for (int i = 0; i < this->Nrow; ++i)
    {
        this->input[i] = new T[this->Ncol];
        std::generate(this->input[i], this->input[i] + this->Ncol, [&distribution, &generator]{return (distribution(generator)) ? (T)1. : -(T)1.;});
    }
}

template<typename T> Patterns<T>& Patterns<T>::operator=(const Patterns<T> &patt)
{
    if(this->Nrow != 0)
    {
        for(int i = 0; i < this->Nrow; ++i) delete[] this->input[i];
        delete[] this->input;
        delete[] this->output;
    }
    this->Nrow = patt.Nrow;
    this->Nout = patt.Nout;
    this->Ncol = patt.Ncol;
    this->output = new int[this->Nout];
    std::memcpy(this->output, patt.output, sizeof(int)*this->Nout);
    this->input = new T*[this->Nrow];
    for(int i = 0; i < this->Nrow; ++i)
    {
        this->input[i] = new T[this->Ncol];
        std::memcpy(this->input[i], patt.input[i], sizeof(T)*this->Ncol);
    }
    return *this;
}

template<typename T> Patterns<T>::Patterns(T **input, const int &row, const int &col, int *out, const int &K)
{
    this->Nrow = row;
    this->Ncol = col;
    this->Nout = K;
    this->input = new T*[this->Nrow];
    this->output = new int[this->Nout];
    std::memcpy(this->output, out, sizeof(int)*this->Nout);
    for(int i = 0; i < this->Nrow; ++i)
    {
        this->input[i] = new T[this->Ncol];
        std::memcpy(this->input[i], input[i], sizeof(T)*this->Ncol);
    }
}

template<typename T> Patterns<T>::Patterns()
{
    this->Nrow = 0;
    this->Ncol = 0;
    this->Nout = 0;
    this->input = nullptr;
    this->output = nullptr;
}

template<typename T> Patterns<T>::~Patterns()
{
    if(this->Nrow)
    {
        for(int i = 0; i < this->Nrow; ++i) delete[] this->input[i];
        delete[] this->input;
        delete[] this->output;
    }
}


