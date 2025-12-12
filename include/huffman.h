#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace huffman {

// Node structure for Huffman tree
struct Node {
    char character;
    int frequency;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    Node(char ch, int freq);
    Node(int freq, std::shared_ptr<Node> l, std::shared_ptr<Node> r);

    bool isLeaf() const;
};

class HuffmanTree {
public:
    HuffmanTree();

    // Build the Huffman tree from input text
    void buildTree(const std::string& text);

    // Encode the input text
    std::string encode(const std::string& text);

    // Decode the encoded text
    std::string decode(const std::string& encodedText);

    // Get the frequency map
    std::unordered_map<char, int> getFrequencies() const;

    // Get the Huffman codes for each character
    std::unordered_map<char, std::string> getCodes() const;

private:
    std::shared_ptr<Node> root;
    std::unordered_map<char, int> frequencies;
    std::unordered_map<char, std::string> huffmanCodes;

    // Calculate character frequencies
    void calculateFrequencies(const std::string& text);

    // Generate Huffman codes
    void generateCodes(std::shared_ptr<Node> node, const std::string& code);
};

} // namespace huffman

#endif // HUFFMAN_H
