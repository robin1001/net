// Created on 2017-06-07
// Author: Binbin Zhang


#ifndef NET_H_
#define NET_H_

#include <fstream>

#include "utils.h"

template <typename DType>
class Matrix {
public: 
    Matrix(int row, int col); 
    virtual ~Matrix() { delete [] data_; }
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
    Vector(int dim); 
    virtual ~Vector() { delete [] data_; }
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

#endif
