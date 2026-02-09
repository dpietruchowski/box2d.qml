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

            fixtures: [
                Fixture {
                    shape: BoxShape {
                        width: 800
                        height: 20
                    }
                }
            ]

            Rectangle {
                anchors.centerIn: parent
                width: 800
                height: 20
                color: "#16213e"
            }
        }

        Body {
            id: platform
            type: Body.DynamicBody
            position: Qt.point(400, 300)
            angularDamping: 0.5

            fixtures: [
                Fixture {
                    shape: BoxShape {
                        width: 200
                        height: 20
                    }
                    density: 1.0
                    friction: 0.3
                }
            ]

            Rectangle {
                anchors.centerIn: parent
                width: 200
                height: 20
                color: "#0f3460"
                border.color: "#e94560"
                border.width: 2
            }
        }

        Body {
            id: box
            type: Body.DynamicBody
            position: Qt.point(400, 100)

            fixtures: [
                Fixture {
                    shape: BoxShape {
                        width: 50
                        height: 50
                    }
                    density: 1.0
                    friction: 0.5
                }
            ]

            Rectangle {
                anchors.centerIn: parent
                width: 50
                height: 50
                color: "#e94560"
                border.color: "#ffffff"
                border.width: 2
            }
        }

        MotorJoint {
            id: motorJoint
            bodyA: ground
            bodyB: platform
            maxVelocityForce: 1000.0
            maxVelocityTorque: 1000.0
            linearVelocity: Qt.point(targetX, targetY)
            angularVelocity: targetAngle

            property real targetX: 0
            property real targetY: 0
            property real targetAngle: 0

            SequentialAnimation on targetX {
                running: true
                loops: Animation.Infinite
                NumberAnimation { from: -50; to: 50; duration: 2000; easing.type: Easing.InOutSine }
                NumberAnimation { from: 50; to: -50; duration: 2000; easing.type: Easing.InOutSine }
            }

            SequentialAnimation on targetAngle {
                running: true
                loops: Animation.Infinite
                NumberAnimation { from: -0.5; to: 0.5; duration: 3000; easing.type: Easing.InOutSine }
                NumberAnimation { from: 0.5; to: -0.5; duration: 3000; easing.type: Easing.InOutSine }
            }
        }
    }

    Column {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        spacing: 5

        Text {
            text: "MotorJoint Demo"
            font.pixelSize: 24
            font.bold: true
            color: "#e94560"
        }

        Text {
            text: "Platform follows animated target velocity"
            font.pixelSize: 14
            color: "#ffffff"
        }

        Text {
            text: "Linear Velocity: (" + motorJoint.targetX.toFixed(1) + ", " + motorJoint.targetY.toFixed(1) + ")"
            font.pixelSize: 12
            color: "#a0a0a0"
        }

        Text {
            text: "Angular Velocity: " + motorJoint.targetAngle.toFixed(2) + " rad/s"
            font.pixelSize: 12
            color: "#a0a0a0"
        }
    }
}
