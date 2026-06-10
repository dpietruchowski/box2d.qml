import QtQuick
import Box2D 1.0

// Kickout hole (saucer): a sensor pocket that captures a ball entering it,
// holds it for holdMs, then kicks it out with ejectVelocity (px/s). Emits
// captured() and ejected(). Re-arms after a short delay so the ball is
// not swallowed again immediately. Works with any ball thanks to Box2D
// sensor events; it captures whichever body fell in.
Body {
    id: hole

    property real radius: 16
    property int holdMs: 900
    property vector2d ejectVelocity: Qt.vector2d(0, 800)
    property int score: 500
    readonly property bool holding: _captured !== null

    property Body _captured: null

    signal captured(int score)
    signal ejected()

    type: Body.Static

    fixtures: [
        Fixture {
            sensor: true
            shape: CircleShape {
                radius: hole.radius
                fillColor: "#111122"
                strokeColor: hole.holding ? "#FFEE58" : "#5C6BC0"
                strokeWidth: 3
            }
        }
    ]

    onBeginContact: (own, other) => {
        if (hole._captured || rearm.running || other.body.type !== Body.Dynamic)
            return
        hole._captured = other.body
        hole.captured(hole.score)
        holdTimer.restart()
    }

    // Pin the captured ball to the center of the hole.
    Timer {
        interval: 16
        repeat: true
        running: hole._captured !== null
        onTriggered: {
            hole._captured.position = hole.position
            hole._captured.velocity = Qt.vector2d(0, 0)
        }
    }

    Timer {
        id: holdTimer
        interval: hole.holdMs
        onTriggered: {
            const ball = hole._captured
            hole._captured = null
            ball.velocity = hole.ejectVelocity
            hole.ejected()
            rearm.restart()
        }
    }

    Timer { id: rearm; interval: 600 }
}
