cmake_minimum_required(VERSION 3.16)

project(box2d_qml VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick)

qt_standard_project_setup()

add_subdirectory(third_party/box2d)

add_subdirectory(lib)
add_subdirectory(app)
