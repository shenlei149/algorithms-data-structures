add_executable(BinarySearchTreeTest BinarySearchTreeTest.cc)
target_link_libraries(BinarySearchTreeTest guozi_tree GTest::gtest_main)

add_test(NAME BinarySearchTreeTest COMMAND BinarySearchTreeTest)

add_executable(HuffmanTreeTest HuffmanTreeTest.cc)
target_link_libraries(HuffmanTreeTest guozi_tree GTest::gtest_main)
add_test(NAME HuffmanTreeTest COMMAND HuffmanTreeTest)

add_executable(OptBSTTest OptBSTTest.cc)
target_link_libraries(OptBSTTest guozi_tree GTest::gtest_main)

add_test(NAME OptBSTTest COMMAND OptBSTTest)
