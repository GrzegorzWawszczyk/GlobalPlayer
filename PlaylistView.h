#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include <QListView>

class PlaylistView : public QListView
{

    Q_OBJECT

public:
    PlaylistView(QWidget* parent = 0);



public slots:
    void moveUp();
    void moveDown();
    void setFocus(int index);
};

#endif // PLAYLISTVIEW_H
