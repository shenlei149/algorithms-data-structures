add_subdirectory(unittest)

add_library(guozi_sort INTERFACE)
target_link_libraries(guozi_sort INTERFACE guozi_basic)
target_include_directories(guozi_sort INTERFACE ..)
