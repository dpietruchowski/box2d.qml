import QtQuick
import Box2D 1.0

// Playable pinball table (no scoring) built from the element library.
//
// Keys:
//   A / Left  - left flipper
//   D / Right - right flipper
//   Space     - hold to pull the plunger, release to launch
//   R         - reset drop targets
//   M         - toggle the magnet
Rectangle {
    id: root
    color: "#0D0D1A"
    focus: true

    property bool leftActive: false
    property bool rightActive: false

    Keys.onPressed: function(event) {
        if (event.key === Qt.Key_A || event.key === Qt.Key_Left)
            leftActive = true
        if (event.key === Qt.Key_D || event.key === Qt.Key_Right)
            rightActive = true
        if (event.isAutoRepeat)
            return
        if (event.key === Qt.Key_Space)
            plunger.pull()
        if (event.key === Qt.Key_R)
            root.resetTargets()
        if (event.key === Qt.Key_M)
            magnet.active = !magnet.active
    }
    Keys.onReleased: function(event) {
        if (event.key === Qt.Key_A || event.key === Qt.Key_Left)
            leftActive = false
        if (event.key === Qt.Key_D || event.key === Qt.Key_Right)
            rightActive = false
        if (event.isAutoRepeat)
            return
        if (event.key === Qt.Key_Space)
            plunger.release()
    }

    function resetTargets() {
        target1.reset(); target2.reset(); target3.reset()
    }

    function ballToPlunger() {
        ball.position = Qt.vector2d(240, -195)
        ball.velocity = Qt.vector2d(0, 0)
    }

    World {
        id: physics
        x: parent.width / 2
        y: parent.height / 2
        pixelsPerMeter: 100
        gravity: Qt.vector2d(0, -9.8)
        running: true
        transform: Scale { yScale: -1 }

        // Outer frame
        Wall { positionA: Qt.vector2d(-260, -280); positionB: Qt.vector2d(-260, 280); thickness: 8 }
        Wall { positionA: Qt.vector2d( 260, -280); positionB: Qt.vector2d( 260, 280); thickness: 8 }
        Wall { positionA: Qt.vector2d(-260,  280); positionB: Qt.vector2d( 260, 280); thickness: 8 }
        Wall { positionA: Qt.vector2d(-260, -280); positionB: Qt.vector2d( 260, -280); thickness: 8 }

        // Plunger lane (right side); the inner wall ends at y = 120 where
        // the one-way gate lets the ball out into the playfield.
        Wall { positionA: Qt.vector2d(220, -280); positionB: Qt.vector2d(220, 120); thickness: 8 }

        Gate {
            hingePosition: Qt.vector2d(220, 130)
            length: 40
        }

        Plunger {
            id: plunger
            restPosition: Qt.vector2d(240, -220)
            travel: 40
            pixelsPerMeter: physics.pixelsPerMeter
        }

        // Curved corners guiding the launched ball across the top
        CurvedRamp {
            center: Qt.vector2d(160, 140)
            radius: 100
            startAngle: 0
            endAngle: Math.PI / 2
            segments: 8
        }
        CurvedRamp {
            center: Qt.vector2d(-160, 140)
            radius: 100
            startAngle: Math.PI / 2
            endAngle: Math.PI
            segments: 8
        }

        // Rollover lights in the top lanes
        Rollover { ball: ball; latch: false; position: Qt.vector2d(-60, 240) }
        Rollover { ball: ball; latch: false; position: Qt.vector2d( 60, 240) }

        // Pop bumpers
        Bumper { ball: ball; position: Qt.vector2d(-90, 160) }
        Bumper { ball: ball; position: Qt.vector2d( 10, 190) }
        Bumper { ball: ball; position: Qt.vector2d(110, 160) }

        // Spinner in the open field
        Spinner { pivotPosition: Qt.vector2d(-160, 60) }

        // Magnet adding chaos on the right
        Magnet {
            id: magnet
            ball: ball
            position: Qt.vector2d(100, 30)
            range: 90
            strength: 1500
        }

        // Drop target bank (auto-resets when all are down)
        DropTarget { id: target1; ball: ball; position: Qt.vector2d(-120, -20)
                     onKnockedDown: bankReset.check() }
        DropTarget { id: target2; ball: ball; position: Qt.vector2d( -80, -20)
                     onKnockedDown: bankReset.check() }
        DropTarget { id: target3; ball: ball; position: Qt.vector2d( -40, -20)
                     onKnockedDown: bankReset.check() }

        // Kickout hole throwing the ball back into play
        KickoutHole {
            ball: ball
            position: Qt.vector2d(-200, -60)
            ejectVelocity: Qt.vector2d(250, 350)
        }

        // Slingshots above the flippers
        Slingshot { ball: ball; side: 1;  position: Qt.vector2d(-150, -140) }
        Slingshot { ball: ball; side: -1; position: Qt.vector2d( 150, -140) }

        // Guides toward the flippers
        Wall { positionA: Qt.vector2d(-260, -120); positionB: Qt.vector2d(-102, -200); thickness: 8 }
        Wall { positionA: Qt.vector2d( 220, -120); positionB: Qt.vector2d( 102, -200); thickness: 8 }

        Flipper {
            side: 1
            pivotPosition: Qt.vector2d(-100, -200)
            motorSpeed: leftActive ? 1000 : -15
        }
        Flipper {
            side: -1
            pivotPosition: Qt.vector2d(100, -200)
            motorSpeed: rightActive ? -1000 : 15
        }

        Ball {
            id: ball
            position: Qt.vector2d(240, -195)
        }
    }

    Timer {
        id: bankReset
        interval: 1500
        onTriggered: root.resetTargets()
        function check() {
            if (target1.down && target2.down && target3.down)
                restart()
        }
    }

    // Drain watcher: a ball lost between the flippers goes back on the plunger.
    Timer {
        interval: 100
        repeat: true
        running: true
        onTriggered: {
            if (ball.position.y < -252 && ball.position.x < 200)
                root.ballToPlunger()
        }
    }

    Text {
        x: 12; y: 8
        color: "#888899"
        font.pixelSize: 12
        text: "A/D or arrows: flippers   Space: plunger   R: reset targets   M: magnet "
              + (magnet.active ? "ON" : "OFF")
    }
}
