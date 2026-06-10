import QtQuick
import Box2D 1.0

// Rollover lane light: a sensor spot that lights up when the ball rolls
// over it and emits rolled() once per pass. With latch: true the light
// stays on until reset(); otherwise it goes out after litMs.
Body {
    id: rollover

    property Body ball: null
    property real radius: 14
    property bool latch: true
    property int litMs: 700
    property int score: 25
    property bool lit: false

    signal rolled(int score)

    function reset() { lit = false }

    property bool _wasInside: false

    type: Body.Static

    fixtures: [
        Fixture {
            sensor: true
            shape: CircleShape {
                radius: rollover.radius
                fillColor: "transparent"
                strokeColor: "#FFD54F"
                strokeWidth: 2
            }
        }
    ]

    // The light itself; the shape above only draws the outline.
    Rectangle {
        anchors.centerIn: parent
        width: rollover.radius * 1.2
        height: width
        radius: width / 2
        color: rollover.lit ? "#FFD54F" : "#3A3A2A"
    }

    Timer {
        interval: 16
        repeat: true
        running: rollover.ball !== null
        onTriggered: {
            const dx = rollover.ball.position.x - rollover.position.x
            const dy = rollover.ball.position.y - rollover.position.y
            const inside = dx * dx + dy * dy <= rollover.radius * rollover.radius
            if (inside && !rollover._wasInside) {
                rollover.lit = true
                rollover.rolled(rollover.score)
                if (!rollover.latch)
                    unlitTimer.restart()
            }
            rollover._wasInside = inside
        }
    }

    Timer {
        id: unlitTimer
        interval: rollover.litMs
        onTriggered: rollover.lit = false
    }
}
