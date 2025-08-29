cmake_minimum_required(VERSION 3.31)

project(guozi_algorithms_data_structures)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-finput-charset=UTF-8)
add_compile_options(-Wall -Wextra -pedantic -Werror)

include_directories(include/)

add_subdirectory(src)
