import QtQuick
import Box2D 1.0

// Pop bumper: kicks the ball away on impact (restitution > 1 adds energy)
// and emits hit() with a score. Detection is proximity-based, so assign
// the ball property. Pulses visually on every hit.
Body {
    id: bumper

    property Body ball: null
    property real radius: 24
    property real kick: 1.3          // restitution; > 1 means the ball gains speed
    property int score: 100
    property color color: "#E64A19"
    property int cooldownMs: 150
    property real ballRadius: ball && ball.radius !== undefined ? ball.radius : 12

    signal hit(int score)

    type: Body.Static

    fixtures: [
        Fixture {
            friction: 0.1
            restitution: bumper.kick
            shape: CircleShape {
                radius: bumper.radius
                fillColor: bumper.color
                strokeColor: "#FFFFFF"
                strokeWidth: 3
            }
        }
    ]

    Timer {
        interval: 16
        repeat: true
        running: bumper.ball !== null && !cooldown.running
        onTriggered: {
            const dx = bumper.ball.position.x - bumper.position.x
            const dy = bumper.ball.position.y - bumper.position.y
            const reach = bumper.radius + bumper.ballRadius + 2
            if (dx * dx + dy * dy <= reach * reach) {
                bumper.hit(bumper.score)
                pulse.restart()
                cooldown.restart()
            }
        }
    }

    Timer { id: cooldown; interval: bumper.cooldownMs }

    SequentialAnimation {
        id: pulse
        NumberAnimation { target: bumper; property: "scale"; to: 1.25; duration: 50 }
        NumberAnimation { target: bumper; property: "scale"; to: 1.0; duration: 120; easing.type: Easing.OutBounce }
    }
}
