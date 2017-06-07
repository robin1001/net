// Created on 2017-06-07
// Author: Binbin Zhang

#include "net.h"

template <typename DType>
Matrix<DType>::Matrix(int row, int col): rows_(row), cols_(col) {
    data_ = new DType[row * col]();
}

template <typename DType>
void Matrix<DType>::Resize(int row, int col) {
    if (row * col != rows_ * cols_) {
        delete [] data_;
        rows_ = row;
        cols_ = col;
        data_ = new DType[row * col]();
    }
}

template <typename DType>
void Matrix<DType>::Read(std::istream &is) {
    is.read((char *)&rows_, sizeof(int)); 
    is.read((char *)&cols_, sizeof(int)); 
    is.read((char *)data_, sizeof(DType) * rows_ * cols_);
}

template <typename DType>
void Matrix<DType>::Write(std::ostream &os) {
    os.write((char *)&rows_, sizeof(int)); 
    os.write((char *)&cols_, sizeof(int)); 
    os.write((char *)data_, sizeof(DType) * rows_ * cols_);

}

template <typename DType>
Vector<DType>::Vector(int dim): dim_(dim) {
    data_ = new DType[dim]();
}

template <typename DType>
void Vector<DType>::Resize(int dim) {
    if (dim != dim_) {
        delete [] data_;
        dim_ = dim;
        data_ = new DType[dim]();
    }
}

template <typename DType>
void Vector<DType>::Read(std::istream &is) {
    is.read((char *)&dim_, sizeof(int)); 
    is.read((char *)data_, sizeof(DType) * dim_);
}

template <typename DType>
void Vector<DType>::Write(std::ostream &os) {
    os.write((char *)&dim_, sizeof(int)); 
    os.write((char *)data_, sizeof(DType) * dim_);
}

