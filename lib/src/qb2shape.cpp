#include "qb2shape.h"
#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QtMath>
#include <cmath>

QB2Shape::QB2Shape(QObject *parent)
    : QObject(parent)
{
}

QB2Shape::~QB2Shape()
{
}

void QB2Shape::setFillColor(const QColor &color)
{
    if (m_fillColor == color)
        return;
    m_fillColor = color;
    emit fillColorChanged();
}

void QB2Shape::setStrokeColor(const QColor &color)
{
    if (m_strokeColor == color)
        return;
    m_strokeColor = color;
    emit strokeColorChanged();
}

void QB2Shape::setStrokeWidth(qreal width)
{
    if (qFuzzyCompare(m_strokeWidth, width))
        return;
    m_strokeWidth = width;
    emit strokeWidthChanged();
}

void QB2Shape::setRenderingEnabled(bool enabled)
{
    if (m_renderingEnabled == enabled)
        return;
    m_renderingEnabled = enabled;
    emit renderingEnabledChanged();
}

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

// Circle Shape
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

// Capsule Shape
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

// Segment Shape
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

// Polygon Shape
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
