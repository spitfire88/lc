CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
LDFLAGS = -lssl -lcrypto

all: test_merkle

MerkleTree.o: MerkleTree.cpp MerkleTree.h
	$(CXX) $(CXXFLAGS) -c MerkleTree.cpp

test_merkle: test_merkle.cpp MerkleTree.o
	$(CXX) $(CXXFLAGS) test_merkle.cpp MerkleTree.o -o test_merkle $(LDFLAGS)

clean:
	rm -f *.o test_merkle
