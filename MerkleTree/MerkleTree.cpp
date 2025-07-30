#include "MerkleTree.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

MerkleTree::MerkleTree(const std::vector<std::string>& leaves) : leaves(leaves) {
    buildTree();
}

std::string MerkleTree::getRoot() const {
    if (tree.empty() || tree.back().empty()) return "";
    return tree.back()[0];
}

std::vector<std::string> MerkleTree::getProof(size_t index) const {
    std::vector<std::string> proof;
    if (index >= leaves.size()) return proof;
    size_t idx = index;
    for (size_t level = 0; level < tree.size() - 1; ++level) {
        size_t sibling = idx ^ 1;
        if (sibling < tree[level].size()) {
            proof.push_back(tree[level][sibling]);
        }
        idx /= 2;
    }
    return proof;
}

bool MerkleTree::verifyProof(const std::string& leaf, const std::vector<std::string>& proof, const std::string& root) const {
    std::string computed = hash(leaf);
    size_t idx = 0;
    for (const auto& sibling : proof) {
        if (idx % 2 == 0) {
            computed = hash(computed + sibling);
        } else {
            computed = hash(sibling + computed);
        }
        ++idx;
    }
    return computed == root;
}

void MerkleTree::addLeaf(const std::string& leaf) {
    leaves.push_back(leaf);
    buildTree();
}

void MerkleTree::removeLeaf(size_t index) {
    if (index < leaves.size()) {
        leaves.erase(leaves.begin() + index);
        buildTree();
    }
}

size_t MerkleTree::getLeafCount() const {
    return leaves.size();
}

std::string MerkleTree::getLeaf(size_t index) const {
    if (index < leaves.size()) return leaves[index];
    return "";
}

void MerkleTree::clear() {
    leaves.clear();
    tree.clear();
}

void MerkleTree::buildTree() {
    tree.clear();
    if (leaves.empty()) return;
    std::vector<std::string> current;
    for (const auto& leaf : leaves) {
        current.push_back(hash(leaf));
    }
    tree.push_back(current);
    while (current.size() > 1) {
        std::vector<std::string> next;
        for (size_t i = 0; i < current.size(); i += 2) {
            if (i + 1 < current.size()) {
                next.push_back(hash(current[i] + current[i + 1]));
            } else {
                next.push_back(current[i]);
            }
        }
        tree.push_back(next);
        current = next;
    }
}

std::string MerkleTree::hash(const std::string& data) const {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)data.c_str(), data.size(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}
