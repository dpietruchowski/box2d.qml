import QtQuick
import Box2D 1.0

Body {
    id: ball

    property real radius: 12

    type: Body.Dynamic
    bullet: true

    fixtures: [
        Fixture {
            density: 1.0
            friction: 0.3
            restitution: 0.5
            shape: CircleShape {
                radius: ball.radius
                fillColor: "#C0C0C0"
                strokeColor: "#FFFFFF"
                strokeWidth: 1
            }
        }
    ]
}
