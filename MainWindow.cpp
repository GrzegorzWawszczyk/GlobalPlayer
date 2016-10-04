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
    QStringList fil;
    fil.append("C:/projects/person-placeholder.jpg");
    fil.append("C:/projects/fighting_cats4.wmv");
    fil.append("C:/projects/swk.wmv");
    fil.append("C:/projects/Daft Punk - Derezzed.mp3");
    playlistModel->addToPlaylist(fil);
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
       hidePlaylist();
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

       player->play();
    });

    player = new QMediaPlayer();

    player->setNotifyInterval(1);

    connect(ui->slider_volume, &QSlider::valueChanged, player, &QMediaPlayer::setVolume);

    connect(ui->pb_mute, &QPushButton::clicked, this, [this]()
    {
       muteOrUnmute();
    });

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

    connect(ui->pb_playpause, &QPushButton::clicked, this, [this](){
        playOrPause();
    });

    KeyObserver::instance().registerKeyEventCallback(Qt::Key_Space, Qt::NoModifier, [this]
    {
        playOrPause();
    });
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_B, Qt::NoModifier, [this]
    {
        player->stop();
    });
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_F, Qt::ControlModifier, [this]
    {
        ui->videoWidget->changeDisplayMode();
    });
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_Escape, Qt::NoModifier, [this]
    {
        ui->videoWidget->exitFullScreen();
    });
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_Left, Qt::NoModifier, [this]
    {
        seekBackward();
    });
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_Right, Qt::NoModifier, [this]
    {
        seekForward();
    });
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_M, Qt::ControlModifier, [this]
    {
        muteOrUnmute();
    });
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_Left, Qt::ControlModifier, [this, playlistModel]
    {
        playlistModel->previous();
    });
    KeyObserver::instance().registerKeyEventCallback(Qt::Key_Right, Qt::ControlModifier, [this, playlistModel]
    {
        playlistModel->next();
    });


    connect(ui->pb_stop, &QPushButton::clicked, this, [this](){
        player->stop();
    });

    connect(ui->pb_previous, &QPushButton::clicked, playlistModel, &PlaylistModel::previous);
    connect(ui->pb_next, &QPushButton::clicked, playlistModel, &PlaylistModel::next);

    connect(ui->pb_seekBackward, &QPushButton::clicked, this, [this]()
    {
       seekBackward();
    });

    connect(ui->pb_seekForward, &QPushButton::clicked, this, [this]()
    {
       seekForward();
    });

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



    connect(player, &QMediaPlayer::videoAvailableChanged, this, [this](bool videoAvailable)
    {
       if (videoAvailable)
       {
           hidePlaylist();
           ui->l_musicPicture->hide();
           ui->videoWidget->show();
       }
       else if (player->isAudioAvailable())
       {
           showPlaylist();
           ui->l_musicPicture->show();
           ui->videoWidget->hide();
       }
       else
       {
           showPlaylist();
           ui->l_musicPicture->hide();
           ui->videoWidget->hide();
       }

    });

    connect(player, &QMediaPlayer::currentMediaChanged, this, [this](){
        ui->videoWidget->update();
   });

    connect(player, &QMediaPlayer::durationChanged, this, [this](){
       if (player->duration() <= 0)
       {
           ui->slider_time->hide();
           ui->l_time->hide();
       }
       else
       {
           ui->slider_time->setMaximum(player->duration());
           fullTime = msecsToQTime(player->duration());
           ui->slider_time->show();
           ui->l_time->show();
       }
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

    connect(ui->pb_add, &QPushButton::clicked, this, [playlistModel]()
    {
       QStringList fileNames = QFileDialog ::getOpenFileNames();

       playlistModel->addToPlaylist(fileNames);
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

void MainWindow::hidePlaylist()
{
    if (!blockHiding)
    {
        ui->lv_playlist->hide();
        ui->pb_add->hide();
        ui->pb_itemUp->hide();
        ui->pb_itemDown->hide();
        ui->pb_showHide->setText("<<");
    }
    else
        blockHiding = false;
}

void MainWindow::showPlaylist()
{
    ui->lv_playlist->show();
    ui->pb_add->show();
    ui->pb_itemUp->show();
    ui->pb_itemDown->show();
    ui->pb_showHide->setText(">>");
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
    if (ui->lv_playlist->isVisible())
        hidePlaylist();
    else
        showPlaylist();
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
