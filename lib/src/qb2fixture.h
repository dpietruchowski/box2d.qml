#pragma once

#include <QObject>
#include <QQmlParserStatus>
#include <QVector2D>
#include <box2d/box2d.h>

class QB2Body;

#include "shapes/qb2shape.h"

class QB2Fixture : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QB2Shape *shape READ shape WRITE setShape NOTIFY shapeChanged)
    Q_PROPERTY(qreal density READ density WRITE setDensity NOTIFY densityChanged)
    Q_PROPERTY(qreal friction READ friction WRITE setFriction NOTIFY frictionChanged)
    Q_PROPERTY(qreal restitution READ restitution WRITE setRestitution NOTIFY restitutionChanged)
    Q_PROPERTY(bool sensor READ sensor WRITE setSensor NOTIFY sensorChanged)
    Q_PROPERTY(bool enableContactEvents READ enableContactEvents WRITE setEnableContactEvents NOTIFY enableContactEventsChanged)
    Q_PROPERTY(bool enableHitEvents READ enableHitEvents WRITE setEnableHitEvents NOTIFY enableHitEventsChanged)
    Q_PROPERTY(QB2Body *body READ body CONSTANT)

public:
    explicit QB2Fixture(QObject *parent = nullptr);
    ~QB2Fixture();

    QB2Shape *shape() const;
    void setShape(QB2Shape *shape);

    qreal density() const;
    void setDensity(qreal density);

    qreal friction() const;
    void setFriction(qreal friction);

    qreal restitution() const;
    void setRestitution(qreal restitution);

    bool sensor() const;
    void setSensor(bool sensor);

    bool enableContactEvents() const;
    void setEnableContactEvents(bool enable);

    bool enableHitEvents() const;
    void setEnableHitEvents(bool enable);

    b2ShapeId shapeId() const { return m_shapeId; }
    void setBody(QB2Body *body);
    QB2Body *body() const { return m_body; }

    // Called by QB2World when dispatching Box2D events after a step
    void notifyBeginContact(QB2Fixture *other);
    void notifyEndContact(QB2Fixture *other);
    void notifyHit(QB2Fixture *other, const QVector2D &point,
                   const QVector2D &normal, qreal approachSpeed);

    void classBegin() override;
    void componentComplete() override;

signals:
    void shapeChanged();
    void densityChanged();
    void frictionChanged();
    void restitutionChanged();
    void sensorChanged();
    void enableContactEventsChanged();
    void enableHitEventsChanged();
    void beginContact(QB2Fixture *other);
    void endContact(QB2Fixture *other);
    void hit(QB2Fixture *other, QVector2D point, QVector2D normal, qreal approachSpeed);

private:
    void createShape();

    b2ShapeId m_shapeId;
    QB2Body *m_body = nullptr;
    QB2Shape *m_shape = nullptr;
    qreal m_density = 1.0;
    qreal m_friction = 0.3;
    qreal m_restitution = 0.0;
    bool m_sensor = false;
    bool m_enableContactEvents = true;
    bool m_enableHitEvents = false;
    bool m_componentComplete = false;
};
