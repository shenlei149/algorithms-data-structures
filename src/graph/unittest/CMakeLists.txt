set(GRAPH_TESTS
	GraphTest
	PathTest
	ConnectedComponentTest
	TopologyTest
	ShortestPathTest
	MinimumSpanningTreeTest
)

foreach(TEST_NAME IN LISTS GRAPH_TESTS)
	add_executable(${TEST_NAME} ${TEST_NAME}.cc)
	target_link_libraries(${TEST_NAME} guozi_graph GTest::gtest_main)
	add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
