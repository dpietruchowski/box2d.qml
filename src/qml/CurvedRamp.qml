import QtQuick

// Curved guide (orbit/ramp): an arc approximated with short Wall
// segments. Angles are in radians, measured counterclockwise from the
// positive x axis around center. Must be placed inside a World.
Item {
    id: ramp

    property vector2d center: Qt.vector2d(0, 0)
    property real radius: 100
    property real startAngle: 0
    property real endAngle: Math.PI
    property int segments: 12
    property real thickness: 8

    Repeater {
        model: ramp.segments

        Wall {
            required property int index
            readonly property real a0: ramp.startAngle
                + (ramp.endAngle - ramp.startAngle) * index / ramp.segments
            readonly property real a1: ramp.startAngle
                + (ramp.endAngle - ramp.startAngle) * (index + 1) / ramp.segments

            positionA: Qt.vector2d(ramp.center.x + ramp.radius * Math.cos(a0),
                                   ramp.center.y + ramp.radius * Math.sin(a0))
            positionB: Qt.vector2d(ramp.center.x + ramp.radius * Math.cos(a1),
                                   ramp.center.y + ramp.radius * Math.sin(a1))
            thickness: ramp.thickness
        }
    }
}
