import QtQuick
import Box2D 1.0

Item {
    id: car
    property real carX: 0
    property real carY: 0
    property real scale: 30.0
    property real motorSpeed: 0.0
    
    // Chassis - based on Box2D car sample
    Body {
        id: chassis
        type: Body.Dynamic
        position: Qt.vector2d(car.carX, car.carY + 1.0 * car.scale)
        
        Timer {
            interval: 500
            running: true
            repeat: true
            onTriggered: {
                console.log("Chassis pos:", chassis.position.x.toFixed(2), chassis.position.y.toFixed(2), 
                           "vel:", chassis.linearVelocityX.toFixed(2), 
                           "motorSpeed:", car.motorSpeed)
            }
        }
        
        fixtures: [
            Fixture {
                density: 1.0 / car.scale
                friction: 0.2
                shape: PolygonShape {
                    vertices: [
                        Qt.point(-1.5 * 0.85 * car.scale, -0.5 * 0.85 * car.scale),
                        Qt.point(1.5 * 0.85 * car.scale, -0.5 * 0.85 * car.scale),
                        Qt.point(1.5 * 0.85 * car.scale, 0.0 * 0.85 * car.scale),
                        Qt.point(0.0 * 0.85 * car.scale, 0.9 * 0.85 * car.scale),
                        Qt.point(-1.15 * 0.85 * car.scale, 0.9 * 0.85 * car.scale),
                        Qt.point(-1.5 * 0.85 * car.scale, 0.2 * 0.85 * car.scale)
                    ]
                    fillColor: "#e74c3c"
                    strokeColor: "white"
                    strokeWidth: 2
                }
            }
        ]
    }
    
    // Rear wheel
    Wheel {
        id: rearWheel
        radius: 0.4 * car.scale
        scale: car.scale
        position: Qt.vector2d(car.carX - 1.0 * car.scale, car.carY + 0.35 * car.scale)
        
        Timer {
            interval: 1000
            running: true
            repeat: true
            onTriggered: {
                console.log("Rear wheel angularVelocity:", rearWheel.angularVelocity, "linearVel:", rearWheel.linearVelocityX)
            }
        }
    }
    
    // Front wheel
    Wheel {
        id: frontWheel
        radius: 0.4 * car.scale
        scale: car.scale
        position: Qt.vector2d(car.carX + 1.0 * car.scale, car.carY + 0.4 * car.scale)
    }
    
    // Rear axle - wheel joint with motor
    WheelJoint {
        bodyA: chassis
        bodyB: rearWheel
        localAnchorA: Qt.point(-1.0 * car.scale, (0.35 - 1.0) * car.scale)
        localAnchorB: Qt.point(0, 0)
        localAxisA: Qt.point(0, 1)
        enableMotor: true
        maxMotorTorque: 2000.0
        motorSpeed: car.motorSpeed
        hertz: 2.0
        dampingRatio: 0.7
        enableLimit: true
        lowerTranslation: -0.25 * car.scale
        upperTranslation: 0.25 * car.scale
    }
    
    // Front axle - wheel joint with motor
    WheelJoint {
        bodyA: chassis
        bodyB: frontWheel
        localAnchorA: Qt.point(1.0 * car.scale, (0.4 - 1.0) * car.scale)
        localAnchorB: Qt.point(0, 0)
        localAxisA: Qt.point(0, 1)
        enableMotor: true
        maxMotorTorque: 2000.0
        motorSpeed: car.motorSpeed
        hertz: 2.0
        dampingRatio: 0.7
        enableLimit: true
        lowerTranslation: -0.25 * car.scale
        upperTranslation: 0.25 * car.scale
    }
}
