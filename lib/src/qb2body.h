#pragma once

#include <QObject>
#include <QVector2D>
#include <QQmlListProperty>
#include <QQmlParserStatus>
#include <box2d/box2d.h>

class QB2Fixture;
class QB2World;

class QB2Body : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QVector2D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(qreal linearVelocityX READ linearVelocityX WRITE setLinearVelocityX NOTIFY linearVelocityXChanged)
    Q_PROPERTY(qreal linearVelocityY READ linearVelocityY WRITE setLinearVelocityY NOTIFY linearVelocityYChanged)
    Q_PROPERTY(qreal angularVelocity READ angularVelocity WRITE setAngularVelocity NOTIFY angularVelocityChanged)
    Q_PROPERTY(BodyType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QQmlListProperty<QB2Fixture> fixtures READ fixtures)
    Q_CLASSINFO("DefaultProperty", "fixtures")

public:
    enum BodyType
    {
        Static = b2_staticBody,
        Kinematic = b2_kinematicBody,
        Dynamic = b2_dynamicBody
    };
    Q_ENUM(BodyType)

    explicit QB2Body(QObject *parent = nullptr);
    ~QB2Body();

    QVector2D position() const;
    void setPosition(const QVector2D &position);

    qreal x() const;
    void setX(qreal x);

    qreal y() const;
    void setY(qreal y);

    qreal angle() const;
    void setAngle(qreal angle);

    qreal linearVelocityX() const;
    void setLinearVelocityX(qreal vx);

    qreal linearVelocityY() const;
    void setLinearVelocityY(qreal vy);

    qreal angularVelocity() const;
    void setAngularVelocity(qreal omega);

    BodyType type() const;
    void setType(BodyType type);

    QQmlListProperty<QB2Fixture> fixtures();

    b2BodyId bodyId() const { return m_bodyId; }
    void setWorld(QB2World *world);

    void classBegin() override;
    void componentComplete() override;

signals:
    void positionChanged();
    void xChanged();
    void yChanged();
    void angleChanged();
    void linearVelocityXChanged();
    void linearVelocityYChanged();
    void angularVelocityChanged();
    void typeChanged();
    void bodyReady();

private:
    void createBody();

    static void appendFixture(QQmlListProperty<QB2Fixture> *list, QB2Fixture *fixture);
    static qsizetype fixtureCount(QQmlListProperty<QB2Fixture> *list);
    static QB2Fixture *fixture(QQmlListProperty<QB2Fixture> *list, qsizetype index);
    static void clearFixtures(QQmlListProperty<QB2Fixture> *list);

    b2BodyId m_bodyId;
    QB2World *m_world = nullptr;
    QVector2D m_position{0.0f, 0.0f};
    BodyType m_type = Dynamic;
    QList<QB2Fixture *> m_fixtures;
    bool m_componentComplete = false;
};
