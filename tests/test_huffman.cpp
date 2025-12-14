#include "huffman.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

// Simple test framework macros
#define TEST(name) void name()
#define ASSERT_TRUE(cond) \
    do { if (!(cond)) { \
        std::cerr << "FAILED: " << #cond << " at " << __FILE__ << ":" << __LINE__ << '\n'; \
        throw std::runtime_error("Test assertion failed"); \
    }} while(0)
#define ASSERT_EQ(a, b) ASSERT_TRUE((a) == (b))
#define ASSERT_THROW(expr, exc_type) \
    do { bool caught = false; \
        try { (void)(expr); } catch (const exc_type&) { caught = true; } \
        if (!caught) { \
            std::cerr << "FAILED: Expected exception " << #exc_type << " at " << __FILE__ << ":" << __LINE__ << '\n'; \
            throw std::runtime_error("Expected exception not thrown"); \
        }} while(0)

#define RUN_TEST(name) \
    do { std::cout << "Running " << #name << "... "; \
        try { name(); std::cout << "PASSED\n"; ++passed; } \
        catch (...) { std::cout << "FAILED\n"; ++failed; }} while(0)

TEST(test_basic_encode_decode) {
    huffman::HuffmanTree tree;
    std::string input = "hello world";
    tree.buildTree(input);

    std::string encoded = tree.encode(input);
    std::string decoded = tree.decode(encoded);

    ASSERT_EQ(decoded, input);
}

TEST(test_single_character) {
    huffman::HuffmanTree tree;
    std::string input = "aaaa";
    tree.buildTree(input);

    std::string encoded = tree.encode(input);
    std::string decoded = tree.decode(encoded);

    ASSERT_EQ(decoded, input);
    ASSERT_EQ(encoded.size(), 4u);  // 4 characters, each encoded as "0"
}

TEST(test_two_characters) {
    huffman::HuffmanTree tree;
    std::string input = "ab";
    tree.buildTree(input);

    std::string encoded = tree.encode(input);
    std::string decoded = tree.decode(encoded);

    ASSERT_EQ(decoded, input);
}

TEST(test_all_unique_characters) {
    huffman::HuffmanTree tree;
    std::string input = "abcdefghij";
    tree.buildTree(input);

    std::string encoded = tree.encode(input);
    std::string decoded = tree.decode(encoded);

    ASSERT_EQ(decoded, input);
}

TEST(test_repeated_pattern) {
    huffman::HuffmanTree tree;
    std::string input = "abababababababab";
    tree.buildTree(input);

    std::string encoded = tree.encode(input);
    std::string decoded = tree.decode(encoded);

    ASSERT_EQ(decoded, input);
}

TEST(test_long_text) {
    huffman::HuffmanTree tree;
    std::string input;
    for (int i = 0; i < 1000; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }
    tree.buildTree(input);

    std::string encoded = tree.encode(input);
    std::string decoded = tree.decode(encoded);

    ASSERT_EQ(decoded, input);
}

TEST(test_special_characters) {
    huffman::HuffmanTree tree;
    std::string input = "Hello!\nWorld?\t123\r\n";
    tree.buildTree(input);

    std::string encoded = tree.encode(input);
    std::string decoded = tree.decode(encoded);

    ASSERT_EQ(decoded, input);
}

TEST(test_compression_ratio) {
    huffman::HuffmanTree tree;
    // Text with skewed frequency distribution should compress well
    std::string input = "aaaaaaaaaaaaaaaaaaaabbbbbccd";
    tree.buildTree(input);

    std::string encoded = tree.encode(input);

    // Encoded should be smaller than original * 8 bits
    ASSERT_TRUE(encoded.size() < input.size() * 8);
}

TEST(test_codes_are_prefix_free) {
    huffman::HuffmanTree tree;
    std::string input = "abcdefghij";
    tree.buildTree(input);

    const auto& codes = tree.getCodes();

    // Verify no code is a prefix of another
    for (const auto& [ch1, code1] : codes) {
        for (const auto& [ch2, code2] : codes) {
            if (ch1 != ch2) {
                ASSERT_TRUE(code1.find(code2) != 0 || code1.size() <= code2.size());
                ASSERT_TRUE(code2.find(code1) != 0 || code2.size() <= code1.size());
            }
        }
    }
}

TEST(test_frequencies) {
    huffman::HuffmanTree tree;
    std::string input = "aaabbc";
    tree.buildTree(input);

    const auto& freqs = tree.getFrequencies();

    ASSERT_EQ(freqs.at('a'), 3);
    ASSERT_EQ(freqs.at('b'), 2);
    ASSERT_EQ(freqs.at('c'), 1);
}

TEST(test_empty_input_throws) {
    huffman::HuffmanTree tree;
    ASSERT_THROW(tree.buildTree(""), std::invalid_argument);
}

TEST(test_encode_before_build_throws) {
    huffman::HuffmanTree tree;
    ASSERT_THROW(tree.encode("test"), std::runtime_error);
}

TEST(test_decode_before_build_throws) {
    huffman::HuffmanTree tree;
    ASSERT_THROW(tree.decode("01"), std::runtime_error);
}

TEST(test_decode_invalid_characters_throws) {
    huffman::HuffmanTree tree;
    tree.buildTree("ab");
    ASSERT_THROW(tree.decode("012"), std::invalid_argument);
}

TEST(test_decode_incomplete_sequence_throws) {
    huffman::HuffmanTree tree;
    tree.buildTree("abc");

    std::string encoded = tree.encode("abc");
    // Truncate the encoded string to create incomplete sequence
    if (encoded.size() > 1) {
        std::string truncated = encoded.substr(0, encoded.size() - 1);
        ASSERT_THROW(tree.decode(truncated), std::runtime_error);
    }
}

TEST(test_is_built) {
    huffman::HuffmanTree tree;
    ASSERT_TRUE(!tree.isBuilt());

    tree.buildTree("test");
    ASSERT_TRUE(tree.isBuilt());
}

TEST(test_rebuild_tree) {
    huffman::HuffmanTree tree;

    tree.buildTree("aaa");
    std::string encoded1 = tree.encode("aaa");
    std::string decoded1 = tree.decode(encoded1);
    ASSERT_EQ(decoded1, "aaa");

    // Rebuild with different text
    tree.buildTree("xyz");
    std::string encoded2 = tree.encode("xyz");
    std::string decoded2 = tree.decode(encoded2);
    ASSERT_EQ(decoded2, "xyz");
}

int main() {
    int passed = 0;
    int failed = 0;

    std::cout << "=== Huffman Unit Tests ===\n\n";

    RUN_TEST(test_basic_encode_decode);
    RUN_TEST(test_single_character);
    RUN_TEST(test_two_characters);
    RUN_TEST(test_all_unique_characters);
    RUN_TEST(test_repeated_pattern);
    RUN_TEST(test_long_text);
    RUN_TEST(test_special_characters);
    RUN_TEST(test_compression_ratio);
    RUN_TEST(test_codes_are_prefix_free);
    RUN_TEST(test_frequencies);
    RUN_TEST(test_empty_input_throws);
    RUN_TEST(test_encode_before_build_throws);
    RUN_TEST(test_decode_before_build_throws);
    RUN_TEST(test_decode_invalid_characters_throws);
    RUN_TEST(test_decode_incomplete_sequence_throws);
    RUN_TEST(test_is_built);
    RUN_TEST(test_rebuild_tree);

    std::cout << "\n=== Results ===\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}
