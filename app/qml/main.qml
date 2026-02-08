import QtQuick
import QtQuick.Window

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("Box2D QML - Hello World")

    Rectangle {
        anchors.fill: parent
        color: "#2b2b2b"

        Text {
            anchors.centerIn: parent
            text: "Hello World!"
            font.pixelSize: 48
            color: "white"
        }
    }
}
