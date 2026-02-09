#include "qb2joint.h"
#include "qb2body.h"
#include "qb2world.h"
#include <QTimer>

QB2Joint::QB2Joint(QObject *parent)
    : QObject(parent), m_jointId(b2_nullJointId)
{
}

QB2Joint::~QB2Joint()
{
    if (b2Joint_IsValid(m_jointId))
    {
        b2DestroyJoint(m_jointId, true);
    }
}

void QB2Joint::setBodyA(QB2Body *body)
{
    if (m_bodyA == body)
        return;
    m_bodyA = body;
    emit bodyAChanged();
}

void QB2Joint::setBodyB(QB2Body *body)
{
    if (m_bodyB == body)
        return;
    m_bodyB = body;
    emit bodyBChanged();
}

void QB2Joint::setCollideConnected(bool collide)
{
    if (m_collideConnected == collide)
        return;
    m_collideConnected = collide;
    emit collideConnectedChanged();
}

void QB2Joint::setWorld(QB2World *world)
{
    m_world = world;
}

void QB2Joint::classBegin()
{
}

void QB2Joint::componentComplete()
{
    m_componentComplete = true;

    if (!m_world)
    {
        QB2World *world = nullptr;
        QObject *p = parent();
        while (p)
        {
            world = qobject_cast<QB2World *>(p);
            if (world)
                break;
            p = p->parent();
        }
        m_world = world;
    }

    if (!m_world)
    {
        qFatal("Joint must be a child of World");
        return;
    }

    if (!m_bodyA || !m_bodyB)
    {
        qWarning("Joint requires both bodyA and bodyB");
        return;
    }

    if (b2Body_IsValid(m_bodyA->bodyId()) && b2Body_IsValid(m_bodyB->bodyId()))
    {
        createJoint();
    }
    else
    {
        QTimer::singleShot(0, this, [this]()
        {
            if (b2Body_IsValid(m_bodyA->bodyId()) && b2Body_IsValid(m_bodyB->bodyId()))
            {
                createJoint();
            }
        });
    }
}

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
    emit jointReady();
}

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
    emit jointReady();
}
