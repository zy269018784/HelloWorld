#include <QApplication>
#include <QMainWindow>

int HelloQWidget(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow w;
    w.show();

    return app.exec();
}