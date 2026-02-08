#pragma once

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QColor>
#include <QPainter>
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
