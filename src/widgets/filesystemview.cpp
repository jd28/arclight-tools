#include "filesystemview.h"

#include "util/restypeicons.h"

#include "ZFontIcon/ZFontIcon.h"
#include "ZFontIcon/ZFont_fa6.h"

#include <QDir>
#include <QFileInfo>

// == FileSystemItem ==========================================================
// ============================================================================

FileSystemItem::FileSystemItem(const QString& path, FileSystemItem* parent)
    : AbstractTreeItem(0, parent)
    , path_{path}
{
    QFileInfo fi(path_);
    is_folder_ = fi.isDir();
    basename_ = is_folder_ ? fi.baseName() : fi.fileName();
    restype_ = nw::ResourceType::from_extension(fi.completeSuffix().toStdString());
}

QVariant FileSystemItem::data(int column, int role) const
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

// == FileSystemModel ========================================================
// ============================================================================

FileSystemModel::FileSystemModel(QString path, QObject* parent)
    : AbstractTreeModel{parent}
    , path_{std::move(path)}
{
}

int FileSystemModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant FileSystemModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) { return {}; }
    auto item = reinterpret_cast<FileSystemItem*>(index.internalPointer());
    return item->data(index.column(), role);
}

void FileSystemModel::loadRootItems()
{
    walkDirectory(path_);
}

void FileSystemModel::walkDirectory(const QString& path, FileSystemItem* parent)
{
    QDir dir(path);

    // Get the list of entries in the directory
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    for (const QFileInfo& fileInfo : list) {
        auto path = fileInfo.absoluteFilePath();
        auto it = new FileSystemItem(path);

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

// == FileSystemProxtModel ====================================================
// ============================================================================

FileSystemProxyModel::FileSystemProxyModel(QObject* parent)
    : FuzzyProxyModel(parent)
{
}

bool FileSystemProxyModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const
{
    auto lhs = static_cast<FileSystemItem*>(source_left.internalPointer());
    auto rhs = static_cast<FileSystemItem*>(source_right.internalPointer());

    if (lhs->is_folder_ && !rhs->is_folder_) {
        return true;
    } else if (!lhs->is_folder_ && rhs->is_folder_) {
        return false;
    }
    return lhs->basename_.compare(rhs->basename_, Qt::CaseInsensitive) < 0;
}

// == FileSystemView ==========================================================
// ============================================================================

FileSystemView::FileSystemView(QString path, QWidget* parent)
    : QTreeView(parent)
    , path_{std::move(path)}
{
    setHeaderHidden(true);
    model_ = new FileSystemModel(path_, this);
    model_->loadRootItems();
    proxy_ = new FileSystemProxyModel(this);
    proxy_->setRecursiveFilteringEnabled(true);
    proxy_->setSourceModel(model_);
    setModel(proxy_);
    proxy_->sort(0);
}
