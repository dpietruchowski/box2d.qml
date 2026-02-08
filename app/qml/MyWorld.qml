import QtQuick
import Box2D 1.0

Item {
    World {
        id: physics
        gravity: Qt.vector2d(0, 9.8)
        running: true

        Body {
            id: ground
            type: Body.Static
            position: Qt.vector2d(0, 300)

            Fixture {
                shape: BoxShape {
                    width: 600
                    height: 20
                }
            }
        }

        Body {
            id: box
            type: Body.Dynamic
            position: Qt.vector2d(0, -200)

            Fixture {
                density: 1.0
                shape: BoxShape {
                    width: 50
                    height: 50
                }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#1a1a1a"

        Scene {
            anchors.fill: parent
            world: physics
            cameraZoom: 1.0
        }
    }
}
