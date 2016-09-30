#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString startFilePath = QString(), QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *event) override;
    void playOrPause();

    void skipBackward();
    void skipForward();

private slots:
    void showOrHidePlaylist();

private:
    Ui::MainWindow *ui;

    QMediaPlayer *player;

    void hidePlaylist();
    void showPlaylist();

//    void focusInEvent(QFocusEvent *event) override;

};

#endif // MAINWINDOW_H
