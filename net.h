// Created on 2017-06-07
// Author: Binbin Zhang


#ifndef NET_H_
#define NET_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <vector>
#include <fstream>

#include "utils.h"

template <typename DType>
class Matrix {
public: 
    Matrix(int row = 0, int col = 0); 
    virtual ~Matrix() { 
        if (data_ != NULL) delete [] data_; 
    }
    void Resize(int row, int col);
    DType *Data() const { return data_; }
    int NumRows() const { return rows_; }
    int NumCols() const { return cols_; }
    void Read(std::istream &is);
    void Write(std::ostream &os);
private:
    int rows_, cols_;
    DType *data_;
    DISALLOW_COPY_AND_ASSIGN(Matrix);
};

template class Matrix<int>;
template class Matrix<float>;
template class Matrix<char>;

template <typename DType>
class Vector {
public: 
    Vector(int dim = 0); 
    virtual ~Vector() { 
        if (data_ != NULL) delete [] data_; 
    }
    void Resize(int dim);
    DType *Data() const { return data_; }
    int Dim() const { return dim_; }
    void Read(std::istream &is);
    void Write(std::ostream &os);
private:
    int dim_;
    DType *data_;
    DISALLOW_COPY_AND_ASSIGN(Vector);
};

template class Vector<int>;
template class Vector<float>;
template class Vector<char>;

typedef enum {
    kFullyConnect = 0x00,
    kReLU,
    kSoftmax, 
    kUnknown
} LayerType;

class Layer {
public:
    Layer(int in_dim = 0, int out_dim = 0): 
        in_dim_(in_dim), out_dim_(out_dim), type_(kUnknown) {}
    void Read(std::istream &is);
    void Write(std::ostream &os);
    int InDim() const { return in_dim_; }
    int OutDim() const { return out_dim_; }
    virtual LayerType Type() const { return type_; };
protected:
    virtual void ReadData(std::istream &is) {};
    virtual void WriteData(std::ostream &os) {};
    int in_dim_,out_dim_;
    LayerType type_;
};

class FullyConnect : public Layer {
public:
    FullyConnect(int in_dim = 0, int out_dim = 0): 
        Layer(in_dim, out_dim) {
        type_ = kFullyConnect;
    }
private:
    void ReadData(std::istream &is);
    void WriteData(std::ostream &os);
    Matrix<float> w_;
    Vector<float> b_;
};

class ReLU: public Layer {
public:
    ReLU(int in_dim = 0, int out_dim = 0): 
        Layer(in_dim, out_dim) {
        type_ = kReLU;
    }
};

class Softmax: public Layer {
public:
    Softmax(int in_dim = 0, int out_dim = 0): 
        Layer(in_dim, out_dim) {
        type_ = kSoftmax;
    }
};

class Net {
public:
    void Read(const std::string &filename);
    void Write(const std::string &filename);
private:
    std::vector<Layer *> layers_;
};

#endif
