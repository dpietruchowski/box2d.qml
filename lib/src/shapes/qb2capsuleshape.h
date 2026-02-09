#pragma once

#include "qb2shape.h"

class QB2CapsuleShape : public QB2Shape
{
    Q_OBJECT
    Q_PROPERTY(qreal length READ length WRITE setLength NOTIFY lengthChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    explicit QB2CapsuleShape(QObject *parent = nullptr);
    ~QB2CapsuleShape();

    qreal length() const;
    void setLength(qreal length);

    qreal radius() const;
    void setRadius(qreal radius);

    b2Capsule *capsule() override;
    void paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId) override;

signals:
    void lengthChanged();
    void radiusChanged();

private:
    void updateShape();

    b2Capsule m_capsule;
    qreal m_length = 1.0;
    qreal m_radius = 0.5;
};
