#ifndef BOX2DWORLD_H
#define BOX2DWORLD_H

#include <QObject>
#include <QPointF>
#include <QTimer>
#include <QQuickItem>
#include <box2d/box2d.h>
#include <memory>

class Box2DBody;
class Box2DContact;

class Box2DWorld : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(qreal timeStep READ timeStep WRITE setTimeStep NOTIFY timeStepChanged)
    Q_PROPERTY(int velocityIterations READ velocityIterations WRITE setVelocityIterations NOTIFY velocityIterationsChanged)
    Q_PROPERTY(int positionIterations READ positionIterations WRITE setPositionIterations NOTIFY positionIterationsChanged)
    Q_PROPERTY(qreal pixelsPerMeter READ pixelsPerMeter WRITE setPixelsPerMeter NOTIFY pixelsPerMeterChanged)

public:
    explicit Box2DWorld(QQuickItem *parent = nullptr);
    ~Box2DWorld() override;

    QPointF gravity() const;
    void setGravity(const QPointF &gravity);

    bool running() const;
    void setRunning(bool running);

    qreal timeStep() const;
    void setTimeStep(qreal timeStep);

    int velocityIterations() const;
    void setVelocityIterations(int iterations);

    int positionIterations() const;
    void setPositionIterations(int iterations);

    qreal pixelsPerMeter() const;
    void setPixelsPerMeter(qreal pixelsPerMeter);

    b2World *world() { return m_world.get(); }

    void registerBody(Box2DBody *body);
    void unregisterBody(Box2DBody *body);

signals:
    void gravityChanged();
    void runningChanged();
    void timeStepChanged();
    void velocityIterationsChanged();
    void positionIterationsChanged();
    void pixelsPerMeterChanged();
    void stepped();
    void beginContact(Box2DContact *contact);
    void endContact(Box2DContact *contact);

protected:
    void componentComplete() override;
    void itemChange(ItemChange change, const ItemChangeData &data) override;

private slots:
    void step();

private:
    std::unique_ptr<b2World> m_world;
    QTimer m_timer;
    QPointF m_gravity;
    bool m_running;
    qreal m_timeStep;
    int m_velocityIterations;
    int m_positionIterations;
    qreal m_pixelsPerMeter;
    QList<Box2DBody*> m_bodies;
    
    class ContactListener;
    std::unique_ptr<ContactListener> m_contactListener;
};

#endif // BOX2DWORLD_H
