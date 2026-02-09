#include "qb2revolutejoint.h"
#include "../qb2body.h"
#include "../qb2world.h"

QB2RevoluteJoint::QB2RevoluteJoint(QObject *parent)
    : QB2Joint(parent)
{
}

QB2RevoluteJoint::~QB2RevoluteJoint()
{
}

void QB2RevoluteJoint::setLocalAnchorA(const QPointF &anchor)
{
    if (m_localAnchorA == anchor)
        return;
    m_localAnchorA = anchor;
    emit localAnchorAChanged();
}

void QB2RevoluteJoint::setLocalAnchorB(const QPointF &anchor)
{
    if (m_localAnchorB == anchor)
        return;
    m_localAnchorB = anchor;
    emit localAnchorBChanged();
}

void QB2RevoluteJoint::setReferenceAngle(qreal angle)
{
    if (qFuzzyCompare(m_referenceAngle, angle))
        return;
    m_referenceAngle = angle;
    emit referenceAngleChanged();
}

void QB2RevoluteJoint::setEnableLimit(bool enable)
{
    if (m_enableLimit == enable)
        return;
    m_enableLimit = enable;
    if (b2Joint_IsValid(m_jointId))
        b2RevoluteJoint_EnableLimit(m_jointId, enable);
    emit enableLimitChanged();
}

void QB2RevoluteJoint::setLowerAngle(qreal angle)
{
    if (qFuzzyCompare(m_lowerAngle, angle))
        return;
    m_lowerAngle = angle;
    if (b2Joint_IsValid(m_jointId))
        b2RevoluteJoint_SetLimits(m_jointId, m_lowerAngle, m_upperAngle);
    emit lowerAngleChanged();
}

void QB2RevoluteJoint::setUpperAngle(qreal angle)
{
    if (qFuzzyCompare(m_upperAngle, angle))
        return;
    m_upperAngle = angle;
    if (b2Joint_IsValid(m_jointId))
        b2RevoluteJoint_SetLimits(m_jointId, m_lowerAngle, m_upperAngle);
    emit upperAngleChanged();
}

void QB2RevoluteJoint::setEnableMotor(bool enable)
{
    if (m_enableMotor == enable)
        return;
    m_enableMotor = enable;
    if (b2Joint_IsValid(m_jointId))
        b2RevoluteJoint_EnableMotor(m_jointId, enable);
    emit enableMotorChanged();
}

void QB2RevoluteJoint::setMotorSpeed(qreal speed)
{
    if (qFuzzyCompare(m_motorSpeed, speed))
        return;
    m_motorSpeed = speed;
    if (b2Joint_IsValid(m_jointId))
        b2RevoluteJoint_SetMotorSpeed(m_jointId, speed);
    emit motorSpeedChanged();
}

void QB2RevoluteJoint::setMaxMotorTorque(qreal torque)
{
    if (qFuzzyCompare(m_maxMotorTorque, torque))
        return;
    m_maxMotorTorque = torque;
    if (b2Joint_IsValid(m_jointId))
        b2RevoluteJoint_SetMaxMotorTorque(m_jointId, torque);
    emit maxMotorTorqueChanged();
}

void QB2RevoluteJoint::createJoint()
{
    if (!m_componentComplete || !m_world || !m_bodyA || !m_bodyB)
        return;

    if (b2Joint_IsValid(m_jointId))
        return;

    if (!b2Body_IsValid(m_bodyA->bodyId()) || !b2Body_IsValid(m_bodyB->bodyId()))
        return;

    b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef();

    b2Vec2 localAnchorA = {static_cast<float>(m_localAnchorA.x()), static_cast<float>(m_localAnchorA.y())};
    b2Vec2 localAnchorB = {static_cast<float>(m_localAnchorB.x()), static_cast<float>(m_localAnchorB.y())};

    jointDef.base.bodyIdA = m_bodyA->bodyId();
    jointDef.base.bodyIdB = m_bodyB->bodyId();
    jointDef.base.localFrameA.p = localAnchorA;
    jointDef.base.localFrameA.q = b2Rot_identity;
    jointDef.base.localFrameB.p = localAnchorB;
    jointDef.base.localFrameB.q = b2Rot_identity;
    jointDef.base.collideConnected = m_collideConnected;

    jointDef.enableLimit = m_enableLimit;
    jointDef.lowerAngle = m_lowerAngle;
    jointDef.upperAngle = m_upperAngle;
    jointDef.enableMotor = m_enableMotor;
    jointDef.motorSpeed = m_motorSpeed;
    jointDef.maxMotorTorque = m_maxMotorTorque;

    m_jointId = b2CreateRevoluteJoint(m_world->worldId(), &jointDef);
    b2Joint_WakeBodies(m_jointId);
    emit jointReady();
}
