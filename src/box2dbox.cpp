#include "box2dbox.h"

Box2DBox::Box2DBox(QQuickItem *parent)
    : Box2DFixture(parent)
{
}

b2Shape *Box2DBox::createShape()
{
    b2PolygonShape *shape = new b2PolygonShape;
    const qreal halfWidth = toMeters(width()) / 2.0;
    const qreal halfHeight = toMeters(height()) / 2.0;
    shape->SetAsBox(halfWidth, halfHeight);
    return shape;
}
