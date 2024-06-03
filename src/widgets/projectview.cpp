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
    if (!is_folder_) {
        basename_ = fi.fileName();
        res_ = nw::Resource::from_filename(basename_.toStdString());
    } else {
        basename_ = fi.baseName();
    }
}

inline bool comparePaths(const QString& path1, const QString& path2)
{
    QString normalizedPath1 = QDir::fromNativeSeparators(QDir::cleanPath(path1));
    QString normalizedPath2 = QDir::fromNativeSeparators(QDir::cleanPath(path2));
    return normalizedPath1 == normalizedPath2;
}

QVariant ProjectItem::data(int column, int role) const
{
    if (column != 0) { return {}; }
    if (role == Qt::DisplayRole) {
        return basename_;
    } else if (role == Qt::DecorationRole) {
        if (!is_folder_) {
            auto path = module_->get_canonical_path(res_);
            if (!comparePaths(QString::fromStdString(path), path_)) {
                return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_circle_exclamation, Qt::red);
            }
            return restypeToIcon(res_.type);
        } else {
            auto color = QColor(42, 130, 218);
            return ZFontIcon::icon(Fa6::FAMILY, Fa6::SOLID, Fa6::fa_folder, color);
        }
    } else if (role == Qt::ToolTipRole) {
        if (!is_folder_) {
            auto path = QString::fromStdString(module_->get_canonical_path(res_));
            if (!comparePaths(path, path_)) {
                return QString("%1 is shadowed by %2").arg(path_, path);
            }
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
        auto p = fileInfo.canonicalFilePath();
        auto it = new ProjectItem(p, module_);

        if (parent) {
            parent->appendChild(it);
        } else {
            addRootItem(it);
        }

        if (fileInfo.isDir()) {
            walkDirectory(fileInfo.canonicalFilePath(), it);
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

    connect(this, &QTreeView::doubleClicked, this, &ProjectView::onDoubleClicked);
}

void ProjectView::onDoubleClicked(const QModelIndex& index)
{
    if (!index.isValid()) { return; }
    QModelIndex sourceIndex = proxy_->mapToSource(index);
    if (!sourceIndex.isValid()) { return; }

    emit itemDoubleClicked(static_cast<ProjectItem*>(sourceIndex.internalPointer()));
}
