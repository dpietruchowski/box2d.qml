#include "box2ddebugdraw.h"
#include "box2dworld.h"
#include <QPainter>

class Box2DDebugDraw::DebugDrawImpl : public b2Draw
{
public:
    explicit DebugDrawImpl(QPainter *painter, qreal pixelsPerMeter)
        : m_painter(painter)
        , m_pixelsPerMeter(pixelsPerMeter)
    {
    }

    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override
    {
        if (!m_painter)
            return;

        QPainterPath path;
        for (int i = 0; i < vertexCount; ++i) {
            QPointF point = toPixels(vertices[i]);
            if (i == 0)
                path.moveTo(point);
            else
                path.lineTo(point);
        }
        path.closeSubpath();

        m_painter->setPen(toQColor(color));
        m_painter->setBrush(Qt::NoBrush);
        m_painter->drawPath(path);
    }

    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override
    {
        if (!m_painter)
            return;

        QPainterPath path;
        for (int i = 0; i < vertexCount; ++i) {
            QPointF point = toPixels(vertices[i]);
            if (i == 0)
                path.moveTo(point);
            else
                path.lineTo(point);
        }
        path.closeSubpath();

        QColor fillColor = toQColor(color);
        fillColor.setAlpha(128);
        
        m_painter->setPen(toQColor(color));
        m_painter->setBrush(fillColor);
        m_painter->drawPath(path);
    }

    void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override
    {
        if (!m_painter)
            return;

        QPointF c = toPixels(center);
        qreal r = radius * m_pixelsPerMeter;

        m_painter->setPen(toQColor(color));
        m_painter->setBrush(Qt::NoBrush);
        m_painter->drawEllipse(c, r, r);
    }

    void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override
    {
        if (!m_painter)
            return;

        QPointF c = toPixels(center);
        qreal r = radius * m_pixelsPerMeter;

        QColor fillColor = toQColor(color);
        fillColor.setAlpha(128);

        m_painter->setPen(toQColor(color));
        m_painter->setBrush(fillColor);
        m_painter->drawEllipse(c, r, r);

        // Draw axis
        QPointF axisEnd = toPixels(b2Vec2(center.x + axis.x * radius, center.y + axis.y * radius));
        m_painter->drawLine(c, axisEnd);
    }

    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override
    {
        if (!m_painter)
            return;

        m_painter->setPen(toQColor(color));
        m_painter->drawLine(toPixels(p1), toPixels(p2));
    }

    void DrawTransform(const b2Transform &xf) override
    {
        if (!m_painter)
            return;

        const float axisScale = 0.4f;
        QPointF p = toPixels(xf.p);

        b2Vec2 pxVec = xf.p + axisScale * xf.q.GetXAxis();
        QPointF px = toPixels(pxVec);

        b2Vec2 pyVec = xf.p + axisScale * xf.q.GetYAxis();
        QPointF py = toPixels(pyVec);

        m_painter->setPen(QPen(Qt::red, 2));
        m_painter->drawLine(p, px);

        m_painter->setPen(QPen(Qt::green, 2));
        m_painter->drawLine(p, py);
    }

    void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override
    {
        if (!m_painter)
            return;

        QPointF point = toPixels(p);
        qreal s = size;

        m_painter->setPen(Qt::NoPen);
        m_painter->setBrush(toQColor(color));
        m_painter->drawEllipse(point, s, s);
    }

private:
    QPointF toPixels(const b2Vec2 &vec) const
    {
        return QPointF(vec.x * m_pixelsPerMeter, -vec.y * m_pixelsPerMeter);
    }

    QColor toQColor(const b2Color &color) const
    {
        return QColor::fromRgbF(color.r, color.g, color.b, color.a);
    }

    QPainter *m_painter;
    qreal m_pixelsPerMeter;
};

Box2DDebugDraw::Box2DDebugDraw(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_world(nullptr)
    , m_drawShapes(true)
    , m_drawJoints(true)
    , m_drawAABBs(false)
    , m_drawCenterOfMass(false)
{
    setFlag(ItemHasContents, true);
}

Box2DDebugDraw::~Box2DDebugDraw()
{
}

Box2DWorld *Box2DDebugDraw::world() const
{
    return m_world;
}

void Box2DDebugDraw::setWorld(Box2DWorld *world)
{
    if (m_world == world)
        return;

    if (m_world)
        disconnect(m_world, &Box2DWorld::stepped, this, &Box2DDebugDraw::onWorldStepped);

    m_world = world;

    if (m_world)
        connect(m_world, &Box2DWorld::stepped, this, &Box2DDebugDraw::onWorldStepped);

    emit worldChanged();
}

bool Box2DDebugDraw::drawShapes() const
{
    return m_drawShapes;
}

void Box2DDebugDraw::setDrawShapes(bool draw)
{
    if (m_drawShapes == draw)
        return;

    m_drawShapes = draw;
    emit drawShapesChanged();
}

bool Box2DDebugDraw::drawJoints() const
{
    return m_drawJoints;
}

void Box2DDebugDraw::setDrawJoints(bool draw)
{
    if (m_drawJoints == draw)
        return;

    m_drawJoints = draw;
    emit drawJointsChanged();
}

bool Box2DDebugDraw::drawAABBs() const
{
    return m_drawAABBs;
}

void Box2DDebugDraw::setDrawAABBs(bool draw)
{
    if (m_drawAABBs == draw)
        return;

    m_drawAABBs = draw;
    emit drawAABBsChanged();
}

bool Box2DDebugDraw::drawCenterOfMass() const
{
    return m_drawCenterOfMass;
}

void Box2DDebugDraw::setDrawCenterOfMass(bool draw)
{
    if (m_drawCenterOfMass == draw)
        return;

    m_drawCenterOfMass = draw;
    emit drawCenterOfMassChanged();
}

void Box2DDebugDraw::paint(QPainter *painter)
{
    if (!m_world || !m_world->world())
        return;

    painter->setRenderHint(QPainter::Antialiasing);

    m_debugDraw = std::make_unique<DebugDrawImpl>(painter, m_world->pixelsPerMeter());

    uint32 flags = 0;
    if (m_drawShapes)
        flags |= b2Draw::e_shapeBit;
    if (m_drawJoints)
        flags |= b2Draw::e_jointBit;
    if (m_drawAABBs)
        flags |= b2Draw::e_aabbBit;
    if (m_drawCenterOfMass)
        flags |= b2Draw::e_centerOfMassBit;

    m_debugDraw->SetFlags(flags);
    m_world->world()->SetDebugDraw(m_debugDraw.get());
    m_world->world()->DebugDraw();
}

void Box2DDebugDraw::onWorldStepped()
{
    update();
}
