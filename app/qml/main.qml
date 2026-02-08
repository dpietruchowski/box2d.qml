import QtQuick
import QtQuick.Window

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("Box2D QML")

    MyWorld {
        anchors.fill: parent
    }
}
