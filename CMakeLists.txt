cmake_minimum_required(VERSION 3.16)

project(box2d_qml VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick)

qt_standard_project_setup()

include(libs/cmake/AppProject.cmake)
app_load_env(${CMAKE_SOURCE_DIR}/app.env)

if(ANDROID AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(third_party/box2d)

add_subdirectory(lib)
add_subdirectory(src)

if(ANDROID)
    app_configure_android(${APP_TARGET}
        PACKAGE_NAME "${ANDROID_PACKAGE}"
        VERSION_CODE ${VERSION_CODE}
        VERSION_NAME "${VERSION_NAME}"
        KEYSTORE "${CMAKE_SOURCE_DIR}/${ANDROID_KEYSTORE}"
        KEY_ALIAS "${ANDROID_KEY_ALIAS}"
        PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android"
    )
endif()
