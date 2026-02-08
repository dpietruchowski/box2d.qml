#include "qb2body.h"
#include "qb2world.h"
#include "qb2fixture.h"

QB2Body::QB2Body(QObject *parent)
    : QObject(parent), m_bodyId(b2_nullBodyId)
{
}

QB2Body::~QB2Body()
{
    if (b2Body_IsValid(m_bodyId))
    {
        b2DestroyBody(m_bodyId);
    }
}

void QB2Body::setWorld(QB2World *world)
{
    m_world = world;
}

void QB2Body::classBegin()
{
}

void QB2Body::componentComplete()
{
    m_componentComplete = true;

    if (!m_world)
    {
        m_world = qobject_cast<QB2World *>(parent());
    }

    if (!m_world)
    {
        qFatal("Body must be a child of World");
        return;
    }

    if (b2World_IsValid(m_world->worldId()))
    {
        createBody();
    }
    else
    {
        connect(m_world, &QB2World::worldReady, this, &QB2Body::createBody);
    }
}

void QB2Body::createBody()
{
    if (!m_world || !m_componentComplete)
        return;

    if (b2Body_IsValid(m_bodyId))
        return;

    if (!b2World_IsValid(m_world->worldId()))
        return;

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = static_cast<b2BodyType>(m_type);
    bodyDef.position = {m_position.x(), m_position.y()};
    m_bodyId = b2CreateBody(m_world->worldId(), &bodyDef);
    emit bodyReady();
}

QVector2D QB2Body::position() const
{
    if (b2Body_IsValid(m_bodyId))
    {
        b2Vec2 pos = b2Body_GetPosition(m_bodyId);
        return QVector2D(pos.x, pos.y);
    }
    return m_position;
}

void QB2Body::setPosition(const QVector2D &position)
{
    if (m_position == position)
        return;
    m_position = position;
    if (b2Body_IsValid(m_bodyId))
    {
        b2Rot rotation = b2Body_GetRotation(m_bodyId);
        b2Body_SetTransform(m_bodyId, {position.x(), position.y()}, rotation);
    }
    emit positionChanged();
    emit xChanged();
    emit yChanged();
}

qreal QB2Body::x() const
{
    if (b2Body_IsValid(m_bodyId))
    {
        b2Vec2 pos = b2Body_GetPosition(m_bodyId);
        return pos.x;
    }
    return m_position.x();
}

void QB2Body::setX(qreal x)
{
    QVector2D pos = position();
    if (qFuzzyCompare(pos.x(), static_cast<float>(x)))
        return;
    pos.setX(x);
    setPosition(pos);
}

qreal QB2Body::y() const
{
    if (b2Body_IsValid(m_bodyId))
    {
        b2Vec2 pos = b2Body_GetPosition(m_bodyId);
        return pos.y;
    }
    return m_position.y();
}

void QB2Body::setY(qreal y)
{
    QVector2D pos = position();
    if (qFuzzyCompare(pos.y(), static_cast<float>(y)))
        return;
    pos.setY(y);
    setPosition(pos);
}

qreal QB2Body::angle() const
{
    if (!b2Body_IsValid(m_bodyId))
        return 0.0;
    b2Rot rot = b2Body_GetRotation(m_bodyId);
    return b2Rot_GetAngle(rot);
}

void QB2Body::setAngle(qreal angle)
{
    if (!b2Body_IsValid(m_bodyId))
        return;
    b2Rot rot = b2Body_GetRotation(m_bodyId);
    if (qFuzzyCompare(b2Rot_GetAngle(rot), static_cast<float>(angle)))
        return;
    b2Vec2 pos = b2Body_GetPosition(m_bodyId);
    b2Body_SetTransform(m_bodyId, pos, b2MakeRot(angle));
    emit angleChanged();
}

qreal QB2Body::linearVelocityX() const
{
    if (!b2Body_IsValid(m_bodyId))
        return 0.0;
    b2Vec2 vel = b2Body_GetLinearVelocity(m_bodyId);
    return vel.x;
}

void QB2Body::setLinearVelocityX(qreal vx)
{
    if (!b2Body_IsValid(m_bodyId))
        return;
    b2Vec2 vel = b2Body_GetLinearVelocity(m_bodyId);
    if (qFuzzyCompare(vel.x, static_cast<float>(vx)))
        return;
    b2Body_SetLinearVelocity(m_bodyId, {static_cast<float>(vx), vel.y});
    emit linearVelocityXChanged();
}

qreal QB2Body::linearVelocityY() const
{
    if (!b2Body_IsValid(m_bodyId))
        return 0.0;
    b2Vec2 vel = b2Body_GetLinearVelocity(m_bodyId);
    return vel.y;
}

void QB2Body::setLinearVelocityY(qreal vy)
{
    if (!b2Body_IsValid(m_bodyId))
        return;
    b2Vec2 vel = b2Body_GetLinearVelocity(m_bodyId);
    if (qFuzzyCompare(vel.y, static_cast<float>(vy)))
        return;
    b2Body_SetLinearVelocity(m_bodyId, {vel.x, static_cast<float>(vy)});
    emit linearVelocityYChanged();
}

qreal QB2Body::angularVelocity() const
{
    if (!b2Body_IsValid(m_bodyId))
        return 0.0;
    return b2Body_GetAngularVelocity(m_bodyId);
}

void QB2Body::setAngularVelocity(qreal omega)
{
    if (!b2Body_IsValid(m_bodyId))
        return;
    if (qFuzzyCompare(b2Body_GetAngularVelocity(m_bodyId), static_cast<float>(omega)))
        return;
    b2Body_SetAngularVelocity(m_bodyId, static_cast<float>(omega));
    emit angularVelocityChanged();
}

QB2Body::BodyType QB2Body::type() const
{
    if (b2Body_IsValid(m_bodyId))
        return static_cast<BodyType>(b2Body_GetType(m_bodyId));
    return m_type;
}

void QB2Body::setType(BodyType type)
{
    if (m_type == type)
        return;
    m_type = type;
    if (b2Body_IsValid(m_bodyId))
        b2Body_SetType(m_bodyId, static_cast<b2BodyType>(type));
    emit typeChanged();
}

QQmlListProperty<QB2Fixture> QB2Body::fixtures()
{
    return QQmlListProperty<QB2Fixture>(this, this,
                                        &QB2Body::appendFixture,
                                        &QB2Body::fixtureCount,
                                        &QB2Body::fixture,
                                        &QB2Body::clearFixtures);
}

void QB2Body::appendFixture(QQmlListProperty<QB2Fixture> *list, QB2Fixture *fixture)
{
    QB2Body *body = qobject_cast<QB2Body *>(list->object);
    if (body && fixture)
    {
        body->m_fixtures.append(fixture);
        fixture->setParent(body);
    }
}

qsizetype QB2Body::fixtureCount(QQmlListProperty<QB2Fixture> *list)
{
    QB2Body *body = qobject_cast<QB2Body *>(list->object);
    return body ? body->m_fixtures.count() : 0;
}

QB2Fixture *QB2Body::fixture(QQmlListProperty<QB2Fixture> *list, qsizetype index)
{
    QB2Body *body = qobject_cast<QB2Body *>(list->object);
    return body ? body->m_fixtures.at(index) : nullptr;
}

void QB2Body::clearFixtures(QQmlListProperty<QB2Fixture> *list)
{
    QB2Body *body = qobject_cast<QB2Body *>(list->object);
    if (body)
        body->m_fixtures.clear();
}
