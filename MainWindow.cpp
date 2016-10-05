#include "KeyObserver.h"
#include "MainWindow.h"
#include "PlaylistModel.h"
#include "PlaylistView.h"
#include "ui_mainwindow.h"

#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QDebug>
#include <QShortcut>
#include <QKeyEvent>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>
#include <functional>


MainWindow::MainWindow(const QString startFilePath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    player(new QMediaPlayer)
{

    ui->setupUi(this);

    QPixmap p("nuta.jpg");
    ui->l_musicPicture->setScaledContents(true);
    ui->l_musicPicture->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->l_musicPicture->setPixmap(p);
    ui->l_musicPicture->setAlignment(Qt::AlignCenter);
//                        .scaled(ui->centralWidget->geometry().width(), ui->centralWidget->geometry().height(), Qt::KeepAspectRatio));

    PlaylistModel *playlistModel = new PlaylistModel(ui->lv_playlist);


//    for testing purposes
//    QStringList fil;
//    fil.append("C:/projects/person-placeholder.jpg");
//    fil.append("C:/projects/fighting_cats4.wmv");
//    fil.append("C:/projects/swk.wmv");
//    fil.append("C:/projects/Daft Punk - Derezzed.mp3");
//    playlistModel->addToPlaylist(fil);
//    **************************************************************

    ui->lv_playlist->setModel(playlistModel);
    ui->lv_playlist->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->pb_playpause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pb_stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pb_previous->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->pb_next->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->pb_seekBackward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pb_seekForward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->pb_itemUp->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    ui->pb_itemDown->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
    ui->pb_mute->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaVolume));

    playlistSetVisible(false);
    ui->videoWidget->hide();
    ui->l_musicPicture->hide();
    ui->pb_showHide->hide();


    connect(ui->videoWidget, &VideoPlayer::fullScreenChanged, this, [this](bool fullScreen)
    {
        setVisible(!fullScreen);
    });

    connect(ui->a_open, &QAction::triggered, this, [this]()
    {
       QString fileName = QFileDialog::getOpenFileName();
       if (fileName.isEmpty())
           return;

       player->setMedia(QMediaContent(QUrl::fromLocalFile(fileName)));
       playlistSetVisible(false);
       ui->pb_showHide->hide();
       player->play();
    });

    connect(ui->a_openFiles, &QAction::triggered, this, [this, playlistModel]()
    {
       QStringList fileNames = QFileDialog::getOpenFileNames();

       if (fileNames.isEmpty())
           return;

       playlistModel->clearPlaylist();
       playlistModel->addToPlaylist(fileNames);
       player->setPlaylist(playlistModel->playlist());
       ui->pb_showHide->show();

       player->play();
    });

    player->setNotifyInterval(1);

    connect(ui->slider_volume, &QSlider::valueChanged, player, &QMediaPlayer::setVolume);

    connect(ui->pb_mute, &QPushButton::clicked, this, &MainWindow::muteOrUnmute);

    connect(ui->a_clear, &QAction::triggered, this, [this, playlistModel](){
       int position = player->position();
       player->setMedia(playlistModel->playlist()->currentMedia());
       player->setPosition(position);
       player->play();
       playlistModel->clearPlaylist();
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

    connect(ui->pb_playpause, &QPushButton::clicked, this, &MainWindow::playOrPause);

    KeyObserver::instance().registerKeyEventCallback(Qt::Key_Space, Qt::NoModifier, std::bind(&MainWindow::playOrPause, this));
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_B, Qt::NoModifier, std::bind(&QMediaPlayer::stop, player));
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_F, Qt::ControlModifier, std::bind(&VideoPlayer::switchFullScreen, ui->videoWidget));
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_Escape, Qt::NoModifier, std::bind(&VideoPlayer::exitFullScreen, ui->videoWidget));
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_Left, Qt::NoModifier, std::bind(&MainWindow::seekBackward, this));
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_Right, Qt::NoModifier, std::bind(&MainWindow::seekForward, this));
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_M, Qt::ControlModifier, std::bind(&MainWindow::muteOrUnmute, this));
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_Left, Qt::ControlModifier, std::bind(&PlaylistModel::previous, playlistModel));
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_Right, Qt::ControlModifier, std::bind(&PlaylistModel::next, playlistModel));

    connect(ui->pb_stop, &QPushButton::clicked, player, &QMediaPlayer::stop);

    connect(ui->pb_previous, &QPushButton::clicked, playlistModel, &PlaylistModel::previous);
    connect(ui->pb_next, &QPushButton::clicked, playlistModel, &PlaylistModel::next);

    connect(ui->pb_seekBackward, &QPushButton::clicked, this, &MainWindow::seekBackward);

    connect(ui->pb_seekForward, &QPushButton::clicked, this, &MainWindow::seekForward);

    connect(ui->pb_itemUp, &QPushButton::clicked, this, &MainWindow::moveUp);
    connect(ui->pb_itemDown, &QPushButton::clicked, this, &MainWindow::moveDown);

    connect(ui->a_moveUp, &QAction::triggered, this, &MainWindow::moveUp);
    connect(ui->a_moveDown, &QAction::triggered, this, &MainWindow::moveDown);

    connect(playlistModel, &PlaylistModel::moved, this, [this](){
       player->setPosition(currentPosition);
       player->play();
    });

    // didn't work in normal way
    connect(playlistModel, SIGNAL(moved(int)), ui->lv_playlist, SLOT(setFocus(int)));

    connect(playlistModel, &PlaylistModel::moved, this, [this](){
        blockHiding = true;
    });



    connect(player, &QMediaPlayer::mediaChanged, this, [this]()
    {
//        playlistSetVisible(!(player->playlist() == nullptr || player->isVideoAvailable()));

//        ui->videoWidget->setVisible(player->isVideoAvailable());
//        qDebug() << !player->isVideoAvailable();
//        qDebug() << player->isAudioAvailable();
//        qDebug() << ((!player->isVideoAvailable()) && player->isAudioAvailable());
//        ui->l_musicPicture->setVisible(((!player->isVideoAvailable()) && player->isAudioAvailable()));
//        if (player->playlist() == nullptr)
//        {
//            playlistSetVisible(false);
//        }
//        else if (videoAvailable)
//        {
//            playlistSetVisible(false);
//            ui->l_musicPicture->hide();
//            ui->videoWidget->show();
//        }
//        else if (player->isAudioAvailable())
//        {
//            playlistSetVisible(true);
//            ui->l_musicPicture->show();
//            ui->videoWidget->hide();
//        }
//        else
//        {
//            playlistSetVisible(true);
//            ui->l_musicPicture->hide();
//            //           ui->videoWidget->hide();
//        }

    });

//    connect(player, &QMediaPlayer::currentMediaChanged, this, [this](){
//        ui->videoWidget->update();
//   });

    connect(player, &QMediaPlayer::durationChanged, this, [this](){
       if (player->duration() <= 0)
       {
           ui->slider_time->hide();
           ui->l_time->hide();
           ui->videoWidget->setVisible(true);
       }
       else
       {
           ui->slider_time->setMaximum(player->duration());
           fullTime = msecsToQTime(player->duration());
           ui->slider_time->show();
           ui->l_time->show();
           ui->videoWidget->setVisible(player->isVideoAvailable());
       }
       ui->l_musicPicture->setVisible(((!player->isVideoAvailable()) && player->isAudioAvailable()));
       playlistSetVisible(!(player->playlist() == nullptr || player->isVideoAvailable()));
    });


    connect(player, &QMediaPlayer::positionChanged, ui->slider_time, &QSlider::setValue);

    connect(ui->slider_time, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);

    connect(player, &QMediaPlayer::positionChanged, this, [this]()
    {
        QTime actualTime = msecsToQTime(player->position());

        if (actualTime.hour() <= 0)
            ui->l_time->setText(actualTime.toString("mm:ss"));
        else
            ui->l_time->setText(actualTime.toString("hh:mm:ss"));

        if (fullTime.hour() <= 0)
            ui->l_time->setText(ui->l_time->text() + "/" + fullTime.toString("mm:ss"));
        else
            ui->l_time->setText(ui->l_time->text() + "/" + fullTime.toString("hh:mm:ss"));
    });


    connect(ui->pb_showHide, &QPushButton::clicked, this, &MainWindow::showOrHidePlaylist);

    connect(ui->pb_add, &QPushButton::clicked, this, [this, playlistModel]()
    {
       QStringList fileNames = QFileDialog ::getOpenFileNames();

       playlistModel->addToPlaylist(fileNames);
    });

    connect(ui->lv_playlist, &QListView::activated, this, [this, playlistModel](const QModelIndex &index)
    {
        playlistModel->setByIndex(index);
        player->play();
    });
    qDebug() << playlistModel->mediaCount();
    if (!startFilePath.isEmpty())
        player->setMedia(QMediaContent(QUrl::fromLocalFile(startFilePath)));
    else if (playlistModel->mediaCount() > 0)
        player->setPlaylist(playlistModel->playlist());

    player->setVideoOutput(ui->videoWidget);

    player->play();
}

MainWindow::~MainWindow()
{
    delete player;
    delete ui;
}

void MainWindow::playOrPause()
{
    if(player->state() == QMediaPlayer::PlayingState)
    {
        player->pause();
    }
    else if(player->state() == QMediaPlayer::PausedState || player->state() == QMediaPlayer::StoppedState)
    {
        player->play();
    }
}

void MainWindow::seekBackward()
{
    player->setPosition(player->position() - 3000);
}

void MainWindow::seekForward()
{
    player->setPosition(player->position() + 3000);
}

void MainWindow::muteOrUnmute()
{
    player->setMuted(!player->isMuted());
}

void MainWindow::playlistSetVisible(bool visible)
{
    if (!blockHiding)
    {
        ui->lv_playlist->setVisible(visible);
        ui->pb_add->setVisible(visible);
        ui->pb_itemUp->setVisible(visible);
        ui->pb_itemDown->setVisible(visible);
        if (visible)
            ui->pb_showHide->setText(">>");
        else
            ui->pb_showHide->setText("<<");
    }
    else
        blockHiding = false;
}

QTime MainWindow::msecsToQTime(int msecs)
{
    int secs = msecs/1000;
    msecs -= secs*1000;
    int mins = secs/60;
    secs -= mins*60;
    int hours = mins/60;

    return QTime(hours, mins, secs, msecs);
}

void MainWindow::showOrHidePlaylist()
{
    playlistSetVisible(!ui->lv_playlist->isVisible());
}

void MainWindow::moveUp()
{
    currentPosition = player->position();
    ui->lv_playlist->moveUp();
}

void MainWindow::moveDown()
{
    currentPosition = player->position();
    ui->lv_playlist->moveDown();
}
