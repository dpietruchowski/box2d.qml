#ifndef BOX2DBODY_H
#define BOX2DBODY_H

#include <QQuickItem>
#include <box2d/box2d.h>

class Box2DWorld;
class Box2DFixture;

namespace Body {
Q_NAMESPACE
enum BodyType {
    Static = 0,
    Kinematic = 1,
    Dynamic = 2
};
Q_ENUM_NS(BodyType)
}

class Box2DBody : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Box2DWorld* world READ world WRITE setWorld NOTIFY worldChanged)
    Q_PROPERTY(qreal linearDamping READ linearDamping WRITE setLinearDamping NOTIFY linearDampingChanged)
    Q_PROPERTY(qreal angularDamping READ angularDamping WRITE setAngularDamping NOTIFY angularDampingChanged)
    Q_PROPERTY(Body::BodyType bodyType READ bodyType WRITE setBodyType NOTIFY bodyTypeChanged)
    Q_PROPERTY(bool bullet READ bullet WRITE setBullet NOTIFY bulletChanged)
    Q_PROPERTY(bool sleepingAllowed READ sleepingAllowed WRITE setSleepingAllowed NOTIFY sleepingAllowedChanged)
    Q_PROPERTY(bool fixedRotation READ fixedRotation WRITE setFixedRotation NOTIFY fixedRotationChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool awake READ awake WRITE setAwake NOTIFY awakeChanged)
    Q_PROPERTY(qreal gravityScale READ gravityScale WRITE setGravityScale NOTIFY gravityScaleChanged)
    Q_PROPERTY(QPointF linearVelocity READ linearVelocity WRITE setLinearVelocity NOTIFY linearVelocityChanged)
    Q_PROPERTY(qreal angularVelocity READ angularVelocity WRITE setAngularVelocity NOTIFY angularVelocityChanged)

public:
    explicit Box2DBody(QQuickItem *parent = nullptr);
    ~Box2DBody() override;

    Box2DWorld *world() const;
    void setWorld(Box2DWorld *world);

    qreal linearDamping() const;
    void setLinearDamping(qreal damping);

    qreal angularDamping() const;
    void setAngularDamping(qreal damping);

    Body::BodyType bodyType() const;
    void setBodyType(Body::BodyType type);

    bool bullet() const;
    void setBullet(bool bullet);

    bool sleepingAllowed() const;
    void setSleepingAllowed(bool allowed);

    bool fixedRotation() const;
    void setFixedRotation(bool fixedRotation);

    bool active() const;
    void setActive(bool active);

    bool awake() const;
    void setAwake(bool awake);

    qreal gravityScale() const;
    void setGravityScale(qreal scale);

    QPointF linearVelocity() const;
    void setLinearVelocity(const QPointF &velocity);

    qreal angularVelocity() const;
    void setAngularVelocity(qreal velocity);

    b2Body *body() { return m_body; }

    Q_INVOKABLE void applyForce(const QPointF &force, const QPointF &point);
    Q_INVOKABLE void applyForceToCenter(const QPointF &force);
    Q_INVOKABLE void applyTorque(qreal torque);
    Q_INVOKABLE void applyLinearImpulse(const QPointF &impulse, const QPointF &point);
    Q_INVOKABLE void applyAngularImpulse(qreal impulse);

    void synchronize();
    void addFixture(Box2DFixture *fixture);
    void removeFixture(Box2DFixture *fixture);

signals:
    void worldChanged();
    void linearDampingChanged();
    void angularDampingChanged();
    void bodyTypeChanged();
    void bulletChanged();
    void sleepingAllowedChanged();
    void fixedRotationChanged();
    void activeChanged();
    void awakeChanged();
    void gravityScaleChanged();
    void linearVelocityChanged();
    void angularVelocityChanged();

protected:
    void componentComplete() override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void createBody();
    void destroyBody();
    b2Vec2 toMeters(const QPointF &point) const;
    QPointF toPixels(const b2Vec2 &vec) const;
    qreal toMeters(qreal value) const;
    qreal toPixels(qreal value) const;

    Box2DWorld *m_world;
    b2Body *m_body;
    qreal m_linearDamping;
    qreal m_angularDamping;
    Body::BodyType m_bodyType;
    bool m_bullet;
    bool m_sleepingAllowed;
    bool m_fixedRotation;
    bool m_active;
    bool m_awake;
    qreal m_gravityScale;
    bool m_synchronizing;
    QList<Box2DFixture*> m_fixtures;
};

#endif // BOX2DBODY_H
