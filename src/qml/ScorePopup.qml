import QtQuick

// Floating "+points" label that drifts up, fades out and destroys itself.
Text {
    id: popup

    color: "#FFE082"
    font.pixelSize: 16
    font.bold: true
    style: Text.Outline
    styleColor: "#000000"

    Component.onCompleted: anim.start()

    ParallelAnimation {
        id: anim
        NumberAnimation { target: popup; property: "y"; to: popup.y - 40; duration: 700; easing.type: Easing.OutCubic }
        NumberAnimation { target: popup; property: "opacity"; to: 0; duration: 700; easing.type: Easing.InQuad }
        onFinished: popup.destroy()
    }
}
