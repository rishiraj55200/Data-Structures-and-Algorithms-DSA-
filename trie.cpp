#include <iostream>
#include <vector>
using namespace std;

// Structure of a Trie Node
class TrieNode {
public:
    // Each node contains 26 pointers (for a-z)
    TrieNode* children[26];

    // Marks whether this node represents the end of a word
    bool isEndOfWord;

    // Constructor
    TrieNode() {
        isEndOfWord = false;

        // Initialize all child pointers to NULL
        for (int i = 0; i < 26; i++) {
            children[i] = NULL;
        }
    }
};

// Trie Class
class Trie {
private:
    TrieNode* root;

public:
    // Constructor
    Trie() {
        root = new TrieNode();
    }

    // ---------------------------------------------------------
    // Function to insert a word into the Trie
    // Time Complexity: O(length of word)
    // ---------------------------------------------------------
    void insert(string word) {

        // Start from the root node
        TrieNode* current = root;

        // Traverse every character
        for (char ch : word) {

            // Convert character into index (0-25)
            int index = ch - 'a';

            // If path doesn't exist, create a new node
            if (current->children[index] == NULL) {
                current->children[index] = new TrieNode();
            }

            // Move to the next node
            current = current->children[index];
        }

        // Mark the last node as end of the word
        current->isEndOfWord = true;
    }

    // ---------------------------------------------------------
    // Function to search a complete word
    // Time Complexity: O(length of word)
    // ---------------------------------------------------------
    bool search(string word) {

        TrieNode* current = root;

        for (char ch : word) {

            int index = ch - 'a';

            // Character not found
            if (current->children[index] == NULL)
                return false;

            current = current->children[index];
        }

        // Return true only if it is the end of a word
        return current->isEndOfWord;
    }

    // ---------------------------------------------------------
    // Function to check whether any word starts
    // with the given prefix
    // Time Complexity: O(length of prefix)
    // ---------------------------------------------------------
    bool startsWith(string prefix) {

        TrieNode* current = root;

        for (char ch : prefix) {

            int index = ch - 'a';

            if (current->children[index] == NULL)
                return false;

            current = current->children[index];
        }

        return true;
    }

    // ---------------------------------------------------------
    // Function to delete the entire Trie recursion hua delete
    // ---------------------------------------------------------
    void deleteTrie(TrieNode* node) {

        if (node == NULL)
            return;

        for (int i = 0; i < 26; i++) {
            deleteTrie(node->children[i]);
        }

        delete node;
    }

    // Destructor
    ~Trie() {
        deleteTrie(root);
    }
};

int main() {

    Trie trie;

    // Insert words
    trie.insert("apple");
    trie.insert("app");
    trie.insert("bat");
    trie.insert("ball");

    // Search words
    cout << "Search apple : " << trie.search("apple") << endl;
    cout << "Search app   : " << trie.search("app") << endl;
    cout << "Search bat   : " << trie.search("bat") << endl;
    cout << "Search cat   : " << trie.search("cat") << endl;

    cout << endl;

    // Prefix Search
    cout << "StartsWith ap : " << trie.startsWith("ap") << endl;
    cout << "StartsWith ba : " << trie.startsWith("ba") << endl;
    cout << "StartsWith ca : " << trie.startsWith("ca") << endl;

    return 0;
}