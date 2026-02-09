#pragma once

#include "qb2joint.h"
#include <QPointF>

class QB2PrismaticJoint : public QB2Joint
{
    Q_OBJECT
    Q_PROPERTY(QPointF localAnchorA READ localAnchorA WRITE setLocalAnchorA NOTIFY localAnchorAChanged)
    Q_PROPERTY(QPointF localAnchorB READ localAnchorB WRITE setLocalAnchorB NOTIFY localAnchorBChanged)
    Q_PROPERTY(QPointF localAxisA READ localAxisA WRITE setLocalAxisA NOTIFY localAxisAChanged)
    Q_PROPERTY(qreal referenceAngle READ referenceAngle WRITE setReferenceAngle NOTIFY referenceAngleChanged)
    Q_PROPERTY(bool enableLimit READ enableLimit WRITE setEnableLimit NOTIFY enableLimitChanged)
    Q_PROPERTY(qreal lowerTranslation READ lowerTranslation WRITE setLowerTranslation NOTIFY lowerTranslationChanged)
    Q_PROPERTY(qreal upperTranslation READ upperTranslation WRITE setUpperTranslation NOTIFY upperTranslationChanged)
    Q_PROPERTY(bool enableMotor READ enableMotor WRITE setEnableMotor NOTIFY enableMotorChanged)
    Q_PROPERTY(qreal motorSpeed READ motorSpeed WRITE setMotorSpeed NOTIFY motorSpeedChanged)
    Q_PROPERTY(qreal maxMotorForce READ maxMotorForce WRITE setMaxMotorForce NOTIFY maxMotorForceChanged)

public:
    explicit QB2PrismaticJoint(QObject *parent = nullptr);
    ~QB2PrismaticJoint();

    QPointF localAnchorA() const { return m_localAnchorA; }
    void setLocalAnchorA(const QPointF &anchor);

    QPointF localAnchorB() const { return m_localAnchorB; }
    void setLocalAnchorB(const QPointF &anchor);

    QPointF localAxisA() const { return m_localAxisA; }
    void setLocalAxisA(const QPointF &axis);

    qreal referenceAngle() const { return m_referenceAngle; }
    void setReferenceAngle(qreal angle);

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

    qreal maxMotorForce() const { return m_maxMotorForce; }
    void setMaxMotorForce(qreal force);

signals:
    void localAnchorAChanged();
    void localAnchorBChanged();
    void localAxisAChanged();
    void referenceAngleChanged();
    void enableLimitChanged();
    void lowerTranslationChanged();
    void upperTranslationChanged();
    void enableMotorChanged();
    void motorSpeedChanged();
    void maxMotorForceChanged();

protected:
    void createJoint() override;

private:
    QPointF m_localAnchorA{0.0, 0.0};
    QPointF m_localAnchorB{0.0, 0.0};
    QPointF m_localAxisA{1.0, 0.0};
    qreal m_referenceAngle = 0.0;
    bool m_enableLimit = false;
    qreal m_lowerTranslation = 0.0;
    qreal m_upperTranslation = 0.0;
    bool m_enableMotor = false;
    qreal m_motorSpeed = 0.0;
    qreal m_maxMotorForce = 0.0;
};
