cmake_minimum_required(VERSION 3.7)

project(ComfortClassesLib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_library(ComfortClasses STATIC CCDecimal.cpp CCString.cpp)