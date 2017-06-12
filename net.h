// Created on 2017-06-07
// Author: Binbin Zhang


#ifndef NET_H_
#define NET_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <vector>
#include <fstream>

#include "utils.h"

template <typename DType>
class Matrix {
public: 
    Matrix(int32_t row = 0, int32_t col = 0); 
    virtual ~Matrix() { 
        if (data_ != NULL) delete [] data_; 
    }
    void Resize(int32_t row, int32_t col);
    DType *Data() const { return data_; }
    int32_t NumRows() const { return rows_; }
    int32_t NumCols() const { return cols_; }
    void Read(std::istream &is);
    void Write(std::ostream &os);
private:
    int32_t rows_, cols_;
    DType *data_;
    DISALLOW_COPY_AND_ASSIGN(Matrix);
};

template class Matrix<int>;
template class Matrix<float>;
template class Matrix<char>;

template <typename DType>
class Vector {
public: 
    Vector(int32_t dim = 0); 
    virtual ~Vector() { 
        if (data_ != NULL) delete [] data_; 
    }
    void Resize(int32_t dim);
    DType *Data() const { return data_; }
    int32_t Dim() const { return dim_; }
    void Read(std::istream &is);
    void Write(std::ostream &os);
private:
    int32_t dim_;
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
    Layer(int32_t in_dim = 0, int32_t out_dim = 0): 
        in_dim_(in_dim), out_dim_(out_dim), type_(kUnknown) {}
    void Read(std::istream &is);
    void Write(std::ostream &os);
    //virtual void Forward(const Matrix<float> &in, Matrix<float> *out);
    int32_t InDim() const { return in_dim_; }
    int32_t OutDim() const { return out_dim_; }
    virtual LayerType Type() const { return type_; };
protected:
    virtual void ReadData(std::istream &is) {};
    virtual void WriteData(std::ostream &os) {};
    int32_t in_dim_,out_dim_;
    LayerType type_;
};

class FullyConnect : public Layer {
public:
    FullyConnect(int32_t in_dim = 0, int32_t out_dim = 0): 
        Layer(in_dim, out_dim) {
        type_ = kFullyConnect;
    }
    //void Forward(const Matrix<float> &in, Matrix<float> *out);
private:
    void ReadData(std::istream &is);
    void WriteData(std::ostream &os);
    Matrix<float> w_;
    Vector<float> b_;
};

class ReLU: public Layer {
public:
    ReLU(int32_t in_dim = 0, int32_t out_dim = 0): 
        Layer(in_dim, out_dim) {
        type_ = kReLU;
    }
};

class Softmax: public Layer {
public:
    Softmax(int32_t in_dim = 0, int32_t out_dim = 0): 
        Layer(in_dim, out_dim) {
        type_ = kSoftmax;
    }
};

class Net {
public:
    void Read(const std::string &filename);
    void Write(const std::string &filename);
    int32_t InDim() const { 
        assert(layers_.size() > 0);
        return layers_[0]->InDim();
    }
    int32_t OutDim() const { 
        assert(layers_.size() > 0);
        return layers_[layers_.size() - 1]->OutDim();
    }
private:
    std::vector<Layer *> layers_;
};

#endif
