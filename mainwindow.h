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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *event) override;
    void playOrPause();

    void skipBackward();
    void skipForward();


private:
    Ui::MainWindow *ui;

    QMediaPlayer *player;

//    void focusInEvent(QFocusEvent *event) override;

};

#endif // MAINWINDOW_H
