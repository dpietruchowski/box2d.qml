import QtQuick
import Box2D 1.0

// Slingshot: triangular kicker usually placed above the flippers.
// Its long edge has high restitution, so the ball is thrown back hard.
// side: 1 = left slingshot (kicking edge faces right), -1 = right one.
Body {
    id: sling

    property Body ball: null
    property real side: 1
    property real baseWidth: 36
    property real legHeight: 70
    property real kick: 1.4          // restitution of the kicking edge
    property int score: 10
    property real ballRadius: ball && ball.radius !== undefined ? ball.radius : 12

    signal fired(int score)

    type: Body.Static

    fixtures: [
        Fixture {
            friction: 0.1
            restitution: sling.kick
            shape: PolygonShape {
                vertices: [
                    Qt.point(-sling.side * sling.baseWidth / 2, -sling.legHeight / 2),
                    Qt.point( sling.side * sling.baseWidth / 2, -sling.legHeight / 2),
                    Qt.point(-sling.side * sling.baseWidth / 2,  sling.legHeight / 2)
                ]
                fillColor: "#AD1457"
                strokeColor: "#FF80AB"
                strokeWidth: 2
            }
        }
    ]

    Timer {
        interval: 16
        repeat: true
        running: sling.ball !== null && !cooldown.running
        onTriggered: {
            const dx = sling.ball.position.x - sling.position.x
            const dy = sling.ball.position.y - sling.position.y
            const reach = Math.max(sling.baseWidth, sling.legHeight) / 2 + sling.ballRadius + 2
            if (dx * dx + dy * dy <= reach * reach) {
                sling.fired(sling.score)
                flash.restart()
                cooldown.restart()
            }
        }
    }

    Timer { id: cooldown; interval: 150 }

    SequentialAnimation {
        id: flash
        NumberAnimation { target: sling; property: "opacity"; to: 0.4; duration: 40 }
        NumberAnimation { target: sling; property: "opacity"; to: 1.0; duration: 120 }
    }
}
