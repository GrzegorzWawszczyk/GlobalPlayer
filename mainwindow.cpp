#include "mainwindow.h"
#include "playlistmodel.h"
#include "ui_mainwindow.h"

#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QDebug>
#include <QShortcut>
#include <QKeyEvent>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>


MainWindow::MainWindow(const QString startFilePath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    QPixmap p("C:/projects/nuta.jpg");
    ui->l_musicPicture->setScaledContents(true);
    ui->l_musicPicture->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->l_musicPicture->setPixmap(p);
    ui->l_musicPicture->setAlignment(Qt::AlignCenter);
//                        .scaled(ui->centralWidget->geometry().width(), ui->centralWidget->geometry().height(), Qt::KeepAspectRatio));

    PlaylistModel *playlistModel = new PlaylistModel(ui->lv_playlist);


//    for testing purposes
    QFileInfoList fil;
    fil.append(QFileInfo("C:/projects/swk.wmv"));
    fil.append(QFileInfo("C:/projects/fighting_cats4.wmv"));
    fil.append(QFileInfo("C:/projects/Daft Punk - Derezzed.mp3"));
    playlistModel->addToPlaylist(fil);
//    **************************************************************

    ui->lv_playlist->setModel(playlistModel);
    ui->lv_playlist->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui->lv_playlist->setBackgroundRole()

    ui->pb_playpause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pb_stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pb_previous->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->pb_next->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->pb_fastBackward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pb_fast_Forward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->pb_itemUp->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    ui->pb_itemDown->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
    ui->pb_mute->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaVolume));


    connect(ui->videoWidget, &VideoPlayer::fullScreenChanged, this, [this](bool fullScreen)
    {
        if (fullScreen)
            hide();
        else
            show();
    });

    connect(ui->a_open, &QAction::triggered, this, [this]()
    {
       QString fileName = QFileDialog::getOpenFileName();
       player->setMedia(QMediaContent(QUrl::fromLocalFile(fileName)));
       hidePlaylist();
       ui->pb_showHide->hide();
       player->play();
    });

    player = new QMediaPlayer();

    connect(ui->slider_volume, &QSlider::valueChanged, player, &QMediaPlayer::setVolume);

    connect(ui->pb_mute, &QPushButton::clicked, this, [this]()
    {
       player->setMuted(!player->isMuted());
    });

    connect(player, &QMediaPlayer::mutedChanged, this, [this](bool muted)
    {
       if (muted)
           ui->pb_mute->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaVolumeMuted));
       else
           ui->pb_mute->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaVolume));
    });

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

    connect(ui->pb_previous, &QPushButton::clicked, playlistModel, &PlaylistModel::previous);
    connect(ui->pb_next, &QPushButton::clicked, playlistModel, &PlaylistModel::next);

    connect(player, &QMediaPlayer::videoAvailableChanged, this, [this](bool videoAvailable)
    {
       if (videoAvailable)
       {
           hidePlaylist();
           ui->l_musicPicture->hide();
           ui->videoWidget->show();
       }
       else
       {
           showPlaylist();
           ui->l_musicPicture->show();
           ui->videoWidget->hide();
       }

    });

    connect(ui->pb_showHide, &QPushButton::clicked, this, &MainWindow::showOrHidePlaylist);

    connect(ui->pb_add, &QPushButton::clicked, this, [playlistModel]()
    {
       QStringList fileNames = QFileDialog ::getOpenFileNames();
       QFileInfoList files;
       for (QString fileName : fileNames)
           files.append(QFileInfo(fileName));

       playlistModel->addToPlaylist(files);
    });

    connect(ui->lv_playlist, &QListView::activated, this, [this, playlistModel](const QModelIndex &index)
    {
        playlistModel->setByIndex(index);
        player->play();
    });

    if (!startFilePath.isEmpty())
        player->setMedia(QMediaContent(QUrl::fromLocalFile(startFilePath)));

    player->setVideoOutput(ui->videoWidget);

    player->setPlaylist(playlistModel->playlist());
    player->play();
}

MainWindow::~MainWindow()
{
    delete player;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_F)
    {
        ui->videoWidget->changeDisplayMode();
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

void MainWindow::hidePlaylist()
{
    ui->lv_playlist->hide();
    ui->pb_add->hide();
    ui->pb_itemUp->hide();
    ui->pb_itemDown->hide();
    ui->pb_showHide->setText("<<");
}

void MainWindow::showPlaylist()
{
    ui->lv_playlist->show();
    ui->pb_add->show();
    ui->pb_itemUp->show();
    ui->pb_itemDown->show();
    ui->pb_showHide->setText(">>");
}

void MainWindow::showOrHidePlaylist()
{
    if (ui->lv_playlist->isVisible())
        hidePlaylist();
    else
        showPlaylist();
}
