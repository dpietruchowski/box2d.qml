#include "qb2chain.h"
#include "qb2body.h"

QB2Chain::QB2Chain(QObject *parent)
    : QObject(parent), m_chainId(b2_nullChainId)
{
}

QB2Chain::~QB2Chain()
{
    if (b2Chain_IsValid(m_chainId))
    {
        b2DestroyChain(m_chainId);
    }
}

QVector<QPointF> QB2Chain::points() const
{
    return m_points;
}

void QB2Chain::setPoints(const QVector<QPointF> &points)
{
    if (m_points == points)
        return;
    m_points = points;
    emit pointsChanged();

    if (m_componentComplete)
    {
        createChain();
    }
}

void QB2Chain::setIsLoop(bool loop)
{
    if (m_isLoop == loop)
        return;
    m_isLoop = loop;
    emit isLoopChanged();

    if (m_componentComplete)
    {
        createChain();
    }
}

void QB2Chain::setFriction(qreal friction)
{
    if (qFuzzyCompare(m_friction, friction))
        return;
    m_friction = friction;
    emit frictionChanged();

    if (m_componentComplete)
    {
        createChain();
    }
}

void QB2Chain::setRestitution(qreal restitution)
{
    if (qFuzzyCompare(m_restitution, restitution))
        return;
    m_restitution = restitution;
    emit restitutionChanged();

    if (m_componentComplete)
    {
        createChain();
    }
}

void QB2Chain::setBody(QB2Body *body)
{
    m_body = body;
}

void QB2Chain::classBegin()
{
}

void QB2Chain::componentComplete()
{
    m_componentComplete = true;

    if (!m_body)
    {
        QB2Body *body = nullptr;
        QObject *p = parent();
        while (p)
        {
            body = qobject_cast<QB2Body *>(p);
            if (body)
                break;
            p = p->parent();
        }
        m_body = body;
    }

    if (m_body && m_points.size() >= 4)
    {
        createChain();
    }
}

void QB2Chain::createChain()
{
    if (!m_componentComplete || !m_body || m_points.size() < 4)
        return;

    if (b2Chain_IsValid(m_chainId))
    {
        b2DestroyChain(m_chainId);
        m_chainId = b2_nullChainId;
    }

    if (!b2Body_IsValid(m_body->bodyId()))
        return;

    b2ChainDef chainDef = b2DefaultChainDef();

    QVector<b2Vec2> b2Points;
    b2Points.reserve(m_points.size());
    for (const QPointF &p : m_points)
    {
        b2Points.append({static_cast<float>(p.x()), static_cast<float>(p.y())});
    }

    chainDef.points = b2Points.data();
    chainDef.count = b2Points.size();
    chainDef.isLoop = m_isLoop;

    b2SurfaceMaterial material;
    material.friction = m_friction;
    material.restitution = m_restitution;
    chainDef.materials = &material;
    chainDef.materialCount = 1;

    m_chainId = b2CreateChain(m_body->bodyId(), &chainDef);
}
