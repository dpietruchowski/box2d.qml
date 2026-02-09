#include "qb2polygonshape.h"
#include <QPen>
#include <QBrush>
#include <cmath>

QB2PolygonShape::QB2PolygonShape(QObject *parent)
    : QB2Shape(parent)
{
}

QB2PolygonShape::~QB2PolygonShape()
{
}

QVector<QPointF> QB2PolygonShape::vertices() const
{
    return m_vertices;
}

void QB2PolygonShape::setVertices(const QVector<QPointF> &vertices)
{
    if (m_vertices == vertices)
        return;
    m_vertices = vertices;
    updateShape();
    emit verticesChanged();
}

b2Polygon *QB2PolygonShape::polygon()
{
    return &m_polygon;
}

void QB2PolygonShape::updateShape()
{
    if (m_vertices.isEmpty())
        return;

    QVector<b2Vec2> points;
    for (const QPointF &p : m_vertices)
    {
        points.append({static_cast<float>(p.x()), static_cast<float>(p.y())});
    }

    b2Hull hull = b2ComputeHull(points.data(), points.size());
    m_polygon = b2MakePolygon(&hull, 0.0f);
}

void QB2PolygonShape::paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId)
{
    if (!renderingEnabled())
        return;

    b2Polygon polygon = b2Shape_GetPolygon(shapeId);
    qreal shrink = strokeWidth() / 2.0;

    QPolygonF qpolygon;
    for (int v = 0; v < polygon.count; ++v)
    {
        qreal vx = polygon.vertices[v].x;
        qreal vy = polygon.vertices[v].y;
        qreal len = std::sqrt(vx * vx + vy * vy);
        if (len > 0 && shrink > 0)
        {
            qreal scale = qMax(0.0, (len - shrink)) / len;
            vx *= scale;
            vy *= scale;
        }
        qpolygon << QPointF(centroid.x() + vx, centroid.y() + vy);
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(fillColor());
    painter->drawPolygon(qpolygon);

    if (strokeWidth() > 0)
    {
        QPen pen(strokeColor(), strokeWidth());
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawPolygon(qpolygon);
    }
}
