#ifndef BOX2DCONTACT_H
#define BOX2DCONTACT_H

#include <QObject>
#include <QPointF>
#include <box2d/box2d.h>

class Box2DFixture;

class Box2DContact : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Box2DFixture* fixtureA READ fixtureA CONSTANT)
    Q_PROPERTY(Box2DFixture* fixtureB READ fixtureB CONSTANT)
    Q_PROPERTY(bool touching READ touching CONSTANT)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)

public:
    explicit Box2DContact(b2Contact *contact, qreal pixelsPerMeter, QObject *parent = nullptr);

    Box2DFixture *fixtureA() const;
    Box2DFixture *fixtureB() const;
    bool touching() const;
    bool enabled() const;
    void setEnabled(bool enabled);

private:
    b2Contact *m_contact;
    qreal m_pixelsPerMeter;
};

#endif // BOX2DCONTACT_H
