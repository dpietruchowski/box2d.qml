import QtQuick
import QtQuick.Window

Window {
    width: 420
    height: 840
    visible: true
    color: "#07070F"
    title: qsTr("Box2D QML - Pinball")

    Component.onCompleted: {
        if (Qt.platform.os === "android")
            showFullScreen()
    }

    Loader {
        objectName: "mainLoader"
        anchors.fill: parent
        focus: true
        source: "PinballGame.qml"
    }
}
