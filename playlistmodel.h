#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QFileInfoList>
#include <QMediaPlaylist>



class PlaylistModel : public QAbstractListModel
{
public:
    PlaylistModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QMediaPlaylist *playlist();

    void addToPlaylist(QFileInfoList files);

    QMediaContent getFile(const QModelIndex &index);

    void refresh();

    void setByIndex(const QModelIndex& index);

public slots:

    void previous();
    void next();

private:
    QMediaPlaylist files;
    QStringList names;
};

#endif // PLAYLISTMODEL_H
