#pragma once

#include "qb2joint.h"
#include <QPointF>

class QB2RevoluteJoint : public QB2Joint
{
    Q_OBJECT
    Q_PROPERTY(QPointF localAnchorA READ localAnchorA WRITE setLocalAnchorA NOTIFY localAnchorAChanged)
    Q_PROPERTY(QPointF localAnchorB READ localAnchorB WRITE setLocalAnchorB NOTIFY localAnchorBChanged)
    Q_PROPERTY(qreal referenceAngle READ referenceAngle WRITE setReferenceAngle NOTIFY referenceAngleChanged)
    Q_PROPERTY(bool enableLimit READ enableLimit WRITE setEnableLimit NOTIFY enableLimitChanged)
    Q_PROPERTY(qreal lowerAngle READ lowerAngle WRITE setLowerAngle NOTIFY lowerAngleChanged)
    Q_PROPERTY(qreal upperAngle READ upperAngle WRITE setUpperAngle NOTIFY upperAngleChanged)
    Q_PROPERTY(bool enableMotor READ enableMotor WRITE setEnableMotor NOTIFY enableMotorChanged)
    Q_PROPERTY(qreal motorSpeed READ motorSpeed WRITE setMotorSpeed NOTIFY motorSpeedChanged)
    Q_PROPERTY(qreal maxMotorTorque READ maxMotorTorque WRITE setMaxMotorTorque NOTIFY maxMotorTorqueChanged)

public:
    explicit QB2RevoluteJoint(QObject *parent = nullptr);
    ~QB2RevoluteJoint();

    QPointF localAnchorA() const { return m_localAnchorA; }
    void setLocalAnchorA(const QPointF &anchor);

    QPointF localAnchorB() const { return m_localAnchorB; }
    void setLocalAnchorB(const QPointF &anchor);

    qreal referenceAngle() const { return m_referenceAngle; }
    void setReferenceAngle(qreal angle);

    bool enableLimit() const { return m_enableLimit; }
    void setEnableLimit(bool enable);

    qreal lowerAngle() const { return m_lowerAngle; }
    void setLowerAngle(qreal angle);

    qreal upperAngle() const { return m_upperAngle; }
    void setUpperAngle(qreal angle);

    bool enableMotor() const { return m_enableMotor; }
    void setEnableMotor(bool enable);

    qreal motorSpeed() const { return m_motorSpeed; }
    void setMotorSpeed(qreal speed);

    qreal maxMotorTorque() const { return m_maxMotorTorque; }
    void setMaxMotorTorque(qreal torque);

signals:
    void localAnchorAChanged();
    void localAnchorBChanged();
    void referenceAngleChanged();
    void enableLimitChanged();
    void lowerAngleChanged();
    void upperAngleChanged();
    void enableMotorChanged();
    void motorSpeedChanged();
    void maxMotorTorqueChanged();

protected:
    void createJoint() override;

private:
    QPointF m_localAnchorA{0.0, 0.0};
    QPointF m_localAnchorB{0.0, 0.0};
    qreal m_referenceAngle = 0.0;
    bool m_enableLimit = false;
    qreal m_lowerAngle = 0.0;
    qreal m_upperAngle = 0.0;
    bool m_enableMotor = false;
    qreal m_motorSpeed = 0.0;
    qreal m_maxMotorTorque = 0.0;
};
