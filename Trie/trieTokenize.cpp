// input="abc def ghi abcd", searchStr={{abc}, {abcd}}

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;

vector<string> tokenize(const string& input, char delimeter = ' ') {
    vector<string> tokens;
    stringstream ss(input);
    string token;

    while(getline(ss, token, delimeter)) {
        if(!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}


class Trie {
private:
    struct TrieNode {
        unordered_map<char, TrieNode*> children;
        bool isEndOfWord;
        
        TrieNode() : isEndOfWord(false) {}
    };
    
    TrieNode* root;

public:
    Trie() : root(new TrieNode()) {}
    
    ~Trie() {
        clear(root);
    }
    
    void clear(TrieNode* node) {
        for (auto& pair : node->children) {
            clear(pair.second);
        }
        delete node;
    }
    
    // Insert a single string into the trie
    void insert(const string& s) {
        TrieNode* current = root;
        
        for (char c : s) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        
        current->isEndOfWord = true;
    }
    
    // Search for a string in the trie
    bool search(const string& s) {
        TrieNode* node = findNode(s);
        return (node != nullptr && node->isEndOfWord);
    }
    
    // Check if there is any string with the given prefix
    bool startsWith(const string& prefix) {
        return findNode(prefix) != nullptr;
    }
    
private:
    TrieNode* findNode(const string& s) {
        TrieNode* current = root;
        
        for (char c : s) {
            if (current->children.find(c) == current->children.end()) {
                return nullptr;
            }
            current = current->children[c];
        }
        
        return current;
    }
};

int main() {
    string s = "abc def ghi abcd";
    vector<string> res = tokenize(s);

    for(string i : res)
        cout << i << endl;


    Trie t;
    t.insert(res[0]);
    t.insert(res[3]);

    for(string i : res) {
        cout << "Search: " << i << " in Trie: " << (t.search(i) ? "Found" : "Not Found") << endl; 
    }
    return 0;
}