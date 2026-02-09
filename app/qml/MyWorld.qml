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
                            fillColor: "#555"
                            strokeColor: "white"
                            strokeWidth: 1
                        }
                    }
                ]
            }

            Body {
                id: box
                type: Body.Dynamic
                position: Qt.vector2d(-200, -200)
                showBoundingBox: true
                
                fixtures: [
                    Fixture {
                        density: 1.0
                        restitution: 0.3
                        friction: 0.5
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
                        density: 0.8
                        restitution: 0.7
                        friction: 0.2
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
                        density: 1.2
                        restitution: 0.5
                        friction: 0.3
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
                        density: 0.6
                        restitution: 0.4
                        friction: 0.6
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
                        density: 1.5
                        restitution: 0.2
                        friction: 0.8
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
                        density: 0.5
                        restitution: 0.9
                        friction: 0.1
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
                id: compound
                type: Body.Dynamic
                position: Qt.vector2d(-100, -100)
                showBoundingBox: true
                
                fixtures: [
                    Fixture {
                        density: 1.0
                        restitution: 0.4
                        friction: 0.5
                        shape: BoxShape {
                            width: 40
                            height: 40
                            fillColor: "#E91E63"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    },
                    Fixture {
                        density: 0.8
                        restitution: 0.6
                        friction: 0.3
                        shape: CircleShape {
                            radius: 12
                            fillColor: "#FFC107"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    },
                    Fixture {
                        density: 0.5
                        restitution: 0.3
                        friction: 0.4
                        shape: PolygonShape {
                            vertices: [
                                Qt.point(0, -15),
                                Qt.point(10, 0),
                                Qt.point(0, 15),
                                Qt.point(-10, 0)
                            ]
                            fillColor: "#8BC34A"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
                
                MouseArea {
                    anchors.fill: parent
                    drag.target: parent
                    onPressed: compound.type = Body.Kinematic
                    onReleased: compound.type = Body.Dynamic
                }
            }

            Body {
                id: pendulumAnchor
                type: Body.Static
                position: Qt.vector2d(100, -100)
                showBoundingBox: false
                
                fixtures: [
                    Fixture {
                        shape: CircleShape {
                            radius: 8
                            fillColor: "#F44336"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
            }

            Body {
                id: pendulumBob
                type: Body.Dynamic
                position: Qt.vector2d(100, 50)
                showBoundingBox: true
                
                fixtures: [
                    Fixture {
                        density: 1.0
                        restitution: 0.1
                        friction: 0.3
                        shape: CircleShape {
                            radius: 20
                            fillColor: "#3F51B5"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
                
                MouseArea {
                    anchors.fill: parent
                    drag.target: parent
                    onPressed: pendulumBob.type = Body.Kinematic
                    onReleased: pendulumBob.type = Body.Dynamic
                }
            }

            RevoluteJoint {
                bodyA: pendulumAnchor
                bodyB: pendulumBob
                localAnchorA: Qt.point(0, 0)
                localAnchorB: Qt.point(0, -20)
            }

            Body {
                id: bridge1
                type: Body.Static
                position: Qt.vector2d(-150, 50)
                showBoundingBox: false
                
                fixtures: [
                    Fixture {
                        shape: BoxShape {
                            width: 20
                            height: 20
                            fillColor: "#795548"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
            }

            Body {
                id: bridge2
                type: Body.Dynamic
                position: Qt.vector2d(-100, 50)
                showBoundingBox: true
                
                fixtures: [
                    Fixture {
                        density: 0.5
                        restitution: 0.0
                        friction: 0.8
                        shape: BoxShape {
                            width: 40
                            height: 10
                            fillColor: "#9E9E9E"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
            }

            Body {
                id: bridge3
                type: Body.Dynamic
                position: Qt.vector2d(-50, 50)
                showBoundingBox: true
                
                fixtures: [
                    Fixture {
                        density: 0.5
                        restitution: 0.0
                        friction: 0.8
                        shape: BoxShape {
                            width: 40
                            height: 10
                            fillColor: "#9E9E9E"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
            }

            Body {
                id: bridge4
                type: Body.Static
                position: Qt.vector2d(0, 50)
                showBoundingBox: false
                
                fixtures: [
                    Fixture {
                        shape: BoxShape {
                            width: 20
                            height: 20
                            fillColor: "#795548"
                            strokeColor: "white"
                            strokeWidth: 2
                        }
                    }
                ]
            }

            DistanceJoint {
                bodyA: bridge1
                bodyB: bridge2
                localAnchorA: Qt.point(10, 0)
                localAnchorB: Qt.point(-20, 0)
                length: 30
                hertz: 2.0
                dampingRatio: 0.5
            }

            DistanceJoint {
                bodyA: bridge2
                bodyB: bridge3
                localAnchorA: Qt.point(20, 0)
                localAnchorB: Qt.point(-20, 0)
                length: 30
                hertz: 2.0
                dampingRatio: 0.5
            }

            DistanceJoint {
                bodyA: bridge3
                bodyB: bridge4
                localAnchorA: Qt.point(20, 0)
                localAnchorB: Qt.point(-10, 0)
                length: 30
                hertz: 2.0
                dampingRatio: 0.5
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
                            strokeColor: "#00BCD4"
                            strokeWidth: 4
                        }
                    }
                ]
            }
        }
    }
}
