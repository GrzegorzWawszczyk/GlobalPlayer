#include "KeyObserver.h"

#include <QApplication>
#include <QDebug>
#include <QKeyEvent>

void KeyObserver::registerKeyEventCallback(Qt::Key key, Qt::KeyboardModifier modifiers, KeyObserver::Callback callback)
{
    listeners[qMakePair(key, modifiers)].append(callback);
}

void KeyObserver::interpretEvent(QKeyEvent* event)
{
    auto callbacks = listeners.find(qMakePair(event->key(), QApplication::keyboardModifiers()));
    if (callbacks != listeners.end())
    {
        for (auto callback : *callbacks)
        {
            callback();
        }
    }
}

KeyObserver& KeyObserver::instance()
{
    static KeyObserver self;
    return self;
}
