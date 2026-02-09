qt_add_executable(box2d_app
    src/main.cpp
)

qt_add_qml_module(box2d_app
    URI App
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/CarDemo.qml
        qml/Car.qml
    RESOURCES
        qml/box.svg
)

target_link_libraries(box2d_app PRIVATE
    Qt6::Core
    Qt6::Quick
    box2d_qml_lib
)

install(TARGETS box2d_app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
