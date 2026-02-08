#ifndef BOX2DCIRCLE_H
#define BOX2DCIRCLE_H

#include "box2dfixture.h"

class Box2DCircle : public Box2DFixture
{
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    explicit Box2DCircle(QQuickItem *parent = nullptr);

    qreal radius() const;
    void setRadius(qreal radius);

signals:
    void radiusChanged();

protected:
    b2Shape *createShape() override;

private:
    qreal m_radius;
};

#endif // BOX2DCIRCLE_H
