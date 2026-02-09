#pragma once

#include "qb2shape.h"
#include <QVector>
#include <QPointF>

class QB2ChainShape : public QB2Shape
{
    Q_OBJECT
    Q_PROPERTY(QVector<QPointF> points READ points WRITE setPoints NOTIFY pointsChanged)
    Q_PROPERTY(bool isLoop READ isLoop WRITE setIsLoop NOTIFY isLoopChanged)

public:
    explicit QB2ChainShape(QObject *parent = nullptr);
    ~QB2ChainShape();

    QVector<QPointF> points() const;
    void setPoints(const QVector<QPointF> &points);

    bool isLoop() const { return m_isLoop; }
    void setIsLoop(bool loop);

    void paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId) override;

signals:
    void pointsChanged();
    void isLoopChanged();

private:
    QVector<QPointF> m_points;
    bool m_isLoop = false;
};
