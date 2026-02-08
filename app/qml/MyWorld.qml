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
                        }
                    }
                ]
                
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    color: "#555"
                    border.color: "white"
                    border.width: 1
                }
            }

            Body {
                id: box
                type: Body.Dynamic
                position: Qt.vector2d(0, -200)
                transformOrigin: Item.Center
                
                fixtures: [
                    Fixture {
                        density: 1.0
                        shape: BoxShape {
                            id: boxShape
                            width: 50
                            height: 50
                        }
                    }
                ]
                
                Image {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    source: "box.svg"
                    sourceSize: Qt.size(width, height)
                }
                
                MouseArea {
                    anchors.fill: parent
                    drag.target: parent
                    
                    onPressed: {
                        console.log("Box pressed at:", mouse.x, mouse.y)
                        box.type = Body.Kinematic
                    }
                    
                    onReleased: {
                        console.log("Box released, switching back to dynamic")
                        box.type = Body.Dynamic
                    }
                }
            }
        }
    }
}
