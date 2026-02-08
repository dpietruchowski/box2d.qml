import QtQuick
import Box2D 1.0

Rectangle {
    color: "#1a1a1a"
    
    Item {
        id: viewport
        anchors.fill: parent
        clip: true
        
        World {
            id: physics
            x: parent.width / 2
            y: parent.height / 2
            gravity: Qt.vector2d(0, 9.8)
            running: true

            Body {
                id: ground
                type: Body.Static
                position: Qt.vector2d(0, 300)
                transformOrigin: Item.Center
                
                fixtures: [
                    Fixture {
                        shape: BoxShape {
                            id: groundShape
                            width: 600
                            height: 20
                        }
                    }
                ]
                
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    color: "#555"
                    border.color: "white"
                    border.width: 1
                }
            }

            Body {
                id: box
                type: Body.Dynamic
                position: Qt.vector2d(-200, -200)
                showBoundingBox: true
                
                fixtures: [
                    Fixture {
                        density: 1.0
                        shape: BoxShape {
                            width: 50
                            height: 50
                            fillColor: "#4CAF50"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
                
                MouseArea {
                    anchors.fill: parent
                    drag.target: parent
                    onPressed: box.type = Body.Kinematic
                    onReleased: box.type = Body.Dynamic
                }
            }

            Body {
                id: circle
                type: Body.Dynamic
                position: Qt.vector2d(-100, -200)
                showBoundingBox: true
                
                fixtures: [
                    Fixture {
                        density: 1.0
                        shape: CircleShape {
                            radius: 25
                            fillColor: "#2196F3"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
                
                MouseArea {
                    anchors.fill: parent
                    drag.target: parent
                    onPressed: circle.type = Body.Kinematic
                    onReleased: circle.type = Body.Dynamic
                }
            }

            Body {
                id: capsule
                type: Body.Dynamic
                position: Qt.vector2d(0, -200)
                showBoundingBox: true
                
                fixtures: [
                    Fixture {
                        density: 1.0
                        shape: CapsuleShape {
                            length: 60
                            radius: 15
                            fillColor: "#FF9800"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
                
                MouseArea {
                    anchors.fill: parent
                    drag.target: parent
                    onPressed: capsule.type = Body.Kinematic
                    onReleased: capsule.type = Body.Dynamic
                }
            }

            Body {
                id: polygon
                type: Body.Dynamic
                position: Qt.vector2d(100, -200)
                showBoundingBox: true
                
                fixtures: [
                    Fixture {
                        density: 1.0
                        shape: PolygonShape {
                            vertices: [
                                Qt.point(0, -30),
                                Qt.point(25, 15),
                                Qt.point(-25, 15)
                            ]
                            fillColor: "#9C27B0"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
                
                MouseArea {
                    anchors.fill: parent
                    drag.target: parent
                    onPressed: polygon.type = Body.Kinematic
                    onReleased: polygon.type = Body.Dynamic
                }
            }

            Body {
                id: pentagon
                type: Body.Dynamic
                position: Qt.vector2d(200, -200)
                showBoundingBox: true
                
                fixtures: [
                    Fixture {
                        density: 1.0
                        shape: PolygonShape {
                            vertices: [
                                Qt.point(0, -25),
                                Qt.point(23, -8),
                                Qt.point(14, 20),
                                Qt.point(-14, 20),
                                Qt.point(-23, -8)
                            ]
                            fillColor: "#FFEB3B"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
                
                MouseArea {
                    anchors.fill: parent
                    drag.target: parent
                    onPressed: pentagon.type = Body.Kinematic
                    onReleased: pentagon.type = Body.Dynamic
                }
            }

            Body {
                id: diamond
                type: Body.Dynamic
                position: Qt.vector2d(-200, -100)
                showBoundingBox: true
                
                fixtures: [
                    Fixture {
                        density: 1.0
                        shape: PolygonShape {
                            vertices: [
                                Qt.point(0, -30),
                                Qt.point(15, 0),
                                Qt.point(0, 30),
                                Qt.point(-15, 0)
                            ]
                            fillColor: "#00BCD4"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
                
                MouseArea {
                    anchors.fill: parent
                    drag.target: parent
                    onPressed: diamond.type = Body.Kinematic
                    onReleased: diamond.type = Body.Dynamic
                }
            }

            Body {
                id: segment
                type: Body.Static
                position: Qt.vector2d(0, 100)
                transformOrigin: Item.Center
                
                fixtures: [
                    Fixture {
                        shape: SegmentShape {
                            point1: Qt.point(-150, 0)
                            point2: Qt.point(150, 0)
                        }
                    }
                ]
                
                Rectangle {
                    anchors.centerIn: parent
                    width: 300
                    height: 4
                    color: "#00BCD4"
                }
            }
        }
    }
}
