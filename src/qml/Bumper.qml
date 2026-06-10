import QtQuick
import Box2D 1.0

// Pop bumper: kicks the ball away on impact (restitution > 1 adds energy)
// and emits hit() with a score. Detection uses Box2D contact events, so
// it works with any number of balls. Pulses visually on every hit.
Body {
    id: bumper

    property real radius: 24
    property real kick: 1.3          // restitution; > 1 means the ball gains speed
    property int score: 100
    property color color: "#E64A19"
    property int cooldownMs: 150

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

    onBeginContact: {
        if (cooldown.running)
            return
        bumper.hit(bumper.score)
        pulse.restart()
        cooldown.restart()
    }

    Timer { id: cooldown; interval: bumper.cooldownMs }

    SequentialAnimation {
        id: pulse
        NumberAnimation { target: bumper; property: "scale"; to: 1.25; duration: 50 }
        NumberAnimation { target: bumper; property: "scale"; to: 1.0; duration: 120; easing.type: Easing.OutBounce }
    }
}
