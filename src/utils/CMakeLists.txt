cmake_minimum_required(VERSION 3.5)

include_directories(${CMAKE_SOURCE_DIR}/include/utils)

add_library(
        utils
        point.cpp
        interpolation.cpp
        tinyxml2.cpp
)
