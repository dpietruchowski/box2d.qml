import QtQuick
import Box2D 1.0
import Pinball 1.0

// Playable portrait pinball for phones (and desktop).
//
// Touch: left half = left flipper, right half = right flipper. While the
// ball sits in the plunger lane, hold anywhere to pull the plunger and
// release to launch; the longer the hold, the stronger the shot.
// Keys: A/Left, D/Right flippers, Space plunger.
//
// Scoring: every hit is multiplied by the current multiplier (x1..x5).
// Light all three top rollovers to raise the multiplier; knock down the
// whole drop target bank for a bonus. Three balls per game.
Rectangle {
    id: root
    color: "#07070F"
    focus: true

    readonly property real tableWidth: 440
    readonly property real tableHeight: 880
    readonly property real hudHeight: 64

    property string phase: "ready"      // ready | playing | over
    property int score: 0
    property int ballNumber: 1
    readonly property int ballsPerGame: 3
    property int multiplier: 1
    readonly property int maxMultiplier: 5

    property bool keyLeft: false
    property bool keyRight: false

    readonly property bool ballInLane: ball.position.x > 182 && ball.position.y < 280
    readonly property bool leftActive: phase === "playing" && (keyLeft || (touch.leftPressed && !ballInLane))
    readonly property bool rightActive: phase === "playing" && (keyRight || (touch.rightPressed && !ballInLane))

    function addPoints(points, worldPos) {
        const gained = points * multiplier
        score += gained
        popupComponent.createObject(popupLayer, {
            text: "+" + gained,
            x: worldPos.x + tableWidth / 2 - 16,
            y: tableHeight / 2 - worldPos.y - 20
        })
    }

    function showMessage(text) {
        message.text = text
        messageAnim.restart()
    }

    function placeBall() {
        ball.position = Qt.vector2d(200, -358)
        ball.velocity = Qt.vector2d(0, 0)
        ball.angularVelocity = 0
    }

    function resetTable() {
        target1.reset(); target2.reset(); target3.reset()
        roll1.reset(); roll2.reset(); roll3.reset()
    }

    function startGame() {
        score = 0
        ballNumber = 1
        multiplier = 1
        resetTable()
        placeBall()
        phase = "playing"
        showMessage("BALL 1")
    }

    function loseBall() {
        if (ballNumber < ballsPerGame) {
            ballNumber++
            multiplier = 1
            placeBall()
            showMessage("BALL " + ballNumber)
            return
        }
        phase = "over"
        if (score > GameSettings.highScore) {
            GameSettings.highScore = score
            newRecord = true
        } else {
            newRecord = false
        }
        placeBall()
    }

    property bool newRecord: false

    function checkRollovers() {
        if (!(roll1.lit && roll2.lit && roll3.lit))
            return
        addPoints(1000, Qt.vector2d(0, 395))
        if (multiplier < maxMultiplier) {
            multiplier++
            showMessage("MULTIPLIER x" + multiplier)
        } else {
            showMessage("MAX MULTIPLIER")
        }
        rolloverReset.restart()
    }

    function checkBank() {
        if (target1.down && target2.down && target3.down) {
            addPoints(2000, Qt.vector2d(0, 40))
            showMessage("BANK BONUS")
            bankReset.restart()
        }
    }

    Keys.onPressed: function(event) {
        if (event.key === Qt.Key_A || event.key === Qt.Key_Left)
            keyLeft = true
        if (event.key === Qt.Key_D || event.key === Qt.Key_Right)
            keyRight = true
        if (event.isAutoRepeat)
            return
        if (event.key === Qt.Key_Space) {
            if (phase !== "playing")
                startGame()
            else
                plunger.pull()
        }
    }
    Keys.onReleased: function(event) {
        if (event.key === Qt.Key_A || event.key === Qt.Key_Left)
            keyLeft = false
        if (event.key === Qt.Key_D || event.key === Qt.Key_Right)
            keyRight = false
        if (event.isAutoRepeat)
            return
        if (event.key === Qt.Key_Space)
            plunger.release()
    }

    // ---- HUD ----
    Item {
        id: hud
        width: parent.width
        height: root.hudHeight

        Column {
            anchors { left: parent.left; leftMargin: 14; verticalCenter: parent.verticalCenter }
            Text { text: "BALL"; color: "#8888AA"; font.pixelSize: 11; font.bold: true }
            Text {
                text: root.ballNumber + "/" + root.ballsPerGame
                color: "#FFFFFF"; font.pixelSize: 20; font.bold: true
            }
        }

        Column {
            anchors.centerIn: parent
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: root.score.toLocaleString(Qt.locale("en_US"), "f", 0)
                color: "#FFE082"; font.pixelSize: 30; font.bold: true
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "x" + root.multiplier
                color: root.multiplier > 1 ? "#FF7043" : "#666688"
                font.pixelSize: 13; font.bold: true
            }
        }

        Column {
            anchors { right: parent.right; rightMargin: 14; verticalCenter: parent.verticalCenter }
            Text {
                anchors.right: parent.right
                text: "HIGH"; color: "#8888AA"; font.pixelSize: 11; font.bold: true
            }
            Text {
                anchors.right: parent.right
                text: GameSettings.highScore.toLocaleString(Qt.locale("en_US"), "f", 0)
                color: "#FFFFFF"; font.pixelSize: 20; font.bold: true
            }
        }
    }

    // ---- Table, scaled to fit below the HUD ----
    Item {
        id: table
        width: root.tableWidth
        height: root.tableHeight
        anchors.horizontalCenter: parent.horizontalCenter
        y: hud.height
        transformOrigin: Item.Top
        scale: Math.min(root.width / root.tableWidth,
                        (root.height - hud.height) / root.tableHeight)

        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#1A1033" }
                GradientStop { position: 1.0; color: "#0D0D1A" }
            }
        }

        World {
            id: physics
            x: root.tableWidth / 2
            y: root.tableHeight / 2
            pixelsPerMeter: 100
            stepsPerTick: 4     // fast flippers would tunnel through the ball at 60 Hz
            gravity: Qt.vector2d(0, -12)
            running: Qt.application.state !== Qt.ApplicationSuspended
                     && Qt.application.state !== Qt.ApplicationHidden
            transform: Scale { yScale: -1 }

            // Outer frame
            Wall { positionA: Qt.vector2d(-220, -440); positionB: Qt.vector2d(-220, 440) }
            Wall { positionA: Qt.vector2d( 220, -440); positionB: Qt.vector2d( 220, 440) }
            Wall { positionA: Qt.vector2d(-220,  440); positionB: Qt.vector2d( 220, 440) }
            Wall { positionA: Qt.vector2d(-220, -440); positionB: Qt.vector2d( 220, -440) }

            // Plunger lane with a one-way gate at the top
            // The closed flap slopes down toward the playfield, so a weak shot
            // that stops on top of it rolls off instead of getting stuck.
            Wall { positionA: Qt.vector2d(180, -440); positionB: Qt.vector2d(180, 280) }
            Gate {
                hingePosition: Qt.vector2d(220, 305)
                side: -1
                length: 40
                restAngle: 0.45
            }

            Plunger {
                id: plunger
                objectName: "plunger"
                restPosition: Qt.vector2d(200, -380)
                travel: 40
                launchSpeed: 18
                minLaunchSpeed: 9
                pixelsPerMeter: physics.pixelsPerMeter
            }

            // Curved top corners
            CurvedRamp {
                center: Qt.vector2d(120, 340); radius: 100
                startAngle: 0; endAngle: Math.PI / 2; segments: 8
            }
            CurvedRamp {
                center: Qt.vector2d(-120, 340); radius: 100
                startAngle: Math.PI / 2; endAngle: Math.PI; segments: 8
            }

            // Top rollovers: light all three to raise the multiplier
            Rollover { id: roll1; position: Qt.vector2d(-60, 395); score: 50
                       onRolled: (s) => { root.addPoints(s, position); root.checkRollovers() } }
            Rollover { id: roll2; position: Qt.vector2d(  0, 395); score: 50
                       onRolled: (s) => { root.addPoints(s, position); root.checkRollovers() } }
            Rollover { id: roll3; position: Qt.vector2d( 60, 395); score: 50
                       onRolled: (s) => { root.addPoints(s, position); root.checkRollovers() } }

            // Pop bumpers
            Bumper { position: Qt.vector2d(-70, 250); radius: 26
                     onHit: (s) => root.addPoints(s, position) }
            Bumper { position: Qt.vector2d( 70, 250); radius: 26; color: "#F4511E"
                     onHit: (s) => root.addPoints(s, position) }
            Bumper { position: Qt.vector2d(  0, 165); radius: 26; color: "#D84315"
                     onHit: (s) => root.addPoints(s, position) }

            Spinner {
                id: spinner
                pivotPosition: Qt.vector2d(-165, 120)
                onSpun: root.addPoints(20, pivotPosition)
            }

            KickoutHole {
                position: Qt.vector2d(140, 110)
                ejectVelocity: Qt.vector2d(-300, 600)
                score: 750
                onCaptured: (s) => root.addPoints(s, position)
            }

            // Drop target bank
            DropTarget { id: target1; position: Qt.vector2d(-40, 40); score: 250
                         onKnockedDown: (s) => { root.addPoints(s, position); root.checkBank() } }
            DropTarget { id: target2; position: Qt.vector2d(  0, 40); score: 250
                         onKnockedDown: (s) => { root.addPoints(s, position); root.checkBank() } }
            DropTarget { id: target3; position: Qt.vector2d( 40, 40); score: 250
                         onKnockedDown: (s) => { root.addPoints(s, position); root.checkBank() } }

            // Slingshots above the flippers
            Slingshot { side: 1;  position: Qt.vector2d(-150, -200); legHeight: 80
                        onFired: (s) => root.addPoints(s, position) }
            Slingshot { side: -1; position: Qt.vector2d( 150, -200); legHeight: 80
                        onFired: (s) => root.addPoints(s, position) }

            // Inlane guides toward the flippers
            Wall { positionA: Qt.vector2d(-220, -240); positionB: Qt.vector2d(-103, -330) }
            Wall { positionA: Qt.vector2d( 180, -240); positionB: Qt.vector2d( 103, -330) }

            Flipper {
                side: 1
                pivotPosition: Qt.vector2d(-95, -335)
                flipperLength: 78
                lowerAngle: -0.5
                upperAngle: 0.5
                motorSpeed: root.leftActive ? 30 : -15
            }
            Flipper {
                side: -1
                pivotPosition: Qt.vector2d(95, -335)
                flipperLength: 78
                lowerAngle: -0.5
                upperAngle: 0.5
                motorSpeed: root.rightActive ? -30 : 15
            }

            Ball {
                id: ball
                objectName: "ball"
                position: Qt.vector2d(200, -358)
            }
        }

        Item {
            id: popupLayer
            anchors.fill: parent
        }

        Text {
            id: message
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -60
            color: "#FFFFFF"
            font.pixelSize: 34
            font.bold: true
            style: Text.Outline
            styleColor: "#E64A19"
            opacity: 0

            SequentialAnimation {
                id: messageAnim
                NumberAnimation { target: message; property: "opacity"; to: 1; duration: 120 }
                PauseAnimation { duration: 1100 }
                NumberAnimation { target: message; property: "opacity"; to: 0; duration: 400 }
            }
        }

        Text {
            anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; bottomMargin: 50 }
            visible: root.phase === "playing" && root.ballInLane
            text: plunger.armed ? "RELEASE TO LAUNCH" : "HOLD TO PULL"
            color: "#FFB300"
            font.pixelSize: 15
            font.bold: true
            opacity: 0.85
        }
    }

    Component {
        id: popupComponent
        ScorePopup {}
    }

    Timer {
        id: bankReset
        interval: 1000
        onTriggered: { target1.reset(); target2.reset(); target3.reset() }
    }

    Timer {
        id: rolloverReset
        interval: 500
        onTriggered: { roll1.reset(); roll2.reset(); roll3.reset() }
    }

    // Drain watcher
    Timer {
        interval: 50
        repeat: true
        running: root.phase === "playing"
        onTriggered: {
            if (ball.position.y < -420 && ball.position.x < 175)
                root.loseBall()
        }
    }

    // Nudges a ball that came to rest somewhere on the playfield.
    Timer {
        property int stillMs: 0
        interval: 250
        repeat: true
        running: root.phase === "playing"
        onTriggered: {
            const v = ball.velocity
            const onPiston = root.ballInLane && ball.position.y < plunger.position.y + 30
            if (onPiston || v.x * v.x + v.y * v.y > 25) {
                stillMs = 0
                return
            }
            stillMs += interval
            if (stillMs >= 2500) {
                stillMs = 0
                ball.velocity = root.ballInLane
                    ? Qt.vector2d(0, -200)
                    : Qt.vector2d((Math.random() - 0.5) * 300, 250)
            }
        }
    }

    // ---- Input ----
    MultiPointTouchArea {
        id: touch
        anchors.fill: parent
        maximumTouchPoints: 4
        touchPoints: [
            TouchPoint { id: tp0 },
            TouchPoint { id: tp1 },
            TouchPoint { id: tp2 },
            TouchPoint { id: tp3 }
        ]

        readonly property var points: [tp0, tp1, tp2, tp3]
        readonly property bool leftPressed: (tp0.pressed && tp0.x < width / 2) || (tp1.pressed && tp1.x < width / 2)
                                     || (tp2.pressed && tp2.x < width / 2) || (tp3.pressed && tp3.x < width / 2)
        readonly property bool rightPressed: (tp0.pressed && tp0.x >= width / 2) || (tp1.pressed && tp1.x >= width / 2)
                                      || (tp2.pressed && tp2.x >= width / 2) || (tp3.pressed && tp3.x >= width / 2)

        function anyPressed() {
            return points.some(p => p.pressed)
        }

        onPressed: {
            if (root.phase !== "playing") {
                if (!startDelay.running)
                    root.startGame()
                return
            }
            if (root.ballInLane)
                plunger.pull()
        }
        onReleased: {
            if (!anyPressed())
                plunger.release()
        }
        onCanceled: {
            if (!anyPressed())
                plunger.release()
        }
    }

    // Keeps a flipper tap that ended the game from starting the next one at once.
    Timer {
        id: startDelay
        interval: 800
    }
    onPhaseChanged: if (phase === "over") startDelay.restart()

    // ---- Overlays ----
    Rectangle {
        anchors.fill: parent
        visible: root.phase !== "playing"
        color: "#B0000000"

        Column {
            anchors.centerIn: parent
            spacing: 14

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: root.phase === "over" ? "GAME OVER" : "PINBALL"
                color: "#FFE082"
                font.pixelSize: 44
                font.bold: true
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                visible: root.phase === "over"
                text: (root.newRecord ? "NEW RECORD!  " : "SCORE  ")
                      + root.score.toLocaleString(Qt.locale("en_US"), "f", 0)
                color: root.newRecord ? "#FF7043" : "#FFFFFF"
                font.pixelSize: 22
                font.bold: true
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Tap to play"
                color: "#FFFFFF"
                font.pixelSize: 18
                opacity: startDelay.running ? 0.3 : 1
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "Left / right half of the screen: flippers\nBall in the lane: hold and release to launch"
                color: "#8888AA"
                font.pixelSize: 13
            }
        }
    }

    FrameAnimation {
        id: frameCounter
        running: true
    }

    Text {
        anchors { left: parent.left; bottom: parent.bottom; margins: 4 }
        color: "#555566"
        font.pixelSize: 10
        text: "FPS " + (frameCounter.smoothFrameTime > 0
                        ? Math.round(1 / frameCounter.smoothFrameTime) : 0)
    }
}
