// Created on 2017-06-07
// Author: Binbin Zhang

#include "net.h"

template <typename DType>
Matrix<DType>::Matrix(int row, int col): rows_(row), cols_(col), data_(NULL) {
    if (row * col != 0) {
        data_ = new DType[row * col]();
    }
}

template <typename DType>
void Matrix<DType>::Resize(int row, int col) {
    if (row * col != rows_ * cols_) {
        if (data_ != NULL) delete [] data_;
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
Vector<DType>::Vector(int dim): dim_(dim), data_(NULL) {
    if (dim_ != 0) {
        data_ = new DType[dim]();
    }
}

template <typename DType>
void Vector<DType>::Resize(int dim) {
    if (dim != dim_) {
        if (data_ != NULL) delete [] data_;
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

void Layer::Read(std::istream &is) {
    char t = static_cast<char>(type_);
    is.read(&t, 1); 
    is.read((char *)&in_dim_, sizeof(int)); 
    is.read((char *)&out_dim_, sizeof(int)); 
    ReadData(is);
}

void Layer::Write(std::ostream &os) {
    char t = static_cast<char>(type_);
    os.write(&t, 1); 
    os.write((char *)&in_dim_, sizeof(int)); 
    os.write((char *)&out_dim_, sizeof(int)); 
    WriteData(os);
}

void FullyConnect::ReadData(std::istream &is) {
    w_.Read(is);
    b_.Read(is);
}

void FullyConnect::WriteData(std::ostream &os) {
    w_.Write(os);
    b_.Write(os);
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

