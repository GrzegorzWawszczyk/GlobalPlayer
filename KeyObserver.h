#ifndef KEYOBSERVER_H
#define KEYOBSERVER_H

#include <QEvent>
#include <QKeyEvent>
#include <QVector>

#include <functional>

class KeyObserver
{
public:
    using Callback = std::function<void()>;

    void registerKeyEventCallback(Qt::Key key, Qt::KeyboardModifier modifiers, Callback callback);
    void interpretEvent(QKeyEvent* event);

    static KeyObserver& instance();

private:
    KeyObserver() = default;

    using Key = QPair<int, int>;
    QMap<Key, QVector<Callback>> listeners;
};

#endif // KEYOBSERVER_H
