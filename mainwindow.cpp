#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QDebug>
#include <QShortcut>
#include <QKeyEvent>
#include <QtConcurrent/QtConcurrent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->pb_playpause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pb_stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pb_skipBackward->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->pb_skipForward->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->pb_fastBackward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pb_fast_Forward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));


    connect(ui->widget, &VideoPlayer::fullScreenChanged, this, [this](bool fullScreen)
    {
        if (fullScreen)
            hide();
        else
            show();
    });

    player = new QMediaPlayer();

    connect(player, &QMediaPlayer::stateChanged, this, [this](QMediaPlayer::State state){
        if (state == QMediaPlayer::State::PlayingState)
            ui->pb_playpause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        else
            ui->pb_playpause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    });

    connect(ui->pb_playpause, &QPushButton::clicked, this, [this](){
        playOrPause();
    });

    connect(ui->pb_stop, &QPushButton::clicked, this, [this](){
        player->stop();
    });


    player->setMedia(QMediaContent(QUrl::fromLocalFile("C:/projects/swk.wmv")));

//    auto playlist = new QMediaPlaylist(player);
//    playlist->addMedia(QMediaContent(QUrl::fromLocalFile("C:/projects/fighting_cats4.wmv")));
//    playlist->addMedia(QMediaContent(QUrl::fromLocalFile("C:/projects/fighting_cats4.wmv")));
//    playlist->setCurrentIndex(0);

//    player->setPlaylist(playlist);
//    playlist->addMedia(QMediaContent(QUrl::fromLocalFile("C:/projects/fighting_cats4.wmv")));

    //auto videoWidget = new QVideoWidget;
    player->setVideoOutput(ui->widget);

//    ui->widget->show();
    player->play();
}

MainWindow::~MainWindow()
{
    delete player;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
//    qDebug() <<"keypressevent";
//    qDebug() << event->modifiers() << " " << event->key();
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_F)
    {
        ui->widget->changeDisplayMode();
    }
    else if (event->key() == Qt::Key_Space)
    {
        playOrPause();
    }
    else
        QMainWindow::keyPressEvent(event);
}

void MainWindow::playOrPause()
{
    if(player->state() == QMediaPlayer::PlayingState)
        player->pause();
    else if(player->state() == QMediaPlayer::PausedState || player->state() == QMediaPlayer::StoppedState)
        player->play();
}

//void MainWindow::focusInEvent(QFocusEvent *event)
//{

//}
