// Copyright (c) 2018 Personal (Binbin Zhang)
// Created on 2017-06-21
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

#include <fstream>

#include "net.h"
#include "parse-option.h"

int BigLittleSwap(int a) {
    return ((((uint32_t)(a) & 0xff000000) >> 24) | \
            (((uint32_t)(a) & 0x00ff0000) >> 8) | \
            (((uint32_t)(a) & 0x0000ff00) << 8) | \
            (((uint32_t)(a) & 0x000000ff) << 24));
}

void ReadMnistLabel(const std::string& filename, std::vector<int>* label) {
  std::ifstream is(filename, std::ios::binary);
  if (is.fail()) {
    ERROR("read file %s error, check!!!", filename.c_str());
  }
  int magic = 0, num_images = 0;
  is.read(reinterpret_cast<char *>(&magic), 4);
  is.read(reinterpret_cast<char *>(&num_images), 4);
  magic = BigLittleSwap(magic);
  num_images = BigLittleSwap(num_images);
  std::cout << magic << " " << num_images << "\n";
  label->resize(num_images);
  unsigned char digit = 0;
  for (int i = 0; i < num_images; i++) {
    is.read(reinterpret_cast<char *>(&digit), 1);
    (*label)[i] = static_cast<int>(digit);
    // std::cout << (*label)[i] << "\n";
  }
}

void ReadMnistImage(const std::string& filename, Matrix<float>* data) {
  std::ifstream is(filename, std::ios::binary);
  if (is.fail()) {
    ERROR("read file %s error, check!!!", filename.c_str());
  }
  int magic = 0, num_images = 0;
  is.read(reinterpret_cast<char *>(&magic), 4);
  is.read(reinterpret_cast<char *>(&num_images), 4);
  magic = BigLittleSwap(magic);
  num_images = BigLittleSwap(num_images);
  std::cout << magic << " " << num_images << "\n";
  int rows = 0, cols = 0;
  is.read(reinterpret_cast<char *>(&rows), 4);
  is.read(reinterpret_cast<char *>(&cols), 4);
  rows = BigLittleSwap(rows);
  cols = BigLittleSwap(cols);
  std::cout << rows << " " << cols << "\n";
  data->Resize(num_images, rows * cols);
  unsigned char digit = 0;
  for (int i = 0; i < num_images; i++) {
    for (int j = 0; j < rows * cols; j++) {
      is.read(reinterpret_cast<char *>(&digit), 1);
      (*data)(i, j) = static_cast<float>(digit) / 255;
      // std::cout << static_cast<int>(digit) << " ";
    }
    // std::cout << "\n";
  }
}

int main(int argc, char* argv[]) {
  const char* usage = "Simple test on mnist data\n";
  ParseOptions option(usage);
  int batch = 32;
  option.Register("batch", &batch, "batch size for net forward");
  option.Read(argc, argv);

  if (option.NumArgs() != 3) {
    option.PrintUsage();
    exit(1);
  }

  std::string net_file = option.GetArg(1),
              image_file = option.GetArg(2),
              label_file = option.GetArg(3);

  Net net(net_file);
  net.Info();
  std::vector<int> label;
  Matrix<float> data;
  ReadMnistLabel(label_file, &label);
  ReadMnistImage(image_file, &data);
  assert(label.size() == data.NumRows());
  int num_images = label.size(), num_correct = 0;
  for (int i = 0; i < num_images; i += batch) {
    int real_batch = i + batch < num_images ? batch : num_images - i;
    Matrix<float> in(real_batch, data.NumCols()), out;
    // copy
    for (int m = 0; m < in.NumRows(); m++) {
      for (int n = 0; n < in.NumCols(); n++) {
        in(m, n) = data(i+m, n);
      }
    }

    net.Forward(in, &out);

    for (int m = 0; m < out.NumRows(); m++) {
      float max = out(m, 0);
      int max_idx = 0;
      for (int n = 1; n < out.NumCols(); n++) {
        if (out(m, n) > max) {
          max = out(m, n);
          max_idx = n;
        }
      }
      if (max_idx == label[i+m]) num_correct++;
    }
  }
  printf("Accuracy %.6lf\n", static_cast<double>(num_correct) / num_images);
  return 0;
}

