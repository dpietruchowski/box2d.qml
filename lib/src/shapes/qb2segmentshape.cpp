#include "qb2segmentshape.h"
#include <QPen>

QB2SegmentShape::QB2SegmentShape(QObject *parent)
    : QB2Shape(parent)
{
    updateShape();
}

QB2SegmentShape::~QB2SegmentShape()
{
}

QPointF QB2SegmentShape::point1() const
{
    return m_point1;
}

void QB2SegmentShape::setPoint1(const QPointF &point)
{
    if (m_point1 == point)
        return;
    m_point1 = point;
    updateShape();
    emit point1Changed();
}

QPointF QB2SegmentShape::point2() const
{
    return m_point2;
}

void QB2SegmentShape::setPoint2(const QPointF &point)
{
    if (m_point2 == point)
        return;
    m_point2 = point;
    updateShape();
    emit point2Changed();
}

b2Segment *QB2SegmentShape::segment()
{
    return &m_segment;
}

void QB2SegmentShape::updateShape()
{
    m_segment.point1 = {static_cast<float>(m_point1.x()), static_cast<float>(m_point1.y())};
    m_segment.point2 = {static_cast<float>(m_point2.x()), static_cast<float>(m_point2.y())};
}

void QB2SegmentShape::paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId)
{
    if (!renderingEnabled())
        return;

    b2Segment segment = b2Shape_GetSegment(shapeId);
    QPointF p1(centroid.x() + segment.point1.x,
               centroid.y() + segment.point1.y);
    QPointF p2(centroid.x() + segment.point2.x,
               centroid.y() + segment.point2.y);

    QPen pen(strokeColor(), strokeWidth());
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawLine(p1, p2);
}
