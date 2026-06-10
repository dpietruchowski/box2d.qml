import QtQuick
import Box2D 1.0

// Dynamic body representing a pinball flipper.
// side: 1 = left flipper (pivot at left end, arm extends right)
//      -1 = right flipper (pivot at right end, arm extends left)
// Set pivotPosition to place the flipper; body center is computed automatically.
Body {
    id: flipper

    property real side: 1
    property real flipperLength: 90
    property real flipperHeight: 14
    property vector2d pivotPosition: Qt.vector2d(0, 0)
    property real motorSpeed: 15       // rad/s
    property real maxMotorTorque: 200  // N·m

    property real lowerAngle: side > 0 ? -Math.PI / 3 : -Math.PI / 4
    property real upperAngle: side > 0 ?  Math.PI / 4 :  Math.PI / 3

    type: Body.Dynamic
    angularDamping: 0.5
    position: Qt.vector2d(pivotPosition.x + side * flipperLength / 2,
                          pivotPosition.y)

    Pivot {
        id: pivot
        position: flipper.pivotPosition
    }

    fixtures: [
        Fixture {
            density: 10.0     // kg/m²
            friction: 0.4
            restitution: 0.05
            shape: BoxShape {
                width: flipper.flipperLength
                height: flipper.flipperHeight
                fillColor: flipper.side > 0 ? "#CC3311" : "#1133CC"
                strokeColor: "#FFFFFF"
                strokeWidth: 1
            }
        }
    ]

    RevoluteJoint {
        bodyA: pivot
        bodyB: flipper
        localAnchorA: Qt.point(0, 0)
        localAnchorB: Qt.point(-flipper.side * flipper.flipperLength / 2, 0)
        enableLimit: true
        lowerAngle: flipper.lowerAngle
        upperAngle: flipper.upperAngle
        enableMotor: true
        motorSpeed: flipper.motorSpeed
        maxMotorTorque: flipper.maxMotorTorque
    }
}
