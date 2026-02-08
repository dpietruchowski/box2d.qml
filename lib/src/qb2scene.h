#pragma once

#include <QQuickItem>
#include <QVector2D>
#include "qb2world.h"

class QB2Scene : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QB2World *world READ world WRITE setWorld NOTIFY worldChanged)
    Q_PROPERTY(QVector2D cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged)
    Q_PROPERTY(qreal cameraZoom READ cameraZoom WRITE setCameraZoom NOTIFY cameraZoomChanged)
    Q_PROPERTY(qreal cameraRotation READ cameraRotation WRITE setCameraRotation NOTIFY cameraRotationChanged)

public:
    explicit QB2Scene(QQuickItem *parent = nullptr);

    QB2World *world() const { return m_world; }
    void setWorld(QB2World *world);

    QVector2D cameraPosition() const { return m_cameraPosition; }
    void setCameraPosition(const QVector2D &position);

    qreal cameraZoom() const { return m_cameraZoom; }
    void setCameraZoom(qreal zoom);

    qreal cameraRotation() const { return m_cameraRotation; }
    void setCameraRotation(qreal rotation);

signals:
    void worldChanged();
    void cameraPositionChanged();
    void cameraZoomChanged();
    void cameraRotationChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private:
    QB2World *m_world = nullptr;
    QVector2D m_cameraPosition{0.0f, 0.0f};
    qreal m_cameraZoom = 1.0;
    qreal m_cameraRotation = 0.0;
};
