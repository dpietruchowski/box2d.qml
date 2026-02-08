#pragma once

#include <QQuickPaintedItem>
#include <QColor>

class QB2Body;

class QB2ShapeRenderer : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool showBoundingBox READ showBoundingBox WRITE setShowBoundingBox NOTIFY showBoundingBoxChanged)

public:
    explicit QB2ShapeRenderer(QQuickItem *parent = nullptr);

    bool showBoundingBox() const { return m_showBoundingBox; }
    void setShowBoundingBox(bool show);

    void paint(QPainter *painter) override;

signals:
    void showBoundingBoxChanged();

private:
    bool m_showBoundingBox{false};
};
