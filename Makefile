CXX = g++
CXXFLAGS = -g -std=c++11 -I . -lopenblas -lpthread -msse4.1 -D USE_BLAS -D QUANTIZE_BIAS

OBJ = net.o

TEST = test/net-test test/matrix-major-test test/matrix-blas-test \
       test/mnist-test test/net-quantization

all: $(TEST)

test/%: test/%.cc $(OBJ)
	$(CXX) $< $(OBJ) $(CXXFLAGS) -o $@

net.o: net.h utils.h

.PHONY: clean

clean:
	rm -rf $(OBJ); rm -rf $(TEST)

