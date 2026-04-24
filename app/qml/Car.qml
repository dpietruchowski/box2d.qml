import QtQuick
import Box2D 1.0

Item {
    id: car
    property real carX: 0
    property real carY: 0
    property real scale: 10.0
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
                density: 0.5 / car.scale
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
                    fillColor: "transparent"
                    strokeColor: "transparent"
                    strokeWidth: 0
                }
            }
        ]
        
        Image {
            source: "car.svg"
            width: 3.0 * car.scale * 0.85
            height: 1.4 * car.scale * 0.85
            x: -width / 2
            y: -height / 2 + 0.2 * car.scale
            smooth: true
        }
    }
    
    // Rear wheel
    Wheel {
        id: rearWheel
        radius: 0.4 * car.scale
        scale: car.scale
        position: Qt.vector2d(car.carX - 1.0 * car.scale, car.carY + 0.35 * car.scale)
        
        chassis: chassis
        localAnchorChassis: Qt.point(-1.0 * car.scale, (0.35 - 1.0) * car.scale)
        enableMotor: true
        motorSpeed: car.motorSpeed
        maxMotorTorque: 200.0
    }
    
    // Front wheel
    Wheel {
        id: frontWheel
        radius: 0.4 * car.scale
        scale: car.scale
        position: Qt.vector2d(car.carX + 1.0 * car.scale, car.carY + 0.4 * car.scale)
        
        chassis: chassis
        localAnchorChassis: Qt.point(1.0 * car.scale, (0.4 - 1.0) * car.scale)
        enableMotor: true
        motorSpeed: car.motorSpeed
        maxMotorTorque: 200.0
    }
}
