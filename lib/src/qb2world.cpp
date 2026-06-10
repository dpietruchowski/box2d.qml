#include "qb2world.h"
#include "qb2body.h"
#include "qb2fixture.h"

QB2World::QB2World(QQuickItem *parent)
    : QQuickItem(parent)
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {m_gravity.x(), m_gravity.y()};
    m_worldId = b2CreateWorld(&worldDef);

    m_stepTimer = new QTimer(this);
    m_stepTimer->setTimerType(Qt::PreciseTimer);
    connect(m_stepTimer, &QTimer::timeout, this, &QB2World::onStepTimer);

    QTimer::singleShot(0, this, [this]()
                       { emit worldReady(); });
}

QB2World::~QB2World()
{
    if (b2World_IsValid(m_worldId))
    {
        b2DestroyWorld(m_worldId);
    }
}

QVector2D QB2World::gravity() const
{
    return m_gravity;
}

void QB2World::setGravity(const QVector2D &gravity)
{
    if (m_gravity == gravity)
        return;
    m_gravity = gravity;
    if (b2World_IsValid(m_worldId))
        b2World_SetGravity(m_worldId, {gravity.x(), gravity.y()});
    emit gravityChanged();
}

void QB2World::setPixelsPerMeter(qreal ppm)
{
    if (qFuzzyCompare(m_pixelsPerMeter, ppm))
        return;
    m_pixelsPerMeter = ppm;
    emit pixelsPerMeterChanged();
}

qreal QB2World::hitEventThreshold() const
{
    if (b2World_IsValid(m_worldId))
        return b2World_GetHitEventThreshold(m_worldId) * m_pixelsPerMeter;
    return 0.0;
}

void QB2World::setHitEventThreshold(qreal threshold)
{
    if (!b2World_IsValid(m_worldId))
        return;
    const float meters = static_cast<float>(threshold / m_pixelsPerMeter);
    if (qFuzzyCompare(b2World_GetHitEventThreshold(m_worldId), meters))
        return;
    b2World_SetHitEventThreshold(m_worldId, meters);
    emit hitEventThresholdChanged();
}

bool QB2World::running() const
{
    return m_running;
}

void QB2World::setRunning(bool running)
{
    if (m_running == running)
        return;
    m_running = running;
    if (running)
        m_stepTimer->start(qRound(m_timeStep * 1000));
    else
        m_stepTimer->stop();
    emit runningChanged();
}

qreal QB2World::timeStep() const
{
    return m_timeStep;
}

void QB2World::setTimeStep(qreal timeStep)
{
    if (qFuzzyCompare(m_timeStep, timeStep))
        return;
    m_timeStep = timeStep;
    if (m_running)
        m_stepTimer->setInterval(qRound(timeStep * 1000));
    emit timeStepChanged();
}

int QB2World::velocityIterations() const
{
    return m_subStepCount;
}

void QB2World::setVelocityIterations(int iterations)
{
    if (m_subStepCount == iterations)
        return;
    m_subStepCount = iterations;
    emit velocityIterationsChanged();
}

int QB2World::positionIterations() const
{
    return m_subStepCount;
}

void QB2World::setPositionIterations(int iterations)
{
    if (m_subStepCount == iterations)
        return;
    m_subStepCount = iterations;
    emit positionIterationsChanged();
}

QQmlListProperty<QB2Body> QB2World::bodies()
{
    return QQmlListProperty<QB2Body>(this, this,
                                     &QB2World::appendBody,
                                     &QB2World::bodyCount,
                                     &QB2World::body,
                                     &QB2World::clearBodies);
}

void QB2World::step()
{
    if (!b2World_IsValid(m_worldId))
        return;
    b2World_Step(m_worldId, m_timeStep, m_subStepCount);
    emit stepped();
    dispatchEvents();
}

QB2Fixture *QB2World::fixtureFromShape(b2ShapeId shapeId)
{
    if (!b2Shape_IsValid(shapeId))
        return nullptr;
    return static_cast<QB2Fixture *>(b2Shape_GetUserData(shapeId));
}

void QB2World::dispatchEvents()
{
    const qreal ppm = m_pixelsPerMeter;

    b2ContactEvents contactEvents = b2World_GetContactEvents(m_worldId);

    for (int i = 0; i < contactEvents.beginCount; ++i)
    {
        const b2ContactBeginTouchEvent &e = contactEvents.beginEvents[i];
        QB2Fixture *a = fixtureFromShape(e.shapeIdA);
        QB2Fixture *b = fixtureFromShape(e.shapeIdB);
        if (!a || !b)
            continue;
        a->notifyBeginContact(b);
        b->notifyBeginContact(a);
    }

    for (int i = 0; i < contactEvents.endCount; ++i)
    {
        const b2ContactEndTouchEvent &e = contactEvents.endEvents[i];
        QB2Fixture *a = fixtureFromShape(e.shapeIdA);
        QB2Fixture *b = fixtureFromShape(e.shapeIdB);
        if (!a || !b)
            continue;
        a->notifyEndContact(b);
        b->notifyEndContact(a);
    }

    for (int i = 0; i < contactEvents.hitCount; ++i)
    {
        const b2ContactHitEvent &e = contactEvents.hitEvents[i];
        QB2Fixture *a = fixtureFromShape(e.shapeIdA);
        QB2Fixture *b = fixtureFromShape(e.shapeIdB);
        if (!a || !b)
            continue;
        const QVector2D point(e.point.x * ppm, e.point.y * ppm);
        const QVector2D normal(e.normal.x, e.normal.y);
        const qreal speed = e.approachSpeed * ppm;
        a->notifyHit(b, point, normal, speed);
        b->notifyHit(a, point, -normal, speed);
    }

    b2SensorEvents sensorEvents = b2World_GetSensorEvents(m_worldId);

    for (int i = 0; i < sensorEvents.beginCount; ++i)
    {
        const b2SensorBeginTouchEvent &e = sensorEvents.beginEvents[i];
        QB2Fixture *sensor = fixtureFromShape(e.sensorShapeId);
        QB2Fixture *visitor = fixtureFromShape(e.visitorShapeId);
        if (!sensor || !visitor)
            continue;
        sensor->notifyBeginContact(visitor);
        visitor->notifyBeginContact(sensor);
    }

    for (int i = 0; i < sensorEvents.endCount; ++i)
    {
        const b2SensorEndTouchEvent &e = sensorEvents.endEvents[i];
        QB2Fixture *sensor = fixtureFromShape(e.sensorShapeId);
        QB2Fixture *visitor = fixtureFromShape(e.visitorShapeId);
        if (!sensor || !visitor)
            continue;
        sensor->notifyEndContact(visitor);
        visitor->notifyEndContact(sensor);
    }
}

void QB2World::onStepTimer()
{
    step();
}

void QB2World::appendBody(QQmlListProperty<QB2Body> *list, QB2Body *body)
{
    QB2World *world = qobject_cast<QB2World *>(list->object);
    if (world && body)
    {
        world->m_bodies.append(body);
        body->setParent(world);
    }
}

qsizetype QB2World::bodyCount(QQmlListProperty<QB2Body> *list)
{
    QB2World *world = qobject_cast<QB2World *>(list->object);
    return world ? world->m_bodies.count() : 0;
}

QB2Body *QB2World::body(QQmlListProperty<QB2Body> *list, qsizetype index)
{
    QB2World *world = qobject_cast<QB2World *>(list->object);
    return world ? world->m_bodies.at(index) : nullptr;
}

void QB2World::clearBodies(QQmlListProperty<QB2Body> *list)
{
    QB2World *world = qobject_cast<QB2World *>(list->object);
    if (world)
        world->m_bodies.clear();
}
