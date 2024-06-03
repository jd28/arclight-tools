#ifndef AREALISTVIEW_H
#define AREALISTVIEW_H

#include "AbstractTreeModel.hpp"
#include "arclighttreeview.h"

#include "nw/resources/Resource.hpp"

#include <QTreeView>
#include <QWidget>

class FuzzyProxyModel;

namespace nw {
struct Area;
struct Module;
}

// == AreaListItem ============================================================
// ============================================================================

enum struct AreaListItemType {
    area,
    // Instances will go here
};

class AreaListItem : public AbstractTreeItem {
public:
    AreaListItem(nw::Area* area, AreaListItem* parent = nullptr);

    virtual QVariant data(int column, int role = Qt::DisplayRole) const override;

    nw::Area* area_ = nullptr;
    QString path_;
    nw::Resource res_;
    AreaListItemType type_;
};

// == AreaListModel ===========================================================
// ============================================================================

class AreaListModel : public AbstractTreeModel {
    Q_OBJECT

public:
    explicit AreaListModel(nw::Module* module, QString path, QObject* parent = nullptr);

    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual void loadRootItems() override;

private:
    nw::Module* module_ = nullptr;
    QString path_;
};

// == AreaListView ============================================================
// ============================================================================

class AreaListView : public ArclightTreeView {
    Q_OBJECT

public:
    explicit AreaListView(nw::Module* module, QString path, QWidget* parent = nullptr);
    ~AreaListView();

    virtual void activateModel() override;
    virtual AreaListModel* loadModel() override;

    nw::Module* module_ = nullptr;
    QString path_;
    AreaListModel* model_ = nullptr;
    FuzzyProxyModel* filter_ = nullptr;

public slots:
    void onDoubleClicked(const QModelIndex& index);

signals:
    void itemDoubleClicked(AreaListItem*);
};

#endif // AREALISTVIEW_H
