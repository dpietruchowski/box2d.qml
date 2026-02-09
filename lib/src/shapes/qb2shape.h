#pragma once

#include <QObject>
#include <QColor>
#include <QPainter>
#include <QPointF>
#include <box2d/box2d.h>

class QB2Shape : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(QColor strokeColor READ strokeColor WRITE setStrokeColor NOTIFY strokeColorChanged)
    Q_PROPERTY(qreal strokeWidth READ strokeWidth WRITE setStrokeWidth NOTIFY strokeWidthChanged)
    Q_PROPERTY(bool renderingEnabled READ renderingEnabled WRITE setRenderingEnabled NOTIFY renderingEnabledChanged)

public:
    explicit QB2Shape(QObject *parent = nullptr);
    virtual ~QB2Shape();

    QColor fillColor() const { return m_fillColor; }
    void setFillColor(const QColor &color);

    QColor strokeColor() const { return m_strokeColor; }
    void setStrokeColor(const QColor &color);

    qreal strokeWidth() const { return m_strokeWidth; }
    void setStrokeWidth(qreal width);

    bool renderingEnabled() const { return m_renderingEnabled; }
    void setRenderingEnabled(bool enabled);

    virtual b2Polygon *polygon() { return nullptr; }
    virtual b2Circle *circle() { return nullptr; }
    virtual b2Capsule *capsule() { return nullptr; }
    virtual b2Segment *segment() { return nullptr; }

    virtual void paint(QPainter *painter, const QPointF &centroid, b2ShapeId shapeId) = 0;

signals:
    void fillColorChanged();
    void strokeColorChanged();
    void strokeWidthChanged();
    void renderingEnabledChanged();

private:
    QColor m_fillColor{0x9C, 0x27, 0xB0};
    QColor m_strokeColor{Qt::white};
    qreal m_strokeWidth{2.0};
    bool m_renderingEnabled{true};
};
