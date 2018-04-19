
template<typename T> T* VecMat(T *x, T *A, const int &Nrow, const int &Ncol)
{
    T *b = new T[Ncol];
    std::memset(b, 0, sizeof(T)*Ncol);
    for (int i = 0; i < Ncol; ++i)
        for (int k = 0; k < Nrow; ++k)
            b[i] += A[k * Ncol + i] * x[k];

    return b;
}
template<typename T> T* MatVec(T *A, T *x, const int &Nrow, const int &Ncol)
{
    T* b = new T[Nrow];
    std::memset(b, 0, sizeof(T)*Nrow);
    for (int i = 0; i < Nrow; ++i)
        for (int k = 0; k < Ncol; ++k)
            b[i] += A[i*Ncol + k] * x[k];
    return b;
}
template<typename T> T* VecVec(T *a, T *b, const int &Na, const int &Nb)
{
    T *A = new T[Na * Nb];
    for (int i = 0; i < Nb; ++i)
        for (int j = 0; j < Na; ++j)
            A[j * Nb + i] = a[j] * b[i];
    return A;
}


template<typename T> inline T Qcost::operator()(T *a, const int &y, const int &Na)
{
    /*Return the cost associated with an output ``a`` and desired output ``y``.*/
    return (T).5 * std::accumulate(a, a + Na, (T)0., [&y](const T &i, const T &j) {return i + (j - y)*(j - y); });
}
template<typename T> inline T * Qcost::operator()(T *zs, T *a, const int &y, const int &Nz)
{
    /*Return the error delta from the output layer.*/
    T *res = new T[Nz];
    std::transform(a, a + Nz, zs, res,
                   [&y](const T &ai, const T &zi)
                   {
                    return (ai - y) * ( ((T)1./((T)1. + exp(-zi))) * ((T)1. - (T)1./((T)1. + exp(-zi))) );
                   });
    return res;
}

template<typename T> inline T Ecost::operator()(T *a, const int &y, const int &Na)
{
    //Return the cost associated with an output ``a`` and desired output ``y``.
    return std::accumulate(a, a + Na, (T)0., [&y](const T &i, const T &j) {return i + ((j == (T)1. && y == 1) ? (T)0. : (j == (T)1.) ? std::numeric_limits<T>::max() : -y*log(j) - (1 - y)*log(1 - j)); });
}
template <typename T> inline T* Ecost::operator()(T* zs, T* a, const int &y, const int &Nz)
{
    /*Return the error delta from the output layer.  Note that the
    parameter ``zs`` is not used by the method.  It is included in
    the method's parameters in order to make the interface
    consistent with the delta method for other cost classes.*/
    T* res = new T[Nz];
    std::transform(a, a + Nz, res, [&y](const T &ai)
                                    {
                                        return ai - y;
                                    });
    return res;
}


template<typename T> NeuralNetwork<T>::NeuralNetwork()
{
    this->weights = nullptr;
    this->biases = nullptr;
    this->sizes = nullptr;
    this->num_layers = 0;
}

template<typename T> NeuralNetwork<T>::~NeuralNetwork()
{
    if(this->num_layers != 0)
    {
        delete[] weights;
        delete[] biases;
        delete[] sizes;
    }
}

template<typename T>
inline void NeuralNetwork<T>::train(const Patterns<T> &data, const hyperparams<NeuralNetwork> &p)
{
    switch(p.cost)
    {
        case 0: this->train(data, p.sizes, p.num_layers, std::get<0>(p.cost_func), p.epochs, p.mini_batch_size, p.eta, p.lambda, p.seed);
        break;
        case 1: this->train(data, p.sizes, p.num_layers, std::get<1>(p.cost_func), p.epochs, p.mini_batch_size, p.eta, p.lambda, p.seed);
        break;
    }
}

template<typename T> 
template<typename Up> 
void NeuralNetwork<T>::train(const Patterns<T> &data, 
                             int *hidden_sizes, 
                             const int &hidden_layers, 
                             Up up, 
                             const int &epochs, 
                             const int &mini_batch_size, 
                             const T &eta, 
                             const T &lambda,
                             unsigned int seed)
{
    T   normalize,
        **nabla_w, **delta_nabla_w,
        **nabla_b, **delta_nabla_b;
    // initialization
    this->num_layers = hidden_layers + 2;
    this->sizes     = new int[this->num_layers];
    std::transform(hidden_sizes, hidden_sizes + hidden_layers, this->sizes + 1, [](const T &val){return val;});
    this->sizes[0]  = data.Ncol;
    std::unordered_set<int> unique(data.output, data.output + data.Nout, data.Nout*sizeof(int));
    this->sizes[this->num_layers - 1] = (int)unique.size();


    this->biases    = new T*[this->num_layers - 1]; 
    nabla_b         = new T*[this->num_layers - 1]; 
    delta_nabla_b   = new T*[this->num_layers - 1];

    this->weights   = new T*[this->num_layers - 1]; 
    nabla_w         = new T*[this->num_layers - 1]; 
    delta_nabla_w   = new T*[this->num_layers - 1];
    for (int i = 0; i < this->num_layers - 1; ++i)
    {
        this->biases[i]     = new T[this->sizes[i + 1]]; 
        nabla_b[i]          = new T[this->sizes[i + 1]]; 
        delta_nabla_b[i]    = new T[this->sizes[i + 1]];
        normalize = sqrt((T)this->sizes[i + 1]);

        this->weights[i]    = new T[this->sizes[i] * this->sizes[i + 1]]; 
        nabla_w[i]          = new T[this->sizes[i] * this->sizes[i + 1]];
        delta_nabla_w[i]    = new T[this->sizes[i] * this->sizes[i + 1]];

        std::generate(this->biases[i],  this->biases[i]  + this->sizes[i + 1],[]{return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);});
        std::generate(this->weights[i], this->weights[i] + this->sizes[i] * this->sizes[i + 1], []{return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);});

    }
    //this->biases[0][1] = -1.22363086;
//  //this->biases[0][0] = 0.26126199;
//  //this->biases[0][2] = -0.11668016;
//  //this->biases[0][3] = 0.42849846;
//  //this->biases[1][0] = 0.4635173;
//  //this->biases[1][1] = -0.54431186;
//
//  //this->weights[0][0] = 0.75638639;
//  //this->weights[0][1] = -0.70799737;
//  //this->weights[0][2] = -0.46334466;
//  //this->weights[0][3] = -0.88191277;
//  //this->weights[0][4] = -0.52307343;
//  //this->weights[0][5] = 1.35898807;
//  //this->weights[0][6] = -0.2599581;
//  //this->weights[0][7] = -0.10235441;
//
    //this->weights[1][0] = 0.19030077;
    //this->weights[1][1] = 0.18520836;
    //this->weights[1][2] = 0.19684286;
    //this->weights[1][3] = -0.62511548;
    //this->weights[1][4] = 0.41484186;
    //this->weights[1][5] = -0.88737718;
    //this->weights[1][6] = -0.34211414;
    //this->weights[1][7] = 0.75695548;

    this->SGD(  data,
                nabla_w,
                nabla_b,
                delta_nabla_w,
                delta_nabla_b,
                epochs,
                mini_batch_size, 
                up,
                eta, 
                lambda);
    for(int i = 0; i < this->num_layers - 1; ++i)
    {
        delete[] nabla_w[i];
        delete[] nabla_b[i];
        delete[] delta_nabla_w[i];
        delete[] delta_nabla_b[i];
    }
    delete[] nabla_w;
    delete[] nabla_b;
    delete[] delta_nabla_w;
    delete[] delta_nabla_b;
    return;
}

template<typename T> 
inline void NeuralNetwork<T>::cv_train( const Patterns<T> &data,
                                        const std::vector<int> &idx,
                                        const hyperparams<NeuralNetwork> &p)
{
    switch(p.cost)
    {
        case 0: this->cv_train(data, idx, p.sizes, p.num_layers, std::get<0>(p.cost_func), p.epochs, p.mini_batch_size, p.eta, p.lambda, p.seed);
        break;
        case 1: this->cv_train(data, idx, p.sizes, p.num_layers, std::get<1>(p.cost_func), p.epochs, p.mini_batch_size, p.eta, p.lambda, p.seed);
        break;
    }
}

template<typename T> 
template<typename Up> 
inline void NeuralNetwork<T>::cv_train( const Patterns<T> &data,
                                        const std::vector<int> &idx,
                                        int *sizes, 
                                        const int &num_layers, 
                                        Up up, 
                                        const int &epochs, 
                                        const int &mini_batch_size, 
                                        const T &eta, 
                                        const T &lambda,
                                        unsigned int seed)
{
    Patterns<T> train;
    train.Nrow = (int)idx.size();
    train.Ncol = data.Ncol;
    train.Nout = (int)idx.size();
    train.output = new int[train.Nout];
    train.input = new T*[train.Nrow];
    std::generate(train.input, train.input + train.Nrow, [&data](){return new T[data.Ncol];});
    for(int i = 0; i < (int)idx.size(); ++i)
    {
        std::memcpy(train.input[i], data.input[idx[i]], sizeof(T)*train.Ncol);
        train.output[i] = data.output[idx[i]];
    }
    this->train(train, sizes, num_layers, up, epochs, mini_batch_size, eta, lambda, seed);
    return;
}

template<typename T>
template<typename Up> 
void NeuralNetwork<T>::SGD( const Patterns<T> &training, 
                            T **nabla_w,
                            T **nabla_b,
                            T **delta_nabla_w,
                            T **delta_nabla_b,
                            const int &epochs, 
                            const int &mini_batch_size, 
                            Up up,
                            const T &eta, 
                            const T &lambda)
{
    //Train the neural network using mini-batch stochastic gradient descent.
    
    int mini_batch_number = (int)(training.Nrow / mini_batch_size) + ((training.Nrow % mini_batch_size) ? 1 : 0),
        **mini_batch = new int*[mini_batch_number], 
        *rand_idx = new int[training.Nrow];

    if (training.Nrow % mini_batch_size)
    {
        std::generate(mini_batch, mini_batch + mini_batch_number - 1, [&mini_batch_size]{return new int[mini_batch_size];});
        mini_batch[mini_batch_number - 1] = new int[training.Nrow % mini_batch_size];
    }
    else
        std::generate(mini_batch, mini_batch + mini_batch_number, [&mini_batch_size]{return new int[mini_batch_size];});

    std::iota(rand_idx, rand_idx + training.Nrow, 0);
    
    for (int j = 0; j < epochs; ++j)
    {
        //RAND SPENTO

        //std::shuffle(rand_idx, rand_idx + training.Nrow, std::mt19937(std::rand()));
        for (int i = 0; i < training.Nrow; ++i)
            mini_batch[(int)(i / mini_batch_size)][i % mini_batch_size] = i;
            //mini_batch[(int)(i / mini_batch_size)][i % mini_batch_size] = rand_idx[i];
        
        std::for_each(mini_batch, mini_batch + mini_batch_number,
                      [&training, &mini_batch_size, &eta, &lambda, &up, nabla_w, nabla_b, delta_nabla_w, delta_nabla_b, this]
                      (int *batch)
                      {
                        this->update_mini_batch(training, nabla_w, nabla_b, delta_nabla_w, delta_nabla_b, batch, mini_batch_size, eta, lambda, up);
                      });

    }


    for(int i = 0; i < mini_batch_number; ++i) delete[] mini_batch[i];
    delete[] mini_batch;
    delete[] rand_idx;
    return;
}

template<typename T>
template<typename Up>
void NeuralNetwork<T>::update_mini_batch(const Patterns<T> &training,
                                        T **nabla_w,
                                        T **nabla_b,
                                        T **delta_nabla_w,
                                        T **delta_nabla_b,
                                        int *mini_batch_i, 
                                        const int &mini_batch_size, 
                                        const T &eta, 
                                        const T &lambda,
                                        Up up)
{
    /*Update the network's weights and biases by applying gradient
    descent using backpropagation to a single mini batch.  ``eta`` is the
    learning rate.*/
    for(int i = 0; i < this->num_layers - 1; ++i)
    {
        std::memset(nabla_b[i], (T)0, sizeof(T)*this->sizes[i+1]);
        std::memset(nabla_w[i], (T)0, sizeof(T)*this->sizes[i]*this->sizes[i+1]);
    }
    //std::cout<<"\n----------------------\n";
    for (int i = 0; i < mini_batch_size; ++i)
    {
        NeuralNetwork::backprop(training, 
                                nabla_w,
                                nabla_b,
                                delta_nabla_w,
                                delta_nabla_b,
                                mini_batch_i[i], 
                                up);
        for (int j = 0; j < this->num_layers - 1; ++j)
        {
            std::transform(delta_nabla_b[j], delta_nabla_b[j] + this->sizes[j+1], 
                           nabla_b[j], nabla_b[j],
                           [&](const T &delta, const T &b)
                           {
                            return b + delta;
                           });
            std::transform(delta_nabla_w[j], delta_nabla_w[j] + this->sizes[j]*this->sizes[j+1], 
                           nabla_w[j], nabla_w[j],
                           [&](const T &delta, const T &w)
                           {
                            return w + delta;
                           });
        }
    }
    for (int j = 0; j < this->num_layers - 1; ++j)
    {
        std::transform(nabla_b[j], nabla_b[j] + this->sizes[j+1], 
                       this->biases[j], this->biases[j], 
                       [&eta, &mini_batch_size](const T &nabla, const T &bias)
                       {
                        return bias - (eta/mini_batch_size)*nabla;
                       });
        std::transform(nabla_w[j], nabla_w[j] + this->sizes[j]*this->sizes[j+1], 
                       this->weights[j], this->weights[j], 
                       [&eta, &mini_batch_size, &lambda, &training](const T &nabla, const T &weight)
                       {
                        return weight * (1 - eta * lambda / training.Nrow) - (eta / mini_batch_size) * nabla;
                       });
    }
    return;
}

template<typename T>
template<typename Up> 
void NeuralNetwork<T>::backprop(const Patterns<T> &training, 
                                T **nabla_w,
                                T **nabla_b,
                                T **delta_nabla_w,
                                T **delta_nabla_b,
                                const int &mini_batch_ii,
                                Up cost)
{
    //feedforward
    T **activations = new T*[this->num_layers], **zs = new T*[this->num_layers - 1];
    activations[0] = new T[this->sizes[0]];
    std::memcpy(activations[0], training.input[mini_batch_ii], sizeof(T)*this->sizes[0]);

    for (int i = 1; i < this->num_layers; ++i)
    {
        activations[i] = new T[this->sizes[i]];
        zs[i - 1] = VecMat(activations[i - 1], this->weights[i - 1], this->sizes[i - 1], this->sizes[i]);
        std::transform(this->biases[i - 1], this->biases[i - 1] + this->sizes[i],
                       zs[i-1], zs[i-1],
                       [&](const T &bias, const T &z)
                       {
                        return z + bias;
                       });
        //DEBUG
        //std::cout<<"zs \n";
        //for(int j = 0; j<this->sizes[i]; ++j)
        //  std::cout<< zs[i-1][j]<< " ";
        //std::cout<<std::endl;
        std::transform(zs[i-1], zs[i-1] + this->sizes[i],
                       activations[i], 
                       [&](const T &z)
                       {
                        return (T)1. / ((T)1. + exp(-z));
                       });
    }
    //std::cin.get();

    // backward pass
    T *delta = cost(zs[this->num_layers - 2], activations[this->num_layers - 1], training.output[mini_batch_ii], this->sizes[this->num_layers - 1]);
    
    std::memcpy(delta_nabla_b[num_layers - 2], delta, sizeof(T)*this->sizes[this->num_layers - 1]);
    delta_nabla_w[this->num_layers - 2] = VecVec(activations[this->num_layers - 2], delta, this->sizes[this->num_layers - 2], this->sizes[this->num_layers - 1]);
    for (int l = 2; l < num_layers; ++l)
    {
        delta = MatVec(this->weights[this->num_layers - l], delta, this->sizes[this->num_layers - l], this->sizes[this->num_layers - l + 1]);
        
        std::transform(zs[this->num_layers - 1 - l], zs[this->num_layers - 1 - l] + this->sizes[this->num_layers - l],
                       delta, delta,
                       [&](const T &z, const T &d)
                       {
                        return d * ( ((T)1./((T)1. + exp(-z))) * ((T)1. - (T)1./((T)1. + exp(-z))) );
                       });
        std::memcpy(delta_nabla_b[this->num_layers - 1 - l], delta, sizeof(T)*this->sizes[this->num_layers - l]);
        delta_nabla_w[this->num_layers - 1 - l] = VecVec(activations[this->num_layers - 1 - l], delta, this->sizes[this->num_layers - 1 - l], this->sizes[this->num_layers - l]);
    }
    delete[] delta;
    for (int i = 0; i < num_layers; ++i) delete[] activations[i];
    for (int i = 0; i < num_layers - 1; ++i) delete[] zs[i];
    delete[] activations;
    delete[] zs;
}

template<typename T> T* NeuralNetwork<T>::feedforward(T *data)
{
    //Return the output of the network if ''a'' is input.
    T *a = new T[this->sizes[0]];
    std::memcpy(a, data, sizeof(T)*this->sizes[0]);
    for (int i = 0; i < this->num_layers - 1; ++i)
    {
        a = VecMat(a, this->weights[i], this->sizes[i], this->sizes[i + 1]);
        std::transform(a, a + this->sizes[i + 1], 
                       this->biases[i], a, 
                       [](const T &ai, const T &b)
                       {
                        return (T)1. / ((T)1. + exp(-(ai + b)));
                       });
    }
    return a;
}

template<typename T>template<typename Up> inline T NeuralNetwork<T>::total_cost(const Patterns<T> &pattern, const T &lambda, Up fn)
{
    T cost_;
    cost_ = std::inner_product(pattern.input, pattern.input + pattern.Nrow, 
                               pattern.output, (T)0.,
                               std::plus<T>(),
                               [&fn, this](T* in, const int &out) mutable
                               {
                                T *a = this->feedforward(in);
                                T res = fn(a, out, this->sizes[0]);
                                delete[] a;
                                return res;
                               }) / pattern.Nrow;
    for (int i = 0; i < this->num_layers - 1; ++i)
        cost_ += (T).5 * (lambda / pattern.Nrow) * std::inner_product(this->weights[i], this->weights[i] + this->sizes[i]*this->sizes[i+1], this->weights[i], (T)0.);

    return cost_;
}

template<typename T> inline int NeuralNetwork<T>::accuracy(const Patterns<T> &pattern)
{
    /*Return the number of inputs in ``pattern`` for which the neural
    network outputs the correct result. The neural network's
    output is assumed to be the index of whichever neuron in the
    final layer has the highest activation. */
    int acc;
    acc = std::inner_product(pattern.input, pattern.input + pattern.Nrow, pattern.output, (T)0., 
                                std::plus<int>(), 
                                [this](T* in, const int &out) mutable 
                                {
                                    T *a = this->feedforward(in);
                                    int res = (std::distance(a, std::max_element(a, a + this->sizes[this->num_layers - 1])) == out) ? 1 : 0;
                                    delete[] a;
                                    return res;
                                });
    return acc;
}


template<typename T> int* NeuralNetwork<T>::test(const Patterns<T> &test)
{
    int *predict = new int[test.Nout];
    std::transform( test.input, test.input + test.Nrow,
                    predict, [this](T *input)
                    {
                        T *out = this->feedforward(input);
                        int res = std::distance(out, std::max_element(out, out + this->sizes[this->num_layers - 1]));
                        delete[] out;
                        return res;
                    });
    return predict;
}

template<typename T> int* NeuralNetwork<T>::cv_test(const Patterns<T> &test, const std::vector<int> &idx)
{
    int *predict = new int[idx.size()];
    for(int i = 0; i < (int)idx.size(); ++i)
    {
        T *out = this->feedforward(test[idx[i]]);
        predict[i] = std::distance(out, std::max_element(out, out + this->sizes[this->num_layers - 1]));
        delete[] out;
    }
    return predict;
}

template<typename T> inline int NeuralNetwork<T>::predict(T *input)
{
    T *out = this->feedforward(input);
    int res = std::distance(out, std::max_element(out, out + this->sizes[this->num_layers - 1]));
    delete[] out;
    return res;
}

template<typename T> void NeuralNetwork<T>::save(const std::string &filename, bool bin)
{
    std::ofstream os;
    switch((int)bin)
    {
        case 1:
        {
            // binary file
            os.open(filename, std::ios::out | std::ios::binary);
            // save training dimensions and network dimensions
            os.write( (const char *) &this->num_layers, sizeof(int));
            std::for_each(this->sizes, this->sizes + this->num_layers, 
                          [&os](const int &val)
                          {
                            os.write( (const char*)&val, sizeof(int));
                          });
            // save weights matrix
            for (int i = 0; i < this->num_layers - 1; ++i)
                std::for_each(this->weights[i], this->weights[i] + this->sizes[i]*this->sizes[i+1], 
                              [&os](const T &val)
                              {
                                os.write( (const char*)&val, sizeof(T));
                              });
            // save biases matrix
            for (int i = 0; i < this->num_layers - 1; ++i)
                std::for_each(this->biases[i], this->biases[i] + this->sizes[i] * this->sizes[i+1], 
                              [&os](const T &val)
                              {
                                os.write( (const char*)&val, sizeof(T));
                              });
        }break;
        case 0:
        {
            os.open(filename);
            //save network dimensions
            os << this->num_layers << "\t";
            std::copy(this->sizes, this->sizes + this->num_layers - 1, std::ostream_iterator<int>(os, "\t"));
            os << this->sizes[this->num_layers - 1] << std::endl;
            //save weights matrix
            for (int i = 0; i < this->num_layers - 1; ++i)
            {
                std::copy(this->weights[i], this->weights[i] + this->sizes[i] * this->sizes[i + 1] - 1, std::ostream_iterator<T>(os, "\t"));
                os << this->weights[i][this->sizes[i] * this->sizes[i + 1] - 1] << std::endl;
            }
            //save biases matrix
            for (int i = 0; i < this->num_layers - 1; ++i)
            {
                std::copy(this->biases[i], this->biases[i] + this->sizes[i + 1] - 1, std::ostream_iterator<T>(os, "\t"));
                os << this->biases[i][this->sizes[i + 1] - 1] << std::endl;
            }
        }break;
    }   
    os.close();
    return;
}

template<typename T> void NeuralNetwork<T>::load(const std::string &filename, bool bin)
{
    std::ifstream is;
    switch((int)bin)
    {
        case 1:
        {
            is.open(filename, std::ios::binary);
            is.read((char *)&this->num_layers, sizeof(int));
            this->sizes = new int[this->num_layers];
            for(int i = 0; i < this->num_layers; ++i) is.read((char*)&this->sizes[i], sizeof(int));
            this->biases = new T*[this->num_layers - 1];
            this->weights = new T*[this->num_layers - 1];
            for(int i = 0; i < this->num_layers - 1; ++i)
            {
                this->weights[i] = new T[this->sizes[i + 1]];
                for(int j = 0; j < this->sizes[i + 1]; ++j) is.read((char*)&this->weights[i][j], sizeof(T));
            }
            for(int i = 0; i < this->num_layers - 1; ++i)
            {
                this->biases[i] = new T[this->sizes[i + 1]];
                for(int j = 0; j < this->sizes[i + 1]; ++j) is.read((char*)&this->biases[i][j], sizeof(T));
            }
        }break;
        case 0:
        {
            is.open(filename);
            is >> this->num_layers;
            this->sizes = new int[this->num_layers];
            for(int i = 0; i < this->num_layers; ++i) is >> this->sizes[i];
            this->biases = new T*[this->num_layers - 1];
            this->weight = new T*[this->num_layers - 1];
            for(int i = 0; i < this->num_layers - 1; ++i)
            {
                this->weights[i] = new T[this->sizes[i + 1]];
                for(int j = 0; j < this->sizes[i + 1]; ++j) is >> this->weights[i][j];
            }
            for(int i = 0; i < this->num_layers - 1; ++i)
            {
                this->biases[i] = new T[this->sizes[i + 1]];
                for(int j = 0; j < this->sizes[i + 1]; ++j) is >> this->biases[i][j];
            }
        }break;
    }
    is.close();
}
