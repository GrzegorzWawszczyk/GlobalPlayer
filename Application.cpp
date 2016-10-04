#include "Application.h"
#include "KeyObserver.h"

#include <QDebug>

Application::Application(int &argc, char **argv, int flags)
    : QApplication(argc, argv, flags)
{

}

bool Application::notify(QObject *receiver, QEvent *event)
{
//    if (event->type() == QEvent::KeyPress && receiver->objectName() != "MainWindowWindow" && receiver->objectName() != "videoWidgetWindow")
//        qDebug() << receiver << event;
    if (event->type() == QEvent::KeyPress && receiver->inherits("QWidgetWindow"))
    {
        qDebug() << receiver << event << receiver->inherits("QWidgetWindow");
        KeyObserver::instance().interpretEvent(dynamic_cast<QKeyEvent*>(event));
    }
    QApplication::notify(receiver, event);
}
