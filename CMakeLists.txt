cmake_minimum_required(VERSION 3.17)
project(InputShare)

set(CMAKE_CXX_STANDARD 17)

include_directories(".")

add_subdirectory(libs)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()

add_executable(InputShare main.cpp)
target_link_libraries(InputShare btstack-lib)