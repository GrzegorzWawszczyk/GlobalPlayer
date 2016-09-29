#include "mainwindow.h"
#include "videoplayer.h"

#include <QDebug>
#include <QKeyEvent>

VideoPlayer::VideoPlayer(QWidget *parent) : QVideoWidget(parent)
{

}

void VideoPlayer::changeDisplayMode()
{
    if (isFullScreen())
        setFullScreen(false);
    else
        setFullScreen(true);
}

void VideoPlayer::keyPressEvent(QKeyEvent *event)
{
    dynamic_cast<MainWindow*>( this->parent()->parent())->keyPressEvent(event);
}

void VideoPlayer::closeEvent(QCloseEvent *event)
{
    event->ignore();
    dynamic_cast<MainWindow*>( this->parent()->parent())->close();
}

void VideoPlayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    changeDisplayMode();
}
