# Tests CMakeLists.txt

add_executable(huffman_test test_huffman.cpp)
target_link_libraries(huffman_test PRIVATE huffman_lib)

add_test(NAME HuffmanTest COMMAND huffman_test)
