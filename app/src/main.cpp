#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <qb2world.h>
#include <qb2body.h>
#include <qb2fixture.h>
#include <shapes/qb2shape.h>
#include <shapes/qb2boxshape.h>
#include <shapes/qb2circleshape.h>
#include <shapes/qb2capsuleshape.h>
#include <shapes/qb2segmentshape.h>
#include <shapes/qb2polygonshape.h>
#include <shapes/qb2chainshape.h>
#include <qb2chain.h>
#include <joints/qb2joint.h>
#include <joints/qb2revolutejoint.h>
#include <joints/qb2distancejoint.h>
#include <joints/qb2prismaticjoint.h>
#include <joints/qb2weldjoint.h>
#include <joints/qb2wheeljoint.h>
#include <joints/qb2motorjoint.h>

static QFile *logFile = nullptr;
static QMutex logMutex;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&logMutex);

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString typeStr;

    switch (type)
    {
    case QtDebugMsg:
        typeStr = "DEBUG";
        break;
    case QtInfoMsg:
        typeStr = "INFO";
        break;
    case QtWarningMsg:
        typeStr = "WARNING";
        break;
    case QtCriticalMsg:
        typeStr = "CRITICAL";
        break;
    case QtFatalMsg:
        typeStr = "FATAL";
        break;
    }

    QString formattedMsg = QString("[%1] [%2] %3").arg(timestamp, typeStr, msg);

    // Output to console
    fprintf(stderr, "%s\n", formattedMsg.toLocal8Bit().constData());
    fflush(stderr);

    // Output to file
    if (logFile && logFile->isOpen())
    {
        QTextStream stream(logFile);
        stream << formattedMsg << "\n";
        stream.flush();
    }

    if (type == QtFatalMsg)
    {
        abort();
    }
}

int main(int argc, char *argv[])
{
    // Setup logging
    logFile = new QFile("box2d_app.log");
    if (logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        qInstallMessageHandler(messageHandler);
        qInfo() << "Application started";
    }

    QGuiApplication app(argc, argv);

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

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/App/qml/main.qml"_qs);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []()
                     { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    engine.load(url);

    int result = app.exec();

    qInfo() << "Application exiting";

    if (logFile)
    {
        logFile->close();
        delete logFile;
    }

    return result;
}
