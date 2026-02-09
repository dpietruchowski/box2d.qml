#pragma once

#include "qb2shape.h"

class QB2CircleShape : public QB2Shape
{
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    explicit QB2CircleShape(QObject *parent = nullptr);
    ~QB2CircleShape();

    qreal radius() const;
    void setRadius(qreal radius);

    b2Circle *circle() override;
    void paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId) override;

signals:
    void radiusChanged();

private:
    b2Circle m_circle;
    qreal m_radius = 1.0;
};
