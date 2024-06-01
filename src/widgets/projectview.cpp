#include "projectview.h"

#include "util/restypeicons.h"

#include "ZFontIcon/ZFontIcon.h"
#include "ZFontIcon/ZFont_fa6.h"

#include <QDir>
#include <QFileInfo>

// == ProjectItem =============================================================
// ============================================================================

ProjectItem::ProjectItem(const QString& path, nw::StaticDirectory* module, ProjectItem* parent)
    : AbstractTreeItem(0, parent)
    , module_{module}
    , path_{path}
{
    QFileInfo fi(path_);
    is_folder_ = fi.isDir();
    basename_ = is_folder_ ? fi.baseName() : fi.fileName();
    restype_ = nw::ResourceType::from_extension(fi.completeSuffix().toStdString());
}

QVariant ProjectItem::data(int column, int role) const
{
    if (column != 0) { return {}; }
    if (role == Qt::DisplayRole) {
        return basename_;
    } else if (role == Qt::DecorationRole) {
        if (!is_folder_) {
            return restypeToIcon(restype_);
        } else {
            auto color = QColor(42, 130, 218);
            return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_folder, color);
        }
    }
    return {};
}

// == ProjectModel ============================================================
// ============================================================================

ProjectModel::ProjectModel(nw::StaticDirectory* module, QObject* parent)
    : AbstractTreeModel{parent}
    , module_{module}
    , path_{QString::fromStdString(module_->path())}
{
}

int ProjectModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant ProjectModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) { return {}; }
    auto item = reinterpret_cast<ProjectItem*>(index.internalPointer());
    return item->data(index.column(), role);
}

void ProjectModel::loadRootItems()
{
    walkDirectory(path_);
}

void ProjectModel::walkDirectory(const QString& path, ProjectItem* parent)
{
    QDir dir(path);

    // Get the list of entries in the directory
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    for (const QFileInfo& fileInfo : list) {
        auto path = fileInfo.absoluteFilePath();
        auto it = new ProjectItem(path, module_);

        if (parent) {
            parent->appendChild(it);
        } else {
            addRootItem(it);
        }

        if (fileInfo.isDir()) {
            walkDirectory(fileInfo.absoluteFilePath(), it);
        }
    }
}

// == ProjectProxyModel =======================================================
// ============================================================================

ProjectProxyModel::ProjectProxyModel(QObject* parent)
    : FuzzyProxyModel(parent)
{
}

bool ProjectProxyModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const
{
    auto lhs = static_cast<ProjectItem*>(source_left.internalPointer());
    auto rhs = static_cast<ProjectItem*>(source_right.internalPointer());

    if (lhs->is_folder_ && !rhs->is_folder_) {
        return true;
    } else if (!lhs->is_folder_ && rhs->is_folder_) {
        return false;
    }
    return lhs->basename_.compare(rhs->basename_, Qt::CaseInsensitive) < 0;
}

// == ProjectView ==========================================================
// ============================================================================

ProjectView::ProjectView(nw::StaticDirectory* module, QWidget* parent)
    : QTreeView(parent)
    , module_{module}
{
    setHeaderHidden(true);
    model_ = new ProjectModel(module_, this);
    model_->loadRootItems();
    proxy_ = new ProjectProxyModel(this);
    proxy_->setRecursiveFilteringEnabled(true);
    proxy_->setSourceModel(model_);
    setModel(proxy_);
    proxy_->sort(0);
}
