#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>



class Application : public QApplication
{
public:
    Application(int &argc, char **argv, int flags = ApplicationFlags);

    bool notify(QObject * receiver, QEvent * event) override;
};

#endif // APPLICATION_H
