# Box2D QML

A Qt/QML wrapper for the Box2D physics engine, allowing you to easily integrate 2D physics simulations into your QML applications.

## Features

- **Easy Integration**: Use Box2D physics directly in QML declarative syntax
- **Core Physics Components**:
  - `World`: Main physics world with configurable gravity and timing
  - `Body`: Physics bodies (Static, Dynamic, Kinematic)
  - `Box`, `Circle`, `Polygon`: Collision shapes
  - `Fixture`: Physics properties (density, friction, restitution)
- **Visual Debugging**: Built-in debug draw component to visualize physics
- **Qt Compatible**: Works with both Qt5 (5.15+) and Qt6
- **Flexible Units**: Configurable pixels-per-meter ratio for scaling

## Building

### Prerequisites

- Qt 5.15+ or Qt 6.x with Qml and Quick modules
- CMake 3.16+ (for CMake build) or qmake (for qmake build)
- C++17 compatible compiler
- Box2D library (will be downloaded automatically if not found)

### Build with CMake (Recommended)

```bash
# Clone the repository
git clone https://github.com/dpietruchowski/box2d.qml.git
cd box2d.qml

# Option 1: Use the build script
./build.sh

# Option 2: Manual build
mkdir build && cd build
cmake ..
cmake --build .

# Install (optional)
sudo cmake --install .
```

### Build with qmake

```bash
# Clone the repository
git clone https://github.com/dpietruchowski/box2d.qml.git
cd box2d.qml

# Build
qmake
make

# Install (optional)
sudo make install
```

### Build Options

**CMake:**
- `BUILD_EXAMPLES`: Build example applications (default: ON)

```bash
cmake -DBUILD_EXAMPLES=OFF ..
```

## Usage

### Basic Example

```qml
import QtQuick 2.15
import QtQuick.Window 2.15
import Box2D 2.0

Window {
    visible: true
    width: 800
    height: 600
    
    World {
        id: physicsWorld
        anchors.fill: parent
        gravity: Qt.point(0, 9.8)
        running: true
        
        // Static ground
        Body {
            x: 0
            y: parent.height - 50
            width: parent.width
            height: 50
            bodyType: Body.Static
            
            Rectangle {
                anchors.fill: parent
                color: "brown"
            }
            
            Box {
                width: parent.width
                height: parent.height
            }
        }
        
        // Dynamic box
        Body {
            x: 400
            y: 100
            width: 60
            height: 60
            bodyType: Body.Dynamic
            
            Rectangle {
                anchors.fill: parent
                color: "red"
            }
            
            Box {
                width: parent.width
                height: parent.height
                density: 1.0
                friction: 0.3
                restitution: 0.5
            }
        }
    }
}
```

## Components

### World

The main physics simulation container.

**Properties:**
- `gravity`: QPointF - World gravity in QML coordinates where positive Y is down (default: Qt.point(0, 10))
- `running`: bool - Whether simulation is running (default: true)
- `timeStep`: real - Time step for physics update in seconds (default: 1/60)
- `velocityIterations`: int - Velocity solver iterations (default: 8)
- `positionIterations`: int - Position solver iterations (default: 3)
- `pixelsPerMeter`: real - Conversion ratio (default: 32)

**Signals:**
- `stepped()` - Emitted after each physics step
- `beginContact(Contact contact)` - Emitted when two fixtures begin touching
- `endContact(Contact contact)` - Emitted when two fixtures stop touching

### Body

A rigid body in the physics world.

**Properties:**
- `world`: World - The physics world (auto-detected from parent)
- `bodyType`: enum - Body.Static, Body.Kinematic, or Body.Dynamic (default: Dynamic)
- `linearDamping`: real - Linear velocity damping (default: 0.0)
- `angularDamping`: real - Angular velocity damping (default: 0.0)
- `bullet`: bool - Enable continuous collision detection (default: false)
- `fixedRotation`: bool - Prevent rotation (default: false)
- `gravityScale`: real - Gravity multiplier (default: 1.0)
- `linearVelocity`: QPointF - Current linear velocity
- `angularVelocity`: real - Current angular velocity

**Methods:**
- `applyForce(QPointF force, QPointF point)` - Apply force at a point
- `applyForceToCenter(QPointF force)` - Apply force to center of mass
- `applyTorque(real torque)` - Apply rotational force
- `applyLinearImpulse(QPointF impulse, QPointF point)` - Apply instant impulse
- `applyAngularImpulse(real impulse)` - Apply instant angular impulse

### Shapes

#### Box

Rectangular collision shape.

**Usage:** Automatically uses parent Body's width and height.

```qml
Body {
    width: 100
    height: 50
    Box {
        density: 1.0
        friction: 0.3
        restitution: 0.5
    }
}
```

#### Circle

Circular collision shape.

**Properties:**
- `radius`: real - Circle radius (auto-calculated from parent size if not set)

```qml
Body {
    width: 80
    height: 80
    Circle {
        radius: 40
        density: 1.0
        friction: 0.3
        restitution: 0.8
    }
}
```

#### Polygon

Custom polygon collision shape.

**Properties:**
- `vertices`: list<QPointF> - Polygon vertices (3-8 points)

```qml
Body {
    Polygon {
        vertices: [
            Qt.point(0, 0),
            Qt.point(50, 0),
            Qt.point(50, 30),
            Qt.point(25, 50)
        ]
        density: 1.0
    }
}
```

### Fixture Properties

All shapes inherit these properties:

- `density`: real - Mass per area (default: 1.0)
- `friction`: real - Surface friction coefficient (default: 0.3)
- `restitution`: real - Bounciness (0-1, default: 0.0)
- `sensor`: bool - Don't generate collision response (default: false)
- `categories`: int - Collision category bits (default: 0x0001)
- `collidesWith`: int - Collision mask bits (default: 0xFFFF)
- `groupIndex`: int - Collision group (default: 0)

### DebugDraw

Visual debugging component to render physics shapes.

**Properties:**
- `world`: World - The world to visualize
- `drawShapes`: bool - Draw collision shapes (default: true)
- `drawJoints`: bool - Draw joints (default: true)
- `drawAABBs`: bool - Draw bounding boxes (default: false)
- `drawCenterOfMass`: bool - Draw center of mass (default: false)

```qml
World {
    id: world
    
    DebugDraw {
        anchors.fill: parent
        world: world
        drawShapes: true
        z: 100  // Draw on top
    }
}
```

## Examples

Run the examples after building:

```bash
cd build/examples
./example_boxes
```

The examples demonstrate:
- **boxes**: Falling boxes with gravity, walls, and mixed shapes

## License

This project is a wrapper for Box2D. Please refer to the Box2D license for the physics engine.

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## Acknowledgments

- [Box2D](https://box2d.org/) - The physics engine
- Qt Project - For the QML framework