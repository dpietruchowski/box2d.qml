#pragma once

#include <QObject>
#include <QQmlParserStatus>
#include <box2d/box2d.h>

// Forward declaration
class QB2World;

// Need complete type for Q_PROPERTY pointers
#include "../qb2body.h"

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
