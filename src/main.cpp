#include "huffman.h"
#include <iostream>
#include <fstream>
#include <sstream>

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options] <text>\n"
              << "Options:\n"
              << "  -h, --help     Show this help message\n"
              << "  -f <file>      Read input from file\n"
              << "Example:\n"
              << "  " << programName << " \"hello world\"\n"
              << "  " << programName << " -f input.txt\n";
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string input;

    // Parse arguments
    if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
        printUsage(argv[0]);
        return 0;
    } else if (std::string(argv[1]) == "-f") {
        if (argc < 3) {
            std::cerr << "Error: -f option requires a filename\n";
            return 1;
        }
        try {
            input = readFile(argv[2]);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
            return 1;
        }
    } else {
        input = argv[1];
    }

    if (input.empty()) {
        std::cerr << "Error: Input text is empty\n";
        return 1;
    }

    try {
        // Create Huffman tree and encode
        huffman::HuffmanTree tree;
        tree.buildTree(input);
        std::string encoded = tree.encode(input);
        std::string decoded = tree.decode(encoded);

        // Display results
        std::cout << "\n=== Huffman Compression ===\n\n";
        std::cout << "Original text: " << input << '\n';
        std::cout << "Original size: " << input.length() * 8 << " bits\n\n";

        std::cout << "Huffman Codes:\n";
        auto codes = tree.getCodes();
        for (const auto& pair : codes) {
            std::cout << "  '" << pair.first << "' -> " << pair.second << '\n';
        }

        std::cout << "\nEncoded: " << encoded << '\n';
        std::cout << "Encoded size: " << encoded.length() << " bits\n";
        std::cout << "Compression ratio: "
                  << (1.0 - static_cast<double>(encoded.length()) / (input.length() * 8)) * 100
                  << "%\n\n";

        std::cout << "Decoded: " << decoded << '\n';
        std::cout << "Verification: " << (input == decoded ? "SUCCESS" : "FAILED") << "\n\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
