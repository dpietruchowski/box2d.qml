import QtQuick
import Box2D 1.0

// Magnet: attracts dynamic bodies while they are inside range. The pull
// is an acceleration (px/s^2) that fades linearly with distance. Toggle
// with the active property. The range circle is a sensor, so bodies are
// tracked via Box2D sensor events and fly through freely — any number
// of balls at once.
Body {
    id: magnet

    property bool active: true
    property real range: 130
    property real strength: 2500     // acceleration in px/s^2 at the core

    property var _attracted: []

    type: Body.Static

    fixtures: [
        Fixture {
            sensor: true
            shape: CircleShape {
                radius: magnet.range
                fillColor: "transparent"
                strokeColor: "transparent"
            }
        }
    ]

    // Range indicator and core visual.
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
    Rectangle {
        anchors.centerIn: parent
        width: 20
        height: 20
        radius: 10
        color: magnet.active ? "#8E24AA" : "#4A275A"
        border.color: "#CE93D8"
        border.width: 2
    }

    onBeginContact: (own, other) => {
        if (other.body.type === Body.Dynamic && magnet._attracted.indexOf(other.body) < 0) {
            magnet._attracted.push(other.body)
            magnet._attractedChanged()
        }
    }
    onEndContact: (own, other) => {
        const i = magnet._attracted.indexOf(other.body)
        if (i >= 0) {
            magnet._attracted.splice(i, 1)
            magnet._attractedChanged()
        }
    }

    Timer {
        interval: 16
        repeat: true
        running: magnet.active && magnet._attracted.length > 0
        onTriggered: {
            for (const body of magnet._attracted) {
                if (!body)
                    continue
                const dx = magnet.position.x - body.position.x
                const dy = magnet.position.y - body.position.y
                const d = Math.sqrt(dx * dx + dy * dy)
                if (d >= magnet.range || d < 1)
                    continue
                const a = magnet.strength * (1 - d / magnet.range) * (interval / 1000)
                const v = body.velocity
                body.velocity = Qt.vector2d(v.x + dx / d * a, v.y + dy / d * a)
            }
        }
    }
}
