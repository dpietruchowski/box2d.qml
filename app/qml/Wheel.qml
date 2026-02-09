import QtQuick
import Box2D 1.0

Body {
    id: wheel
    property real radius: 10
    type: Body.Dynamic
    showShape: false
    
    Component.onCompleted: {
        console.log("Wheel created at position:", position)
    }
    
    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            console.log("Wheel angle:", wheel.angle, "angularVelocity:", wheel.angularVelocity)
        }
    }
    
    fixtures: [
        Fixture {
            density: 0.5
            friction: 0.9
            shape: CircleShape {
                radius: wheel.radius
            }
        }
    ]
    
    Image {
        source: "wheel.svg"
        width: wheel.radius * 2
        height: wheel.radius * 2
        x: 0
        y: 0
        rotation: wheel.angle * 180 / Math.PI
        transformOrigin: Item.Center
    }
}
