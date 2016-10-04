#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QFileInfoList>
#include <QMediaPlaylist>



class PlaylistModel : public QAbstractListModel
{

    Q_OBJECT

public:
    PlaylistModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QMediaPlaylist *playlist();

    void addToPlaylist(const QStringList &files);

    QMediaContent getFile(const QModelIndex &index);

    void refresh();

    void setByIndex(const QModelIndex& index);

    void clearPlaylist();

    void moveUp(int index);
    void moveDown(int index);

public slots:

    void previous();
    void next();

signals:
    void moved(int index);
    void currentMoved();

private:
    QMediaPlaylist files;
    QStringList names;
};

#endif // PLAYLISTMODEL_H
