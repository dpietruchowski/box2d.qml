import QtQuick
import Box2D 1.0

// One-way gate: a flap hinged at one end. The joint limit blocks rotation
// below the rest angle, so the ball can push through from one side only;
// a weak motor swings the flap back shut. side: 1 = arm extends right
// from the hinge and opens counterclockwise, -1 = mirrored.
Body {
    id: gate

    property vector2d hingePosition: Qt.vector2d(0, 0)
    property real side: 1
    property real length: 44
    property real maxOpenAngle: 1.6      // radians
    property real closeSpeed: 3          // rad/s of the return motor
    readonly property bool open: Math.abs(angle) > 0.15

    type: Body.Dynamic
    angularDamping: 0.4
    position: Qt.vector2d(hingePosition.x + side * length / 2, hingePosition.y)

    fixtures: [
        Fixture {
            density: 0.3
            friction: 0.1
            restitution: 0.1
            shape: BoxShape {
                width: gate.length
                height: 5
                fillColor: "#43A047"
                strokeColor: "#A5D6A7"
                strokeWidth: 1
            }
        }
    ]

    Pivot {
        id: hinge
        position: gate.hingePosition
    }

    RevoluteJoint {
        bodyA: hinge
        bodyB: gate
        localAnchorA: Qt.point(0, 0)
        localAnchorB: Qt.point(-gate.side * gate.length / 2, 0)
        enableLimit: true
        lowerAngle: gate.side > 0 ? 0 : -gate.maxOpenAngle
        upperAngle: gate.side > 0 ? gate.maxOpenAngle : 0
        enableMotor: true
        motorSpeed: -gate.side * gate.closeSpeed
        maxMotorTorque: 1.0
    }
}
