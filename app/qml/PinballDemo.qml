import QtQuick
import Box2D 1.0

Rectangle {
    color: "#0D0D1A"
    focus: true

    property bool leftActive: false
    property bool rightActive: false

    Keys.onPressed: function(event) {
        if (event.key === Qt.Key_A || event.key === Qt.Key_Left)
            leftActive = true
        if (event.key === Qt.Key_D || event.key === Qt.Key_Right)
            rightActive = true
        if (event.key === Qt.Key_Space) {
            ball.position = Qt.vector2d(50, 150)
            ball.velocity = Qt.vector2d(0, -2000)
        }
    }
    Keys.onReleased: function(event) {
        if (event.key === Qt.Key_A || event.key === Qt.Key_Left)
            leftActive = false
        if (event.key === Qt.Key_D || event.key === Qt.Key_Right)
            rightActive = false
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

            Flipper {
                side: 1
                pivotPosition: Qt.vector2d(-100, -200)
                motorSpeed: leftActive ? 1000 : -15
            }

            Flipper {
                side: -1
                pivotPosition: Qt.vector2d(100, -200)
                motorSpeed: rightActive ? -1000 : 15
            }

            // left wall
            Wall { positionA: Qt.vector2d(-140, 260); positionB: Qt.vector2d(-140, -120); thickness: 8 }
            // right wall
            Wall { positionA: Qt.vector2d(140, 260);  positionB: Qt.vector2d(140, -120);  thickness: 8 }
            // top wall
            Wall { positionA: Qt.vector2d(-140, 260); positionB: Qt.vector2d(140, 260);   thickness: 8 }
            // left diagonal guide toward left flipper pivot
            Wall { positionA: Qt.vector2d(-140, -120); positionB: Qt.vector2d(-102, -200); thickness: 8 }
            // right diagonal guide toward right flipper pivot
            Wall { positionA: Qt.vector2d(140, -120);  positionB: Qt.vector2d(102, -200);  thickness: 8 }

            Ball {
                id: ball
                position: Qt.vector2d(50, 150)
            }
        }
    }
}
