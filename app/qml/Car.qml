import QtQuick
import Box2D 1.0

Item {
    id: car
    property real carX: 0
    property real carY: 0
    
    // Chassis - main body
    Body {
        id: chassis
        type: Body.Dynamic
        position: Qt.vector2d(car.carX, car.carY)
        
        Timer {
            interval: 1000
            running: true
            repeat: true
            onTriggered: {
                console.log("Chassis pos:", chassis.position, "velocity:", chassis.linearVelocityX)
            }
        }
        
        fixtures: [
            Fixture {
                density: 0.3
                shape: BoxShape {
                    width: 100
                    height: 20
                    fillColor: "#e74c3c"
                    strokeColor: "white"
                    strokeWidth: 2
                }
            }
        ]
    }
    
    // Left wheel
    Wheel {
        id: leftWheel
        radius: 10
        position: Qt.vector2d(car.carX - 30, car.carY + 20)
    }
    
    // Right wheel
    Wheel {
        id: rightWheel
        radius: 10
        position: Qt.vector2d(car.carX + 30, car.carY + 20)
    }
    
    // Left wheel joint
    WheelJoint {
        bodyA: chassis
        bodyB: leftWheel
        localAnchorA: Qt.point(-30, 10)
        localAnchorB: Qt.point(0, 0)
        localAxisA: Qt.point(0, 1)
        enableMotor: true
        maxMotorTorque: 5000
        motorSpeed: 250
        hertz: 4.0
        dampingRatio: 0.7
    }
    
    // Right wheel joint
    WheelJoint {
        bodyA: chassis
        bodyB: rightWheel
        localAnchorA: Qt.point(30, 10)
        localAnchorB: Qt.point(0, 0)
        localAxisA: Qt.point(0, 1)
        enableMotor: true
        maxMotorTorque: 5000
        motorSpeed: 250
        hertz: 4.0
        dampingRatio: 0.7
    }
}
