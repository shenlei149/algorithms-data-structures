add_executable(BigIntegerTest BigIntegerTest.cc)
target_link_libraries(BigIntegerTest guozi_numeric GTest::gtest_main)

add_test(NAME BigIntegerTest COMMAND BigIntegerTest)
