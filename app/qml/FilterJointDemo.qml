import QtQuick
import Box2D 1.0

Rectangle {
    id: root
    width: 800
    height: 600
    color: "#1a1a2e"

    World {
        id: world
        gravity: Qt.point(0, 10)
        timeStep: 1.0 / 60.0
        running: true

        Body {
            id: ground
            type: Body.StaticBody
            position: Qt.point(400, 550)

            Fixture {
                BoxShape {
                    width: 800
                    height: 20
                }
            }

            Rectangle {
                anchors.centerIn: parent
                width: 800
                height: 20
                color: "#16213e"
            }
        }

        Body {
            id: redBox
            type: Body.DynamicBody
            position: Qt.point(300, 100)

            Fixture {
                BoxShape {
                    width: 60
                    height: 60
                }
                density: 1.0
                friction: 0.3
            }

            Rectangle {
                anchors.centerIn: parent
                width: 60
                height: 60
                color: "#e63946"
                border.color: "#ffffff"
                border.width: 2

                Text {
                    anchors.centerIn: parent
                    text: "RED"
                    color: "white"
                    font.bold: true
                }
            }
        }

        Body {
            id: blueBox
            type: Body.DynamicBody
            position: Qt.point(500, 100)

            Fixture {
                BoxShape {
                    width: 60
                    height: 60
                }
                density: 1.0
                friction: 0.3
            }

            Rectangle {
                anchors.centerIn: parent
                width: 60
                height: 60
                color: "#457b9d"
                border.color: "#ffffff"
                border.width: 2

                Text {
                    anchors.centerIn: parent
                    text: "BLUE"
                    color: "white"
                    font.bold: true
                }
            }
        }

        Body {
            id: greenBox
            type: Body.DynamicBody
            position: Qt.point(400, 200)

            Fixture {
                BoxShape {
                    width: 60
                    height: 60
                }
                density: 1.0
                friction: 0.3
            }

            Rectangle {
                anchors.centerIn: parent
                width: 60
                height: 60
                color: "#06a77d"
                border.color: "#ffffff"
                border.width: 2

                Text {
                    anchors.centerIn: parent
                    text: "GREEN"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 11
                }
            }
        }

        // FilterJoint disables collision between red and blue boxes
        FilterJoint {
            id: filterJoint
            bodyA: redBox
            bodyB: blueBox
        }
    }

    Column {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        spacing: 5

        Text {
            text: "FilterJoint Demo"
            font.pixelSize: 24
            font.bold: true
            color: "#e94560"
        }

        Text {
            text: "Red and Blue boxes DON'T collide (FilterJoint)"
            font.pixelSize: 14
            color: "#e63946"
        }

        Text {
            text: "Green box collides with both"
            font.pixelSize: 14
            color: "#06a77d"
        }

        Text {
            text: "Drop boxes to see the effect"
            font.pixelSize: 12
            color: "#a0a0a0"
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            // Reset positions
            redBox.position = Qt.point(300, 100)
            redBox.linearVelocity = Qt.point(0, 0)
            redBox.angularVelocity = 0
            
            blueBox.position = Qt.point(500, 100)
            blueBox.linearVelocity = Qt.point(0, 0)
            blueBox.angularVelocity = 0
            
            greenBox.position = Qt.point(400, 200)
            greenBox.linearVelocity = Qt.point(0, 0)
            greenBox.angularVelocity = 0
        }
    }

    Text {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        text: "Click to reset"
        font.pixelSize: 12
        color: "#a0a0a0"
    }
}
