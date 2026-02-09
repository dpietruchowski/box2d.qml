#include "qb2motorjoint.h"
#include "../qb2body.h"
#include "../qb2world.h"

QB2MotorJoint::QB2MotorJoint(QObject *parent)
    : QB2Joint(parent)
{
}

QB2MotorJoint::~QB2MotorJoint()
{
}

void QB2MotorJoint::setLinearVelocity(const QPointF &velocity)
{
    if (m_linearVelocity == velocity)
        return;
    m_linearVelocity = velocity;
    if (b2Joint_IsValid(m_jointId))
    {
        b2Vec2 vel = {static_cast<float>(velocity.x()), static_cast<float>(velocity.y())};
        b2MotorJoint_SetLinearVelocity(m_jointId, vel);
    }
    emit linearVelocityChanged();
}

void QB2MotorJoint::setMaxVelocityForce(qreal force)
{
    if (qFuzzyCompare(m_maxVelocityForce, force))
        return;
    m_maxVelocityForce = force;
    if (b2Joint_IsValid(m_jointId))
        b2MotorJoint_SetMaxVelocityForce(m_jointId, force);
    emit maxVelocityForceChanged();
}

void QB2MotorJoint::setAngularVelocity(qreal velocity)
{
    if (qFuzzyCompare(m_angularVelocity, velocity))
        return;
    m_angularVelocity = velocity;
    if (b2Joint_IsValid(m_jointId))
        b2MotorJoint_SetAngularVelocity(m_jointId, velocity);
    emit angularVelocityChanged();
}

void QB2MotorJoint::setMaxVelocityTorque(qreal torque)
{
    if (qFuzzyCompare(m_maxVelocityTorque, torque))
        return;
    m_maxVelocityTorque = torque;
    if (b2Joint_IsValid(m_jointId))
        b2MotorJoint_SetMaxVelocityTorque(m_jointId, torque);
    emit maxVelocityTorqueChanged();
}

void QB2MotorJoint::setLinearHertz(qreal hertz)
{
    if (qFuzzyCompare(m_linearHertz, hertz))
        return;
    m_linearHertz = hertz;
    if (b2Joint_IsValid(m_jointId))
        b2MotorJoint_SetLinearHertz(m_jointId, hertz);
    emit linearHertzChanged();
}

void QB2MotorJoint::setLinearDampingRatio(qreal ratio)
{
    if (qFuzzyCompare(m_linearDampingRatio, ratio))
        return;
    m_linearDampingRatio = ratio;
    if (b2Joint_IsValid(m_jointId))
        b2MotorJoint_SetLinearDampingRatio(m_jointId, ratio);
    emit linearDampingRatioChanged();
}

void QB2MotorJoint::setMaxSpringForce(qreal force)
{
    if (qFuzzyCompare(m_maxSpringForce, force))
        return;
    m_maxSpringForce = force;
    if (b2Joint_IsValid(m_jointId))
        b2MotorJoint_SetMaxSpringForce(m_jointId, force);
    emit maxSpringForceChanged();
}

void QB2MotorJoint::setAngularHertz(qreal hertz)
{
    if (qFuzzyCompare(m_angularHertz, hertz))
        return;
    m_angularHertz = hertz;
    if (b2Joint_IsValid(m_jointId))
        b2MotorJoint_SetAngularHertz(m_jointId, hertz);
    emit angularHertzChanged();
}

void QB2MotorJoint::setAngularDampingRatio(qreal ratio)
{
    if (qFuzzyCompare(m_angularDampingRatio, ratio))
        return;
    m_angularDampingRatio = ratio;
    if (b2Joint_IsValid(m_jointId))
        b2MotorJoint_SetAngularDampingRatio(m_jointId, ratio);
    emit angularDampingRatioChanged();
}

void QB2MotorJoint::setMaxSpringTorque(qreal torque)
{
    if (qFuzzyCompare(m_maxSpringTorque, torque))
        return;
    m_maxSpringTorque = torque;
    if (b2Joint_IsValid(m_jointId))
        b2MotorJoint_SetMaxSpringTorque(m_jointId, torque);
    emit maxSpringTorqueChanged();
}

void QB2MotorJoint::createJoint()
{
    if (!m_componentComplete || !m_world || !m_bodyA || !m_bodyB)
        return;

    if (b2Joint_IsValid(m_jointId))
        return;

    if (!b2Body_IsValid(m_bodyA->bodyId()) || !b2Body_IsValid(m_bodyB->bodyId()))
        return;

    b2MotorJointDef jointDef = b2DefaultMotorJointDef();

    jointDef.base.bodyIdA = m_bodyA->bodyId();
    jointDef.base.bodyIdB = m_bodyB->bodyId();
    jointDef.base.collideConnected = m_collideConnected;

    jointDef.linearVelocity = {static_cast<float>(m_linearVelocity.x()), static_cast<float>(m_linearVelocity.y())};
    jointDef.maxVelocityForce = m_maxVelocityForce;
    jointDef.angularVelocity = m_angularVelocity;
    jointDef.maxVelocityTorque = m_maxVelocityTorque;
    jointDef.linearHertz = m_linearHertz;
    jointDef.linearDampingRatio = m_linearDampingRatio;
    jointDef.maxSpringForce = m_maxSpringForce;
    jointDef.angularHertz = m_angularHertz;
    jointDef.angularDampingRatio = m_angularDampingRatio;
    jointDef.maxSpringTorque = m_maxSpringTorque;

    m_jointId = b2CreateMotorJoint(m_world->worldId(), &jointDef);
}
