#ifndef EXPLORERVIEW_H
#define EXPLORERVIEW_H

#include "AbstractTreeModel.hpp"
#include "arclighttreeview.h"
#include "proxymodels.h"

#include "nw/resources/Container.hpp"

#include <QSortFilterProxyModel>
#include <QTreeView>


// == ExplorerItem ============================================================
// ============================================================================

enum struct ExplorerItemKind {
    category,
    container,
    resource,
};

class ExplorerItem : public AbstractTreeItem {
public:
    ExplorerItem(QString name, AbstractTreeItem* parent = 0);
    ExplorerItem(nw::Container* container, AbstractTreeItem* parent = 0);
    ExplorerItem(nw::ResourceDescriptor res, AbstractTreeItem* parent = 0);

    virtual QVariant data(int column, int role) const override;

    QString name_;
    nw::Container* container_ = nullptr;
    nw::ResourceDescriptor descriptor_;
    ExplorerItemKind kind_;
};

// == ExplorerModel ===========================================================
// ============================================================================

class ExplorerModel : public AbstractTreeModel {
    Q_OBJECT
public:
    explicit ExplorerModel(QObject* parent = nullptr);

    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual void loadRootItems() override;
};

// == ExplorerProxy ===========================================================
// ============================================================================

class ExplorerProxy : public FuzzyProxyModel {
public:
    ExplorerProxy(QObject* parent = nullptr);

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
    virtual bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;
};

// == ExplorerView ============================================================
// ============================================================================

class ExplorerView : public ArclightTreeView {
public:
    ExplorerView(QWidget* parent = nullptr);
    ~ExplorerView();

    virtual void activateModel() override;
    virtual AbstractTreeModel* loadModel() override;

    ExplorerModel* model_;
    ExplorerProxy* proxy_;
};

#endif // EXPLORERVIEW_H
