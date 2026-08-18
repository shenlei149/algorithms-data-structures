add_executable(MWISTest MWISTest.cc)
target_link_libraries(MWISTest guozi_dp GTest::gtest_main)

add_test(NAME MWISTest COMMAND MWISTest)

add_executable(KnapsackTest KnapsackTest.cc)
target_link_libraries(KnapsackTest guozi_dp GTest::gtest_main)

add_test(NAME KnapsackTest COMMAND KnapsackTest)
