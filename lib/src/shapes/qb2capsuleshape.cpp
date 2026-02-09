#include "qb2capsuleshape.h"
#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QtMath>
#include <cmath>

QB2CapsuleShape::QB2CapsuleShape(QObject *parent)
    : QB2Shape(parent)
{
    updateShape();
}

QB2CapsuleShape::~QB2CapsuleShape()
{
}

qreal QB2CapsuleShape::length() const
{
    return m_length;
}

void QB2CapsuleShape::setLength(qreal length)
{
    if (qFuzzyCompare(m_length, length))
        return;
    m_length = length;
    updateShape();
    emit lengthChanged();
}

qreal QB2CapsuleShape::radius() const
{
    return m_radius;
}

void QB2CapsuleShape::setRadius(qreal radius)
{
    if (qFuzzyCompare(m_radius, radius))
        return;
    m_radius = radius;
    updateShape();
    emit radiusChanged();
}

b2Capsule *QB2CapsuleShape::capsule()
{
    return &m_capsule;
}

void QB2CapsuleShape::updateShape()
{
    float halfLength = m_length / 2.0f;
    m_capsule.center1 = {0.0f, -halfLength};
    m_capsule.center2 = {0.0f, halfLength};
    m_capsule.radius = m_radius;
}

void QB2CapsuleShape::paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId)
{
    if (!renderingEnabled())
        return;

    b2Capsule capsule = b2Shape_GetCapsule(shapeId);

    qreal shrink = strokeWidth() / 2.0;
    qreal adjustedRadius = qMax(0.0, capsule.radius - shrink);

    QPointF c1(centroid.x() + capsule.center1.x, centroid.y() + capsule.center1.y);
    QPointF c2(centroid.x() + capsule.center2.x, centroid.y() + capsule.center2.y);

    qreal dx = c2.x() - c1.x();
    qreal dy = c2.y() - c1.y();
    qreal length = std::sqrt(dx * dx + dy * dy);

    if (length > 0)
    {
        qreal nx = -dy / length * adjustedRadius;
        qreal ny = dx / length * adjustedRadius;

        QPainterPath path;
        path.moveTo(c1.x() + nx, c1.y() + ny);
        path.lineTo(c2.x() + nx, c2.y() + ny);
        path.arcTo(QRectF(c2.x() - adjustedRadius, c2.y() - adjustedRadius,
                          adjustedRadius * 2, adjustedRadius * 2),
                   qRadiansToDegrees(std::atan2(ny, nx)), 180);
        path.lineTo(c1.x() - nx, c1.y() - ny);
        path.arcTo(QRectF(c1.x() - adjustedRadius, c1.y() - adjustedRadius,
                          adjustedRadius * 2, adjustedRadius * 2),
                   qRadiansToDegrees(std::atan2(-ny, -nx)), 180);
        path.closeSubpath();

        painter->setPen(Qt::NoPen);
        painter->setBrush(fillColor());
        painter->drawPath(path);

        if (strokeWidth() > 0)
        {
            QPen pen(strokeColor(), strokeWidth());
            painter->setPen(pen);
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(path);
        }
    }
    else
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(fillColor());
        painter->drawEllipse(c1, adjustedRadius, adjustedRadius);

        if (strokeWidth() > 0)
        {
            QPen pen(strokeColor(), strokeWidth());
            painter->setPen(pen);
            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(c1, adjustedRadius, adjustedRadius);
        }
    }
}
