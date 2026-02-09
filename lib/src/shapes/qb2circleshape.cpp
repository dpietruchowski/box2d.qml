#include "qb2circleshape.h"
#include <QPen>
#include <QBrush>

QB2CircleShape::QB2CircleShape(QObject *parent)
    : QB2Shape(parent)
{
    m_circle.center = {0.0f, 0.0f};
    m_circle.radius = m_radius;
}

QB2CircleShape::~QB2CircleShape()
{
}

qreal QB2CircleShape::radius() const
{
    return m_radius;
}

void QB2CircleShape::setRadius(qreal radius)
{
    if (qFuzzyCompare(m_radius, radius))
        return;
    m_radius = radius;
    m_circle.radius = radius;
    emit radiusChanged();
}

b2Circle *QB2CircleShape::circle()
{
    return &m_circle;
}

void QB2CircleShape::paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId)
{
    if (!renderingEnabled())
        return;

    b2Circle circle = b2Shape_GetCircle(shapeId);
    QPointF center(centroid.x() + circle.center.x,
                   centroid.y() + circle.center.y);

    qreal adjustedRadius = qMax(0.0, circle.radius - strokeWidth() / 2.0);

    painter->setPen(Qt::NoPen);
    painter->setBrush(fillColor());
    painter->drawEllipse(center, adjustedRadius, adjustedRadius);

    if (strokeWidth() > 0)
    {
        QPen pen(strokeColor(), strokeWidth());
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(center, adjustedRadius, adjustedRadius);
    }
}
