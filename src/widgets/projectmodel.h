#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include "AbstractTreeModel.hpp"

#include "nw/resources/Resource.hpp"

#include <QSortFilterProxyModel>

namespace nw {
struct Area;
struct Module;
}

enum struct ProjectItemType {
    category,
    area,
    dialog,
};

// == ProjectItem =============================================================
// ============================================================================

class ProjectItem : public AbstractTreeItem {
public:
    ProjectItem(nw::Area* area, ProjectItemType type, ProjectItem* parent = nullptr);
    ProjectItem(QString name, ProjectItemType type, ProjectItem* parent = nullptr);
    ProjectItem(nw::Resource res, ProjectItemType type, ProjectItem* parent);

    virtual QVariant data(int column, int role = Qt::DisplayRole) const override;

    nw::Area* area_ = nullptr;
    QString path_;
    nw::Resource res_;
    ProjectItemType type_;
};

// == ProjectModel ============================================================
// ============================================================================

class ProjectModel : public AbstractTreeModel {
    Q_OBJECT

public:
    explicit ProjectModel(nw::Module* module, QString path, QObject* parent = nullptr);

    // Header:
    // QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // QAbstractItemModel interface
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

    // AbstractTreeModel interface
    virtual void loadRootItems() override;

private:
    nw::Module* module_ = nullptr;
    QString path_;
};

#endif // PROJECTMODEL_H
