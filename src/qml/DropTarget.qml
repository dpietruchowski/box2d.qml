import QtQuick
import Box2D 1.0

// Drop target: a small wall that drops when the ball hits it fast enough.
// Uses Box2D hit events; approachSpeed is the real impact speed in px/s.
// Note: World.hitEventThreshold gates which impacts generate events, so
// keep minHitSpeed at or above it. Once down, the fixture turns into a
// sensor so the ball passes through. Emits knockedDown(); call reset()
// to raise the target again.
Body {
    id: target

    property real targetWidth: 30
    property real targetHeight: 10
    property int score: 50
    property real minHitSpeed: 100       // px/s required to knock the target down
    property bool down: false

    signal knockedDown(int score)

    function reset() { down = false }

    type: Body.Static
    opacity: down ? 0.2 : 1.0

    fixtures: [
        Fixture {
            sensor: target.down
            enableHitEvents: true
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

    onHit: (own, other, point, normal, approachSpeed) => {
        if (!target.down && approachSpeed >= target.minHitSpeed) {
            target.down = true
            target.knockedDown(target.score)
        }
    }
}
