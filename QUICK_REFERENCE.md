# Box2D QML Quick Reference

## Import
```qml
import Box2D 2.0
```

## World Setup
```qml
World {
    gravity: Qt.point(0, 9.8)  // Downward gravity
    running: true              // Auto-start simulation
    pixelsPerMeter: 32         // Scale factor
}
```

## Body Types

### Static (walls, ground)
```qml
Body {
    bodyType: Body.Static
    Box { }
}
```

### Dynamic (moving objects)
```qml
Body {
    bodyType: Body.Dynamic
    Box { density: 1.0 }
}
```

### Kinematic (moving platforms)
```qml
Body {
    bodyType: Body.Kinematic
    Box { }
}
```

## Shapes

### Box
```qml
Body {
    width: 100; height: 50
    Box {
        density: 1.0
        friction: 0.3
        restitution: 0.5  // bounciness
    }
}
```

### Circle
```qml
Body {
    width: 60; height: 60
    Circle {
        radius: 30
        density: 1.0
        restitution: 0.8
    }
}
```

### Polygon
```qml
Body {
    Polygon {
        vertices: [
            Qt.point(0, 0),
            Qt.point(50, 0),
            Qt.point(50, 50)
        ]
        density: 1.0
    }
}
```

## Forces & Impulses

```qml
Body {
    id: obj
    
    // Apply continuous force
    function thrust() {
        obj.applyForceToCenter(Qt.point(0, -100))
    }
    
    // Apply instant impulse
    function jump() {
        obj.applyLinearImpulse(Qt.point(0, -50), Qt.point(x, y))
    }
    
    // Set velocity directly
    function move() {
        obj.linearVelocity = Qt.point(10, 0)
    }
}
```

## Collision Detection

```qml
World {
    onBeginContact: function(contact) {
        console.log("Collision started")
        if (contact.touching) {
            // Handle collision
        }
    }
    
    onEndContact: function(contact) {
        console.log("Collision ended")
    }
}
```

## Properties

### Body Properties
- `linearDamping` - Slow down linear motion
- `angularDamping` - Slow down rotation
- `gravityScale` - Multiply gravity (0 = no gravity)
- `bullet` - Enable CCD for fast objects
- `fixedRotation` - Prevent rotation
- `linearVelocity` - Current velocity
- `angularVelocity` - Current rotation speed

### Fixture Properties
- `density` - Mass per area (kg/m²)
- `friction` - Surface friction (0-1)
- `restitution` - Bounciness (0-1)
- `sensor` - No collision response

### Collision Filtering
- `categories` - What I am (bitmask)
- `collidesWith` - What I collide with (bitmask)
- `groupIndex` - Group index (same = collide)

## Debug Drawing

```qml
World {
    id: world
    
    DebugDraw {
        anchors.fill: parent
        world: world
        drawShapes: true
        drawJoints: true
        visible: true
        z: 1000
    }
}
```

## Common Patterns

### Ground
```qml
Body {
    y: parent.height - 50
    width: parent.width; height: 50
    bodyType: Body.Static
    Box { friction: 0.5 }
}
```

### Bouncing Ball
```qml
Body {
    width: 50; height: 50
    bodyType: Body.Dynamic
    Circle {
        radius: 25
        density: 1.0
        restitution: 0.9
    }
}
```

### Moving Platform
```qml
Body {
    bodyType: Body.Kinematic
    Box { }
    
    SequentialAnimation on x {
        loops: Animation.Infinite
        NumberAnimation { to: 400; duration: 2000 }
        NumberAnimation { to: 100; duration: 2000 }
    }
}
```

### Player Control
```qml
Body {
    id: player
    focus: true
    
    Keys.onLeftPressed: {
        player.linearVelocity = Qt.point(-5, player.linearVelocity.y)
    }
    Keys.onRightPressed: {
        player.linearVelocity = Qt.point(5, player.linearVelocity.y)
    }
    Keys.onSpacePressed: {
        player.applyLinearImpulse(Qt.point(0, -100), Qt.point(x, y))
    }
}
```

## Tips

1. **Units**: Keep objects 0.1 to 10 meters (3-320 pixels at default scale)
2. **Time step**: Use 1/60 (16.67ms) for smooth animation
3. **Iterations**: Higher = more accurate but slower (velocity: 8, position: 3)
4. **Restitution > 1**: Adds energy, can cause instability
5. **Fast objects**: Set `bullet: true` to prevent tunneling
6. **Sensors**: Use for triggers (no physical response)

## Building

```bash
# CMake
mkdir build && cd build
cmake ..
cmake --build .

# qmake  
qmake
make
```

## Links

- Repository: https://github.com/dpietruchowski/box2d.qml
- Box2D Manual: https://box2d.org/documentation/
