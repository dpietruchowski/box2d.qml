#include "qb2fixture.h"
#include "qb2shape.h"
#include "qb2body.h"

QB2Fixture::QB2Fixture(QObject *parent)
    : QObject(parent), m_shapeId(b2_nullShapeId)
{
}

QB2Fixture::~QB2Fixture()
{
    if (b2Shape_IsValid(m_shapeId))
    {
        b2DestroyShape(m_shapeId, true);
    }
}

QB2Shape *QB2Fixture::shape() const
{
    return m_shape;
}

void QB2Fixture::setShape(QB2Shape *shape)
{
    if (m_shape == shape)
        return;
    m_shape = shape;
    if (m_shape)
        m_shape->setParent(this);
    emit shapeChanged();

    if (m_componentComplete)
    {
        createShape();
    }
}

qreal QB2Fixture::density() const
{
    if (b2Shape_IsValid(m_shapeId))
        return b2Shape_GetDensity(m_shapeId);
    return m_density;
}

void QB2Fixture::setDensity(qreal density)
{
    if (qFuzzyCompare(m_density, density))
        return;
    m_density = density;
    if (b2Shape_IsValid(m_shapeId))
        b2Shape_SetDensity(m_shapeId, density, true);
    emit densityChanged();
}

qreal QB2Fixture::friction() const
{
    if (b2Shape_IsValid(m_shapeId))
        return b2Shape_GetFriction(m_shapeId);
    return m_friction;
}

void QB2Fixture::setFriction(qreal friction)
{
    if (qFuzzyCompare(m_friction, friction))
        return;
    m_friction = friction;
    if (b2Shape_IsValid(m_shapeId))
        b2Shape_SetFriction(m_shapeId, friction);
    emit frictionChanged();
}

qreal QB2Fixture::restitution() const
{
    if (b2Shape_IsValid(m_shapeId))
        return b2Shape_GetRestitution(m_shapeId);
    return m_restitution;
}

void QB2Fixture::setRestitution(qreal restitution)
{
    if (qFuzzyCompare(m_restitution, restitution))
        return;
    m_restitution = restitution;
    if (b2Shape_IsValid(m_shapeId))
        b2Shape_SetRestitution(m_shapeId, restitution);
    emit restitutionChanged();
}

bool QB2Fixture::sensor() const
{
    if (b2Shape_IsValid(m_shapeId))
        return b2Shape_IsSensor(m_shapeId);
    return m_sensor;
}

void QB2Fixture::setSensor(bool sensor)
{
    if (m_sensor == sensor)
        return;
    m_sensor = sensor;
    if (b2Shape_IsValid(m_shapeId))
        b2Shape_EnableSensorEvents(m_shapeId, sensor);
    emit sensorChanged();
}

void QB2Fixture::setBody(QB2Body *body)
{
    m_body = body;
}

void QB2Fixture::classBegin()
{
}

void QB2Fixture::componentComplete()
{
    m_componentComplete = true;

    if (!m_body)
    {
        m_body = qobject_cast<QB2Body *>(parent());
    }

    if (!m_body)
    {
        qFatal("Fixture must be a child of Body");
        return;
    }

    if (b2Body_IsValid(m_body->bodyId()))
    {
        if (m_shape)
        {
            createShape();
        }
    }
    else
    {
        connect(m_body, &QB2Body::bodyReady, this, &QB2Fixture::createShape);
    }
}

void QB2Fixture::createShape()
{
    if (!m_componentComplete || !m_body || !m_shape)
        return;

    if (b2Shape_IsValid(m_shapeId))
        return;

    if (!b2Body_IsValid(m_body->bodyId()))
        return;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = m_density;
    shapeDef.isSensor = m_sensor;

    b2Polygon *polygon = m_shape->polygon();
    if (polygon)
    {
        m_shapeId = b2CreatePolygonShape(m_body->bodyId(), &shapeDef, polygon);
        b2Shape_SetFriction(m_shapeId, m_friction);
        b2Shape_SetRestitution(m_shapeId, m_restitution);
    }
}
