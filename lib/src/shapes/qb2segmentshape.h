#pragma once

#include "qb2shape.h"
#include <QPointF>

class QB2SegmentShape : public QB2Shape
{
    Q_OBJECT
    Q_PROPERTY(QPointF point1 READ point1 WRITE setPoint1 NOTIFY point1Changed)
    Q_PROPERTY(QPointF point2 READ point2 WRITE setPoint2 NOTIFY point2Changed)

public:
    explicit QB2SegmentShape(QObject *parent = nullptr);
    ~QB2SegmentShape();

    QPointF point1() const;
    void setPoint1(const QPointF &point);

    QPointF point2() const;
    void setPoint2(const QPointF &point);

    b2Segment *segment() override;
    void paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId) override;

signals:
    void point1Changed();
    void point2Changed();

private:
    void updateShape();

    b2Segment m_segment;
    QPointF m_point1{-1.0, 0.0};
    QPointF m_point2{1.0, 0.0};
};
