cmake_minimum_required(VERSION 3.17)
project(InputShare)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

include_directories(".")

add_subdirectory(libs)

add_executable(InputShare main.cpp inputhook.cpp resources.qrc)
target_link_libraries(InputShare btstack-lib Qt5::Widgets Qt5::Core)