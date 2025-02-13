# Makefile for CheckersClash
# 
# Commands:
# make            - Build the checkers_tests executable
# make run_tests  - Run the tests
# make debug_tests - Debug the tests using gdb
# make valgrind_tests - Run the tests with Valgrind for memory checking
# make clean      - Clean the build directory and remove the executable

CXX = g++
CXXFLAGS = -std=c++11 -Wall -I$(GTEST_DIR)/include -pthread
GTEST_DIR = /usr/local/opt/googletest

# Directories
SRC_DIR = .
TEST_DIR = .
BUILD_DIR = ./build

# Source files
SRC_FILES = $(SRC_DIR)/ai_checkers.cpp $(SRC_DIR)/main.cpp

# Test files
TEST_FILES = $(TEST_DIR)/checkers_Gtests.cpp

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_FILES))

# Targets
.PHONY: all clean run_tests debug_tests valgrind_tests

all: checkers_tests

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

checkers_tests: $(OBJ_FILES) $(TEST_OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ -L$(GTEST_DIR)/lib -lgtest -lgtest_main

run_tests: checkers_tests
	./checkers_tests

debug_tests: checkers_tests
	lldb ./checkers_tests

valgrind_tests: checkers_tests
	valgrind --leak-check=full ./checkers_tests

clean:
	rm -rf $(BUILD_DIR) checkers_tests