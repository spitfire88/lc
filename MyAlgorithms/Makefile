# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pthread

# Debug flags
DEBUG_FLAGS = -g -O0 -DDEBUG

# Release flags
RELEASE_FLAGS = -O3 -DNDEBUG

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
BUILD_DIR = build
EXAMPLE_DIR = examples

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Targets
LIBRARY = $(BUILD_DIR)/libMyAlgorithms.a
TEST_EXEC = $(BUILD_DIR)/testRunner
EXAMPLE_EXEC = $(BUILD_DIR)/example

# Default target
all: $(LIBRARY) $(TEST_EXEC) $(EXAMPLE_EXEC)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Create static library
$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

# Build test executable
$(TEST_EXEC): $(TEST_DIR)/testRunner.cpp $(LIBRARY)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< -L$(BUILD_DIR) -lMyAlgorithms -o $@

# Build example executable
$(EXAMPLE_EXEC): $(EXAMPLE_DIR)/example.cpp $(LIBRARY)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< -L$(BUILD_DIR) -lMyAlgorithms -o $@

# Debug build
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Release build
release: CXXFLAGS += $(RELEASE_FLAGS)
release: clean all

# Run tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Run example
run-example: $(EXAMPLE_EXEC)
	./$(EXAMPLE_EXEC)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Install (simple copy to /usr/local)
install: $(LIBRARY)
	sudo cp $(LIBRARY) /usr/local/lib/
	sudo cp -r $(INCLUDE_DIR)/* /usr/local/include/

# Create documentation (requires doxygen)
docs:
	doxygen Doxyfile

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build library, tests, and examples"
	@echo "  debug        - Build with debug flags"
	@echo "  release      - Build with release flags"
	@echo "  test         - Run test suite"
	@echo "  run-example  - Run example program"
	@echo "  clean        - Remove build artifacts"
	@echo "  install      - Install library and headers"
	@echo "  docs         - Generate documentation"
	@echo "  help         - Show this help message"

# Phony targets
.PHONY: all debug release test run-example clean install docs help
