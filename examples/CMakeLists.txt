cmake_minimum_required(VERSION 3.16)

# Find Qt
find_package(Qt6 COMPONENTS Core Qml Quick QUIET)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Qml Quick)
endif()

# Example 1: Simple falling boxes
add_executable(example_boxes
    boxes/main.cpp
)

if(Qt6_FOUND)
    target_link_libraries(example_boxes PRIVATE
        Qt6::Core
        Qt6::Qml
        Qt6::Quick
    )
else()
    target_link_libraries(example_boxes PRIVATE
        Qt5::Core
        Qt5::Qml
        Qt5::Quick
    )
endif()

# Copy QML files to build directory
file(COPY boxes/main.qml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/boxes/)
