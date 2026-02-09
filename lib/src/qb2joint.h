#pragma once

#include <QObject>
#include <QQmlParserStatus>
#include <QPointF>
#include <box2d/box2d.h>

class QB2Body;
class QB2World;

class QB2Joint : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QB2Body *bodyA READ bodyA WRITE setBodyA NOTIFY bodyAChanged)
    Q_PROPERTY(QB2Body *bodyB READ bodyB WRITE setBodyB NOTIFY bodyBChanged)
    Q_PROPERTY(bool collideConnected READ collideConnected WRITE setCollideConnected NOTIFY collideConnectedChanged)

public:
    explicit QB2Joint(QObject *parent = nullptr);
    virtual ~QB2Joint();

    QB2Body *bodyA() const { return m_bodyA; }
    void setBodyA(QB2Body *body);

    QB2Body *bodyB() const { return m_bodyB; }
    void setBodyB(QB2Body *body);

    bool collideConnected() const { return m_collideConnected; }
    void setCollideConnected(bool collide);

    b2JointId jointId() const { return m_jointId; }
    void setWorld(QB2World *world);

    void classBegin() override;
    void componentComplete() override;

signals:
    void bodyAChanged();
    void bodyBChanged();
    void collideConnectedChanged();
    void jointReady();

protected:
    virtual void createJoint() = 0;

    b2JointId m_jointId;
    QB2World *m_world = nullptr;
    QB2Body *m_bodyA = nullptr;
    QB2Body *m_bodyB = nullptr;
    bool m_collideConnected = false;
    bool m_componentComplete = false;
};

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

class QB2DistanceJoint : public QB2Joint
{
    Q_OBJECT
    Q_PROPERTY(QPointF localAnchorA READ localAnchorA WRITE setLocalAnchorA NOTIFY localAnchorAChanged)
    Q_PROPERTY(QPointF localAnchorB READ localAnchorB WRITE setLocalAnchorB NOTIFY localAnchorBChanged)
    Q_PROPERTY(qreal length READ length WRITE setLength NOTIFY lengthChanged)
    Q_PROPERTY(qreal minLength READ minLength WRITE setMinLength NOTIFY minLengthChanged)
    Q_PROPERTY(qreal maxLength READ maxLength WRITE setMaxLength NOTIFY maxLengthChanged)
    Q_PROPERTY(qreal hertz READ hertz WRITE setHertz NOTIFY hertzChanged)
    Q_PROPERTY(qreal dampingRatio READ dampingRatio WRITE setDampingRatio NOTIFY dampingRatioChanged)

public:
    explicit QB2DistanceJoint(QObject *parent = nullptr);
    ~QB2DistanceJoint();

    QPointF localAnchorA() const { return m_localAnchorA; }
    void setLocalAnchorA(const QPointF &anchor);

    QPointF localAnchorB() const { return m_localAnchorB; }
    void setLocalAnchorB(const QPointF &anchor);

    qreal length() const { return m_length; }
    void setLength(qreal length);

    qreal minLength() const { return m_minLength; }
    void setMinLength(qreal length);

    qreal maxLength() const { return m_maxLength; }
    void setMaxLength(qreal length);

    qreal hertz() const { return m_hertz; }
    void setHertz(qreal hertz);

    qreal dampingRatio() const { return m_dampingRatio; }
    void setDampingRatio(qreal ratio);

signals:
    void localAnchorAChanged();
    void localAnchorBChanged();
    void lengthChanged();
    void minLengthChanged();
    void maxLengthChanged();
    void hertzChanged();
    void dampingRatioChanged();

protected:
    void createJoint() override;

private:
    QPointF m_localAnchorA{0.0, 0.0};
    QPointF m_localAnchorB{0.0, 0.0};
    qreal m_length = 1.0;
    qreal m_minLength = 0.0;
    qreal m_maxLength = std::numeric_limits<float>::max();
    qreal m_hertz = 0.0;
    qreal m_dampingRatio = 0.0;
};
