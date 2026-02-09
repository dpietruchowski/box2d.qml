#pragma once

#include "qb2joint.h"
#include <QPointF>

class QB2MotorJoint : public QB2Joint
{
    Q_OBJECT
    Q_PROPERTY(QPointF linearVelocity READ linearVelocity WRITE setLinearVelocity NOTIFY linearVelocityChanged)
    Q_PROPERTY(qreal maxVelocityForce READ maxVelocityForce WRITE setMaxVelocityForce NOTIFY maxVelocityForceChanged)
    Q_PROPERTY(qreal angularVelocity READ angularVelocity WRITE setAngularVelocity NOTIFY angularVelocityChanged)
    Q_PROPERTY(qreal maxVelocityTorque READ maxVelocityTorque WRITE setMaxVelocityTorque NOTIFY maxVelocityTorqueChanged)
    Q_PROPERTY(qreal linearHertz READ linearHertz WRITE setLinearHertz NOTIFY linearHertzChanged)
    Q_PROPERTY(qreal linearDampingRatio READ linearDampingRatio WRITE setLinearDampingRatio NOTIFY linearDampingRatioChanged)
    Q_PROPERTY(qreal maxSpringForce READ maxSpringForce WRITE setMaxSpringForce NOTIFY maxSpringForceChanged)
    Q_PROPERTY(qreal angularHertz READ angularHertz WRITE setAngularHertz NOTIFY angularHertzChanged)
    Q_PROPERTY(qreal angularDampingRatio READ angularDampingRatio WRITE setAngularDampingRatio NOTIFY angularDampingRatioChanged)
    Q_PROPERTY(qreal maxSpringTorque READ maxSpringTorque WRITE setMaxSpringTorque NOTIFY maxSpringTorqueChanged)

public:
    explicit QB2MotorJoint(QObject *parent = nullptr);
    ~QB2MotorJoint();

    QPointF linearVelocity() const { return m_linearVelocity; }
    void setLinearVelocity(const QPointF &velocity);

    qreal maxVelocityForce() const { return m_maxVelocityForce; }
    void setMaxVelocityForce(qreal force);

    qreal angularVelocity() const { return m_angularVelocity; }
    void setAngularVelocity(qreal velocity);

    qreal maxVelocityTorque() const { return m_maxVelocityTorque; }
    void setMaxVelocityTorque(qreal torque);

    qreal linearHertz() const { return m_linearHertz; }
    void setLinearHertz(qreal hertz);

    qreal linearDampingRatio() const { return m_linearDampingRatio; }
    void setLinearDampingRatio(qreal ratio);

    qreal maxSpringForce() const { return m_maxSpringForce; }
    void setMaxSpringForce(qreal force);

    qreal angularHertz() const { return m_angularHertz; }
    void setAngularHertz(qreal hertz);

    qreal angularDampingRatio() const { return m_angularDampingRatio; }
    void setAngularDampingRatio(qreal ratio);

    qreal maxSpringTorque() const { return m_maxSpringTorque; }
    void setMaxSpringTorque(qreal torque);

signals:
    void linearVelocityChanged();
    void maxVelocityForceChanged();
    void angularVelocityChanged();
    void maxVelocityTorqueChanged();
    void linearHertzChanged();
    void linearDampingRatioChanged();
    void maxSpringForceChanged();
    void angularHertzChanged();
    void angularDampingRatioChanged();
    void maxSpringTorqueChanged();

protected:
    void createJoint() override;

private:
    QPointF m_linearVelocity{0.0, 0.0};
    qreal m_maxVelocityForce = 1.0;
    qreal m_angularVelocity = 0.0;
    qreal m_maxVelocityTorque = 1.0;
    qreal m_linearHertz = 0.0;
    qreal m_linearDampingRatio = 1.0;
    qreal m_maxSpringForce = 0.0;
    qreal m_angularHertz = 0.0;
    qreal m_angularDampingRatio = 1.0;
    qreal m_maxSpringTorque = 0.0;
};
