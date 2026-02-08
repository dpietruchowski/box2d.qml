#pragma once

#include <QObject>
#include <QQmlParserStatus>
#include <box2d/box2d.h>

class QB2Body;

#include "qb2shape.h"

class QB2Fixture : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QB2Shape *shape READ shape WRITE setShape NOTIFY shapeChanged)
    Q_PROPERTY(qreal density READ density WRITE setDensity NOTIFY densityChanged)
    Q_PROPERTY(qreal friction READ friction WRITE setFriction NOTIFY frictionChanged)
    Q_PROPERTY(qreal restitution READ restitution WRITE setRestitution NOTIFY restitutionChanged)
    Q_PROPERTY(bool sensor READ sensor WRITE setSensor NOTIFY sensorChanged)

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

    b2ShapeId shapeId() const { return m_shapeId; }
    void setBody(QB2Body *body);

    void classBegin() override;
    void componentComplete() override;

signals:
    void shapeChanged();
    void densityChanged();
    void frictionChanged();
    void restitutionChanged();
    void sensorChanged();

private:
    void createShape();

    b2ShapeId m_shapeId;
    QB2Body *m_body = nullptr;
    QB2Shape *m_shape = nullptr;
    qreal m_density = 1.0;
    qreal m_friction = 0.3;
    qreal m_restitution = 0.0;
    bool m_sensor = false;
    bool m_componentComplete = false;
};
