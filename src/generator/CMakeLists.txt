cmake_minimum_required(VERSION 3.5)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/generator)

add_executable(
        generator
        main.cpp
        primitive.cpp
        patch.cpp
        sphere.cpp
        cone.cpp
        plane.cpp
        box.cpp
)

target_link_libraries(
        generator
        utils
)
if (WIN32)
    set_property(TARGET generator PROPERTY VS_SOLUTION_DEPLOY ON) #screw this, isto agr vai ser for�ado pq tou farto dos erros
    set_property(TARGET generator PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
endif (WIN32)
