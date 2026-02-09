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
            gravity: Qt.vector2d(0, 9.8)
            running: true

            Body {
                id: ground
                type: Body.Static
                position: Qt.vector2d(0, 300)
                transformOrigin: Item.Center
                
                fixtures: [
                    Fixture {
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
                carY: -100
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
