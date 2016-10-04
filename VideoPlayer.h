#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QVideoWidget>



class VideoPlayer : public QVideoWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget* parent = 0);

    void changeDisplayMode();
    void exitFullScreen();

    void playOrPause();

private:
    void closeEvent(QCloseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;


};

#endif // VIDEOPLAYER_H
