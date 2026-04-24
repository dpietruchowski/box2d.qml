#include "qb2qmlmodule.h"

#include <QtQml>

#include "qb2world.h"
#include "qb2body.h"
#include "qb2fixture.h"
#include "qb2chain.h"
#include "shapes/qb2shape.h"
#include "shapes/qb2boxshape.h"
#include "shapes/qb2circleshape.h"
#include "shapes/qb2capsuleshape.h"
#include "shapes/qb2segmentshape.h"
#include "shapes/qb2polygonshape.h"
#include "shapes/qb2chainshape.h"
#include "joints/qb2joint.h"
#include "joints/qb2revolutejoint.h"
#include "joints/qb2distancejoint.h"
#include "joints/qb2prismaticjoint.h"
#include "joints/qb2weldjoint.h"
#include "joints/qb2wheeljoint.h"
#include "joints/qb2motorjoint.h"
#include "joints/qb2filterjoint.h"

void QB2QmlModule::registerTypes()
{
    qmlRegisterUncreatableType<QB2Shape>("Box2D", 1, 0, "Shape", "Shape is abstract");
    qmlRegisterUncreatableType<QB2Joint>("Box2D", 1, 0, "Joint", "Joint is abstract");
    qmlRegisterType<QB2World>("Box2D", 1, 0, "World");
    qmlRegisterType<QB2Body>("Box2D", 1, 0, "Body");
    qmlRegisterType<QB2Fixture>("Box2D", 1, 0, "Fixture");
    qmlRegisterType<QB2BoxShape>("Box2D", 1, 0, "BoxShape");
    qmlRegisterType<QB2CircleShape>("Box2D", 1, 0, "CircleShape");
    qmlRegisterType<QB2CapsuleShape>("Box2D", 1, 0, "CapsuleShape");
    qmlRegisterType<QB2SegmentShape>("Box2D", 1, 0, "SegmentShape");
    qmlRegisterType<QB2PolygonShape>("Box2D", 1, 0, "PolygonShape");
    qmlRegisterType<QB2Chain>("Box2D", 1, 0, "Chain");
    qmlRegisterType<QB2RevoluteJoint>("Box2D", 1, 0, "RevoluteJoint");
    qmlRegisterType<QB2DistanceJoint>("Box2D", 1, 0, "DistanceJoint");
    qmlRegisterType<QB2PrismaticJoint>("Box2D", 1, 0, "PrismaticJoint");
    qmlRegisterType<QB2WeldJoint>("Box2D", 1, 0, "WeldJoint");
    qmlRegisterType<QB2WheelJoint>("Box2D", 1, 0, "WheelJoint");
    qmlRegisterType<QB2MotorJoint>("Box2D", 1, 0, "MotorJoint");
    qmlRegisterType<QB2FilterJoint>("Box2D", 1, 0, "FilterJoint");
}
