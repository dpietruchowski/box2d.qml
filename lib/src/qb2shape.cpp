#include "qb2shape.h"

QB2Shape::QB2Shape(QObject *parent)
    : QObject(parent)
{
}

QB2Shape::~QB2Shape()
{
}

QB2BoxShape::QB2BoxShape(QObject *parent)
    : QB2Shape(parent)
{
    updateShape();
}

QB2BoxShape::~QB2BoxShape()
{
}

qreal QB2BoxShape::width() const
{
    return m_width;
}

void QB2BoxShape::setWidth(qreal width)
{
    if (qFuzzyCompare(m_width, width))
        return;
    m_width = width;
    updateShape();
    emit widthChanged();
}

qreal QB2BoxShape::height() const
{
    return m_height;
}

void QB2BoxShape::setHeight(qreal height)
{
    if (qFuzzyCompare(m_height, height))
        return;
    m_height = height;
    updateShape();
    emit heightChanged();
}

b2Polygon *QB2BoxShape::polygon()
{
    return &m_polygon;
}

void QB2BoxShape::updateShape()
{
    m_polygon = b2MakeBox(m_width / 2.0f, m_height / 2.0f);
}
