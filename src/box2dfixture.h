#ifndef BOX2DFIXTURE_H
#define BOX2DFIXTURE_H

#include <QQuickItem>
#include <box2d/box2d.h>

class Box2DBody;

class Box2DFixture : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal density READ density WRITE setDensity NOTIFY densityChanged)
    Q_PROPERTY(qreal friction READ friction WRITE setFriction NOTIFY frictionChanged)
    Q_PROPERTY(qreal restitution READ restitution WRITE setRestitution NOTIFY restitutionChanged)
    Q_PROPERTY(bool sensor READ sensor WRITE setSensor NOTIFY sensorChanged)
    Q_PROPERTY(int categories READ categories WRITE setCategories NOTIFY categoriesChanged)
    Q_PROPERTY(int collidesWith READ collidesWith WRITE setCollidesWith NOTIFY collidesWithChanged)
    Q_PROPERTY(int groupIndex READ groupIndex WRITE setGroupIndex NOTIFY groupIndexChanged)

public:
    explicit Box2DFixture(QQuickItem *parent = nullptr);
    ~Box2DFixture() override;

    qreal density() const;
    void setDensity(qreal density);

    qreal friction() const;
    void setFriction(qreal friction);

    qreal restitution() const;
    void setRestitution(qreal restitution);

    bool sensor() const;
    void setSensor(bool sensor);

    int categories() const;
    void setCategories(int categories);

    int collidesWith() const;
    void setCollidesWith(int collidesWith);

    int groupIndex() const;
    void setGroupIndex(int groupIndex);

    virtual void createFixture(b2Body *body);
    b2Fixture *fixture() { return m_fixture; }

signals:
    void densityChanged();
    void frictionChanged();
    void restitutionChanged();
    void sensorChanged();
    void categoriesChanged();
    void collidesWithChanged();
    void groupIndexChanged();

protected:
    void componentComplete() override;
    virtual b2Shape *createShape() = 0;
    qreal toMeters(qreal value) const;
    Box2DBody *body() const;

    b2Fixture *m_fixture;
    qreal m_density;
    qreal m_friction;
    qreal m_restitution;
    bool m_sensor;
    int m_categories;
    int m_collidesWith;
    int m_groupIndex;
};

#endif // BOX2DFIXTURE_H
