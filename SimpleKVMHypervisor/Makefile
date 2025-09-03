# Simple KVM Hypervisor - Version 1.0
# Makefile for building the modular hypervisor

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
INCLUDES = -Iinclude
SRCDIR = src
OBJDIR = obj
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TARGET = simple_kvm_hypervisor

# Default target
all: $(TARGET)

# Create object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Build target
$(TARGET): $(OBJDIR) $(OBJECTS) main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJECTS) main.cpp -o $(TARGET)

# Build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Install target (optional)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Run the hypervisor
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -DDEBUG -g3 -O0
debug: $(TARGET)

# Show project structure
structure:
	@echo "Project Structure:"
	@echo "├── main.cpp"
	@echo "├── Makefile"
	@echo "├── include/"
	@find include -name "*.h" | sed 's/^/│   ├── /'
	@echo "├── src/"
	@find src -name "*.cpp" | sed 's/^/│   ├── /'
	@echo "└── obj/ (created during build)"

# Show version info
version:
	@echo "Simple KVM Hypervisor - Version 1.0"
	@echo "Features: Single VCPU, Basic Memory Management, Simple Guest Code Loading"

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the hypervisor (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  run       - Build and run the hypervisor"
	@echo "  debug     - Build with debug symbols"
	@echo "  install   - Install to /usr/local/bin (requires sudo)"
	@echo "  structure - Show project structure"
	@echo "  version   - Show version information"
	@echo "  help      - Show this help message"

.PHONY: all clean install run debug structure version help
