qt_add_library(box2d_qml_lib STATIC
    src/qb2world.h
    src/qb2world.cpp
    src/shapes/qb2shape.h
    src/shapes/qb2shape.cpp
    src/shapes/qb2boxshape.h
    src/shapes/qb2boxshape.cpp
    src/shapes/qb2circleshape.h
    src/shapes/qb2circleshape.cpp
    src/shapes/qb2capsuleshape.h
    src/shapes/qb2capsuleshape.cpp
    src/shapes/qb2segmentshape.h
    src/shapes/qb2segmentshape.cpp
    src/shapes/qb2polygonshape.h
    src/shapes/qb2polygonshape.cpp
    src/qb2fixture.h
    src/qb2fixture.cpp
    src/qb2body.h
    src/qb2body.cpp
    src/joints/qb2joint.h
    src/joints/qb2joint.cpp
    src/joints/qb2revolutejoint.h
    src/joints/qb2revolutejoint.cpp
    src/joints/qb2distancejoint.h
    src/joints/qb2distancejoint.cpp
)

target_include_directories(box2d_qml_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(box2d_qml_lib PUBLIC
    Qt6::Core
    Qt6::Quick
    box2d
)
