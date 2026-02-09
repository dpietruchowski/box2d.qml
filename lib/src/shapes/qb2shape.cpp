#include "qb2shape.h"

QB2Shape::QB2Shape(QObject *parent)
    : QObject(parent)
{
}

QB2Shape::~QB2Shape()
{
}

void QB2Shape::setFillColor(const QColor &color)
{
    if (m_fillColor == color)
        return;
    m_fillColor = color;
    emit fillColorChanged();
}

void QB2Shape::setStrokeColor(const QColor &color)
{
    if (m_strokeColor == color)
        return;
    m_strokeColor = color;
    emit strokeColorChanged();
}

void QB2Shape::setStrokeWidth(qreal width)
{
    if (qFuzzyCompare(m_strokeWidth, width))
        return;
    m_strokeWidth = width;
    emit strokeWidthChanged();
}

void QB2Shape::setRenderingEnabled(bool enabled)
{
    if (m_renderingEnabled == enabled)
        return;
    m_renderingEnabled = enabled;
    emit renderingEnabledChanged();
}
