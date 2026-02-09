#include "qb2weldjoint.h"
#include "../qb2body.h"
#include "../qb2world.h"

QB2WeldJoint::QB2WeldJoint(QObject *parent)
    : QB2Joint(parent)
{
}

QB2WeldJoint::~QB2WeldJoint()
{
}

void QB2WeldJoint::setLocalAnchorA(const QPointF &anchor)
{
    if (m_localAnchorA == anchor)
        return;
    m_localAnchorA = anchor;
    emit localAnchorAChanged();
}

void QB2WeldJoint::setLocalAnchorB(const QPointF &anchor)
{
    if (m_localAnchorB == anchor)
        return;
    m_localAnchorB = anchor;
    emit localAnchorBChanged();
}

void QB2WeldJoint::setReferenceAngle(qreal angle)
{
    if (qFuzzyCompare(m_referenceAngle, angle))
        return;
    m_referenceAngle = angle;
    emit referenceAngleChanged();
}

void QB2WeldJoint::setLinearHertz(qreal hertz)
{
    if (qFuzzyCompare(m_linearHertz, hertz))
        return;
    m_linearHertz = hertz;
    if (b2Joint_IsValid(m_jointId))
        b2WeldJoint_SetLinearHertz(m_jointId, hertz);
    emit linearHertzChanged();
}

void QB2WeldJoint::setLinearDampingRatio(qreal ratio)
{
    if (qFuzzyCompare(m_linearDampingRatio, ratio))
        return;
    m_linearDampingRatio = ratio;
    if (b2Joint_IsValid(m_jointId))
        b2WeldJoint_SetLinearDampingRatio(m_jointId, ratio);
    emit linearDampingRatioChanged();
}

void QB2WeldJoint::setAngularHertz(qreal hertz)
{
    if (qFuzzyCompare(m_angularHertz, hertz))
        return;
    m_angularHertz = hertz;
    if (b2Joint_IsValid(m_jointId))
        b2WeldJoint_SetAngularHertz(m_jointId, hertz);
    emit angularHertzChanged();
}

void QB2WeldJoint::setAngularDampingRatio(qreal ratio)
{
    if (qFuzzyCompare(m_angularDampingRatio, ratio))
        return;
    m_angularDampingRatio = ratio;
    if (b2Joint_IsValid(m_jointId))
        b2WeldJoint_SetAngularDampingRatio(m_jointId, ratio);
    emit angularDampingRatioChanged();
}

void QB2WeldJoint::createJoint()
{
    if (!m_componentComplete || !m_world || !m_bodyA || !m_bodyB)
        return;

    if (b2Joint_IsValid(m_jointId))
        return;

    if (!b2Body_IsValid(m_bodyA->bodyId()) || !b2Body_IsValid(m_bodyB->bodyId()))
        return;

    b2WeldJointDef jointDef = b2DefaultWeldJointDef();

    b2Vec2 localAnchorA = {static_cast<float>(m_localAnchorA.x()), static_cast<float>(m_localAnchorA.y())};
    b2Vec2 localAnchorB = {static_cast<float>(m_localAnchorB.x()), static_cast<float>(m_localAnchorB.y())};

    jointDef.base.bodyIdA = m_bodyA->bodyId();
    jointDef.base.bodyIdB = m_bodyB->bodyId();
    jointDef.base.localFrameA.p = localAnchorA;
    jointDef.base.localFrameA.q = b2Rot_identity;
    jointDef.base.localFrameB.p = localAnchorB;
    jointDef.base.localFrameB.q = b2Rot_identity;
    jointDef.base.collideConnected = m_collideConnected;

    jointDef.linearHertz = m_linearHertz;
    jointDef.linearDampingRatio = m_linearDampingRatio;
    jointDef.angularHertz = m_angularHertz;
    jointDef.angularDampingRatio = m_angularDampingRatio;

    m_jointId = b2CreateWeldJoint(m_world->worldId(), &jointDef);
    b2Joint_WakeBodies(m_jointId);
    emit jointReady();
}
