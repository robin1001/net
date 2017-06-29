// Created on 2017-06-07
// Author: Binbin Zhang
#ifndef NET_H_
#define NET_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <vector>
#include <iostream>
#include <fstream>

#include "utils.h"

template <typename DType>
class Vector;

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
    const DType operator () (int r, int c) const {
        assert(r < rows_);
        assert(c < cols_);
        return *(data_ + r * cols_ + c);
    }
    DType& operator () (int r, int c) {
        assert(r < rows_);
        assert(c < cols_);
        return *(data_ + r * cols_ + c);
    }
    // *this = alpha*this + mat1*mat2
    void Mul(const Matrix<DType> &mat1, const Matrix<DType> &mat2, 
             bool transpose = false, float alpha = 0.0);
    void Transpose(const Matrix<DType> &mat);
    void AddVec(const Vector<DType> &vec);
private:
    int32_t rows_, cols_;
    DType *data_;
    DISALLOW_COPY_AND_ASSIGN(Matrix);
};


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
    const DType operator () (int n) const {
        assert(n < dim_);
        return *(data_ + n);
    }
    DType& operator () (int n) {
        assert(n < dim_);
        return *(data_ + n);
    }
private:
    int32_t dim_;
    DType *data_;
    DISALLOW_COPY_AND_ASSIGN(Vector);
};


typedef enum {
    kFullyConnect = 0x00,
    kReLU,
    kSoftmax, 
    kUnknown
} LayerType;

std::string LayerTypeToString(LayerType type); 

class Layer {
public:
    Layer(int32_t in_dim = 0, int32_t out_dim = 0): 
        in_dim_(in_dim), out_dim_(out_dim), type_(kUnknown) {}
    void Read(std::istream &is);
    void Write(std::ostream &os);
    void Forward(const Matrix<float> &in, Matrix<float> *out) const;
    void Forward(const Matrix<uint8_t> &in, Matrix<uint8_t> *out) const; 
    int32_t InDim() const { return in_dim_; }
    int32_t OutDim() const { return out_dim_; }
    virtual LayerType Type() const { return type_; };
    void Info() const {
        std::cout << LayerTypeToString(type_) << " in_dim " << in_dim_ 
                  << " out_dim " << out_dim_ << "\n";
    }
protected:
    virtual void ForwardFunc(const Matrix<float> &in, Matrix<float> *out) const {
        ERROR("not implement, or call this method by a quantize layer "
              "layer type: %s", LayerTypeToString(type_).c_str());
    }
    virtual void ForwardFunc(const Matrix<uint8_t> &in, Matrix<uint8_t> *out) const {
        ERROR("not implement, or call this method by a float layer "
              "layer type: %s", LayerTypeToString(type_).c_str());
    }
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
private:
    void ReadData(std::istream &is);
    void WriteData(std::ostream &os);
    void ForwardFunc(const Matrix<float> &in, Matrix<float> *out) const;
    Matrix<float> w_; // w_ is cols major, so it's size (out_dim, in_dim)
    Vector<float> b_; // size(out_dim)
};

class ReLU: public Layer {
public:
    ReLU(int32_t in_dim = 0, int32_t out_dim = 0): 
        Layer(in_dim, out_dim) {
        type_ = kReLU;
    }
private:
    void ForwardFunc(const Matrix<float> &in, Matrix<float> *out) const;
};

class Softmax: public Layer {
public:
    Softmax(int32_t in_dim = 0, int32_t out_dim = 0): 
        Layer(in_dim, out_dim) {
        type_ = kSoftmax;
    }
private:
    void ForwardFunc(const Matrix<float> &in, Matrix<float> *out) const;
};

class Net {
public:
    Net(std::string filename) {
        Read(filename);
    }
    ~Net();
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

    void Forward(const Matrix<float> &in, Matrix<float> *out); 
    void Info() const;
protected:
    std::vector<Layer *> layers_;
    std::vector<Matrix<float> *> forward_buf_;
};


#endif
