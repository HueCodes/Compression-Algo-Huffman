# Huffman Compression Algorithm - C++ Implementation

A C++ implementation of the Huffman coding algorithm for lossless data compression.

## About Huffman Coding

Huffman coding is a compression algorithm that uses variable-length codes to represent characters based on their frequency. More frequent characters get shorter codes, resulting in efficient compression.

## Project Structure

```
Huffman-Cpp/
├── include/          # Header files
├── src/              # Source files
├── tests/            # Unit tests
├── build/            # Build directory (generated)
└── CMakeLists.txt    # CMake build configuration
```

## Building the Project

```bash
mkdir -p build
cd build
cmake ..
make
```

## Usage

```bash
./huffman [options] <input_file>
```

## Features

- Huffman tree construction
- Encoding/compression
- Decoding/decompression
- File I/O support

## License

MIT License
