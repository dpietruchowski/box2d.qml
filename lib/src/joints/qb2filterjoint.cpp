#include "qb2filterjoint.h"
#include "../qb2body.h"
#include "../qb2world.h"

QB2FilterJoint::QB2FilterJoint(QObject *parent)
    : QB2Joint(parent)
{
}

QB2FilterJoint::~QB2FilterJoint()
{
}

void QB2FilterJoint::createJoint()
{
    if (!m_componentComplete || !m_world || !m_bodyA || !m_bodyB)
        return;

    if (b2Joint_IsValid(m_jointId))
        return;

    if (!b2Body_IsValid(m_bodyA->bodyId()) || !b2Body_IsValid(m_bodyB->bodyId()))
        return;

    b2FilterJointDef jointDef = b2DefaultFilterJointDef();

    jointDef.base.bodyIdA = m_bodyA->bodyId();
    jointDef.base.bodyIdB = m_bodyB->bodyId();
    jointDef.base.collideConnected = m_collideConnected;

    m_jointId = b2CreateFilterJoint(m_world->worldId(), &jointDef);
    b2Joint_WakeBodies(m_jointId);
}
