qt_add_library(box2d_qml_lib STATIC
    src/qb2world.h
    src/qb2world.cpp
    src/qb2body.h
    src/qb2body.cpp
    src/qb2fixture.h
    src/qb2fixture.cpp
    src/qb2shape.h
    src/qb2shape.cpp
)

target_include_directories(box2d_qml_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(box2d_qml_lib PUBLIC
    Qt6::Core
    Qt6::Quick
    box2d
)
