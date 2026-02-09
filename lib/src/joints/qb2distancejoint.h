#pragma once

#include "qb2joint.h"
#include <QPointF>
#include <limits>

class QB2DistanceJoint : public QB2Joint
{
    Q_OBJECT
    Q_PROPERTY(QPointF localAnchorA READ localAnchorA WRITE setLocalAnchorA NOTIFY localAnchorAChanged)
    Q_PROPERTY(QPointF localAnchorB READ localAnchorB WRITE setLocalAnchorB NOTIFY localAnchorBChanged)
    Q_PROPERTY(qreal length READ length WRITE setLength NOTIFY lengthChanged)
    Q_PROPERTY(qreal minLength READ minLength WRITE setMinLength NOTIFY minLengthChanged)
    Q_PROPERTY(qreal maxLength READ maxLength WRITE setMaxLength NOTIFY maxLengthChanged)
    Q_PROPERTY(qreal hertz READ hertz WRITE setHertz NOTIFY hertzChanged)
    Q_PROPERTY(qreal dampingRatio READ dampingRatio WRITE setDampingRatio NOTIFY dampingRatioChanged)

public:
    explicit QB2DistanceJoint(QObject *parent = nullptr);
    ~QB2DistanceJoint();

    QPointF localAnchorA() const { return m_localAnchorA; }
    void setLocalAnchorA(const QPointF &anchor);

    QPointF localAnchorB() const { return m_localAnchorB; }
    void setLocalAnchorB(const QPointF &anchor);

    qreal length() const { return m_length; }
    void setLength(qreal length);

    qreal minLength() const { return m_minLength; }
    void setMinLength(qreal length);

    qreal maxLength() const { return m_maxLength; }
    void setMaxLength(qreal length);

    qreal hertz() const { return m_hertz; }
    void setHertz(qreal hertz);

    qreal dampingRatio() const { return m_dampingRatio; }
    void setDampingRatio(qreal ratio);

signals:
    void localAnchorAChanged();
    void localAnchorBChanged();
    void lengthChanged();
    void minLengthChanged();
    void maxLengthChanged();
    void hertzChanged();
    void dampingRatioChanged();

protected:
    void createJoint() override;

private:
    QPointF m_localAnchorA{0.0, 0.0};
    QPointF m_localAnchorB{0.0, 0.0};
    qreal m_length = 1.0;
    qreal m_minLength = 0.0;
    qreal m_maxLength = std::numeric_limits<float>::max();
    qreal m_hertz = 0.0;
    qreal m_dampingRatio = 0.0;
};
