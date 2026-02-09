#include "qb2distancejoint.h"
#include "../qb2body.h"
#include "../qb2world.h"

QB2DistanceJoint::QB2DistanceJoint(QObject *parent)
    : QB2Joint(parent)
{
}

QB2DistanceJoint::~QB2DistanceJoint()
{
}

void QB2DistanceJoint::setLocalAnchorA(const QPointF &anchor)
{
    if (m_localAnchorA == anchor)
        return;
    m_localAnchorA = anchor;
    emit localAnchorAChanged();
}

void QB2DistanceJoint::setLocalAnchorB(const QPointF &anchor)
{
    if (m_localAnchorB == anchor)
        return;
    m_localAnchorB = anchor;
    emit localAnchorBChanged();
}

void QB2DistanceJoint::setLength(qreal length)
{
    if (qFuzzyCompare(m_length, length))
        return;
    m_length = length;
    if (b2Joint_IsValid(m_jointId))
        b2DistanceJoint_SetLength(m_jointId, length);
    emit lengthChanged();
}

void QB2DistanceJoint::setMinLength(qreal length)
{
    if (qFuzzyCompare(m_minLength, length))
        return;
    m_minLength = length;
    if (b2Joint_IsValid(m_jointId))
        b2DistanceJoint_SetLengthRange(m_jointId, m_minLength, m_maxLength);
    emit minLengthChanged();
}

void QB2DistanceJoint::setMaxLength(qreal length)
{
    if (qFuzzyCompare(m_maxLength, length))
        return;
    m_maxLength = length;
    if (b2Joint_IsValid(m_jointId))
        b2DistanceJoint_SetLengthRange(m_jointId, m_minLength, m_maxLength);
    emit maxLengthChanged();
}

void QB2DistanceJoint::setHertz(qreal hertz)
{
    if (qFuzzyCompare(m_hertz, hertz))
        return;
    m_hertz = hertz;
    if (b2Joint_IsValid(m_jointId))
        b2DistanceJoint_SetSpringHertz(m_jointId, m_hertz);
    emit hertzChanged();
}

void QB2DistanceJoint::setDampingRatio(qreal ratio)
{
    if (qFuzzyCompare(m_dampingRatio, ratio))
        return;
    m_dampingRatio = ratio;
    if (b2Joint_IsValid(m_jointId))
        b2DistanceJoint_SetSpringDampingRatio(m_jointId, m_dampingRatio);
    emit dampingRatioChanged();
}

void QB2DistanceJoint::createJoint()
{
    if (!m_componentComplete || !m_world || !m_bodyA || !m_bodyB)
        return;

    if (b2Joint_IsValid(m_jointId))
        return;

    if (!b2Body_IsValid(m_bodyA->bodyId()) || !b2Body_IsValid(m_bodyB->bodyId()))
        return;

    b2DistanceJointDef jointDef = b2DefaultDistanceJointDef();

    b2Vec2 localAnchorA = {static_cast<float>(m_localAnchorA.x()), static_cast<float>(m_localAnchorA.y())};
    b2Vec2 localAnchorB = {static_cast<float>(m_localAnchorB.x()), static_cast<float>(m_localAnchorB.y())};

    jointDef.base.bodyIdA = m_bodyA->bodyId();
    jointDef.base.bodyIdB = m_bodyB->bodyId();
    jointDef.base.localFrameA.p = localAnchorA;
    jointDef.base.localFrameA.q = b2Rot_identity;
    jointDef.base.localFrameB.p = localAnchorB;
    jointDef.base.localFrameB.q = b2Rot_identity;
    jointDef.base.collideConnected = m_collideConnected;

    jointDef.length = m_length;
    jointDef.enableSpring = (m_hertz > 0.0);
    jointDef.hertz = m_hertz;
    jointDef.dampingRatio = m_dampingRatio;
    jointDef.enableLimit = (m_minLength > 0.0 || m_maxLength < std::numeric_limits<float>::max());
    jointDef.minLength = m_minLength;
    jointDef.maxLength = m_maxLength;

    m_jointId = b2CreateDistanceJoint(m_world->worldId(), &jointDef);
    b2Joint_WakeBodies(m_jointId);
    emit jointReady();
}
