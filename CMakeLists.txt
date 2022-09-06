cmake_minimum_required(VERSION 3.9)

project(Assignment)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin ) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -pedantic")
set(CMAKE_BUILD_TYPE Release)


if  (WIN32)
	set(TOOLKITS_FOLDER ${CMAKE_SOURCE_DIR}/toolkits CACHE PATH "Path to Toolkits folder")
	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
endif(WIN32)


add_subdirectory(src/utils)
add_subdirectory(src/generator)
add_subdirectory(src/engine)