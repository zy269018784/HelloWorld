#include <QApplication>
#include <QMainWindow>
int HelloQML(int argc, char* argv[]);
int HelloQWidget(int argc, char* argv[]);
int HelloQuickWidget(int argc, char* argv[]);
int main(int argc, char* argv[])
{
   // return HelloQuickWidget(argc, argv);
    return HelloQML(argc, argv);
    return HelloQWidget(argc, argv);
}