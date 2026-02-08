#include "box2dworld.h"
#include "box2dbody.h"
#include "box2dcontact.h"
#include <QDebug>

class Box2DWorld::ContactListener : public b2ContactListener
{
public:
    explicit ContactListener(Box2DWorld *world) : m_world(world) {}

    void BeginContact(b2Contact *contact) override
    {
        Box2DContact contactWrapper(contact, m_world->pixelsPerMeter());
        emit m_world->beginContact(&contactWrapper);
    }

    void EndContact(b2Contact *contact) override
    {
        Box2DContact contactWrapper(contact, m_world->pixelsPerMeter());
        emit m_world->endContact(&contactWrapper);
    }

private:
    Box2DWorld *m_world;
};

Box2DWorld::Box2DWorld(QQuickItem *parent)
    : QQuickItem(parent)
    , m_gravity(0.0, 10.0)
    , m_running(true)
    , m_timeStep(1.0 / 60.0)
    , m_velocityIterations(8)
    , m_positionIterations(3)
    , m_pixelsPerMeter(32.0)
{
    // Convert from QML coordinates (Y down) to Box2D coordinates (Y up)
    m_world = std::make_unique<b2World>(b2Vec2(m_gravity.x(), -m_gravity.y()));
    m_contactListener = std::make_unique<ContactListener>(this);
    m_world->SetContactListener(m_contactListener.get());

    connect(&m_timer, &QTimer::timeout, this, &Box2DWorld::step);
    m_timer.setInterval(m_timeStep * 1000);
}

Box2DWorld::~Box2DWorld()
{
    m_timer.stop();
}

QPointF Box2DWorld::gravity() const
{
    return m_gravity;
}

void Box2DWorld::setGravity(const QPointF &gravity)
{
    if (m_gravity == gravity)
        return;

    m_gravity = gravity;
    if (m_world)
        // Convert from QML coordinates (Y down) to Box2D coordinates (Y up)
        m_world->SetGravity(b2Vec2(gravity.x(), -gravity.y()));
    emit gravityChanged();
}

bool Box2DWorld::running() const
{
    return m_running;
}

void Box2DWorld::setRunning(bool running)
{
    if (m_running == running)
        return;

    m_running = running;
    
    if (m_running && isComponentComplete())
        m_timer.start();
    else
        m_timer.stop();
        
    emit runningChanged();
}

qreal Box2DWorld::timeStep() const
{
    return m_timeStep;
}

void Box2DWorld::setTimeStep(qreal timeStep)
{
    if (qFuzzyCompare(m_timeStep, timeStep))
        return;

    m_timeStep = timeStep;
    m_timer.setInterval(timeStep * 1000);
    emit timeStepChanged();
}

int Box2DWorld::velocityIterations() const
{
    return m_velocityIterations;
}

void Box2DWorld::setVelocityIterations(int iterations)
{
    if (m_velocityIterations == iterations)
        return;

    m_velocityIterations = iterations;
    emit velocityIterationsChanged();
}

int Box2DWorld::positionIterations() const
{
    return m_positionIterations;
}

void Box2DWorld::setPositionIterations(int iterations)
{
    if (m_positionIterations == iterations)
        return;

    m_positionIterations = iterations;
    emit positionIterationsChanged();
}

qreal Box2DWorld::pixelsPerMeter() const
{
    return m_pixelsPerMeter;
}

void Box2DWorld::setPixelsPerMeter(qreal pixelsPerMeter)
{
    if (qFuzzyCompare(m_pixelsPerMeter, pixelsPerMeter))
        return;

    m_pixelsPerMeter = pixelsPerMeter;
    emit pixelsPerMeterChanged();
}

void Box2DWorld::registerBody(Box2DBody *body)
{
    if (!m_bodies.contains(body))
        m_bodies.append(body);
}

void Box2DWorld::unregisterBody(Box2DBody *body)
{
    m_bodies.removeOne(body);
}

void Box2DWorld::componentComplete()
{
    QQuickItem::componentComplete();
    
    if (m_running)
        m_timer.start();
}

void Box2DWorld::itemChange(ItemChange change, const ItemChangeData &data)
{
    if (change == ItemSceneChange && !data.item) {
        m_timer.stop();
    }
    QQuickItem::itemChange(change, data);
}

void Box2DWorld::step()
{
    if (!m_world)
        return;

    m_world->Step(m_timeStep, m_velocityIterations, m_positionIterations);
    
    // Update all registered bodies
    for (Box2DBody *body : m_bodies) {
        body->synchronize();
    }
    
    emit stepped();
}
