#include "huffman.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options] <text>\n"
              << "Options:\n"
              << "  -h, --help     Show this help message\n"
              << "  -f <file>      Read input from file\n"
              << "Example:\n"
              << "  " << programName << " \"hello world\"\n"
              << "  " << programName << " -f input.txt\n";
}

[[nodiscard]] std::string readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    if (file.bad()) {
        throw std::runtime_error("Error reading file: " + filename);
    }

    return buffer.str();
}

void printCharacter(char ch) {
    if (ch == ' ') {
        std::cout << "' '";
    } else if (ch == '\n') {
        std::cout << "'\\n'";
    } else if (ch == '\t') {
        std::cout << "'\\t'";
    } else if (ch == '\r') {
        std::cout << "'\\r'";
    } else {
        std::cout << "'" << ch << "'";
    }
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    std::string input;

    // Parse arguments
    std::string arg1(argv[1]);
    if (arg1 == "-h" || arg1 == "--help") {
        printUsage(argv[0]);
        return EXIT_SUCCESS;
    }

    if (arg1 == "-f") {
        if (argc < 3) {
            std::cerr << "Error: -f option requires a filename\n";
            return EXIT_FAILURE;
        }
        try {
            input = readFile(argv[2]);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
            return EXIT_FAILURE;
        }
    } else {
        input = arg1;
    }

    if (input.empty()) {
        std::cerr << "Error: Input text is empty\n";
        return EXIT_FAILURE;
    }

    try {
        huffman::HuffmanTree tree;
        tree.buildTree(input);

        const std::string encoded = tree.encode(input);
        const std::string decoded = tree.decode(encoded);

        const size_t originalBits = input.size() * 8;
        const size_t encodedBits = encoded.size();
        const double compressionRatio =
            (1.0 - static_cast<double>(encodedBits) / static_cast<double>(originalBits)) * 100.0;

        std::cout << "\n=== Huffman Compression ===\n\n";

        // Truncate display for large inputs
        constexpr size_t maxDisplayLen = 100;
        if (input.size() <= maxDisplayLen) {
            std::cout << "Original text: " << input << '\n';
        } else {
            std::cout << "Original text: " << input.substr(0, maxDisplayLen)
                      << "... (" << input.size() << " chars total)\n";
        }
        std::cout << "Original size: " << originalBits << " bits (" << input.size() << " bytes)\n\n";

        std::cout << "Huffman Codes:\n";
        const auto& codes = tree.getCodes();
        for (const auto& [ch, code] : codes) {
            std::cout << "  ";
            printCharacter(ch);
            std::cout << " -> " << code << '\n';
        }

        std::cout << '\n';
        if (encoded.size() <= maxDisplayLen) {
            std::cout << "Encoded: " << encoded << '\n';
        } else {
            std::cout << "Encoded: " << encoded.substr(0, maxDisplayLen)
                      << "... (" << encoded.size() << " bits total)\n";
        }
        std::cout << "Encoded size: " << encodedBits << " bits\n";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Compression ratio: " << compressionRatio << "%\n\n";

        std::cout << "Verification: " << (input == decoded ? "SUCCESS" : "FAILED") << "\n\n";

        if (input != decoded) {
            return EXIT_FAILURE;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
