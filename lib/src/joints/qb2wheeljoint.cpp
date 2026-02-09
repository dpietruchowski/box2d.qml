#include "qb2wheeljoint.h"
#include "../qb2body.h"
#include "../qb2world.h"
#include <cmath>
#include <QDebug>

QB2WheelJoint::QB2WheelJoint(QObject *parent)
    : QB2Joint(parent)
{
}

QB2WheelJoint::~QB2WheelJoint()
{
}

void QB2WheelJoint::setLocalAnchorA(const QPointF &anchor)
{
    if (m_localAnchorA == anchor)
        return;
    m_localAnchorA = anchor;
    emit localAnchorAChanged();
}

void QB2WheelJoint::setLocalAnchorB(const QPointF &anchor)
{
    if (m_localAnchorB == anchor)
        return;
    m_localAnchorB = anchor;
    emit localAnchorBChanged();
}

void QB2WheelJoint::setLocalAxisA(const QPointF &axis)
{
    if (m_localAxisA == axis)
        return;
    m_localAxisA = axis;
    emit localAxisAChanged();
}

void QB2WheelJoint::setEnableSpring(bool enable)
{
    if (m_enableSpring == enable)
        return;
    m_enableSpring = enable;
    if (b2Joint_IsValid(m_jointId))
        b2WheelJoint_EnableSpring(m_jointId, enable);
    emit enableSpringChanged();
}

void QB2WheelJoint::setHertz(qreal hertz)
{
    if (qFuzzyCompare(m_hertz, hertz))
        return;
    m_hertz = hertz;
    if (b2Joint_IsValid(m_jointId))
        b2WheelJoint_SetSpringHertz(m_jointId, hertz);
    emit hertzChanged();
}

void QB2WheelJoint::setDampingRatio(qreal ratio)
{
    if (qFuzzyCompare(m_dampingRatio, ratio))
        return;
    m_dampingRatio = ratio;
    if (b2Joint_IsValid(m_jointId))
        b2WheelJoint_SetSpringDampingRatio(m_jointId, ratio);
    emit dampingRatioChanged();
}

void QB2WheelJoint::setEnableLimit(bool enable)
{
    if (m_enableLimit == enable)
        return;
    m_enableLimit = enable;
    if (b2Joint_IsValid(m_jointId))
        b2WheelJoint_EnableLimit(m_jointId, enable);
    emit enableLimitChanged();
}

void QB2WheelJoint::setLowerTranslation(qreal translation)
{
    if (qFuzzyCompare(m_lowerTranslation, translation))
        return;
    m_lowerTranslation = translation;
    if (b2Joint_IsValid(m_jointId))
        b2WheelJoint_SetLimits(m_jointId, m_lowerTranslation, m_upperTranslation);
    emit lowerTranslationChanged();
}

void QB2WheelJoint::setUpperTranslation(qreal translation)
{
    if (qFuzzyCompare(m_upperTranslation, translation))
        return;
    m_upperTranslation = translation;
    if (b2Joint_IsValid(m_jointId))
        b2WheelJoint_SetLimits(m_jointId, m_lowerTranslation, m_upperTranslation);
    emit upperTranslationChanged();
}

void QB2WheelJoint::setEnableMotor(bool enable)
{
    if (m_enableMotor == enable)
        return;
    m_enableMotor = enable;
    if (b2Joint_IsValid(m_jointId))
        b2WheelJoint_EnableMotor(m_jointId, enable);
    emit enableMotorChanged();
}

void QB2WheelJoint::setMotorSpeed(qreal speed)
{
    if (qFuzzyCompare(m_motorSpeed, speed))
        return;
    m_motorSpeed = speed;
    if (b2Joint_IsValid(m_jointId))
    {
        b2WheelJoint_SetMotorSpeed(m_jointId, speed);
        b2Joint_WakeBodies(m_jointId);
    }
    emit motorSpeedChanged();
}

void QB2WheelJoint::setMaxMotorTorque(qreal torque)
{
    if (qFuzzyCompare(m_maxMotorTorque, torque))
        return;
    m_maxMotorTorque = torque;
    if (b2Joint_IsValid(m_jointId))
        b2WheelJoint_SetMaxMotorTorque(m_jointId, torque);
    emit maxMotorTorqueChanged();
}

void QB2WheelJoint::createJoint()
{
    if (!m_componentComplete || !m_world || !m_bodyA || !m_bodyB)
        return;

    if (b2Joint_IsValid(m_jointId))
        return;

    if (!b2Body_IsValid(m_bodyA->bodyId()) || !b2Body_IsValid(m_bodyB->bodyId()))
        return;

    b2WheelJointDef jointDef = b2DefaultWheelJointDef();

    b2Vec2 localAnchorA = {static_cast<float>(m_localAnchorA.x()), static_cast<float>(m_localAnchorA.y())};
    b2Vec2 localAnchorB = {static_cast<float>(m_localAnchorB.x()), static_cast<float>(m_localAnchorB.y())};

    float angle = std::atan2(static_cast<float>(m_localAxisA.y()), static_cast<float>(m_localAxisA.x()));
    b2Rot axisRotation = b2MakeRot(angle);

    jointDef.base.bodyIdA = m_bodyA->bodyId();
    jointDef.base.bodyIdB = m_bodyB->bodyId();
    jointDef.base.localFrameA.p = localAnchorA;
    jointDef.base.localFrameA.q = axisRotation;
    jointDef.base.localFrameB.p = localAnchorB;
    jointDef.base.localFrameB.q = b2Rot_identity;
    jointDef.base.collideConnected = m_collideConnected;

    jointDef.enableSpring = m_enableSpring;
    jointDef.hertz = m_hertz;
    jointDef.dampingRatio = m_dampingRatio;
    jointDef.enableLimit = m_enableLimit;
    jointDef.lowerTranslation = m_lowerTranslation;
    jointDef.upperTranslation = m_upperTranslation;
    jointDef.enableMotor = m_enableMotor;
    jointDef.motorSpeed = m_motorSpeed;
    jointDef.maxMotorTorque = m_maxMotorTorque;

    m_jointId = b2CreateWheelJoint(m_world->worldId(), &jointDef);

    qDebug() << "WheelJoint created:"
             << "enableMotor=" << m_enableMotor
             << "motorSpeed=" << m_motorSpeed
             << "maxMotorTorque=" << m_maxMotorTorque
             << "localAxisA=" << m_localAxisA;

    // Wake bodies so motor starts working immediately
    b2Joint_WakeBodies(m_jointId);

    emit jointReady();
}
