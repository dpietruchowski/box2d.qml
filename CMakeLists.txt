cmake_minimum_required(VERSION 3.16)

project(Box2DQml VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find required Qt packages
find_package(Qt6 COMPONENTS Core Qml Quick QUIET)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Qml Quick)
endif()

# Box2D library
find_package(box2d QUIET)
if(NOT box2d_FOUND)
    message(STATUS "Box2D not found, checking for bundled version")
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/box2d/CMakeLists.txt")
        message(STATUS "Downloading Box2D...")
        include(FetchContent)
        FetchContent_Declare(
            box2d
            GIT_REPOSITORY https://github.com/erincatto/box2d.git
            GIT_TAG v2.4.1
        )
        FetchContent_MakeAvailable(box2d)
    else()
        add_subdirectory(box2d)
    endif()
endif()

# Source files
set(SOURCES
    src/box2dplugin.cpp
    src/box2dworld.cpp
    src/box2dbody.cpp
    src/box2dfixture.cpp
    src/box2dbox.cpp
    src/box2dcircle.cpp
    src/box2dpolygon.cpp
    src/box2dcontact.cpp
    src/box2ddebugdraw.cpp
)

set(HEADERS
    src/box2dplugin.h
    src/box2dworld.h
    src/box2dbody.h
    src/box2dfixture.h
    src/box2dbox.h
    src/box2dcircle.h
    src/box2dpolygon.h
    src/box2dcontact.h
    src/box2ddebugdraw.h
)

# Create the QML plugin
if(Qt6_FOUND)
    qt_add_qml_module(Box2DQml
        URI Box2D
        VERSION 2.0
        SOURCES ${SOURCES} ${HEADERS}
        OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Box2D
    )
    
    target_link_libraries(Box2DQml PRIVATE
        Qt6::Core
        Qt6::Qml
        Qt6::Quick
        box2d
    )
else()
    add_library(Box2DQml SHARED ${SOURCES} ${HEADERS})
    
    target_link_libraries(Box2DQml PRIVATE
        Qt5::Core
        Qt5::Qml
        Qt5::Quick
        box2d
    )
endif()

target_include_directories(Box2DQml PRIVATE src)

# Install targets
install(TARGETS Box2DQml
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Examples
option(BUILD_EXAMPLES "Build example applications" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
