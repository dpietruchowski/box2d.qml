qt_add_executable(box2d_app
    src/main.cpp
)

qt_add_qml_module(box2d_app
    URI App
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/PinballDemo.qml
        qml/Flipper.qml
        qml/Pivot.qml
        qml/Wall.qml
        qml/Ball.qml
        qml/Bumper.qml
        qml/Slingshot.qml
        qml/Plunger.qml
        qml/Spinner.qml
        qml/Gate.qml
        qml/DropTarget.qml
        qml/KickoutHole.qml
        qml/Magnet.qml
        qml/Rollover.qml
        qml/CurvedRamp.qml
)

target_link_libraries(box2d_app PRIVATE
    Qt6::Core
    Qt6::Quick
    box2d_qml_lib
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(box2d_app PRIVATE
        QML_LIVE_ENABLED
        QML_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/qml"
    )
endif()

install(TARGETS box2d_app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
