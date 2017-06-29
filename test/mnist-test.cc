// Created on 2017-06-21
// Author: Binbin Zhang
#include <fstream>

#include "net.h"
#include "parse-option.h"

int BigLittleSwap(int a) {
    return ((((uint32_t)(a) & 0xff000000) >> 24) | \
            (((uint32_t)(a) & 0x00ff0000) >> 8) | \
            (((uint32_t)(a) & 0x0000ff00) << 8) | \
            (((uint32_t)(a) & 0x000000ff) << 24));
}

void ReadMnistLabel(std::string filename, std::vector<int> *label) {
    std::ifstream is(filename, std::ios::binary);
    if (is.fail()) {
       ERROR("read file %s error, check!!!", filename.c_str()); 
    }
    int magic = 0, num_images = 0;
    is.read((char *)&magic, 4);
    is.read((char *)&num_images, 4);
    magic = BigLittleSwap(magic);
    num_images = BigLittleSwap(num_images);
    std::cout << magic << " " << num_images << "\n";
    label->resize(num_images);
    unsigned char digit = 0;
    for (int i = 0; i < num_images; i++) {
        is.read((char *)&digit, 1);
        (*label)[i] = static_cast<int>(digit);
        //std::cout << (*label)[i] << "\n";
    }
}

void ReadMnistImage(std::string filename, Matrix<float> *data) {
    std::ifstream is(filename, std::ios::binary);
    if (is.fail()) {
       ERROR("read file %s error, check!!!", filename.c_str()); 
    }
    int magic = 0, num_images = 0;
    is.read((char *)&magic, 4);
    is.read((char *)&num_images, 4);
    magic = BigLittleSwap(magic);
    num_images = BigLittleSwap(num_images);
    std::cout << magic << " " << num_images << "\n";
    int rows = 0, cols = 0;
    is.read((char *)&rows, 4);
    is.read((char *)&cols, 4);
    rows = BigLittleSwap(rows);
    cols = BigLittleSwap(cols);
    std::cout << rows << " " << cols << "\n";
    data->Resize(num_images, rows * cols);
    unsigned char digit = 0;
    for (int i = 0; i < num_images; i++) {
        for (int j = 0; j < rows * cols; j++) {
            is.read((char *)&digit, 1);
            (*data)(i, j) = static_cast<float>(digit) / 255;
            //std::cout << static_cast<int>(digit) << " ";
        }
        //std::cout << "\n";
    }
}

int main(int argc, char *argv[]) {
    const char *usage = "Simple mnist test\n";
    ParseOptions option(usage);
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
    return 0;
}


