# Object Creation Order in QML vs Box2D

## Box2D C API - Linear, Explicit Order

In pure Box2D v3, creation is **synchronous** and **explicit**. You control the exact order:

```cpp
// 1. Create World first
b2WorldDef worldDef = b2DefaultWorldDef();
worldDef.gravity = {0.0f, 10.0f};
b2WorldId worldId = b2CreateWorld(&worldDef);

// 2. Create Body (requires worldId)
b2BodyDef bodyDef = b2DefaultBodyDef();
bodyDef.type = b2_dynamicBody;
bodyDef.position = {0.0f, 4.0f};
b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);  // ← World already exists!

// 3. Create Shape/Fixture (requires bodyId)
b2ShapeDef shapeDef = b2DefaultShapeDef();
shapeDef.density = 1.0f;
b2Circle circle = {{0.0f, 0.0f}, 0.5f};
b2ShapeId shapeId = b2CreateCircleShape(bodyId, &shapeDef, &circle);  // ← Body exists!

// 4. Create second Body for joint
b2BodyDef bodyDef2 = b2DefaultBodyDef();
bodyDef2.type = b2_staticBody;
b2BodyId groundId = b2CreateBody(worldId, &bodyDef2);

// 5. Create Joint (requires both bodyIds)
b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef();
jointDef.base.bodyIdA = groundId;
jointDef.base.bodyIdB = bodyId;
b2JointId jointId = b2CreateRevoluteJoint(worldId, &jointDef);  // ← Both bodies exist!

// 6. Simulate
b2World_Step(worldId, 1.0f/60.0f, 4);

// 7. Cleanup (reverse order!)
b2DestroyJoint(jointId);
b2DestroyShape(shapeId);
b2DestroyBody(bodyId);
b2DestroyBody(groundId);
b2DestroyWorld(worldId);
```

**Key points:**
- Everything is **synchronous** - created immediately
- Must pass IDs: `worldId` → `bodyId` → `shapeId`
- **You** control the exact order
- Cleanup in **reverse** order

## QML - Declarative, Deferred Initialization

In QML, creation is **declarative** and **asynchronous**. QML decides the order:

```qml
World {                      // 1. World() constructor called
    id: physics              // 2. World.classBegin()
    gravity: Qt.vector2d(0, 9.8)
    
    Body {                   // 3. Body() constructor (parent not set yet!)
        id: box              // 4. Body.classBegin()
        position: Qt.vector2d(0, -100)
        
        fixtures: [          // 5. Fixture() constructor
            Fixture {        // 6. Fixture.classBegin()
                shape: BoxShape {      // 7. BoxShape() constructor
                    width: 50          // 8. Properties set
                    height: 50
                }            // 9. BoxShape.componentComplete()
            }                // 10. Fixture.componentComplete()
        ]
    }                        // 11. Body.componentComplete() ← Body ready!
}                            // 12. World.componentComplete() ← Everything ready!
```

### QML Creation Phases

1. **Constructor** - Object allocated, parent = nullptr
2. **classBegin()** - QML starts initialization
3. **Property setters** - Properties are assigned
4. **Parent assigned** - `setParentItem()` called
5. **Children created** - Recursively create children
6. **componentComplete()** - Object fully initialized (children first, parent last)

### Critical Point: Parent Availability

**In Constructor:** ❌
```cpp
QB2Body::QB2Body(QQuickItem *parent) {
    qDebug() << parentItem();  // ← nullptr! Parent not set yet!
}
```

**In componentComplete():** ✅
```cpp
void QB2Body::componentComplete() {
    qDebug() << parentItem();  // ← World (or Item)! Parent is set!
}
```

## box2d.qml Implementation Strategy

### World Creation (Immediate)

World creates `b2WorldId` **immediately** in constructor:

```cpp
QB2World::QB2World(QQuickItem *parent) : QQuickItem(parent) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {m_gravity.x(), m_gravity.y()};
    m_worldId = b2CreateWorld(&worldDef);  // ← Created immediately!
}
```

### Body Creation (Deferred)

Body **defers** `b2BodyId` creation until `componentComplete()`:

```cpp
void QB2Body::componentComplete() {
    QQuickItem::componentComplete();
    m_componentComplete = true;
    
    // Auto-find World in parent hierarchy
    if (!m_world) {
        QQuickItem *item = parentItem();  // ← Now available!
        while (item) {
            if (auto *world = qobject_cast<QB2World*>(item)) {
                m_world = world;
                break;
            }
            item = item->parentItem();
        }
    }
    
    if (!m_world) {
        qFatal("Body must be a child of World");
        return;
    }
    
    createBody();  // ← Now creates b2BodyId
}
```

### Fixture Creation (Deferred)

Fixture waits for Body to be ready:

```cpp
void QB2Fixture::componentComplete() {
    m_componentComplete = true;
    
    if (!m_body) {
        m_body = qobject_cast<QB2Body*>(parent());
    }
    
    if (!m_body) {
        qFatal("Fixture must be a child of Body");
        return;
    }
    
    if (b2Body_IsValid(m_body->bodyId())) {
        createShape();  // Body ready, create now
    } else {
        // Body not ready yet, wait for signal
        connect(m_body, &QB2Body::bodyReady, this, &QB2Fixture::createShape);
    }
}
```

## Hierarchy and Auto-Finding

### Direct Child (Simple)

```qml
World {
    Body {  // ← parentItem() = World
        fixtures: [
            Fixture {  // ← parent() = Body
                shape: BoxShape { }  // ← parent = Fixture
            }
        ]
    }
}
```

### Nested Component (Auto-Finding)

```qml
// Car.qml
Item {
    id: car
    property real carX: 0
    property real carY: 0
    
    Body {
        id: chassis
        position: Qt.vector2d(car.carX, car.carY)
        // Auto-finds World by walking up hierarchy
    }
    
    Body {
        id: wheel
        position: Qt.vector2d(car.carX + 30, car.carY + 20)
        // Also auto-finds World
    }
    
    WheelJoint {
        bodyA: chassis
        bodyB: wheel
        // Also auto-finds World
    }
}

// Usage:
World {
    id: physics
    
    Car {  // ← Car's bodies find World automatically!
        carX: 100
        carY: 50
    }
}
```

**How it works:**
1. `Car` is created as child of `World`
2. `Body` inside `Car` has `parentItem()` = `Car`
3. In `Body.componentComplete()`, walks up: `Car → World`
4. Finds `World`, uses its `worldId`
5. Creates `b2BodyId`

## Object Hierarchy

```
World (QQuickItem)
  ↓ parentItem()
  ├─ Body (QQuickPaintedItem) [auto-finds World]
  │   ↓ parent() (QObject)
  │   └─ fixtures: [
  │       Fixture (QObject) [checks direct parent]
  │         ↓ parent() (QObject)
  │         └─ shape: BoxShape (QObject) [parent set by Fixture]
  │   ]
  │
  ├─ Item (custom component)
  │   ↓ parentItem()
  │   └─ Body [auto-finds World by walking up]
  │
  └─ Joint (QObject) [auto-finds World via parent()]
```

## Summary

| Aspect | Box2D C API | box2d.qml |
|--------|-------------|-----------|
| **Creation** | Synchronous | Asynchronous |
| **Order** | You control | QML controls |
| **Dependencies** | Pass IDs explicitly | Auto-find via hierarchy |
| **World creation** | Manual | Constructor |
| **Body creation** | Manual | componentComplete() |
| **Hierarchy** | No hierarchy | QML scene graph |
| **Flexibility** | Linear only | Can nest in components |

The key insight: **Box2D expects linear creation**, but **QML is tree-based**. We bridge this by:
1. Creating Box2D World immediately
2. Deferring Box2D Body/Joint creation to `componentComplete()`
3. Auto-finding World by walking up the QML tree
4. Enabling flexible component composition (like `Car.qml`)
