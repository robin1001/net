CXX = g++

CXXFLAGS = -g -std=c++11 -MMD -Wall -I . -lopenblas -lpthread -msse4.1 -D USE_BLAS # -D QUANTIZE_BIAS
# CXXFLAGS for arm/android
#CXXFLAGS = -std=c++11 -I . -lopenblas -lpthread -D USE_BLAS \
#           -mhard-float -D_NDK_MATH_NO_SOFTFP=1 \
#           -Wl,--no-warn-mismatch

OBJ = net.o

TEST = test/net-test test/matrix-major-test test/matrix-blas-test \
       test/mnist-test

TOOL = tools/net-quantization

all: $(TEST) $(TOOL)

test: $(TEST)
	@for x in $(TEST); do \
		printf "Running $$x ..."; \
		./$$x;  \
		if [ $$? -ne 0 ]; then \
			echo "... Fail $$x"; \
		else \
			echo "... Success $$x"; \
		fi \
	done

check:
	for file in *.h *.cc test/*.h test/*.cc tools/*.cc; do \
		echo $$file; \
        cpplint --filter=-build/header_guard,-readability/check,-build/include_subdir $$file; \
	done

test/%: test/%.cc $(OBJ)
	$(CXX) $< $(OBJ) $(CXXFLAGS) -o $@

tools/%: tools/%.cc $(OBJ)
	$(CXX) $< $(OBJ) $(CXXFLAGS) -o $@

.PHONY: clean

clean:
	rm -rf $(OBJ); rm -rf $(TEST)

