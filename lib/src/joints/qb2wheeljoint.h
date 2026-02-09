#pragma once

#include "qb2joint.h"
#include <QPointF>

class QB2WheelJoint : public QB2Joint
{
    Q_OBJECT
    Q_PROPERTY(QPointF localAnchorA READ localAnchorA WRITE setLocalAnchorA NOTIFY localAnchorAChanged)
    Q_PROPERTY(QPointF localAnchorB READ localAnchorB WRITE setLocalAnchorB NOTIFY localAnchorBChanged)
    Q_PROPERTY(QPointF localAxisA READ localAxisA WRITE setLocalAxisA NOTIFY localAxisAChanged)
    Q_PROPERTY(bool enableSpring READ enableSpring WRITE setEnableSpring NOTIFY enableSpringChanged)
    Q_PROPERTY(qreal hertz READ hertz WRITE setHertz NOTIFY hertzChanged)
    Q_PROPERTY(qreal dampingRatio READ dampingRatio WRITE setDampingRatio NOTIFY dampingRatioChanged)
    Q_PROPERTY(bool enableLimit READ enableLimit WRITE setEnableLimit NOTIFY enableLimitChanged)
    Q_PROPERTY(qreal lowerTranslation READ lowerTranslation WRITE setLowerTranslation NOTIFY lowerTranslationChanged)
    Q_PROPERTY(qreal upperTranslation READ upperTranslation WRITE setUpperTranslation NOTIFY upperTranslationChanged)
    Q_PROPERTY(bool enableMotor READ enableMotor WRITE setEnableMotor NOTIFY enableMotorChanged)
    Q_PROPERTY(qreal motorSpeed READ motorSpeed WRITE setMotorSpeed NOTIFY motorSpeedChanged)
    Q_PROPERTY(qreal maxMotorTorque READ maxMotorTorque WRITE setMaxMotorTorque NOTIFY maxMotorTorqueChanged)

public:
    explicit QB2WheelJoint(QObject *parent = nullptr);
    ~QB2WheelJoint();

    QPointF localAnchorA() const { return m_localAnchorA; }
    void setLocalAnchorA(const QPointF &anchor);

    QPointF localAnchorB() const { return m_localAnchorB; }
    void setLocalAnchorB(const QPointF &anchor);

    QPointF localAxisA() const { return m_localAxisA; }
    void setLocalAxisA(const QPointF &axis);

    bool enableSpring() const { return m_enableSpring; }
    void setEnableSpring(bool enable);

    qreal hertz() const { return m_hertz; }
    void setHertz(qreal hertz);

    qreal dampingRatio() const { return m_dampingRatio; }
    void setDampingRatio(qreal ratio);

    bool enableLimit() const { return m_enableLimit; }
    void setEnableLimit(bool enable);

    qreal lowerTranslation() const { return m_lowerTranslation; }
    void setLowerTranslation(qreal translation);

    qreal upperTranslation() const { return m_upperTranslation; }
    void setUpperTranslation(qreal translation);

    bool enableMotor() const { return m_enableMotor; }
    void setEnableMotor(bool enable);

    qreal motorSpeed() const { return m_motorSpeed; }
    void setMotorSpeed(qreal speed);

    qreal maxMotorTorque() const { return m_maxMotorTorque; }
    void setMaxMotorTorque(qreal torque);

signals:
    void localAnchorAChanged();
    void localAnchorBChanged();
    void localAxisAChanged();
    void enableSpringChanged();
    void hertzChanged();
    void dampingRatioChanged();
    void enableLimitChanged();
    void lowerTranslationChanged();
    void upperTranslationChanged();
    void enableMotorChanged();
    void motorSpeedChanged();
    void maxMotorTorqueChanged();

protected:
    void createJoint() override;

private:
    QPointF m_localAnchorA{0.0, 0.0};
    QPointF m_localAnchorB{0.0, 0.0};
    QPointF m_localAxisA{0.0, 1.0};
    bool m_enableSpring = true;
    qreal m_hertz = 1.0;
    qreal m_dampingRatio = 0.7;
    bool m_enableLimit = false;
    qreal m_lowerTranslation = 0.0;
    qreal m_upperTranslation = 0.0;
    bool m_enableMotor = false;
    qreal m_motorSpeed = 0.0;
    qreal m_maxMotorTorque = 0.0;
};
