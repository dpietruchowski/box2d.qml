#include "qb2prismaticjoint.h"
#include "../qb2body.h"
#include "../qb2world.h"
#include <cmath>

QB2PrismaticJoint::QB2PrismaticJoint(QObject *parent)
    : QB2Joint(parent)
{
}

QB2PrismaticJoint::~QB2PrismaticJoint()
{
}

void QB2PrismaticJoint::setLocalAnchorA(const QPointF &anchor)
{
    if (m_localAnchorA == anchor)
        return;
    m_localAnchorA = anchor;
    emit localAnchorAChanged();
}

void QB2PrismaticJoint::setLocalAnchorB(const QPointF &anchor)
{
    if (m_localAnchorB == anchor)
        return;
    m_localAnchorB = anchor;
    emit localAnchorBChanged();
}

void QB2PrismaticJoint::setLocalAxisA(const QPointF &axis)
{
    if (m_localAxisA == axis)
        return;
    m_localAxisA = axis;
    emit localAxisAChanged();
}

void QB2PrismaticJoint::setReferenceAngle(qreal angle)
{
    if (qFuzzyCompare(m_referenceAngle, angle))
        return;
    m_referenceAngle = angle;
    emit referenceAngleChanged();
}

void QB2PrismaticJoint::setEnableLimit(bool enable)
{
    if (m_enableLimit == enable)
        return;
    m_enableLimit = enable;
    if (b2Joint_IsValid(m_jointId))
        b2PrismaticJoint_EnableLimit(m_jointId, enable);
    emit enableLimitChanged();
}

void QB2PrismaticJoint::setLowerTranslation(qreal translation)
{
    if (qFuzzyCompare(m_lowerTranslation, translation))
        return;
    m_lowerTranslation = translation;
    if (b2Joint_IsValid(m_jointId))
        b2PrismaticJoint_SetLimits(m_jointId, m_lowerTranslation, m_upperTranslation);
    emit lowerTranslationChanged();
}

void QB2PrismaticJoint::setUpperTranslation(qreal translation)
{
    if (qFuzzyCompare(m_upperTranslation, translation))
        return;
    m_upperTranslation = translation;
    if (b2Joint_IsValid(m_jointId))
        b2PrismaticJoint_SetLimits(m_jointId, m_lowerTranslation, m_upperTranslation);
    emit upperTranslationChanged();
}

void QB2PrismaticJoint::setEnableMotor(bool enable)
{
    if (m_enableMotor == enable)
        return;
    m_enableMotor = enable;
    if (b2Joint_IsValid(m_jointId))
        b2PrismaticJoint_EnableMotor(m_jointId, enable);
    emit enableMotorChanged();
}

void QB2PrismaticJoint::setMotorSpeed(qreal speed)
{
    if (qFuzzyCompare(m_motorSpeed, speed))
        return;
    m_motorSpeed = speed;
    if (b2Joint_IsValid(m_jointId))
        b2PrismaticJoint_SetMotorSpeed(m_jointId, speed);
    emit motorSpeedChanged();
}

void QB2PrismaticJoint::setMaxMotorForce(qreal force)
{
    if (qFuzzyCompare(m_maxMotorForce, force))
        return;
    m_maxMotorForce = force;
    if (b2Joint_IsValid(m_jointId))
        b2PrismaticJoint_SetMaxMotorForce(m_jointId, force);
    emit maxMotorForceChanged();
}

void QB2PrismaticJoint::createJoint()
{
    if (!m_componentComplete || !m_world || !m_bodyA || !m_bodyB)
        return;

    if (b2Joint_IsValid(m_jointId))
        return;

    if (!b2Body_IsValid(m_bodyA->bodyId()) || !b2Body_IsValid(m_bodyB->bodyId()))
        return;

    b2PrismaticJointDef jointDef = b2DefaultPrismaticJointDef();
    
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
    
    jointDef.enableSpring = false;
    jointDef.enableLimit = m_enableLimit;
    jointDef.lowerTranslation = m_lowerTranslation;
    jointDef.upperTranslation = m_upperTranslation;
    jointDef.enableMotor = m_enableMotor;
    jointDef.motorSpeed = m_motorSpeed;
    jointDef.maxMotorForce = m_maxMotorForce;

    m_jointId = b2CreatePrismaticJoint(m_world->worldId(), &jointDef);
    emit jointReady();
}
