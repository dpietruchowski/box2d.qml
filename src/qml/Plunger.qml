import QtQuick
import Box2D 1.0

// Plunger: spring launcher on a PrismaticJoint. Call pull() to draw the
// piston down against the lower limit, release() to fire it upward and
// launch a ball resting on top. The piston re-arms itself automatically.
Body {
    id: piston

    property vector2d restPosition: Qt.vector2d(0, 0)
    property real travel: 60             // pull distance in pixels
    property real plateWidth: 28
    property real plateHeight: 16
    property real pullSpeed: 1.5         // m/s (joint motor units)
    property real launchSpeed: 12        // m/s at full pull
    property real minLaunchSpeed: launchSpeed  // m/s with no pull; lower it to make power depend on pull
    property real pixelsPerMeter: 100
    readonly property bool armed: _armed
    readonly property real pullFraction:
        Math.max(0, Math.min(1, (restPosition.y - position.y) / travel))

    property bool _armed: false
    property real _launchSpeed: launchSpeed

    signal launched()

    function pull()    { _armed = true }
    function release() {
        if (!_armed)
            return
        _launchSpeed = minLaunchSpeed + (launchSpeed - minLaunchSpeed) * pullFraction
        _armed = false
        launched()
    }

    type: Body.Dynamic
    position: restPosition

    fixtures: [
        Fixture {
            density: 2.0
            friction: 0.5
            restitution: 0.05
            shape: BoxShape {
                width: piston.plateWidth
                height: piston.plateHeight
                fillColor: piston._armed ? "#FFB300" : "#FF6F00"
                strokeColor: "#FFE082"
                strokeWidth: 2
            }
        }
    ]

    Body {
        id: frame
        type: Body.Static
        position: piston.restPosition
    }

    PrismaticJoint {
        bodyA: frame
        bodyB: piston
        localAnchorA: Qt.point(0, 0)
        localAnchorB: Qt.point(0, 0)
        localAxisA: Qt.point(0, 1)
        enableLimit: true
        lowerTranslation: -piston.travel / piston.pixelsPerMeter
        upperTranslation: 0.001
        enableMotor: true
        motorSpeed: piston._armed ? -piston.pullSpeed : piston._launchSpeed
        maxMotorForce: piston._armed ? 60 : 400
    }
}
