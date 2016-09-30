#include "playlistmodel.h"

#include <QBrush>
#include <QDebug>

PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int PlaylistModel::rowCount(const QModelIndex &/*parent*/) const
{
    return files.mediaCount();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || index.row() >= files.mediaCount())
            return QVariant();

    if (role == Qt::DisplayRole)
    {
//        return files.at(index.row()).fileName();
        return names.at(index.row());
    }
//    else if (role == Qt::BackgroundRole && index.row() != files.currentIndex())
//    {
//        return QBrush(Qt::black);
//    }
    else if (role == Qt::BackgroundRole && index.row() == files.currentIndex())
    {
        return QBrush(Qt::white);
    }
//    else if (role == Qt::ForegroundRole && index.row() != files.currentIndex())
//    {
//        return QBrush(Qt::white);
//    }
    else if (role == Qt::ForegroundRole && index.row() == files.currentIndex())
    {
        return QBrush(Qt::black);
    }

    return QVariant();
}

QMediaPlaylist *PlaylistModel::playlist()
{
    return &files;
}

void PlaylistModel::addToPlaylist(QFileInfoList filesToAdd)
{
    beginResetModel();

    for (QFileInfo file : filesToAdd)
    {
        files.addMedia(QMediaContent(QUrl::fromLocalFile(file.filePath())));
        names.append(file.fileName());
    }

    endResetModel();
}

QMediaContent PlaylistModel::getFile(const QModelIndex &index)
{
    return files.media(index.row());
}

void PlaylistModel::refresh()
{
    beginResetModel();
    endResetModel();
}

void PlaylistModel::setByIndex(const QModelIndex &index)
{
    files.setCurrentIndex(index.row());
}

void PlaylistModel::previous()
{
    if (files.currentIndex() == 0)
        files.setCurrentIndex(files.mediaCount() - 1);
    else
        files.previous();
}

void PlaylistModel::next()
{
    if (files.currentIndex() == files.mediaCount() - 1)
        files.setCurrentIndex(0);
    else
        files.next();
}



