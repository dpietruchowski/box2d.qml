#pragma once

#include "qb2shape.h"

class QB2BoxShape : public QB2Shape
{
    Q_OBJECT
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)

public:
    explicit QB2BoxShape(QObject *parent = nullptr);
    ~QB2BoxShape();

    qreal width() const;
    void setWidth(qreal width);

    qreal height() const;
    void setHeight(qreal height);

    b2Polygon *polygon() override;
    void paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId) override;

signals:
    void widthChanged();
    void heightChanged();

private:
    void updateShape();

    b2Polygon m_polygon;
    qreal m_width = 1.0;
    qreal m_height = 1.0;
};
