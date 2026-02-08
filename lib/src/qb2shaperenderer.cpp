#include "qb2shaperenderer.h"
#include "qb2body.h"
#include "qb2fixture.h"
#include "qb2shape.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <box2d/box2d.h>
#include <cmath>

QB2ShapeRenderer::QB2ShapeRenderer(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
}

void QB2ShapeRenderer::setShowBoundingBox(bool show)
{
    if (m_showBoundingBox == show)
        return;
    m_showBoundingBox = show;
    update();
    emit showBoundingBoxChanged();
}

void QB2ShapeRenderer::paint(QPainter *painter)
{
    QB2Body *body = qobject_cast<QB2Body *>(parentItem());
    if (!body)
        return;

    painter->setRenderHint(QPainter::Antialiasing);

    // Draw bounding box if requested
    if (m_showBoundingBox)
    {
        QPen bboxPen(Qt::red, 1);
        painter->setPen(bboxPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
    }

    // Get transform origin (centroid position in item coordinates)
    QPointF centroid = body->transformOriginPoint();

    // Get body's bodyId to fetch shapes from Box2D
    b2BodyId bodyId = body->bodyId();
    if (!b2Body_IsValid(bodyId))
        return;

    int shapeCount = b2Body_GetShapeCount(bodyId);
    if (shapeCount == 0)
        return;

    QVector<b2ShapeId> shapes(shapeCount);
    b2Body_GetShapes(bodyId, shapes.data(), shapeCount);

    // Get fixtures to access QB2Shape styling properties
    QList<QB2Fixture *> fixtures = body->findChildren<QB2Fixture *>();

    // Draw each shape
    for (int i = 0; i < shapeCount && i < fixtures.count(); ++i)
    {
        QB2Shape *qbShape = fixtures[i]->shape();
        if (!qbShape)
            continue;

        QPen pen(qbShape->strokeColor(), qbShape->strokeWidth());
        QBrush brush(qbShape->fillColor());
        painter->setPen(pen);
        painter->setBrush(brush);

        b2ShapeType shapeType = b2Shape_GetType(shapes[i]);

        switch (shapeType)
        {
        case b2_polygonShape:
        {
            b2Polygon polygon = b2Shape_GetPolygon(shapes[i]);
            QPolygonF qpolygon;
            for (int v = 0; v < polygon.count; ++v)
            {
                QPointF point(centroid.x() + polygon.vertices[v].x,
                              centroid.y() + polygon.vertices[v].y);
                qpolygon << point;
            }
            painter->drawPolygon(qpolygon);
            break;
        }
        case b2_circleShape:
        {
            b2Circle circle = b2Shape_GetCircle(shapes[i]);
            QPointF center(centroid.x() + circle.center.x,
                           centroid.y() + circle.center.y);
            painter->drawEllipse(center, circle.radius, circle.radius);
            break;
        }
        case b2_capsuleShape:
        {
            b2Capsule capsule = b2Shape_GetCapsule(shapes[i]);
            QPointF c1(centroid.x() + capsule.center1.x,
                       centroid.y() + capsule.center1.y);
            QPointF c2(centroid.x() + capsule.center2.x,
                       centroid.y() + capsule.center2.y);

            // Draw two circles and a rectangle connecting them
            painter->drawEllipse(c1, capsule.radius, capsule.radius);
            painter->drawEllipse(c2, capsule.radius, capsule.radius);

            // Calculate perpendicular offset for rectangle
            qreal dx = c2.x() - c1.x();
            qreal dy = c2.y() - c1.y();
            qreal length = std::sqrt(dx * dx + dy * dy);
            if (length > 0)
            {
                qreal nx = -dy / length * capsule.radius;
                qreal ny = dx / length * capsule.radius;

                QPolygonF rect;
                rect << QPointF(c1.x() + nx, c1.y() + ny);
                rect << QPointF(c2.x() + nx, c2.y() + ny);
                rect << QPointF(c2.x() - nx, c2.y() - ny);
                rect << QPointF(c1.x() - nx, c1.y() - ny);
                painter->drawPolygon(rect);
            }
            break;
        }
        case b2_segmentShape:
        {
            b2Segment segment = b2Shape_GetSegment(shapes[i]);
            QPointF p1(centroid.x() + segment.point1.x,
                       centroid.y() + segment.point1.y);
            QPointF p2(centroid.x() + segment.point2.x,
                       centroid.y() + segment.point2.y);
            painter->drawLine(p1, p2);
            break;
        }
        default:
            break;
        }
    }
}
