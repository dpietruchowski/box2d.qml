#pragma once

#include "qb2joint.h"
#include <QPointF>

class QB2WeldJoint : public QB2Joint
{
    Q_OBJECT
    Q_PROPERTY(QPointF localAnchorA READ localAnchorA WRITE setLocalAnchorA NOTIFY localAnchorAChanged)
    Q_PROPERTY(QPointF localAnchorB READ localAnchorB WRITE setLocalAnchorB NOTIFY localAnchorBChanged)
    Q_PROPERTY(qreal referenceAngle READ referenceAngle WRITE setReferenceAngle NOTIFY referenceAngleChanged)
    Q_PROPERTY(qreal linearHertz READ linearHertz WRITE setLinearHertz NOTIFY linearHertzChanged)
    Q_PROPERTY(qreal linearDampingRatio READ linearDampingRatio WRITE setLinearDampingRatio NOTIFY linearDampingRatioChanged)
    Q_PROPERTY(qreal angularHertz READ angularHertz WRITE setAngularHertz NOTIFY angularHertzChanged)
    Q_PROPERTY(qreal angularDampingRatio READ angularDampingRatio WRITE setAngularDampingRatio NOTIFY angularDampingRatioChanged)

public:
    explicit QB2WeldJoint(QObject *parent = nullptr);
    ~QB2WeldJoint();

    QPointF localAnchorA() const { return m_localAnchorA; }
    void setLocalAnchorA(const QPointF &anchor);

    QPointF localAnchorB() const { return m_localAnchorB; }
    void setLocalAnchorB(const QPointF &anchor);

    qreal referenceAngle() const { return m_referenceAngle; }
    void setReferenceAngle(qreal angle);

    qreal linearHertz() const { return m_linearHertz; }
    void setLinearHertz(qreal hertz);

    qreal linearDampingRatio() const { return m_linearDampingRatio; }
    void setLinearDampingRatio(qreal ratio);

    qreal angularHertz() const { return m_angularHertz; }
    void setAngularHertz(qreal hertz);

    qreal angularDampingRatio() const { return m_angularDampingRatio; }
    void setAngularDampingRatio(qreal ratio);

signals:
    void localAnchorAChanged();
    void localAnchorBChanged();
    void referenceAngleChanged();
    void linearHertzChanged();
    void linearDampingRatioChanged();
    void angularHertzChanged();
    void angularDampingRatioChanged();

protected:
    void createJoint() override;

private:
    QPointF m_localAnchorA{0.0, 0.0};
    QPointF m_localAnchorB{0.0, 0.0};
    qreal m_referenceAngle = 0.0;
    qreal m_linearHertz = 0.0;
    qreal m_linearDampingRatio = 0.0;
    qreal m_angularHertz = 0.0;
    qreal m_angularDampingRatio = 0.0;
};
