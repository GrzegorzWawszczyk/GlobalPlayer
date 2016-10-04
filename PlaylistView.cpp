#include "PlaylistView.h"
#include "PlaylistModel.h"

PlaylistView::PlaylistView(QWidget *parent)
    : QListView(parent)
{

}

void PlaylistView::setFocus(int index)
{
    QListView::setFocus();
    setCurrentIndex(currentIndex().sibling(index,0));
}

void PlaylistView::moveUp()
{
    if (currentIndex().isValid())
        dynamic_cast<PlaylistModel*>(model())->moveUp(currentIndex().row());

}

void PlaylistView::moveDown()
{
    if (currentIndex().isValid())
        dynamic_cast<PlaylistModel*>(model())->moveDown(currentIndex().row());

}
