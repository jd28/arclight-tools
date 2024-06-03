#ifndef ARCLIGHTTREEVIEW_H
#define ARCLIGHTTREEVIEW_H

#include <QTreeView>

class AbstractTreeModel;

class ArclightTreeView : public QTreeView {
    Q_OBJECT
public:
    ArclightTreeView(QWidget* parent = nullptr);

    virtual void activateModel() = 0;
    virtual AbstractTreeModel* loadModel() = 0;
};

#endif // ARCLIGHTTREEVIEW_H
