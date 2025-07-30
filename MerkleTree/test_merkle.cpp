#include "MerkleTree.h"
#include <iostream>
#include <vector>

int main() {
    std::vector<std::string> leaves = {"a", "b", "c", "d"};
    MerkleTree tree(leaves);

    std::cout << "Root: " << tree.getRoot() << std::endl;
    for (size_t i = 0; i < leaves.size(); ++i) {
        auto proof = tree.getProof(i);
        std::cout << "Proof for leaf " << i << ": ";
        for (const auto& p : proof) std::cout << p << " ";
        std::cout << std::endl;
        bool valid = tree.verifyProof(leaves[i], proof, tree.getRoot());
        std::cout << "Verification: " << (valid ? "PASS" : "FAIL") << std::endl;
    }

    tree.addLeaf("e");
    std::cout << "New root after adding 'e': " << tree.getRoot() << std::endl;
    tree.removeLeaf(0);
    std::cout << "Root after removing first leaf: " << tree.getRoot() << std::endl;
    std::cout << "Leaf count: " << tree.getLeafCount() << std::endl;
    std::cout << "Leaf 0: " << tree.getLeaf(0) << std::endl;
    tree.clear();
    std::cout << "Root after clear: " << tree.getRoot() << std::endl;
    return 0;
}
