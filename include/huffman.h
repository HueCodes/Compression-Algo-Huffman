#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace huffman {

struct Node {
    char character;
    int frequency;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    Node(char ch, int freq) noexcept;
    Node(int freq, std::unique_ptr<Node> l, std::unique_ptr<Node> r) noexcept;

    [[nodiscard]] constexpr bool isLeaf() const noexcept {
        return left == nullptr && right == nullptr;
    }
};

class HuffmanTree {
public:
    HuffmanTree() = default;

    void buildTree(std::string_view text);

    [[nodiscard]] std::string encode(std::string_view text) const;
    [[nodiscard]] std::string decode(std::string_view encodedText) const;

    [[nodiscard]] const std::unordered_map<char, int>& getFrequencies() const noexcept;
    [[nodiscard]] const std::unordered_map<char, std::string>& getCodes() const noexcept;

    [[nodiscard]] bool isBuilt() const noexcept { return root_ != nullptr; }

private:
    std::unique_ptr<Node> root_;
    std::unordered_map<char, int> frequencies_;
    std::unordered_map<char, std::string> huffmanCodes_;

    void calculateFrequencies(std::string_view text);
    void generateCodes(const Node* node, std::string& code);
};

} // namespace huffman

#endif // HUFFMAN_H
