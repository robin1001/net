# NET

A simple C++ feedforward deep nerual network forward/inference library. 
Both float and 8bits quantize model are supported in inference.
Openblas is used for float GEMM(General Matrix Multiplication), 
and [gemmlowp](https://github.com/google/gemmlowp) is used for low precision/quantize GEMM.

## Tools

* tools/net-quantization: quantize float model to 8bits quantization model.
* tools/convert_keras_model.py: convert keras model to net model.
* tools/convert_kaldi_nnet1_model.py: convert kaldi nnet1 txt model to net model.

## Examples

Here is an example [nn-vad](https://github.com/robin1001/nn-vad), 
which use net as inference framework for vad task.

## Quantization 

Experiment on Mnist, test on mnist all dev dataset(10000 images)

### Precision

| Model                                      | Size | Acurracy | Regression |
|--------------------------------------------|------|----------|------------|
| origin keras model test on keras           | 5.2M | 0.9825   | -          |
| net float model                            | 2.6M | 0.9825   | 0          |
| Net 8bits model(only w_ is quantized)      | 658K | 0.9823   | 0.02035%   |
| Net 8bits model(w_ and bias all quantized) | 655K | 0.9824   | 0.10178%   |

### Time

Float model use openBlas as GEMM, and 8bits model use gemmlowp as GEMM.

> Environment: 14.04.1-Ubuntu x86_64 GNU/Linux, 24core, 1.2GHZ.

| BatchSize/Time(s) | Float Model(2.6M) | 8bits Quantize Model(256K) |
|-------------------|-------------------|----------------------------|
| 16                | 1.802             | 3.055                      |
| 32                | 1.613             | 2.409                      |
| 64                | 1.218             | 2.100                      |


> Environment: EMUI OS,  Kirin 920, 2.5GHz, 8 cores, 3G RAM.

| BatchSize/Time(ms) | Float Model(2.6M) | 8bits Quantize Model(256K) |
|--------------------|-------------------|----------------------------|
| 16                 | 6101              | 4284                       |
| 32                 | 4476              | 3788                       |
| 64                 | 3826              | 3551                       |
| 128                | 3671              | 3407                       |
| 256                | 2473              | 3387                       |

## Addational Test

### Naive vs OpenBlas GEMM

Environment: 14.04.1-Ubuntu x86_64 GNU/Linux, 24core, 1.2GHZ, Memory 
All matrix sampled from normal distribution, naïve calculate 10 times, openblas 100times.

| Size | naive     | openblas    | R        |
|------|-----------|-------------|----------|
| 128  | 0.0435292 | 0.000142205 | 306.101  |
| 256  | 0.345951  | 0.000384407 | 899.960  |
| 512  | 3.08562   | 0.00190383  | 1620.743 |
| 1024 | 24.5289   | 0.0171441   | 1430.748 |

### Row/Col major GEMM(OpenBlas)

| Size/major | Row time    | Col time    | R      |
|------------|-------------|-------------|--------|
| 256        | 0.000424027 | 0.000322676 | 1.3140 |
| 512        | 0.00222023  | 0.00188339  | 1.1788 |
| 1024       | 0.0258088   | 0.0196058   | 1.3163 |

