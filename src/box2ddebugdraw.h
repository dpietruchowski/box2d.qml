#ifndef BOX2DDEBUGDRAW_H
#define BOX2DDEBUGDRAW_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <box2d/box2d.h>

class Box2DWorld;

class Box2DDebugDraw : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(Box2DWorld* world READ world WRITE setWorld NOTIFY worldChanged)
    Q_PROPERTY(bool drawShapes READ drawShapes WRITE setDrawShapes NOTIFY drawShapesChanged)
    Q_PROPERTY(bool drawJoints READ drawJoints WRITE setDrawJoints NOTIFY drawJointsChanged)
    Q_PROPERTY(bool drawAABBs READ drawAABBs WRITE setDrawAABBs NOTIFY drawAABBsChanged)
    Q_PROPERTY(bool drawCenterOfMass READ drawCenterOfMass WRITE setDrawCenterOfMass NOTIFY drawCenterOfMassChanged)

public:
    explicit Box2DDebugDraw(QQuickItem *parent = nullptr);
    ~Box2DDebugDraw() override;

    Box2DWorld *world() const;
    void setWorld(Box2DWorld *world);

    bool drawShapes() const;
    void setDrawShapes(bool draw);

    bool drawJoints() const;
    void setDrawJoints(bool draw);

    bool drawAABBs() const;
    void setDrawAABBs(bool draw);

    bool drawCenterOfMass() const;
    void setDrawCenterOfMass(bool draw);

    void paint(QPainter *painter) override;

signals:
    void worldChanged();
    void drawShapesChanged();
    void drawJointsChanged();
    void drawAABBsChanged();
    void drawCenterOfMassChanged();

private slots:
    void onWorldStepped();

private:
    class DebugDrawImpl;

    Box2DWorld *m_world;
    std::unique_ptr<DebugDrawImpl> m_debugDraw;
    bool m_drawShapes;
    bool m_drawJoints;
    bool m_drawAABBs;
    bool m_drawCenterOfMass;
};

#endif // BOX2DDEBUGDRAW_H
