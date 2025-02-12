CXX = g++
CXXFLAGS = -std=c++11 -Wall -I$(GTEST_DIR)/include -pthread
GTEST_DIR = /usr/src/gtest

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
all: checkers_tests

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

checkers_tests: $(OBJ_FILES) $(TEST_OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lgtest -lgtest_main

run_tests: checkers_tests
	./checkers_tests

debug_tests: checkers_tests
	gdb ./checkers_tests

valgrind_tests: checkers_tests
	valgrind --leak-check=full ./checkers_tests

clean:
	rm -rf $(BUILD_DIR)/*.o checkers_tests
