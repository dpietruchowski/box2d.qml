TEMPLATE = lib
TARGET = Box2DQml
CONFIG += plugin c++17
QT += qml quick

# Determine Qt version
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

# Box2D dependency
# Make sure Box2D is installed or specify path
INCLUDEPATH += /usr/include/box2d
LIBS += -lbox2d

# Sources
SOURCES += \
    src/box2dplugin.cpp \
    src/box2dworld.cpp \
    src/box2dbody.cpp \
    src/box2dfixture.cpp \
    src/box2dbox.cpp \
    src/box2dcircle.cpp \
    src/box2dpolygon.cpp \
    src/box2dcontact.cpp \
    src/box2ddebugdraw.cpp

HEADERS += \
    src/box2dplugin.h \
    src/box2dworld.h \
    src/box2dbody.h \
    src/box2dfixture.h \
    src/box2dbox.h \
    src/box2dcircle.h \
    src/box2dpolygon.h \
    src/box2dcontact.h \
    src/box2ddebugdraw.h

# Install paths
target.path = $$[QT_INSTALL_QML]/Box2D
qmldir.files = src/qmldir
qmldir.path = $$[QT_INSTALL_QML]/Box2D

INSTALLS += target qmldir

# Build artifacts
OBJECTS_DIR = .obj
MOC_DIR = .moc
