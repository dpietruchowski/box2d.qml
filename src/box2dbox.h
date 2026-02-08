#ifndef BOX2DBOX_H
#define BOX2DBOX_H

#include "box2dfixture.h"

class Box2DBox : public Box2DFixture
{
    Q_OBJECT

public:
    explicit Box2DBox(QQuickItem *parent = nullptr);

protected:
    b2Shape *createShape() override;
};

#endif // BOX2DBOX_H
