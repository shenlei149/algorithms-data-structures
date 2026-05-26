add_subdirectory(unittest)

add_library(guozi_graph INTERFACE)
target_link_libraries(guozi_graph INTERFACE guozi_basic)
target_include_directories(guozi_graph INTERFACE ..)
