#include "box2dpolygon.h"
#include <QPointF>

Box2DPolygon::Box2DPolygon(QQuickItem *parent)
    : Box2DFixture(parent)
{
}

QVariantList Box2DPolygon::vertices() const
{
    return m_vertices;
}

void Box2DPolygon::setVertices(const QVariantList &vertices)
{
    if (m_vertices == vertices)
        return;

    m_vertices = vertices;
    emit verticesChanged();
}

b2Shape *Box2DPolygon::createShape()
{
    if (m_vertices.isEmpty())
        return nullptr;

    b2PolygonShape *shape = new b2PolygonShape;
    
    QVector<b2Vec2> points;
    for (const QVariant &v : m_vertices) {
        QPointF point = v.toPointF();
        points.append(b2Vec2(toMeters(point.x()), -toMeters(point.y())));
    }

    if (points.size() >= 3 && points.size() <= b2_maxPolygonVertices) {
        shape->Set(points.data(), points.size());
    } else {
        delete shape;
        return nullptr;
    }

    return shape;
}
