CXX = g++
CXXFLAGS = -g -std=c++11

OBJ = net.o

all: net-test

net-test: net-test.cc $(OBJ)
	$(CXX) $(CXXFLAGS) $< $(OBJ) -o $@

net.o: net.h utils.h


.PHONY: clean

clean:
	rm -rf $(OBJ); rm -rf net-test
