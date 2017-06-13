// Created on 2017-06-07
// Author: Binbin Zhang
#include "net.h"

#include <math.h>

#include <algorithm>

template <typename DType>
Matrix<DType>::Matrix(int32_t row, int32_t col): rows_(row), cols_(col), data_(NULL) {
    if (row * col != 0) {
        data_ = new DType[row * col]();
    }
}

template <typename DType>
void Matrix<DType>::Resize(int32_t row, int32_t col) {
    if (row * col != rows_ * cols_) {
        if (data_ != NULL) delete [] data_;
        rows_ = row;
        cols_ = col;
        data_ = new DType[row * col]();
    }
}

template <typename DType>
void Matrix<DType>::Read(std::istream &is) {
    is.read((char *)&rows_, sizeof(int32_t)); 
    is.read((char *)&cols_, sizeof(int32_t)); 
    is.read((char *)data_, sizeof(DType) * rows_ * cols_);
}

template <typename DType>
void Matrix<DType>::Write(std::ostream &os) {
    os.write((char *)&rows_, sizeof(int32_t)); 
    os.write((char *)&cols_, sizeof(int32_t)); 
    os.write((char *)data_, sizeof(DType) * rows_ * cols_);

}

template <typename DType>
void Matrix<DType>::Mul(const Matrix<DType> &mat1, const Matrix<DType> &mat2, 
        bool transpose) {
    if (!transpose) {
        assert(mat1.NumCols() == mat2.NumRows());
        this->Resize(mat1.NumRows(), mat2.NumCols());
        for (int i = 0; i < mat1.NumRows(); i++) {
            for (int j = 0; j < mat2.NumCols(); j++) {
                for (int k = 0; k < mat1.NumCols(); k++) {
                    (*this)(i, j) += mat1(i, k) * mat2(k, j); 
                }
            }
        }
    }
    else {
        assert(mat1.NumCols() == mat2.NumCols());
        this->Resize(mat1.NumRows(), mat2.NumRows());
        for (int i = 0; i < mat1.NumRows(); i++) {
            for (int j = 0; j < mat2.NumRows(); j++) {
                for (int k = 0; k < mat1.NumCols(); k++) {
                    (*this)(i, j) += mat1(i, k) * mat2(j, k); 
                }
            }
        }
    }
}

template <typename DType>
void Matrix<DType>::Transpose(const Matrix<DType> &mat) {
    this->Resize(mat.NumCols(), mat.NumRows());
    for (int i = 0; i < mat.NumRows(); i++) {
        for (int j = 0; j < mat.NumCols(); j++) {
            (*this)(j, i) = mat(i, j);
        }
    }
}


template <typename DType>
Vector<DType>::Vector(int32_t dim): dim_(dim), data_(NULL) {
    if (dim_ != 0) {
        data_ = new DType[dim]();
    }
}

template <typename DType>
void Vector<DType>::Resize(int32_t dim) {
    if (dim != dim_) {
        if (data_ != NULL) delete [] data_;
        dim_ = dim;
        data_ = new DType[dim]();
    }
}

template <typename DType>
void Vector<DType>::Read(std::istream &is) {
    is.read((char *)&dim_, sizeof(int32_t)); 
    is.read((char *)data_, sizeof(DType) * dim_);
}

template <typename DType>
void Vector<DType>::Write(std::ostream &os) {
    os.write((char *)&dim_, sizeof(int32_t)); 
    os.write((char *)data_, sizeof(DType) * dim_);
}

std::string LayerTypeToString(LayerType type) {
    switch (type) {
        case kFullyConnect: return "<FullyConnect>";
        case kReLU: return "<ReLU>";
        case kSoftmax: return "<Softmax>";
        defaut: return "<Unknown>";
    }
}

void Layer::Read(std::istream &is) {
    char t = static_cast<char>(type_);
    is.read(&t, 1); 
    is.read((char *)&in_dim_, sizeof(int32_t)); 
    is.read((char *)&out_dim_, sizeof(int32_t)); 
    ReadData(is);
}

void Layer::Write(std::ostream &os) {
    char t = static_cast<char>(type_);
    os.write(&t, 1); 
    os.write((char *)&in_dim_, sizeof(int32_t)); 
    os.write((char *)&out_dim_, sizeof(int32_t)); 
    WriteData(os);
}

void Layer::Forward(const Matrix<float> &in, Matrix<float> *out) const {
    assert(in.NumRows() != 0);
    assert(in.NumCols() != 0);
    assert(out != NULL);
    out->Resize(in.NumRows(), out_dim_);
    ForwardFunc(in, out);
}

void Layer::Forward(const Matrix<uint8_t> &in, Matrix<uint8_t> *out) const {
    assert(in.NumRows() != 0);
    assert(in.NumCols() != 0);
    assert(out != NULL);
    out->Resize(in.NumRows(), out_dim_);
    ForwardFunc(in, out);
}

void Softmax::ForwardFunc(const Matrix<float> &in, Matrix<float> *out) const {
    for (int i = 0; i < in.NumRows(); i++) {
        float max = in(i, 0), sum = 0.0; 
        for (int j = 1; j < in.NumCols(); j++) {
            max = std::max(in(i, j), max);
        }
        for (int j = 0; j < in.NumCols(); j++) {
            sum += (*out)(i, j) = exp(in(i, j) - max);
        }
        for (int j = 0; j < in.NumCols(); j++) {
            (*out)(i, j) /= sum;
        }
    }
}

void ReLU::ForwardFunc(const Matrix<float> &in, Matrix<float> *out) const {
    for (int i = 0; i < in.NumRows(); i++) {
        for (int j = 0; j < in.NumCols(); j++) {
            (*out)(i, j) = std::max(in(i, j), 0.0f);
        }
    }
}

void FullyConnect::ReadData(std::istream &is) {
    w_.Read(is);
    b_.Read(is);
}

void FullyConnect::WriteData(std::ostream &os) {
    w_.Write(os);
    b_.Write(os);
}

void FullyConnect::ForwardFunc(const Matrix<float> &in, Matrix<float> *out) const {
    for (int i = 0; i < out->NumRows(); i++) {
        for (int j = 0; j < out->NumCols(); j++) {
            for (int k = 0; k < in.NumCols(); k++) {
                (*out)(i, j) += in(i, k) * w_(k, j);
            }
        }
    }
}

void Net::Read(const std::string &filename) {
    std::ifstream is(filename, std::ifstream::binary);   
    if (is.fail()) {
       ERROR("read file %s error, check!!!", filename.c_str()); 
    } 
    while (!is.eof()) {
        int t = is.peek(); 
        if (t == EOF) break;
        LayerType type = static_cast<LayerType>(t);
        Layer *layer = NULL;
        switch (type) {
            case kFullyConnect:
                layer = new FullyConnect();
                break;
            case kReLU:
                layer = new ReLU();
                break;
            case kSoftmax:
                layer = new Softmax();
                break;
            default:
                ERROR("Unknown layer type %d", t);
        }
        assert(layer != NULL);
        layer->Read(is);
        layers_.push_back(layer);
    }
}

void Net::Write(const std::string &filename) {
    std::ofstream os(filename, std::ofstream::binary);   
    if (os.fail()) {
       ERROR("write file %s error, check!!!", filename.c_str()); 
    } 
    for (int i = 0; i < layers_.size(); i++) {
        layers_[i]->Write(os);
    }
}

template class Matrix<uint8_t>;
template class Matrix<int>;
template class Matrix<float>;
template class Vector<uint8_t>;
template class Vector<int>;
template class Vector<float>;

