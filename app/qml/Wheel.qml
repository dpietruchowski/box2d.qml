import QtQuick
import Box2D 1.0

Body {
    id: wheel
    property real radius: 10
    property real scale: 1.0
    
    // WheelJoint properties
    property Body chassis: null
    property point localAnchorChassis: Qt.point(0, 0)
    property bool enableMotor: false
    property real motorSpeed: 0.0
    property real maxMotorTorque: 0.0
    property real hertz: 2.0
    property real dampingRatio: 0.7
    property bool enableLimit: true
    property real lowerTranslation: -0.25 * scale
    property real upperTranslation: 0.25 * scale
    
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
    
    WheelJoint {
        bodyA: wheel.chassis
        bodyB: wheel
        localAnchorA: wheel.localAnchorChassis
        localAnchorB: Qt.point(0, 0)
        localAxisA: Qt.point(0, 1)
        enableMotor: wheel.enableMotor
        maxMotorTorque: wheel.maxMotorTorque
        motorSpeed: wheel.motorSpeed
        hertz: wheel.hertz
        dampingRatio: wheel.dampingRatio
        enableLimit: wheel.enableLimit
        lowerTranslation: wheel.lowerTranslation
        upperTranslation: wheel.upperTranslation
    }
}
