#include "qb2boxshape.h"
#include <QPen>
#include <QBrush>
#include <cmath>

QB2BoxShape::QB2BoxShape(QObject *parent)
    : QB2Shape(parent)
{
    updateShape();
}

QB2BoxShape::~QB2BoxShape()
{
}

qreal QB2BoxShape::width() const
{
    return m_width;
}

void QB2BoxShape::setWidth(qreal width)
{
    if (qFuzzyCompare(m_width, width))
        return;
    m_width = width;
    updateShape();
    emit widthChanged();
}

qreal QB2BoxShape::height() const
{
    return m_height;
}

void QB2BoxShape::setHeight(qreal height)
{
    if (qFuzzyCompare(m_height, height))
        return;
    m_height = height;
    updateShape();
    emit heightChanged();
}

b2Polygon *QB2BoxShape::polygon()
{
    return &m_polygon;
}

void QB2BoxShape::updateShape()
{
    m_polygon = b2MakeBox(m_width / 2.0f, m_height / 2.0f);
}

void QB2BoxShape::paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId)
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
