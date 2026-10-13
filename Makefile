CXX ?= c++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -pedantic -O2
.PHONY: test clean

test: build/test-arm9-state build/test-address-space build/test-arm9-interpreter
	./build/test-arm9-state
	./build/test-address-space
	./build/test-arm9-interpreter

build:
	mkdir -p build
build/test-arm9-state: tests/test_arm9_state.cpp src/arm9/arm9_state.cpp | build
	$(CXX) $(CXXFLAGS) $^ -o $@
build/test-address-space: tests/test_address_space.cpp src/memory/address_space.cpp | build
	$(CXX) $(CXXFLAGS) $^ -o $@
build/test-arm9-interpreter: tests/test_arm9_interpreter.cpp src/arm9/arm9_interpreter.cpp src/arm9/arm9_state.cpp | build
	$(CXX) $(CXXFLAGS) $^ -o $@
clean:
	rm -rf build
