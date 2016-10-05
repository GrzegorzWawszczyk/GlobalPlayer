#include "Application.h"
#include "MainWindow.h"
#include <QApplication>
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[])
{
    QString path = "";
    Application a(argc, argv);
    if (argc > 1)
        path = QString(argv[1]);

    std::cout << path.toStdString() << std::endl;

    MainWindow w(path);
    w.show();

    return a.exec();
}
