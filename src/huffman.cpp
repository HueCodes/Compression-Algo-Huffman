#include "huffman.h"
#include <queue>
#include <stdexcept>

namespace huffman {

// Node implementations
Node::Node(char ch, int freq)
    : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

Node::Node(int freq, std::shared_ptr<Node> l, std::shared_ptr<Node> r)
    : character('\0'), frequency(freq), left(l), right(r) {}

bool Node::isLeaf() const {
    return left == nullptr && right == nullptr;
}

// Comparator for priority queue (min-heap)
struct NodeComparator {
    bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
        return a->frequency > b->frequency;
    }
};

// HuffmanTree implementations
HuffmanTree::HuffmanTree() : root(nullptr) {}

void HuffmanTree::calculateFrequencies(const std::string& text) {
    frequencies.clear();
    for (char ch : text) {
        frequencies[ch]++;
    }
}

void HuffmanTree::buildTree(const std::string& text) {
    if (text.empty()) {
        throw std::invalid_argument("Input text cannot be empty");
    }

    calculateFrequencies(text);

    // Create priority queue (min-heap)
    std::priority_queue<std::shared_ptr<Node>,
                       std::vector<std::shared_ptr<Node>>,
                       NodeComparator> pq;

    // Create leaf nodes for each character
    for (const auto& pair : frequencies) {
        pq.push(std::make_shared<Node>(pair.first, pair.second));
    }

    // Build the Huffman tree
    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();

        int sumFreq = left->frequency + right->frequency;
        auto parent = std::make_shared<Node>(sumFreq, left, right);
        pq.push(parent);
    }

    root = pq.top();

    // Generate Huffman codes
    huffmanCodes.clear();
    generateCodes(root, "");
}

void HuffmanTree::generateCodes(std::shared_ptr<Node> node, const std::string& code) {
    if (!node) return;

    if (node->isLeaf()) {
        huffmanCodes[node->character] = code.empty() ? "0" : code;
        return;
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

std::string HuffmanTree::encode(const std::string& text) {
    if (!root) {
        throw std::runtime_error("Tree not built. Call buildTree first.");
    }

    std::string encoded;
    for (char ch : text) {
        if (huffmanCodes.find(ch) == huffmanCodes.end()) {
            throw std::runtime_error("Character not in Huffman tree");
        }
        encoded += huffmanCodes[ch];
    }
    return encoded;
}

std::string HuffmanTree::decode(const std::string& encodedText) {
    if (!root) {
        throw std::runtime_error("Tree not built. Call buildTree first.");
    }

    std::string decoded;
    auto current = root;

    for (char bit : encodedText) {
        if (bit == '0') {
            current = current->left;
        } else if (bit == '1') {
            current = current->right;
        } else {
            throw std::invalid_argument("Invalid encoded text. Must contain only 0s and 1s.");
        }

        if (!current) {
            throw std::runtime_error("Invalid encoded text");
        }

        if (current->isLeaf()) {
            decoded += current->character;
            current = root;
        }
    }

    return decoded;
}

std::unordered_map<char, int> HuffmanTree::getFrequencies() const {
    return frequencies;
}

std::unordered_map<char, std::string> HuffmanTree::getCodes() const {
    return huffmanCodes;
}

} // namespace huffman
