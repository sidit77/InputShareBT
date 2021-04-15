cmake_minimum_required(VERSION 3.17)
project(InputShare)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

add_subdirectory(libs)

add_executable(InputShare
        libs/btstack/platform/qt/btstack_run_loop_qt.cpp
        main.cpp
        inputhook.cpp
        resources.qrc
        bluetooth.cpp
        hid.cpp)
target_link_libraries(InputShare btstack-lib Qt5::Widgets Qt5::Core)