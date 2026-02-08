#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qb2world.h>
#include <qb2body.h>
#include <qb2fixture.h>
#include <qb2shape.h>
#include <qb2scene.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterUncreatableType<QB2Shape>("Box2D", 1, 0, "Shape", "Shape is abstract");
    qmlRegisterType<QB2World>("Box2D", 1, 0, "World");
    qmlRegisterType<QB2Body>("Box2D", 1, 0, "Body");
    qmlRegisterType<QB2Fixture>("Box2D", 1, 0, "Fixture");
    qmlRegisterType<QB2BoxShape>("Box2D", 1, 0, "BoxShape");
    qmlRegisterType<QB2Scene>("Box2D", 1, 0, "Scene");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/App/qml/main.qml"_qs);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []()
                     { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
