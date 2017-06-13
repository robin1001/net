// Created on 2017-06-13
// Author: Binbin Zhang
#include <iostream>
#include <random>

#include "net.h"
#include "timer.h"
#include "parse-option.h"

int main(int argc, char *argv[]) {
    const char *usage = "Simple test\n";
    ParseOptions option(usage);
    int dim = 256;
    option.Register("dim", &dim, "dim of the matrix");
    option.Read(argc, argv);

    if (option.NumArgs() != 0) {
        option.PrintUsage();
        exit(1);
    }

    std::default_random_engine generator(777);
    std::normal_distribution<float> distribution(0.0, 1.0);
    int epoch = 10;
    Timer timer;
    double t1 = 0, t2 = 0;
    for (int n = 0; n < epoch; n++) {
        Matrix<float> mat1(dim, dim), mat2(dim, dim);
        for (int i = 0; i < mat1.NumRows(); i++) {
            for (int j = 0; j < mat1.NumCols(); j++) {
                mat1(i, j) = distribution(generator);
            }
        }
        for (int i = 0; i < mat2.NumRows(); i++) {
            for (int j = 0; j < mat2.NumCols(); j++) {
                mat2(i, j) = distribution(generator);
            }
        }
        Matrix<float> mat2_trans;
        mat2_trans.Transpose(mat2);
        Matrix<float> out1, out2;
        timer.Reset();
        out1.Mul(mat1, mat2);
        t1 += timer.Elapsed();
        
        timer.Reset();
        out2.Mul(mat1, mat2, true);
        t2 += timer.Elapsed();
    }

    std::cout << t1 / epoch << " " << t2 / epoch << "\n";
    return 0;
}

