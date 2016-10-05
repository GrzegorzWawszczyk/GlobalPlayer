#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString startFilePath = QString(), QWidget *parent = 0);
    ~MainWindow();


public slots:
    void playOrPause();
    void seekBackward();
    void seekForward();
    void muteOrUnmute();

private slots:
    void showOrHidePlaylist();

    void moveUp();
    void moveDown();

    void playlistSetVisible(bool visible);

private:
    Ui::MainWindow *ui;

    QMediaPlayer *player;

    QTime fullTime;

    qint64 currentPosition;

    QTime msecsToQTime(int msecs);

    bool blockHiding = false;

};

#endif // MAINWINDOW_H
