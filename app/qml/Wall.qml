import QtQuick
import Box2D 1.0

Body {
    id: wall

    property vector2d positionA: Qt.vector2d(0, 0)
    property vector2d positionB: Qt.vector2d(100, 0)
    property real thickness: 8

    readonly property real dx: positionB.x - positionA.x
    readonly property real dy: positionB.y - positionA.y
    readonly property real segmentLength: Math.sqrt(dx * dx + dy * dy)

    type: Body.Static
    position: Qt.vector2d((positionA.x + positionB.x) / 2,
                           (positionA.y + positionB.y) / 2)
    angle: Math.atan2(dy, dx)

    fixtures: [
        Fixture {
            friction: 0.5
            restitution: 0.3
            shape: PolygonShape {
                vertices: [
                    Qt.point(-wall.segmentLength / 2, -wall.thickness / 2),
                    Qt.point( wall.segmentLength / 2, -wall.thickness / 2),
                    Qt.point( wall.segmentLength / 2,  wall.thickness / 2),
                    Qt.point(-wall.segmentLength / 2,  wall.thickness / 2)
                ]
                fillColor: "#334455"
                strokeColor: "#556677"
                strokeWidth: 1
            }
        }
    ]
}
