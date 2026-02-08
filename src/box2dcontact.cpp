#include "box2dcontact.h"
#include "box2dfixture.h"

Box2DContact::Box2DContact(b2Contact *contact, qreal pixelsPerMeter, QObject *parent)
    : QObject(parent)
    , m_contact(contact)
    , m_pixelsPerMeter(pixelsPerMeter)
{
}

Box2DFixture *Box2DContact::fixtureA() const
{
    if (!m_contact)
        return nullptr;
    
    b2Fixture *fixture = m_contact->GetFixtureA();
    if (!fixture)
        return nullptr;
    
    return reinterpret_cast<Box2DFixture*>(fixture->GetUserData().pointer);
}

Box2DFixture *Box2DContact::fixtureB() const
{
    if (!m_contact)
        return nullptr;
    
    b2Fixture *fixture = m_contact->GetFixtureB();
    if (!fixture)
        return nullptr;
    
    return reinterpret_cast<Box2DFixture*>(fixture->GetUserData().pointer);
}

bool Box2DContact::touching() const
{
    return m_contact && m_contact->IsTouching();
}

bool Box2DContact::enabled() const
{
    return m_contact && m_contact->IsEnabled();
}

void Box2DContact::setEnabled(bool enabled)
{
    if (m_contact)
        m_contact->SetEnabled(enabled);
}
