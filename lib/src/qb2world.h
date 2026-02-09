#pragma once

#include <QQuickItem>
#include <QVector2D>
#include <QQmlListProperty>
#include <QTimer>
#include <box2d/box2d.h>

class QB2Body;

class QB2World : public QQuickItem
{
    Q_OBJECT
    Q_CLASSINFO("DefaultProperty", "bodies")
    Q_PROPERTY(QVector2D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(qreal timeStep READ timeStep WRITE setTimeStep NOTIFY timeStepChanged)
    Q_PROPERTY(int velocityIterations READ velocityIterations WRITE setVelocityIterations NOTIFY velocityIterationsChanged)
    Q_PROPERTY(int positionIterations READ positionIterations WRITE setPositionIterations NOTIFY positionIterationsChanged)
    Q_PROPERTY(QQmlListProperty<QB2Body> bodies READ bodies)

public:
    explicit QB2World(QQuickItem *parent = nullptr);
    ~QB2World();

    QVector2D gravity() const;
    void setGravity(const QVector2D &gravity);

    bool running() const;
    void setRunning(bool running);

    qreal timeStep() const;
    void setTimeStep(qreal timeStep);

    int velocityIterations() const;
    void setVelocityIterations(int iterations);

    int positionIterations() const;
    void setPositionIterations(int iterations);

    QQmlListProperty<QB2Body> bodies();

    b2WorldId worldId() const { return m_worldId; }

    Q_INVOKABLE void step();

signals:
    void gravityChanged();
    void runningChanged();
    void timeStepChanged();
    void velocityIterationsChanged();
    void positionIterationsChanged();
    void stepped();
    void worldReady();

private slots:
    void onStepTimer();

private:
    static void appendBody(QQmlListProperty<QB2Body> *list, QB2Body *body);
    static qsizetype bodyCount(QQmlListProperty<QB2Body> *list);
    static QB2Body *body(QQmlListProperty<QB2Body> *list, qsizetype index);
    static void clearBodies(QQmlListProperty<QB2Body> *list);

    b2WorldId m_worldId;
    QVector2D m_gravity{0.0f, 9.8f};
    bool m_running = false;
    qreal m_timeStep = 1.0 / 60.0;
    int m_subStepCount = 4;
    QTimer *m_stepTimer = nullptr;
    QList<QB2Body *> m_bodies;
};
