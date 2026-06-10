#include "qb2fixture.h"
#include "shapes/qb2shape.h"
#include "qb2body.h"
#include "qb2world.h"

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
    // Box2D v3 does not allow toggling the sensor flag on an existing shape,
    // so recreate the shape with the new definition.
    if (b2Shape_IsValid(m_shapeId))
    {
        b2DestroyShape(m_shapeId, true);
        m_shapeId = b2_nullShapeId;
        createShape();
    }
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
    shapeDef.enableSensorEvents = m_sensor;

    const float invPpm = m_body->world()
                       ? static_cast<float>(1.0 / m_body->world()->pixelsPerMeter())
                       : 1.0f;

    b2Polygon *polygon = m_shape->polygon();
    if (polygon)
    {
        b2Polygon scaled = *polygon;
        for (int i = 0; i < scaled.count; ++i)
        {
            scaled.vertices[i].x *= invPpm;
            scaled.vertices[i].y *= invPpm;
        }
        scaled.centroid.x *= invPpm;
        scaled.centroid.y *= invPpm;
        m_shapeId = b2CreatePolygonShape(m_body->bodyId(), &shapeDef, &scaled);
    }

    b2Circle *circle = m_shape->circle();
    if (circle)
    {
        b2Circle scaled = *circle;
        scaled.center.x *= invPpm;
        scaled.center.y *= invPpm;
        scaled.radius  *= invPpm;
        m_shapeId = b2CreateCircleShape(m_body->bodyId(), &shapeDef, &scaled);
    }

    b2Capsule *capsule = m_shape->capsule();
    if (capsule)
    {
        b2Capsule scaled = *capsule;
        scaled.center1.x *= invPpm;  scaled.center1.y *= invPpm;
        scaled.center2.x *= invPpm;  scaled.center2.y *= invPpm;
        scaled.radius    *= invPpm;
        m_shapeId = b2CreateCapsuleShape(m_body->bodyId(), &shapeDef, &scaled);
    }

    b2Segment *segment = m_shape->segment();
    if (segment)
    {
        b2Segment scaled = *segment;
        scaled.point1.x *= invPpm;  scaled.point1.y *= invPpm;
        scaled.point2.x *= invPpm;  scaled.point2.y *= invPpm;
        m_shapeId = b2CreateSegmentShape(m_body->bodyId(), &shapeDef, &scaled);
    }

    if (b2Shape_IsValid(m_shapeId))
    {
        b2Shape_SetFriction(m_shapeId, m_friction);
        b2Shape_SetRestitution(m_shapeId, m_restitution);
    }
}
