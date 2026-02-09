#include "qb2joint.h"
#include "../qb2body.h"
#include "../qb2world.h"
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
        // Auto-find World in parent hierarchy
        QObject *obj = parent();
        while (obj)
        {
            if (auto *world = qobject_cast<QB2World *>(obj))
            {
                m_world = world;
                break;
            }
            obj = obj->parent();
        }
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
            } });
    }
}
