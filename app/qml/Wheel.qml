import QtQuick
import Box2D 1.0

Body {
    id: wheel
    property real radius: 10
    property real scale: 1.0
    type: Body.Dynamic
    showShape: false
    
    fixtures: [
        Fixture {
            density: 2.0 / wheel.scale
            friction: 1.5
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
    }
}
