import QtQuick
import Box2D 1.0

// Kickout hole (saucer): a sensor pocket that captures the ball, holds it
// for holdMs, then kicks it out with ejectVelocity (px/s). Emits
// captured() and ejected(). Re-arms after a short delay so the ball is
// not swallowed again immediately.
Body {
    id: hole

    property Body ball: null
    property real radius: 16
    property int holdMs: 900
    property vector2d ejectVelocity: Qt.vector2d(0, 800)
    property int score: 500
    readonly property bool holding: _holding

    property bool _holding: false

    signal captured(int score)
    signal ejected()

    type: Body.Static

    fixtures: [
        Fixture {
            sensor: true
            shape: CircleShape {
                radius: hole.radius
                fillColor: "#111122"
                strokeColor: hole._holding ? "#FFEE58" : "#5C6BC0"
                strokeWidth: 3
            }
        }
    ]

    Timer {
        interval: 16
        repeat: true
        running: hole.ball !== null && !hole._holding && !rearm.running
        onTriggered: {
            const dx = hole.ball.position.x - hole.position.x
            const dy = hole.ball.position.y - hole.position.y
            if (dx * dx + dy * dy <= hole.radius * hole.radius) {
                hole._holding = true
                hole.captured(hole.score)
                holdTimer.restart()
            }
        }
    }

    // Pin the captured ball to the center of the hole.
    Timer {
        interval: 16
        repeat: true
        running: hole._holding
        onTriggered: {
            hole.ball.position = hole.position
            hole.ball.velocity = Qt.vector2d(0, 0)
        }
    }

    Timer {
        id: holdTimer
        interval: hole.holdMs
        onTriggered: {
            hole._holding = false
            hole.ball.velocity = hole.ejectVelocity
            hole.ejected()
            rearm.restart()
        }
    }

    Timer { id: rearm; interval: 600 }
}
