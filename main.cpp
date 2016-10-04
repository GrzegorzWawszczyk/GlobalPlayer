#include "Application.h"
#include "MainWindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QString path = "C:/projects/Daft Punk - Derezzed.mp3";
    Application a(argc, argv);
    if (argc > 1)
        path = QString(argv[1]);

    MainWindow w(path);
    w.show();

    return a.exec();
}
