#include "projectmodel.h"

#include "nw/kernel/Strings.hpp"
#include "nw/objects/Area.hpp"
#include "nw/objects/Module.hpp"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>

// == ProjectItem =============================================================
// ============================================================================

ProjectItem::ProjectItem(QString name, ProjectItemType type, ProjectItem* parent)
    : AbstractTreeItem(0, parent)
    , path_(std::move(name))
    , type_{type}
{
}

ProjectItem::ProjectItem(nw::Area* area, ProjectItemType type, ProjectItem* parent)
    : AbstractTreeItem(0, parent)
    , area_(area)
    , type_{type}
{
}

QVariant ProjectItem::data(int column) const
{
    if (column != 0) { return {}; }
    if (!path_.isEmpty()) {
        return path_;
    } else if (area_) {
        return QString::fromStdString(nw::kernel::strings().get(area_->name));
    }
    return {};
}

// == ProjectModel ============================================================
// ============================================================================

ProjectModel::ProjectModel(nw::Module* module, QString path, QObject* parent)
    : AbstractTreeModel(parent)
    , module_{module}
    , path_{std::move(path)}
{
}

int ProjectModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant ProjectModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) { return {}; }

    ProjectItem* node = static_cast<ProjectItem*>(index.internalPointer());
    if (role == Qt::DisplayRole) {
        return node->data(0);
    }

    return {};
}

void ProjectModel::loadRootItems()
{
    auto areas = new ProjectItem("Areas", ProjectItemType::folder);
    addRootItem(areas);
    for (size_t i = 0; i < module_->area_count(); ++i) {
        auto it = new ProjectItem(module_->get_area(i), ProjectItemType::resource);
        areas->appendChild(it);
    }
}

// == ProjectSortFilterProxyModel =============================================
// ============================================================================

ProjectSortFilterProxyModel::ProjectSortFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

bool ProjectSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    return true;
}

// bool ProjectSortFilterProxyModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const
// {

//     return true;
// }
