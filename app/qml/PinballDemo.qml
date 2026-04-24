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
        else if (event.key === Qt.Key_L || event.key === Qt.Key_Right)
            rightActive = true
    }
    Keys.onReleased: function(event) {
        if (event.key === Qt.Key_A || event.key === Qt.Key_Left)
            leftActive = false
        else if (event.key === Qt.Key_L || event.key === Qt.Key_Right)
            rightActive = false
    }

    Item {
        id: viewport
        anchors.fill: parent

        World {
            id: physics
            x: parent.width / 2
            y: parent.height / 2
            gravity: Qt.vector2d(0, -600)
            running: true
            transform: Scale { yScale: -1 }

            // Left wall
            Body {
                type: Body.Static
                position: Qt.vector2d(-145, 10)
                fixtures: [
                    Fixture {
                        friction: 0.3
                        restitution: 0.5
                        shape: BoxShape {
                            width: 10
                            height: 570
                            fillColor: "#1A1A33"
                            strokeColor: "#4455BB"
                            strokeWidth: 2
                        }
                    }
                ]
            }

            // Right wall
            Body {
                type: Body.Static
                position: Qt.vector2d(145, 10)
                fixtures: [
                    Fixture {
                        friction: 0.3
                        restitution: 0.5
                        shape: BoxShape {
                            width: 10
                            height: 570
                            fillColor: "#1A1A33"
                            strokeColor: "#4455BB"
                            strokeWidth: 2
                        }
                    }
                ]
            }

            // Top wall
            Body {
                type: Body.Static
                position: Qt.vector2d(0, 280)
                fixtures: [
                    Fixture {
                        friction: 0.3
                        restitution: 0.7
                        shape: BoxShape {
                            width: 280
                            height: 10
                            fillColor: "#1A1A33"
                            strokeColor: "#4455BB"
                            strokeWidth: 2
                        }
                    }
                ]
            }

            // Left drain guide (angled wall directing ball toward left flipper)
            Body {
                type: Body.Static
                position: Qt.vector2d(-97.5, -220)
                angle: Math.atan2(-70, 85)
                fixtures: [
                    Fixture {
                        friction: 0.2
                        restitution: 0.5
                        shape: BoxShape {
                            width: 110
                            height: 8
                            fillColor: "#1A1A33"
                            strokeColor: "#4455BB"
                            strokeWidth: 2
                        }
                    }
                ]
            }

            // Right drain guide (mirror of left)
            Body {
                type: Body.Static
                position: Qt.vector2d(97.5, -220)
                angle: Math.atan2(70, 85)
                fixtures: [
                    Fixture {
                        friction: 0.2
                        restitution: 0.5
                        shape: BoxShape {
                            width: 110
                            height: 8
                            fillColor: "#1A1A33"
                            strokeColor: "#4455BB"
                            strokeWidth: 2
                        }
                    }
                ]
            }

            // Ball
            Body {
                id: ball
                type: Body.Dynamic
                position: Qt.vector2d(0, 150)
                linearDamping: 0.1

                fixtures: [
                    Fixture {
                        density: 1.0
                        friction: 0.3
                        restitution: 0.6
                        shape: CircleShape {
                            radius: 12
                            fillColor: "#C8C8C8"
                            strokeColor: "#FFFFFF"
                            strokeWidth: 2
                        }
                    }
                ]
            }

            // Left flipper pivot — must be declared before Flipper that references it
            Body {
                id: leftPivot
                type: Body.Static
                position: Qt.vector2d(-55, -250)
                fixtures: [
                    Fixture {
                        sensor: true
                        shape: CircleShape {
                            radius: 5
                            fillColor: "#FFEE00"
                        }
                    }
                ]
            }

            // Left flipper: center = pivot + (length/2, 0) = (-55+45, -250) = (-10, -250)
            Flipper {
                side: 1
                pivotBody: leftPivot
                position: Qt.vector2d(-10, -250)
                motorSpeed: leftActive ? 500 : -80
            }

            // Right flipper pivot
            Body {
                id: rightPivot
                type: Body.Static
                position: Qt.vector2d(55, -250)
                fixtures: [
                    Fixture {
                        sensor: true
                        shape: CircleShape {
                            radius: 5
                            fillColor: "#FFEE00"
                        }
                    }
                ]
            }

            // Right flipper: center = pivot + (-length/2, 0) = (55-45, -250) = (10, -250)
            Flipper {
                side: -1
                pivotBody: rightPivot
                position: Qt.vector2d(10, -250)
                motorSpeed: rightActive ? -500 : 80
            }
        }
    }

    Text {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        text: "PINBbALL"
        font.pixelSize: 22
        font.bold: true
        font.letterSpacing: 6
        color: "#6677EE"
    }

    Text {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10
        text: "A / ←  left flipper          L / →  right flipper"
        font.pixelSize: 13
        color: "#445599"
    }
}
