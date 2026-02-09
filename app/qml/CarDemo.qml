import QtQuick
import Box2D 1.0

Rectangle {
    color: "#1a1a1a"
    
    Item {
        id: viewport
        anchors.fill: parent
        clip: true
        
        World {
            id: physics
            x: parent.width / 2
            y: parent.height / 2
            gravity: Qt.vector2d(0, -9.8)
            running: true
            transform: Scale { yScale: -1 }

            Body {
                id: ground
                type: Body.Static
                position: Qt.vector2d(0, -200)
                transformOrigin: Item.Center
                
                fixtures: [
                    Fixture {
                        friction: 0.9
                        shape: BoxShape {
                            id: groundShape
                            width: 600
                            height: 20
                            fillColor: "#555"
                            strokeColor: "white"
                            strokeWidth: 1
                        }
                    }
                ]
            }
            
            Car {
                carX: 0
                carY: -188
                motorSpeed: -20.0
            }
            Car {
                carX: 0
                carY: -188
                motorSpeed: -20.0
            }
            Car {
                carX: 0
                carY: -188
                motorSpeed: -20.0
            }
        }
    }
    
    Text {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        text: "Car Demo - Auto-finding World hierarchy"
        font.pixelSize: 16
        color: "white"
    }
}
