#include "qb2chainshape.h"
#include <QPen>
#include <QBrush>

QB2ChainShape::QB2ChainShape(QObject *parent)
    : QB2Shape(parent)
{
}

QB2ChainShape::~QB2ChainShape()
{
}

QVector<QPointF> QB2ChainShape::points() const
{
    return m_points;
}

void QB2ChainShape::setPoints(const QVector<QPointF> &points)
{
    if (m_points == points)
        return;
    m_points = points;
    emit pointsChanged();
}

void QB2ChainShape::setIsLoop(bool loop)
{
    if (m_isLoop == loop)
        return;
    m_isLoop = loop;
    emit isLoopChanged();
}

void QB2ChainShape::paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId)
{
    if (!renderingEnabled() || m_points.size() < 2)
        return;

    QPen pen(strokeColor(), strokeWidth());
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    for (int i = 0; i < m_points.size() - 1; ++i)
    {
        QPointF p1(centroid.x() + m_points[i].x(), centroid.y() + m_points[i].y());
        QPointF p2(centroid.x() + m_points[i + 1].x(), centroid.y() + m_points[i + 1].y());
        painter->drawLine(p1, p2);
    }

    if (m_isLoop && m_points.size() > 2)
    {
        QPointF p1(centroid.x() + m_points.last().x(), centroid.y() + m_points.last().y());
        QPointF p2(centroid.x() + m_points.first().x(), centroid.y() + m_points.first().y());
        painter->drawLine(p1, p2);
    }
}
