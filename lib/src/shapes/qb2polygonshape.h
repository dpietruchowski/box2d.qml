#pragma once

#include "qb2shape.h"
#include <QVector>
#include <QPointF>

class QB2PolygonShape : public QB2Shape
{
    Q_OBJECT
    Q_PROPERTY(QVector<QPointF> vertices READ vertices WRITE setVertices NOTIFY verticesChanged)

public:
    explicit QB2PolygonShape(QObject *parent = nullptr);
    ~QB2PolygonShape();

    QVector<QPointF> vertices() const;
    void setVertices(const QVector<QPointF> &vertices);

    b2Polygon *polygon() override;
    void paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId) override;

signals:
    void verticesChanged();

private:
    void updateShape();

    b2Polygon m_polygon;
    QVector<QPointF> m_vertices;
};
