#include <QGuiApplication>
#include <QQmlApplicationEngine>

int HelloQML(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    engine.load(QUrl(QStringLiteral("MyQML/MyWindow.qml")));

    return app.exec();
}