# box2d.qml

QML wrapper for Box2D v3 physics engine with example application.

## Features

- **QML Integration**: Native QML types for Box2D physics (World, Body, Fixture, Shape)
- **Visual Rendering**: Bodies are QQuickItems that automatically sync with physics simulation
- **Interactive**: Mouse drag support to move physics objects
- **Automatic Layout**: Bodies automatically calculate their size from fixtures
- **SVG Support**: Use custom graphics for physics bodies

## Building

```bash
cmake -S . -B build/Desktop-Debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/Desktop-Debug --parallel
```

## Running

```bash
./build/Desktop-Debug/app/box2d_app
```

## Example Usage

```qml
World {
    gravity: Qt.vector2d(0, 9.8)
    running: true
    
    Body {
        type: Body.Dynamic
        position: Qt.vector2d(0, -200)
        
        fixtures: [
            Fixture {
                density: 1.0
                shape: BoxShape {
                    width: 50
                    height: 50
                }
            }
        ]
        
        Image {
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
            source: "box.svg"
        }
    }
}
```

## License

See LICENSE file.
