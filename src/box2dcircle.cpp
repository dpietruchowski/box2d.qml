#include "box2dcircle.h"

Box2DCircle::Box2DCircle(QQuickItem *parent)
    : Box2DFixture(parent)
    , m_radius(0.0)
{
}

qreal Box2DCircle::radius() const
{
    return m_radius;
}

void Box2DCircle::setRadius(qreal radius)
{
    if (qFuzzyCompare(m_radius, radius))
        return;

    m_radius = radius;
    emit radiusChanged();
}

b2Shape *Box2DCircle::createShape()
{
    b2CircleShape *shape = new b2CircleShape;
    const qreal r = m_radius > 0 ? m_radius : qMax(width(), height()) / 2.0;
    shape->m_radius = toMeters(r);
    return shape;
}
