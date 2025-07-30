#ifndef MERKLETREE_H
#define MERKLETREE_H

#include <vector>
#include <string>

class MerkleTree {
public:
    MerkleTree(const std::vector<std::string>& leaves);
    std::string getRoot() const;
    std::vector<std::string> getProof(size_t index) const;
    bool verifyProof(const std::string& leaf, const std::vector<std::string>& proof, const std::string& root) const;
    void addLeaf(const std::string& leaf);
    void removeLeaf(size_t index);
    size_t getLeafCount() const;
    std::string getLeaf(size_t index) const;
    void clear();

private:
    std::vector<std::string> leaves;
    std::vector<std::vector<std::string>> tree;
    void buildTree();
    std::string hash(const std::string& data) const;
};

#endif // MERKLETREE_H
