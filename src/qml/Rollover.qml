import QtQuick
import Box2D 1.0

// Rollover lane light: a sensor spot that lights up when a ball rolls
// over it and emits rolled() once per pass (Box2D sensor events fire
// once per entry). With latch: true the light stays on until reset();
// otherwise it goes out after litMs.
Body {
    id: rollover

    property real radius: 14
    property bool latch: true
    property int litMs: 700
    property int score: 25
    property bool lit: false

    signal rolled(int score)

    function reset() { lit = false }

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

    onBeginContact: {
        rollover.lit = true
        rollover.rolled(rollover.score)
        if (!rollover.latch)
            unlitTimer.restart()
    }

    Timer {
        id: unlitTimer
        interval: rollover.litMs
        onTriggered: rollover.lit = false
    }
}
