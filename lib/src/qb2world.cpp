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
        m_stepTimer->start(static_cast<int>(m_timeStep * 1000));
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
        m_stepTimer->setInterval(static_cast<int>(timeStep * 1000));
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
