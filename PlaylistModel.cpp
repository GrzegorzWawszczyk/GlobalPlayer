#include "PlaylistModel.h"

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
        return names.at(index.row());
    }
    else if (role == Qt::BackgroundRole && index.row() == files.currentIndex())
    {
        return QBrush(Qt::white);
    }
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

void PlaylistModel::addToPlaylist(const QStringList& filesToAdd)
{
    beginResetModel();

    for (QString file : filesToAdd)
    {
        files.addMedia(QMediaContent(QUrl::fromLocalFile(file)));
        names.append(QFileInfo(file).fileName());
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

void PlaylistModel::clearPlaylist()
{
    beginResetModel();
    files.clear();
    names.clear();
    endResetModel();
}

void PlaylistModel::moveUp(int index)
{
    int previousIndex = files.currentIndex();
    if (index == 0)
    {
        emit moved(index);
        return;
    }
    beginResetModel();
    files.moveMedia(index, index - 1);
    names.move(index, index - 1);
    endResetModel();
    if (index == previousIndex)
    {
        files.setCurrentIndex(index - 1);
    }
    else if ((index - 1) == previousIndex)
    {
        files.setCurrentIndex(index);
    }
    else
    {
        files.setCurrentIndex(previousIndex);
    }

    emit moved(index - 1);
}

void PlaylistModel::moveDown(int index)
{
    int previousIndex = files.currentIndex();
    if (index == files.mediaCount() - 1)
    {
        emit moved(index);
        return;
    }
    beginResetModel();
    files.moveMedia(index, index + 1);
    names.move(index, index + 1);
    endResetModel();
    if (index == previousIndex)
    {
        files.setCurrentIndex(index + 1);
    }
    else if ((index + 1) == previousIndex)
    {
        files.setCurrentIndex(index);
    }
    else
    {
        files.setCurrentIndex(previousIndex);
    }

    emit moved(index + 1);
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



