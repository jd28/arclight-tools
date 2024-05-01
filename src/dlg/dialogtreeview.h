#ifndef DIALOGTREEVIEW_H
#define DIALOGTREEVIEW_H

#include <QTreeView>

class DialogTreeView : public QTreeView {
    Q_OBJECT
public:
    DialogTreeView(QWidget* parent = nullptr);

    void dragMoveEvent(QDragMoveEvent* event);
};

#endif // DIALOGTREEVIEW_H
