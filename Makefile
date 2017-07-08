CXX = g++
CXXFLAGS = -std=c++11 -I . -lopenblas -lpthread -msse4.1 -D USE_BLAS # -D QUANTIZE_BIAS

OBJ = net.o

TEST = test/net-test test/matrix-major-test test/matrix-blas-test \
       test/mnist-test

BIN = tools/net-quantization

all: $(TEST) $(BIN)

test/%: test/%.cc $(OBJ)
	$(CXX) $< $(OBJ) $(CXXFLAGS) -o $@

tools/%: tools/%.cc $(OBJ)
	$(CXX) $< $(OBJ) $(CXXFLAGS) -o $@

net.o: net.h utils.h

.PHONY: clean

clean:
	rm -rf $(OBJ); rm -rf $(TEST)

