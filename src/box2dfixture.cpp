#include "box2dfixture.h"
#include "box2dbody.h"
#include "box2dworld.h"

Box2DFixture::Box2DFixture(QQuickItem *parent)
    : QQuickItem(parent)
    , m_fixture(nullptr)
    , m_density(1.0)
    , m_friction(0.3)
    , m_restitution(0.0)
    , m_sensor(false)
    , m_categories(0x0001)
    , m_collidesWith(0xFFFF)
    , m_groupIndex(0)
{
}

Box2DFixture::~Box2DFixture()
{
    if (m_fixture && body() && body()->body()) {
        body()->body()->DestroyFixture(m_fixture);
    }
}

qreal Box2DFixture::density() const
{
    return m_density;
}

void Box2DFixture::setDensity(qreal density)
{
    if (qFuzzyCompare(m_density, density))
        return;

    m_density = density;
    if (m_fixture)
        m_fixture->SetDensity(density);
    emit densityChanged();
}

qreal Box2DFixture::friction() const
{
    return m_friction;
}

void Box2DFixture::setFriction(qreal friction)
{
    if (qFuzzyCompare(m_friction, friction))
        return;

    m_friction = friction;
    if (m_fixture)
        m_fixture->SetFriction(friction);
    emit frictionChanged();
}

qreal Box2DFixture::restitution() const
{
    return m_restitution;
}

void Box2DFixture::setRestitution(qreal restitution)
{
    if (qFuzzyCompare(m_restitution, restitution))
        return;

    m_restitution = restitution;
    if (m_fixture)
        m_fixture->SetRestitution(restitution);
    emit restitutionChanged();
}

bool Box2DFixture::sensor() const
{
    return m_sensor;
}

void Box2DFixture::setSensor(bool sensor)
{
    if (m_sensor == sensor)
        return;

    m_sensor = sensor;
    if (m_fixture)
        m_fixture->SetSensor(sensor);
    emit sensorChanged();
}

int Box2DFixture::categories() const
{
    return m_categories;
}

void Box2DFixture::setCategories(int categories)
{
    if (m_categories == categories)
        return;

    m_categories = categories;
    if (m_fixture) {
        b2Filter filter = m_fixture->GetFilterData();
        filter.categoryBits = categories;
        m_fixture->SetFilterData(filter);
    }
    emit categoriesChanged();
}

int Box2DFixture::collidesWith() const
{
    return m_collidesWith;
}

void Box2DFixture::setCollidesWith(int collidesWith)
{
    if (m_collidesWith == collidesWith)
        return;

    m_collidesWith = collidesWith;
    if (m_fixture) {
        b2Filter filter = m_fixture->GetFilterData();
        filter.maskBits = collidesWith;
        m_fixture->SetFilterData(filter);
    }
    emit collidesWithChanged();
}

int Box2DFixture::groupIndex() const
{
    return m_groupIndex;
}

void Box2DFixture::setGroupIndex(int groupIndex)
{
    if (m_groupIndex == groupIndex)
        return;

    m_groupIndex = groupIndex;
    if (m_fixture) {
        b2Filter filter = m_fixture->GetFilterData();
        filter.groupIndex = groupIndex;
        m_fixture->SetFilterData(filter);
    }
    emit groupIndexChanged();
}

void Box2DFixture::createFixture(b2Body *body)
{
    if (m_fixture || !body)
        return;

    b2FixtureDef fixtureDef;
    b2Shape *shape = createShape();
    if (!shape)
        return;

    fixtureDef.shape = shape;
    fixtureDef.density = m_density;
    fixtureDef.friction = m_friction;
    fixtureDef.restitution = m_restitution;
    fixtureDef.isSensor = m_sensor;
    fixtureDef.filter.categoryBits = m_categories;
    fixtureDef.filter.maskBits = m_collidesWith;
    fixtureDef.filter.groupIndex = m_groupIndex;

    m_fixture = body->CreateFixture(&fixtureDef);
    m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    delete shape;
}

void Box2DFixture::componentComplete()
{
    QQuickItem::componentComplete();

    Box2DBody *b = body();
    if (b) {
        b->addFixture(this);
        if (b->body())
            createFixture(b->body());
    }
}

qreal Box2DFixture::toMeters(qreal value) const
{
    Box2DBody *b = body();
    if (b && b->world())
        return value / b->world()->pixelsPerMeter();
    return value / 32.0;
}

Box2DBody *Box2DFixture::body() const
{
    return qobject_cast<Box2DBody*>(parentItem());
}
