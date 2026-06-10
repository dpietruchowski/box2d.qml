import QtQuick
import Box2D 1.0

// Spinner: thin plate rotating freely around its center. A passing ball
// sets it spinning; every completed revolution increments rotations and
// emits spun(). Angular damping slowly brings it to rest.
Body {
    id: plate

    property vector2d pivotPosition: Qt.vector2d(0, 0)
    property real length: 44
    property real thickness: 5
    property int rotations: 0

    signal spun()

    property real _lastAngle: 0
    property real _accum: 0

    type: Body.Dynamic
    angularDamping: 1.2
    position: pivotPosition

    fixtures: [
        Fixture {
            density: 0.4
            friction: 0.2
            restitution: 0.3
            shape: BoxShape {
                width: plate.length
                height: plate.thickness
                fillColor: "#00ACC1"
                strokeColor: "#B2EBF2"
                strokeWidth: 1
            }
        }
    ]

    Pivot {
        id: axis
        position: plate.pivotPosition
    }

    RevoluteJoint {
        bodyA: axis
        bodyB: plate
        localAnchorA: Qt.point(0, 0)
        localAnchorB: Qt.point(0, 0)
    }

    onAngleChanged: {
        let d = angle - _lastAngle
        if (d > Math.PI)
            d -= 2 * Math.PI
        else if (d < -Math.PI)
            d += 2 * Math.PI
        _accum += d
        _lastAngle = angle
        if (Math.abs(_accum) >= 2 * Math.PI) {
            _accum -= Math.sign(_accum) * 2 * Math.PI
            rotations++
            spun()
        }
    }
}
