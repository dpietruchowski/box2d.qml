import QtQuick 2.15
import QtQuick.Window 2.15
import Box2D 2.0

Window {
    id: window
    visible: true
    width: 800
    height: 600
    title: "Box2D QML Example - Falling Boxes"
    
    // Physics world
    World {
        id: physicsWorld
        anchors.fill: parent
        gravity: Qt.point(0, -9.8)
        running: true
        pixelsPerMeter: 32
        
        // Ground
        Body {
            id: ground
            x: 0
            y: parent.height - 50
            width: parent.width
            height: 50
            bodyType: Body.Static
            
            Rectangle {
                anchors.fill: parent
                color: "#2c3e50"
            }
            
            Box {
                width: parent.width
                height: parent.height
            }
        }
        
        // Left wall
        Body {
            x: 0
            y: 0
            width: 20
            height: parent.height
            bodyType: Body.Static
            
            Rectangle {
                anchors.fill: parent
                color: "#34495e"
            }
            
            Box {
                width: parent.width
                height: parent.height
            }
        }
        
        // Right wall
        Body {
            x: parent.width - 20
            y: 0
            width: 20
            height: parent.height
            bodyType: Body.Static
            
            Rectangle {
                anchors.fill: parent
                color: "#34495e"
            }
            
            Box {
                width: parent.width
                height: parent.height
            }
        }
        
        // Dynamic boxes
        Repeater {
            model: 10
            
            Body {
                id: dynamicBox
                x: 200 + (index % 5) * 80
                y: 50 + Math.floor(index / 5) * 80
                width: 60
                height: 60
                bodyType: Body.Dynamic
                
                Rectangle {
                    anchors.fill: parent
                    color: Qt.hsla((index * 36) / 360, 0.8, 0.6, 1.0)
                    border.color: "white"
                    border.width: 2
                    radius: 4
                    
                    Text {
                        anchors.centerIn: parent
                        text: index + 1
                        color: "white"
                        font.bold: true
                        font.pixelSize: 24
                    }
                }
                
                Box {
                    width: parent.width
                    height: parent.height
                    density: 1.0
                    friction: 0.3
                    restitution: 0.5
                }
            }
        }
        
        // Circle
        Body {
            id: circle
            x: 400
            y: 100
            width: 80
            height: 80
            bodyType: Body.Dynamic
            
            Rectangle {
                anchors.fill: parent
                radius: width / 2
                color: "#e74c3c"
                border.color: "white"
                border.width: 2
            }
            
            Circle {
                radius: 40
                density: 1.0
                friction: 0.3
                restitution: 0.8
            }
        }
        
        // Debug draw (optional - can be enabled to see physics shapes)
        DebugDraw {
            id: debugDraw
            anchors.fill: parent
            world: physicsWorld
            visible: false
            drawShapes: true
            drawJoints: true
            z: 100
        }
    }
    
    // Controls
    Rectangle {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 10
        width: 200
        height: 100
        color: "#2c3e50"
        opacity: 0.9
        radius: 5
        
        Column {
            anchors.centerIn: parent
            spacing: 10
            
            Text {
                text: "Space: Pause/Resume"
                color: "white"
                font.pixelSize: 12
            }
            
            Text {
                text: "D: Toggle Debug Draw"
                color: "white"
                font.pixelSize: 12
            }
            
            Text {
                text: "Running: " + (physicsWorld.running ? "Yes" : "No")
                color: physicsWorld.running ? "#2ecc71" : "#e74c3c"
                font.pixelSize: 12
            }
        }
    }
    
    // Keyboard controls
    focus: true
    Keys.onSpacePressed: {
        physicsWorld.running = !physicsWorld.running
    }
    
    Keys.onPressed: {
        if (event.key === Qt.Key_D) {
            debugDraw.visible = !debugDraw.visible
        }
    }
}
