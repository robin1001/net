// Copyright (c) 2017 Personal (Binbin Zhang)
// Created on 2017-06-15
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include <random>

#include "net.h"
#include "timer.h"
#include "parse-option.h"

int main(int argc, char* argv[]) {
  const char* usage = "Simple blas matrix test\n";
  ParseOptions option(usage);
  int dim = 5;
  option.Register("dim", &dim, "dim of the matrix");
  option.Read(argc, argv);

  if (option.NumArgs() != 0) {
    option.PrintUsage();
    exit(1);
  }

  std::default_random_engine generator(777);
  std::normal_distribution<float> distribution(0.0, 1.0);
  int epoch = 100;
  Timer timer;
  double t1 = 0;
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
    Matrix<float> out1(dim, dim);
    timer.Reset();
    out1.Mul(mat1, mat2);
    t1 += timer.Elapsed();
    // for (int i = 0; i < out1.NumRows(); i++) {
    //     for (int j = 0; j < out1.NumCols(); j++) {
    //         std::cout << out1(i, j) << " ";
    //     }
    //     std::cout << "\n";
    // }
  }

  std::cout << t1 / epoch << "\n";
  return 0;
}

