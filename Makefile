CXX = g++
CXXFLAGS = -g -std=c++11 -I . -lopenblas -D USE_BLAS

OBJ = net.o

TEST = test/net-test test/matrix-major-test test/matrix-blas-test

all: $(TEST)

test/%-test: test/%-test.cc $(OBJ)
	$(CXX) $< $(OBJ) $(CXXFLAGS) -o $@

net.o: net.h utils.h

.PHONY: clean

clean:
	rm -rf $(OBJ); rm -rf $(TEST)

