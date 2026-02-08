import QtQuick
import QtQuick.Window

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("Box2D QML")

    Rectangle {
        anchors.fill: parent
        color: "#2b2b2b"

        MyWorld {
            anchors.fill: parent
        }
    }
}
