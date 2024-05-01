#include "dialogtreeview.h"

#include <QDragMoveEvent>
#include <QHeaderView>

DialogTreeView::DialogTreeView(QWidget* parent)
    : QTreeView(parent)
{
}

void DialogTreeView::dragMoveEvent(QDragMoveEvent* event)
{
    QTreeView::dragMoveEvent(event);
}
