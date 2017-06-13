CXX = g++
CXXFLAGS = -g -std=c++11 -I .

OBJ = net.o

TEST = test/net-test test/matrix-test

all: $(TEST)

test/%-test: test/%-test.cc $(OBJ)
	$(CXX) $(CXXFLAGS) $< $(OBJ) -o $@

net.o: net.h utils.h

.PHONY: clean

clean:
	rm -rf $(OBJ); rm -rf $(TEST)

