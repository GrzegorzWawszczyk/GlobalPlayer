#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QVideoWidget>



class VideoPlayer : public QVideoWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget* parent = 0);

    void changeDisplayMode();

    void playOrPause();

private:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;


};

#endif // VIDEOPLAYER_H
