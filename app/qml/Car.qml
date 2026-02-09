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
        
        fixtures: [
            Fixture {
                density: 1.0
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
    Body {
        id: leftWheel
        type: Body.Dynamic
        position: Qt.vector2d(car.carX - 30, car.carY + 20)
        
        fixtures: [
            Fixture {
                density: 0.5
                friction: 0.9
                shape: CircleShape {
                    radius: 10
                    fillColor: "#34495e"
                    strokeColor: "white"
                    strokeWidth: 2
                }
            }
        ]
    }
    
    // Right wheel
    Body {
        id: rightWheel
        type: Body.Dynamic
        position: Qt.vector2d(car.carX + 30, car.carY + 20)
        
        fixtures: [
            Fixture {
                density: 0.5
                friction: 0.9
                shape: CircleShape {
                    radius: 10
                    fillColor: "#34495e"
                    strokeColor: "white"
                    strokeWidth: 2
                }
            }
        ]
    }
    
    // Left wheel joint
    WheelJoint {
        bodyA: chassis
        bodyB: leftWheel
        localAnchorA: Qt.point(-30, 10)
        localAnchorB: Qt.point(0, 0)
        localAxisA: Qt.point(0, 1)
        enableMotor: false
        maxMotorTorque: 0
        motorSpeed: 0
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
        enableMotor: false
        maxMotorTorque: 0
        motorSpeed: 0
        hertz: 4.0
        dampingRatio: 0.7
    }
}
