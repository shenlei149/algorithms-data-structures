add_executable(HashMapTest HashMapTest.cc)
target_link_libraries(HashMapTest guozi_hash GTest::gtest_main)

add_test(NAME HashMapTest COMMAND HashMapTest)

add_executable(HashSetTest HashSetTest.cc)
target_link_libraries(HashSetTest guozi_hash GTest::gtest_main)

add_test(NAME HashSetTest COMMAND HashSetTest)
