import QtQuick
import Box2D 1.0

// Drop target: a small wall that drops when the ball hits it fast enough.
// Once down, the fixture turns into a sensor so the ball passes through.
// Emits knockedDown(); call reset() to raise the target again.
Body {
    id: target

    property Body ball: null
    property real targetWidth: 30
    property real targetHeight: 10
    property int score: 50
    property real minHitSpeed: 100       // px/s required to knock the target down
    property bool down: false
    property real ballRadius: ball && ball.radius !== undefined ? ball.radius : 12

    signal knockedDown(int score)

    function reset() { down = false }

    type: Body.Static
    opacity: down ? 0.2 : 1.0

    fixtures: [
        Fixture {
            sensor: target.down
            friction: 0.3
            restitution: 0.3
            shape: BoxShape {
                width: target.targetWidth
                height: target.targetHeight
                fillColor: "#FBC02D"
                strokeColor: "#FFF9C4"
                strokeWidth: 2
            }
        }
    ]

    Timer {
        interval: 16
        repeat: true
        running: target.ball !== null && !target.down
        onTriggered: {
            const dx = target.ball.position.x - target.position.x
            const dy = target.ball.position.y - target.position.y
            const reach = Math.max(target.targetWidth, target.targetHeight) / 2
                        + target.ballRadius + 2
            if (dx * dx + dy * dy > reach * reach)
                return
            const v = target.ball.velocity
            if (Math.sqrt(v.x * v.x + v.y * v.y) >= target.minHitSpeed) {
                target.down = true
                target.knockedDown(target.score)
            }
        }
    }
}
