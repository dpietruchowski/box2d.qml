#include "box2dbody.h"
#include "box2dworld.h"
#include "box2dfixture.h"
#include <QDebug>

Box2DBody::Box2DBody(QQuickItem *parent)
    : QQuickItem(parent)
    , m_world(nullptr)
    , m_body(nullptr)
    , m_linearDamping(0.0)
    , m_angularDamping(0.0)
    , m_bodyType(Body::Dynamic)
    , m_bullet(false)
    , m_sleepingAllowed(true)
    , m_fixedRotation(false)
    , m_active(true)
    , m_awake(true)
    , m_gravityScale(1.0)
    , m_synchronizing(false)
{
}

Box2DBody::~Box2DBody()
{
    destroyBody();
}

Box2DWorld *Box2DBody::world() const
{
    return m_world;
}

void Box2DBody::setWorld(Box2DWorld *world)
{
    if (m_world == world)
        return;

    if (m_world)
        m_world->unregisterBody(this);

    m_world = world;

    if (m_world) {
        m_world->registerBody(this);
        if (isComponentComplete())
            createBody();
    }

    emit worldChanged();
}

qreal Box2DBody::linearDamping() const
{
    return m_linearDamping;
}

void Box2DBody::setLinearDamping(qreal damping)
{
    if (qFuzzyCompare(m_linearDamping, damping))
        return;

    m_linearDamping = damping;
    if (m_body)
        m_body->SetLinearDamping(damping);
    emit linearDampingChanged();
}

qreal Box2DBody::angularDamping() const
{
    return m_angularDamping;
}

void Box2DBody::setAngularDamping(qreal damping)
{
    if (qFuzzyCompare(m_angularDamping, damping))
        return;

    m_angularDamping = damping;
    if (m_body)
        m_body->SetAngularDamping(damping);
    emit angularDampingChanged();
}

Body::BodyType Box2DBody::bodyType() const
{
    return m_bodyType;
}

void Box2DBody::setBodyType(Body::BodyType type)
{
    if (m_bodyType == type)
        return;

    m_bodyType = type;
    if (m_body)
        m_body->SetType(static_cast<b2BodyType>(type));
    emit bodyTypeChanged();
}

bool Box2DBody::bullet() const
{
    return m_bullet;
}

void Box2DBody::setBullet(bool bullet)
{
    if (m_bullet == bullet)
        return;

    m_bullet = bullet;
    if (m_body)
        m_body->SetBullet(bullet);
    emit bulletChanged();
}

bool Box2DBody::sleepingAllowed() const
{
    return m_sleepingAllowed;
}

void Box2DBody::setSleepingAllowed(bool allowed)
{
    if (m_sleepingAllowed == allowed)
        return;

    m_sleepingAllowed = allowed;
    if (m_body)
        m_body->SetSleepingAllowed(allowed);
    emit sleepingAllowedChanged();
}

bool Box2DBody::fixedRotation() const
{
    return m_fixedRotation;
}

void Box2DBody::setFixedRotation(bool fixedRotation)
{
    if (m_fixedRotation == fixedRotation)
        return;

    m_fixedRotation = fixedRotation;
    if (m_body)
        m_body->SetFixedRotation(fixedRotation);
    emit fixedRotationChanged();
}

bool Box2DBody::active() const
{
    return m_active;
}

void Box2DBody::setActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;
    if (m_body)
        m_body->SetEnabled(active);
    emit activeChanged();
}

bool Box2DBody::awake() const
{
    return m_awake;
}

void Box2DBody::setAwake(bool awake)
{
    if (m_awake == awake)
        return;

    m_awake = awake;
    if (m_body)
        m_body->SetAwake(awake);
    emit awakeChanged();
}

qreal Box2DBody::gravityScale() const
{
    return m_gravityScale;
}

void Box2DBody::setGravityScale(qreal scale)
{
    if (qFuzzyCompare(m_gravityScale, scale))
        return;

    m_gravityScale = scale;
    if (m_body)
        m_body->SetGravityScale(scale);
    emit gravityScaleChanged();
}

QPointF Box2DBody::linearVelocity() const
{
    if (m_body) {
        const b2Vec2 &velocity = m_body->GetLinearVelocity();
        return toPixels(velocity);
    }
    return QPointF();
}

void Box2DBody::setLinearVelocity(const QPointF &velocity)
{
    if (m_body)
        m_body->SetLinearVelocity(toMeters(velocity));
    emit linearVelocityChanged();
}

qreal Box2DBody::angularVelocity() const
{
    if (m_body)
        return m_body->GetAngularVelocity();
    return 0.0;
}

void Box2DBody::setAngularVelocity(qreal velocity)
{
    if (m_body)
        m_body->SetAngularVelocity(velocity);
    emit angularVelocityChanged();
}

void Box2DBody::applyForce(const QPointF &force, const QPointF &point)
{
    if (m_body)
        m_body->ApplyForce(toMeters(force), toMeters(point), true);
}

void Box2DBody::applyForceToCenter(const QPointF &force)
{
    if (m_body)
        m_body->ApplyForceToCenter(toMeters(force), true);
}

void Box2DBody::applyTorque(qreal torque)
{
    if (m_body)
        m_body->ApplyTorque(torque, true);
}

void Box2DBody::applyLinearImpulse(const QPointF &impulse, const QPointF &point)
{
    if (m_body)
        m_body->ApplyLinearImpulse(toMeters(impulse), toMeters(point), true);
}

void Box2DBody::applyAngularImpulse(qreal impulse)
{
    if (m_body)
        m_body->ApplyAngularImpulse(impulse, true);
}

void Box2DBody::synchronize()
{
    if (!m_body || m_synchronizing)
        return;

    m_synchronizing = true;

    const b2Vec2 &position = m_body->GetPosition();
    const qreal angle = m_body->GetAngle();

    setX(toPixels(position.x) - width() / 2.0);
    setY(-toPixels(position.y) - height() / 2.0);
    setRotation(-angle * 180.0 / M_PI);

    m_synchronizing = false;
}

void Box2DBody::addFixture(Box2DFixture *fixture)
{
    if (!m_fixtures.contains(fixture))
        m_fixtures.append(fixture);
}

void Box2DBody::removeFixture(Box2DFixture *fixture)
{
    m_fixtures.removeOne(fixture);
}

void Box2DBody::componentComplete()
{
    QQuickItem::componentComplete();

    if (!m_world) {
        // Try to find world in parent hierarchy
        QQuickItem *item = parentItem();
        while (item) {
            if (Box2DWorld *world = qobject_cast<Box2DWorld*>(item)) {
                setWorld(world);
                break;
            }
            item = item->parentItem();
        }
    }

    if (m_world)
        createBody();
}

void Box2DBody::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    if (!m_synchronizing && m_body) {
        const qreal ppm = m_world ? m_world->pixelsPerMeter() : 32.0;
        const qreal angle = -rotation() * M_PI / 180.0;
        
        b2Vec2 position;
        position.x = (newGeometry.x() + newGeometry.width() / 2.0) / ppm;
        position.y = -(newGeometry.y() + newGeometry.height() / 2.0) / ppm;
        
        m_body->SetTransform(position, angle);
    }

    QQuickItem::geometryChange(newGeometry, oldGeometry);
}

void Box2DBody::createBody()
{
    if (!m_world || m_body)
        return;

    b2BodyDef bodyDef;
    bodyDef.type = static_cast<b2BodyType>(m_bodyType);
    
    const qreal ppm = m_world->pixelsPerMeter();
    bodyDef.position.Set((x() + width() / 2.0) / ppm,
                         -(y() + height() / 2.0) / ppm);
    bodyDef.angle = -rotation() * M_PI / 180.0;
    bodyDef.linearDamping = m_linearDamping;
    bodyDef.angularDamping = m_angularDamping;
    bodyDef.bullet = m_bullet;
    bodyDef.allowSleep = m_sleepingAllowed;
    bodyDef.fixedRotation = m_fixedRotation;
    bodyDef.enabled = m_active;
    bodyDef.awake = m_awake;
    bodyDef.gravityScale = m_gravityScale;

    m_body = m_world->world()->CreateBody(&bodyDef);
    m_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    // Create fixtures for all child fixtures
    for (Box2DFixture *fixture : m_fixtures) {
        fixture->createFixture(m_body);
    }
}

void Box2DBody::destroyBody()
{
    if (m_body && m_world && m_world->world()) {
        m_world->world()->DestroyBody(m_body);
        m_body = nullptr;
    }
    
    if (m_world) {
        m_world->unregisterBody(this);
    }
}

b2Vec2 Box2DBody::toMeters(const QPointF &point) const
{
    const qreal ppm = m_world ? m_world->pixelsPerMeter() : 32.0;
    return b2Vec2(point.x() / ppm, -point.y() / ppm);
}

QPointF Box2DBody::toPixels(const b2Vec2 &vec) const
{
    const qreal ppm = m_world ? m_world->pixelsPerMeter() : 32.0;
    return QPointF(vec.x * ppm, -vec.y * ppm);
}

qreal Box2DBody::toMeters(qreal value) const
{
    const qreal ppm = m_world ? m_world->pixelsPerMeter() : 32.0;
    return value / ppm;
}

qreal Box2DBody::toPixels(qreal value) const
{
    const qreal ppm = m_world ? m_world->pixelsPerMeter() : 32.0;
    return value * ppm;
}
