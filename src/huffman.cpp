#include "huffman.h"

#include <queue>
#include <stdexcept>
#include <functional>

namespace huffman {

Node::Node(char ch, int freq) noexcept
    : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

Node::Node(int freq, std::unique_ptr<Node> l, std::unique_ptr<Node> r) noexcept
    : character('\0'), frequency(freq), left(std::move(l)), right(std::move(r)) {}

void HuffmanTree::calculateFrequencies(std::string_view text) {
    frequencies_.clear();
    for (char ch : text) {
        ++frequencies_[ch];
    }
}

void HuffmanTree::buildTree(std::string_view text) {
    if (text.empty()) {
        throw std::invalid_argument("Input text cannot be empty");
    }

    calculateFrequencies(text);

    // Comparator for min-heap based on frequency
    auto cmp = [](const std::unique_ptr<Node>& a, const std::unique_ptr<Node>& b) {
        return a->frequency > b->frequency;
    };

    std::priority_queue<std::unique_ptr<Node>,
                        std::vector<std::unique_ptr<Node>>,
                        decltype(cmp)> pq(cmp);

    // Create leaf nodes for each character
    for (const auto& [ch, freq] : frequencies_) {
        pq.push(std::make_unique<Node>(ch, freq));
    }

    // Handle single unique character case
    if (pq.size() == 1) {
        auto leaf = std::move(const_cast<std::unique_ptr<Node>&>(pq.top()));
        pq.pop();
        root_ = std::make_unique<Node>(leaf->frequency, std::move(leaf), nullptr);
    } else {
        // Build the Huffman tree
        while (pq.size() > 1) {
            auto left = std::move(const_cast<std::unique_ptr<Node>&>(pq.top()));
            pq.pop();
            auto right = std::move(const_cast<std::unique_ptr<Node>&>(pq.top()));
            pq.pop();

            int sumFreq = left->frequency + right->frequency;
            pq.push(std::make_unique<Node>(sumFreq, std::move(left), std::move(right)));
        }

        root_ = std::move(const_cast<std::unique_ptr<Node>&>(pq.top()));
    }

    // Generate Huffman codes
    huffmanCodes_.clear();
    std::string code;
    code.reserve(32);  // Reasonable initial capacity for codes
    generateCodes(root_.get(), code);
}

void HuffmanTree::generateCodes(const Node* node, std::string& code) {
    if (!node) return;

    if (node->isLeaf()) {
        // Single character case: assign "0" if code is empty
        huffmanCodes_[node->character] = code.empty() ? "0" : code;
        return;
    }

    code.push_back('0');
    generateCodes(node->left.get(), code);
    code.pop_back();

    code.push_back('1');
    generateCodes(node->right.get(), code);
    code.pop_back();
}

std::string HuffmanTree::encode(std::string_view text) const {
    if (!root_) {
        throw std::runtime_error("Tree not built. Call buildTree first.");
    }

    // Pre-calculate total encoded size for efficient allocation
    size_t totalSize = 0;
    for (char ch : text) {
        auto it = huffmanCodes_.find(ch);
        if (it == huffmanCodes_.end()) {
            throw std::runtime_error(
                std::string("Character '") + ch + "' not found in Huffman tree");
        }
        totalSize += it->second.size();
    }

    std::string encoded;
    encoded.reserve(totalSize);

    for (char ch : text) {
        encoded += huffmanCodes_.at(ch);
    }

    return encoded;
}

std::string HuffmanTree::decode(std::string_view encodedText) const {
    if (!root_) {
        throw std::runtime_error("Tree not built. Call buildTree first.");
    }

    // Handle single character tree specially
    if (root_->left && root_->left->isLeaf() && !root_->right) {
        std::string decoded;
        decoded.reserve(encodedText.size());
        for (char bit : encodedText) {
            if (bit != '0') {
                throw std::invalid_argument(
                    "Invalid encoded text for single-character tree");
            }
            decoded += root_->left->character;
        }
        return decoded;
    }

    std::string decoded;
    decoded.reserve(encodedText.size() / 4);  // Estimate: avg 4 bits per char

    const Node* current = root_.get();

    for (char bit : encodedText) {
        if (bit == '0') {
            current = current->left.get();
        } else if (bit == '1') {
            current = current->right.get();
        } else {
            throw std::invalid_argument(
                "Invalid encoded text. Must contain only '0' and '1' characters.");
        }

        if (!current) {
            throw std::runtime_error("Invalid encoded text: traversal went beyond tree");
        }

        if (current->isLeaf()) {
            decoded += current->character;
            current = root_.get();
        }
    }

    // Verify we ended at the root (complete decoding)
    if (current != root_.get()) {
        throw std::runtime_error(
            "Invalid encoded text: incomplete sequence (does not end at a character)");
    }

    return decoded;
}

const std::unordered_map<char, int>& HuffmanTree::getFrequencies() const noexcept {
    return frequencies_;
}

const std::unordered_map<char, std::string>& HuffmanTree::getCodes() const noexcept {
    return huffmanCodes_;
}

} // namespace huffman
