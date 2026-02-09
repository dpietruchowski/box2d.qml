#pragma once

#include <QObject>
#include <QQmlParserStatus>
#include <QVector>
#include <QPointF>
#include <box2d/box2d.h>

class QB2Body;

class QB2Chain : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QVector<QPointF> points READ points WRITE setPoints NOTIFY pointsChanged)
    Q_PROPERTY(bool isLoop READ isLoop WRITE setIsLoop NOTIFY isLoopChanged)
    Q_PROPERTY(qreal friction READ friction WRITE setFriction NOTIFY frictionChanged)
    Q_PROPERTY(qreal restitution READ restitution WRITE setRestitution NOTIFY restitutionChanged)

public:
    explicit QB2Chain(QObject *parent = nullptr);
    ~QB2Chain();

    QVector<QPointF> points() const;
    void setPoints(const QVector<QPointF> &points);

    bool isLoop() const { return m_isLoop; }
    void setIsLoop(bool loop);

    qreal friction() const { return m_friction; }
    void setFriction(qreal friction);

    qreal restitution() const { return m_restitution; }
    void setRestitution(qreal restitution);

    b2ChainId chainId() const { return m_chainId; }
    void setBody(QB2Body *body);

    void classBegin() override;
    void componentComplete() override;

signals:
    void pointsChanged();
    void isLoopChanged();
    void frictionChanged();
    void restitutionChanged();

private:
    void createChain();

    b2ChainId m_chainId;
    QB2Body *m_body = nullptr;
    QVector<QPointF> m_points;
    bool m_isLoop = false;
    qreal m_friction = 0.3;
    qreal m_restitution = 0.0;
    bool m_componentComplete = false;
};
