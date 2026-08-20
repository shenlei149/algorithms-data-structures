add_subdirectory(unittest)

add_library(guozi_tree OptBST.cc)
target_include_directories(guozi_tree PUBLIC ..)
target_include_directories(guozi_tree PUBLIC ${CMAKE_SOURCE_DIR}/include)
