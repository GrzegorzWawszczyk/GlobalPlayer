#include "MainWindow.h"
#include "VideoPlayer.h"

#include <QDebug>
#include <QKeyEvent>

VideoPlayer::VideoPlayer(QWidget *parent) : QVideoWidget(parent)
{
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::black);
    setPalette(p);
}

void VideoPlayer::switchFullScreen()
{
    if (isFullScreen())
        setFullScreen(false);
    else
        setFullScreen(true);
}

void VideoPlayer::exitFullScreen()
{
    setFullScreen(false);
}

void VideoPlayer::closeEvent(QCloseEvent *event)
{
    event->ignore();
    dynamic_cast<MainWindow*>( this->parent()->parent())->close();
}

void VideoPlayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    switchFullScreen();
}
