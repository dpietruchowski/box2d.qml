#include "qb2body.h"
#include "qb2world.h"
#include "qb2fixture.h"
#include <QtMath>
#include <QTimer>
#include <limits>

QB2Body::QB2Body(QQuickItem *parent)
    : QQuickItem(parent), m_bodyId(b2_nullBodyId)
{
    connect(this, &QQuickItem::xChanged, this, &QB2Body::onXYChanged);
    connect(this, &QQuickItem::yChanged, this, &QB2Body::onXYChanged);
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
    QQuickItem::componentComplete();
    m_componentComplete = true;

    if (!m_world)
    {
        m_world = qobject_cast<QB2World *>(parentItem());
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

    connect(m_world, &QB2World::stepped, this, &QB2Body::updateTransform);
    updateTransform();

    emit bodyReady();

    QTimer::singleShot(0, this, [this]()
                       { updateBoundingBox(); });
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

void QB2Body::updateTransform()
{
    if (!b2Body_IsValid(m_bodyId))
        return;

    m_updatingTransform = true;

    b2Vec2 pos = b2Body_GetPosition(m_bodyId);
    qreal angle = b2Rot_GetAngle(b2Body_GetRotation(m_bodyId));

    // Box2D position is center, QQuickItem x/y is top-left
    // So we need to offset by half width/height
    setX(pos.x - width() / 2.0);
    setY(pos.y - height() / 2.0);
    setRotation(qRadiansToDegrees(angle));

    m_updatingTransform = false;
}

void QB2Body::onXYChanged()
{
    if (!b2Body_IsValid(m_bodyId) || m_updatingTransform)
        return;

    // QQuickItem x/y is top-left, Box2D position is center
    // So we need to add half width/height
    QVector2D centerPos(x() + width() / 2.0, y() + height() / 2.0);
    if (centerPos != m_position)
    {
        m_position = centerPos;
        b2Rot rotation = b2Body_GetRotation(m_bodyId);
        b2Body_SetTransform(m_bodyId, {centerPos.x(), centerPos.y()}, rotation);
        emit positionChanged();
    }
}

void QB2Body::updateBoundingBox()
{
    if (!b2Body_IsValid(m_bodyId))
        return;

    int shapeCount = b2Body_GetShapeCount(m_bodyId);
    if (shapeCount == 0)
        return;

    QVector<b2ShapeId> shapes(shapeCount);
    b2Body_GetShapes(m_bodyId, shapes.data(), shapeCount);

    qreal minX = std::numeric_limits<qreal>::max();
    qreal minY = std::numeric_limits<qreal>::max();
    qreal maxX = std::numeric_limits<qreal>::lowest();
    qreal maxY = std::numeric_limits<qreal>::lowest();

    for (int i = 0; i < shapeCount; ++i)
    {
        b2AABB aabb = b2Shape_GetAABB(shapes[i]);

        minX = qMin(minX, static_cast<qreal>(aabb.lowerBound.x));
        minY = qMin(minY, static_cast<qreal>(aabb.lowerBound.y));
        maxX = qMax(maxX, static_cast<qreal>(aabb.upperBound.x));
        maxY = qMax(maxY, static_cast<qreal>(aabb.upperBound.y));
    }

    qreal newWidth = maxX - minX;
    qreal newHeight = maxY - minY;

    if (newWidth > 0 && newHeight > 0)
    {
        setWidth(newWidth);
        setHeight(newHeight);
    }
}
