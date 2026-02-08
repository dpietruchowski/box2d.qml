#include "qb2scene.h"
#include "qb2world.h"
#include "qb2body.h"
#include "qb2fixture.h"
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QSGTransformNode>

QB2Scene::QB2Scene(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

void QB2Scene::setWorld(QB2World *world)
{
    if (m_world == world)
        return;

    if (m_world)
    {
        disconnect(m_world, nullptr, this, nullptr);
    }

    m_world = world;

    if (m_world)
    {
        connect(m_world, &QB2World::stepped, this, &QB2Scene::update);
    }

    emit worldChanged();
    update();
}

void QB2Scene::setCameraPosition(const QVector2D &position)
{
    if (m_cameraPosition == position)
        return;
    m_cameraPosition = position;
    emit cameraPositionChanged();
    update();
}

void QB2Scene::setCameraZoom(qreal zoom)
{
    if (qFuzzyCompare(m_cameraZoom, zoom))
        return;
    m_cameraZoom = zoom;
    emit cameraZoomChanged();
    update();
}

void QB2Scene::setCameraRotation(qreal rotation)
{
    if (qFuzzyCompare(m_cameraRotation, rotation))
        return;
    m_cameraRotation = rotation;
    emit cameraRotationChanged();
    update();
}

QSGNode *QB2Scene::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if (!m_world)
    {
        delete oldNode;
        return nullptr;
    }

    QSGTransformNode *rootNode = static_cast<QSGTransformNode *>(oldNode);
    if (!rootNode)
    {
        rootNode = new QSGTransformNode();
    }

    rootNode->removeAllChildNodes();

    QMatrix4x4 transform;
    transform.translate(width() / 2.0f, height() / 2.0f);
    transform.scale(m_cameraZoom);
    transform.rotate(m_cameraRotation, 0, 0, 1);
    transform.translate(-m_cameraPosition.x(), -m_cameraPosition.y());
    rootNode->setMatrix(transform);

    const auto &bodies = m_world->findChildren<QB2Body *>();
    for (QB2Body *body : bodies)
    {
        if (!b2Body_IsValid(body->bodyId()))
            continue;

        QVector2D pos = body->position();
        qreal angle = body->angle();

        const auto &fixtures = body->findChildren<QB2Fixture *>();
        for (QB2Fixture *fixture : fixtures)
        {
            if (!b2Shape_IsValid(fixture->shapeId()))
                continue;

            QSGTransformNode *bodyNode = new QSGTransformNode();
            QMatrix4x4 bodyTransform;
            bodyTransform.translate(pos.x(), pos.y());
            bodyTransform.rotate(qRadiansToDegrees(angle), 0, 0, 1);
            bodyNode->setMatrix(bodyTransform);

            auto shape = fixture->shape();
            if (auto boxShape = qobject_cast<QB2BoxShape *>(shape))
            {
                float w = boxShape->width();
                float h = boxShape->height();

                QSGGeometryNode *boxNode = new QSGGeometryNode();
                QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 5);
                geometry->setDrawingMode(QSGGeometry::DrawLineStrip);

                QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
                vertices[0].set(-w / 2, -h / 2);
                vertices[1].set(w / 2, -h / 2);
                vertices[2].set(w / 2, h / 2);
                vertices[3].set(-w / 2, h / 2);
                vertices[4].set(-w / 2, -h / 2);

                boxNode->setGeometry(geometry);
                boxNode->setFlag(QSGNode::OwnsGeometry);

                QSGFlatColorMaterial *material = new QSGFlatColorMaterial();
                material->setColor(QColor(0, 255, 0));
                boxNode->setMaterial(material);
                boxNode->setFlag(QSGNode::OwnsMaterial);

                bodyNode->appendChildNode(boxNode);
            }

            rootNode->appendChildNode(bodyNode);
        }
    }

    return rootNode;
}
