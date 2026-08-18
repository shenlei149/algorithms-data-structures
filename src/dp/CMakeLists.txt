add_subdirectory(unittest)

add_library(guozi_dp MWIS.cc Knapsack.cc)
target_include_directories(guozi_dp PUBLIC ..)
