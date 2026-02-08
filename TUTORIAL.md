# Box2D QML Tutorial

This tutorial will help you get started with Box2D QML wrapper.

## Table of Contents
1. [Installation](#installation)
2. [Your First Physics Scene](#your-first-physics-scene)
3. [Understanding Bodies](#understanding-bodies)
4. [Working with Shapes](#working-with-shapes)
5. [Applying Forces](#applying-forces)
6. [Collision Detection](#collision-detection)
7. [Debug Visualization](#debug-visualization)

## Installation

### Building from Source

```bash
git clone https://github.com/dpietruchowski/box2d.qml.git
cd box2d.qml
mkdir build && cd build
cmake ..
cmake --build .
sudo cmake --install .
```

### Using in Your Project

Add to your QML file:
```qml
import Box2D 2.0
```

## Your First Physics Scene

Let's create a simple scene with a ball falling onto a ground:

```qml
import QtQuick 2.15
import QtQuick.Window 2.15
import Box2D 2.0

Window {
    visible: true
    width: 800
    height: 600
    
    // The physics world
    World {
        id: world
        anchors.fill: parent
        gravity: Qt.point(0, 9.8)  // Standard Earth gravity (positive Y is down in QML)
        running: true
        
        // Static ground
        Body {
            x: 0
            y: parent.height - 40
            width: parent.width
            height: 40
            bodyType: Body.Static
            
            Rectangle {
                anchors.fill: parent
                color: "brown"
            }
            
            Box {
                density: 1.0
                friction: 0.5
            }
        }
        
        // Dynamic ball
        Body {
            x: 400
            y: 100
            width: 50
            height: 50
            bodyType: Body.Dynamic
            
            Rectangle {
                anchors.fill: parent
                radius: width / 2
                color: "red"
            }
            
            Circle {
                radius: 25
                density: 1.0
                restitution: 0.7  // Bouncy!
            }
        }
    }
}
```

## Understanding Bodies

There are three types of bodies:

### Static Bodies
- Don't move
- Have infinite mass
- Perfect for walls, floors, platforms

```qml
Body {
    bodyType: Body.Static
    x: 0; y: 500
    width: 800; height: 100
    
    Box { }
}
```

### Dynamic Bodies
- Move and rotate
- Affected by forces and gravity
- Use for moving objects

```qml
Body {
    bodyType: Body.Dynamic
    x: 100; y: 100
    width: 50; height: 50
    gravityScale: 1.0  // 1.0 = normal gravity
    
    Box {
        density: 1.0
    }
}
```

### Kinematic Bodies
- Move but not affected by forces
- Perfect for moving platforms

```qml
Body {
    id: platform
    bodyType: Body.Kinematic
    x: 200; y: 300
    width: 100; height: 20
    
    Box { }
    
    // Animate position
    SequentialAnimation on x {
        loops: Animation.Infinite
        NumberAnimation { to: 600; duration: 3000 }
        NumberAnimation { to: 200; duration: 3000 }
    }
}
```

## Working with Shapes

### Box Shape

```qml
Body {
    width: 60
    height: 40
    
    Box {
        density: 1.0
        friction: 0.3
        restitution: 0.2
    }
}
```

### Circle Shape

```qml
Body {
    width: 50
    height: 50
    
    Circle {
        radius: 25
        density: 1.0
        restitution: 0.8
    }
}
```

### Polygon Shape

```qml
Body {
    Polygon {
        vertices: [
            Qt.point(0, 0),
            Qt.point(50, 0),
            Qt.point(50, 30),
            Qt.point(25, 50),
            Qt.point(0, 30)
        ]
        density: 1.0
    }
}
```

## Applying Forces

### Continuous Force

```qml
Body {
    id: rocket
    bodyType: Body.Dynamic
    
    Box { }
    
    // Apply thrust when space is pressed
    Keys.onPressed: {
        if (event.key === Qt.Key_Space) {
            rocket.applyForceToCenter(Qt.point(0, 500))
        }
    }
}
```

### Impulse (Instant)

```qml
Body {
    id: ball
    bodyType: Body.Dynamic
    
    Circle { }
    
    MouseArea {
        anchors.fill: parent
        onClicked: {
            // Jump!
            ball.applyLinearImpulse(
                Qt.point(0, 100),
                Qt.point(ball.x, ball.y)
            )
        }
    }
}
```

### Setting Velocity Directly

```qml
Body {
    id: player
    
    Box { }
    
    Keys.onLeftPressed: {
        player.linearVelocity = Qt.point(-5, player.linearVelocity.y)
    }
    
    Keys.onRightPressed: {
        player.linearVelocity = Qt.point(5, player.linearVelocity.y)
    }
}
```

## Collision Detection

Handle collisions using signals:

```qml
World {
    id: world
    
    onBeginContact: function(contact) {
        console.log("Collision started!")
        
        // Get the fixtures involved
        var fixtureA = contact.fixtureA
        var fixtureB = contact.fixtureB
        
        // Check if touching
        if (contact.touching) {
            console.log("Bodies are touching")
        }
    }
    
    onEndContact: function(contact) {
        console.log("Collision ended!")
    }
    
    Body {
        id: player
        objectName: "player"
        // ... player setup
    }
    
    Body {
        id: enemy
        objectName: "enemy"
        // ... enemy setup
    }
}
```

## Debug Visualization

Enable debug drawing to see physics shapes:

```qml
World {
    id: world
    
    // Your bodies here...
    
    DebugDraw {
        anchors.fill: parent
        world: world
        drawShapes: true
        drawJoints: false
        drawAABBs: false
        z: 1000  // Draw on top
    }
}
```

Toggle debug draw with keyboard:

```qml
Window {
    Keys.onPressed: {
        if (event.key === Qt.Key_D) {
            debugDraw.visible = !debugDraw.visible
        }
    }
    
    World {
        id: world
        
        DebugDraw {
            id: debugDraw
            world: world
            visible: false
        }
    }
}
```

## Advanced Topics

### Damping

Control how quickly objects slow down:

```qml
Body {
    linearDamping: 0.5   // Linear motion damping
    angularDamping: 0.3  // Rotation damping
}
```

### Bullet Bodies

For fast-moving objects that might tunnel through thin walls:

```qml
Body {
    bullet: true  // Enable CCD (Continuous Collision Detection)
}
```

### Collision Filtering

Control which objects collide with each other:

```qml
// Player can only collide with ground
Body {
    id: player
    Box {
        categories: 0x0002  // Player category
        collidesWith: 0x0001  // Only collide with ground
    }
}

// Ground
Body {
    Box {
        categories: 0x0001  // Ground category
        collidesWith: 0xFFFF  // Collide with everything
    }
}
```

### Sensors

Create trigger zones that don't generate collision response:

```qml
Body {
    bodyType: Body.Static
    
    Box {
        sensor: true  // No physical collision
    }
}

World {
    onBeginContact: function(contact) {
        if (contact.fixtureA.sensor || contact.fixtureB.sensor) {
            console.log("Entered trigger zone!")
        }
    }
}
```

## Tips and Best Practices

1. **Scaling**: Keep your world units reasonable. Use `pixelsPerMeter` to scale.
   - Good: 1 meter = 32 pixels
   - Box2D works best with objects between 0.1 and 10 meters

2. **Time Step**: Use a fixed time step (1/60 or 1/50) for stability

3. **Iterations**: Higher iteration counts = more accurate but slower
   - Velocity: 6-10 iterations
   - Position: 2-4 iterations

4. **Performance**: 
   - Avoid very small or very large objects
   - Use simple shapes when possible
   - Limit the number of dynamic bodies

5. **Restitution**: 
   - 0 = no bounce
   - 1 = perfect bounce
   - Values > 1 can add energy to the system

## Next Steps

- Check out the examples in the `examples/` directory
- Read the Box2D manual for deeper understanding
- Experiment with different shapes and forces!

Happy physics coding! 🎮
