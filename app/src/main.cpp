#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <qb2qmlmodule.h>

#ifdef QML_LIVE_ENABLED
#include <filewatcher.h>
#endif

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

    QB2QmlModule::registerTypes();

    QQmlApplicationEngine engine;

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []()
                     { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

#ifdef QML_LIVE_ENABLED
    const QString qmlDir = QStringLiteral(QML_SOURCE_DIR);
    engine.load(QUrl::fromLocalFile(qmlDir + "/main.qml"));

    auto *watcher = new FileWatcher([&engine, qmlDir]()
    {
        auto roots = engine.rootObjects();
        if (roots.isEmpty())
            return;
        QObject *loader = roots.first()->findChild<QObject *>("mainLoader");
        if (!loader)
            return;
        engine.clearComponentCache();
        loader->setProperty("source", QUrl());
        loader->setProperty("source", QUrl::fromLocalFile(qmlDir + "/PinballDemo.qml"));
    }, &app);
    watcher->setDirectory(qmlDir);
    qInfo() << "QML live reload enabled, watching:" << qmlDir;
#else
    engine.load(QUrl(u"qrc:/App/qml/main.qml"_qs));
#endif

    int result = app.exec();

    qInfo() << "Application exiting";

    if (logFile)
    {
        logFile->close();
        delete logFile;
    }

    return result;
}
