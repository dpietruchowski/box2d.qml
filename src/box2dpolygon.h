#ifndef BOX2DPOLYGON_H
#define BOX2DPOLYGON_H

#include "box2dfixture.h"
#include <QVariantList>

class Box2DPolygon : public Box2DFixture
{
    Q_OBJECT
    Q_PROPERTY(QVariantList vertices READ vertices WRITE setVertices NOTIFY verticesChanged)

public:
    explicit Box2DPolygon(QQuickItem *parent = nullptr);

    QVariantList vertices() const;
    void setVertices(const QVariantList &vertices);

signals:
    void verticesChanged();

protected:
    b2Shape *createShape() override;

private:
    QVariantList m_vertices;
};

#endif // BOX2DPOLYGON_H
