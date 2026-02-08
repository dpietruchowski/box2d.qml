#include "box2dplugin.h"
#include "box2dworld.h"
#include "box2dbody.h"
#include "box2dfixture.h"
#include "box2dbox.h"
#include "box2dcircle.h"
#include "box2dpolygon.h"
#include "box2dcontact.h"
#include "box2ddebugdraw.h"

#include <qqml.h>

void Box2DPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QStringLiteral("Box2D"));
    
    // Register types for QML
    qmlRegisterType<Box2DWorld>(uri, 2, 0, "World");
    qmlRegisterType<Box2DBody>(uri, 2, 0, "Body");
    qmlRegisterUncreatableType<Box2DFixture>(uri, 2, 0, "Fixture",
                                               QStringLiteral("Fixture is abstract"));
    qmlRegisterType<Box2DBox>(uri, 2, 0, "Box");
    qmlRegisterType<Box2DCircle>(uri, 2, 0, "Circle");
    qmlRegisterType<Box2DPolygon>(uri, 2, 0, "Polygon");
    qmlRegisterUncreatableType<Box2DContact>(uri, 2, 0, "Contact",
                                              QStringLiteral("Contact is created by World"));
    qmlRegisterType<Box2DDebugDraw>(uri, 2, 0, "DebugDraw");
    
    // Register enums
    qmlRegisterUncreatableMetaObject(
        Body::staticMetaObject,
        uri, 2, 0, "Body",
        QStringLiteral("Error: only enums"));
}
