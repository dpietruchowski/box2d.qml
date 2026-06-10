import QtQuick
import Box2D 1.0

// Magnet: attracts the ball while it is inside range. The pull is an
// acceleration (px/s^2) that fades linearly with distance. Toggle with
// the active property. The core is a sensor, so the ball flies through.
Body {
    id: magnet

    property Body ball: null
    property bool active: true
    property real range: 130
    property real strength: 2500     // acceleration in px/s^2 at the core

    type: Body.Static

    fixtures: [
        Fixture {
            sensor: true
            shape: CircleShape {
                radius: 10
                fillColor: magnet.active ? "#8E24AA" : "#4A275A"
                strokeColor: "#CE93D8"
                strokeWidth: 2
            }
        }
    ]

    // Range indicator drawn around the core.
    Rectangle {
        anchors.centerIn: parent
        width: magnet.range * 2
        height: magnet.range * 2
        radius: magnet.range
        color: "transparent"
        border.color: "#8E24AA"
        border.width: 1
        opacity: magnet.active ? 0.5 : 0.15
    }

    Timer {
        interval: 16
        repeat: true
        running: magnet.active && magnet.ball !== null
        onTriggered: {
            const dx = magnet.position.x - magnet.ball.position.x
            const dy = magnet.position.y - magnet.ball.position.y
            const d = Math.sqrt(dx * dx + dy * dy)
            if (d >= magnet.range || d < 1)
                return
            const a = magnet.strength * (1 - d / magnet.range) * (interval / 1000)
            const v = magnet.ball.velocity
            magnet.ball.velocity = Qt.vector2d(v.x + dx / d * a, v.y + dy / d * a)
        }
    }
}
