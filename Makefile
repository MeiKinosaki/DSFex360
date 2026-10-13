CXX ?= c++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -Werror -pedantic
BUILD := build/host
.PHONY: test host-tests clean
test: host-tests
host-tests: $(BUILD)/test-header $(BUILD)/test-memory $(BUILD)/test-arm9-state $(BUILD)/test-arm9-interpreter
	$(BUILD)/test-header
	$(BUILD)/test-memory
	$(BUILD)/test-arm9-state
$(BUILD):
	mkdir -p $(BUILD)
$(BUILD)/test-header: tests/test_nds_header.cpp src/cartridge/nds_header.cpp src/cartridge/nds_header.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -Isrc/cartridge tests/test_nds_header.cpp src/cartridge/nds_header.cpp -o $@
$(BUILD)/test-memory: tests/test_address_space.cpp src/memory/address_space.cpp src/memory/address_space.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -Isrc/memory tests/test_address_space.cpp src/memory/address_space.cpp -o $@
$(BUILD)/test-arm9-state: tests/test_arm9_state.cpp src/arm9/arm9_state.cpp src/arm9/arm9_state.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -Isrc/arm9 tests/test_arm9_state.cpp src/arm9/arm9_state.cpp -o $@
clean:
	rm -rf build

$(BUILD)/test-arm9-interpreter: tests/test_arm9_interpreter.cpp src/arm9/arm9_interpreter.cpp src/arm9/arm9_state.cpp src/arm9/arm9_interpreter.h src/arm9/arm9_state.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -Isrc/arm9 tests/test_arm9_interpreter.cpp src/arm9/arm9_interpreter.cpp src/arm9/arm9_state.cpp -o $@
