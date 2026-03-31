add_executable(GraphTest GraphTest.cc)
target_link_libraries(GraphTest guozi_graph GTest::gtest_main)

add_test(NAME GraphTest COMMAND GraphTest)
