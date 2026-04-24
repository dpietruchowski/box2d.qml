import QtQuick
import Box2D 1.0

Rectangle {
    color: "#0D0D1A"
    focus: true

    property bool leftActive: false

    Keys.onPressed: function(event) {
        if (event.key === Qt.Key_A || event.key === Qt.Key_Left)
            leftActive = true
    }
    Keys.onReleased: function(event) {
        if (event.key === Qt.Key_A || event.key === Qt.Key_Left)
            leftActive = false
    }

    Item {
        anchors.fill: parent

        World {
            id: physics
            x: parent.width / 2
            y: parent.height / 2
            pixelsPerMeter: 100
            gravity: Qt.vector2d(0, -9.8)
            running: true
            transform: Scale { yScale: -1 }

            Body {
                id: pivot
                type: Body.Static
                position: Qt.vector2d(-45, 0)
                fixtures: [
                    Fixture {
                        sensor: true
                        shape: CircleShape { radius: 5; fillColor: "#FFEE00" }
                    }
                ]
            }

            Flipper {
                side: 1
                pivotBody: pivot
                position: Qt.vector2d(0, 0)
                motorSpeed: leftActive ? 20 : -3
            }
        }
    }
}
