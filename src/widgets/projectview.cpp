#include "projectview.h"

extern "C" {
#include "fzy/match.h"
}
#include "ZFontIcon/ZFontIcon.h"
#include "ZFontIcon/ZFont_fa6.h"
#include "nw/kernel/Objects.hpp"
#include "util/restypeicons.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMimeData>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

inline QString read_object_name(const QString& path)
{
    QFileInfo fi(path);
    auto basename = fi.fileName();

    auto res_ = nw::Resource::from_filename(basename.toStdString());
    if (res_.type == nw::ResourceType::uti) {
        auto name = nw::Item::get_name_from_file(path.toStdString());
        if (!name.empty()) {
            return QString::fromStdString(name);
        }
    } else if (res_.type == nw::ResourceType::utc) {
        auto name = nw::Creature::get_name_from_file(path.toStdString());
        if (!name.empty()) {
            return QString::fromStdString(name);
        }
    } else if (res_.type == nw::ResourceType::are) {
        auto name = nw::Area::get_name_from_file(path.toStdString());
        if (!name.empty()) {
            return QString::fromStdString(name);
        }
    } else if (res_.type == nw::ResourceType::utd) {
        auto name = nw::Door::get_name_from_file(path.toStdString());
        if (!name.empty()) {
            return QString::fromStdString(name);
        }
    } else if (res_.type == nw::ResourceType::ute) {
        auto name = nw::Encounter::get_name_from_file(path.toStdString());
        if (!name.empty()) {
            return QString::fromStdString(name);
        }
    } else if (res_.type == nw::ResourceType::utm) {
        auto name = nw::Store::get_name_from_file(path.toStdString());
        if (!name.empty()) {
            return QString::fromStdString(name);
        }
    } else if (res_.type == nw::ResourceType::utp) {
        auto name = nw::Placeable::get_name_from_file(path.toStdString());
        if (!name.empty()) {
            return QString::fromStdString(name);
        }
    } else if (res_.type == nw::ResourceType::uts) {
        auto name = nw::Sound::get_name_from_file(path.toStdString());
        if (!name.empty()) {
            return QString::fromStdString(name);
        }
    } else if (res_.type == nw::ResourceType::utt) {
        auto name = nw::Trigger::get_name_from_file(path.toStdString());
        if (!name.empty()) {
            return QString::fromStdString(name);
        }
    }

    return basename;
}

// == ProjectItem =============================================================
// ============================================================================

ProjectItem::ProjectItem(const QString& name, const QString& path, nw::StaticDirectory* module, ProjectItem* parent)
    : AbstractTreeItem(0, parent)
    , module_{module}
    , path_{path}
    , name_{name}
    , is_folder_{true}
{
}

ProjectItem::ProjectItem(const QString& name, const QString& path, nw::Resource res, nw::StaticDirectory* module, ProjectItem* parent)
    : AbstractTreeItem(0, parent)
    , module_{module}
    , path_{path}
    , name_{name}
    , res_{res}
    , is_folder_{false}
{
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
        return name_;
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
            } else {
                return QString::fromStdString(res_.filename());
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
    setupDatabase();
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
    watcher_.addPath(path);

    // Get the list of entries in the directory
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);

    for (const QFileInfo& fileInfo : list) {
        auto p = fileInfo.canonicalFilePath();

        ProjectItem* it = nullptr;
        nw::Resource res;
        if (fileInfo.isDir()) {
            it = new ProjectItem(fileInfo.baseName(), p, module_);
            walkDirectory(fileInfo.canonicalFilePath(), it);
        } else {
            res = nw::Resource::from_path(p.toStdString());
            if (res.valid()) {
                auto meta = getMetadata(p);
                if (fileInfo.lastModified() > meta.lastModified) {
                    meta.object_name = read_object_name(p);
                    meta.size = fileInfo.size();
                    meta.lastModified = fileInfo.lastModified();
                    insertMetadata(p, meta);
                }

                it = new ProjectItem(meta.object_name, p, res, module_);
            }
        }

        if (it) {
            if (parent) {
                parent->appendChild(it);
            } else {
                addRootItem(it);
            }
        }
    }
}

bool ProjectModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
    // Note parent parameter if dropping direcly on to a node, is that node, i.e. the new parent.
    // If dropping between nodes it's the parent of the new siblings.

    if (!data->hasFormat("application/x-arclight-projectitem")) {
        LOG_F(INFO, "wrong format");
        return false;
    }

    QByteArray data_ = data->data("application/x-arclight-projectitem");
    QDataStream stream(&data_, QIODevice::ReadOnly);
    qint64 senderPid;
    stream >> senderPid;
    if (senderPid != QCoreApplication::applicationPid()) {
        // Let's not cast pointers that come from another process...
        LOG_F(INFO, "wrong pid");
        return false;
    }

    qlonglong ptr;
    stream >> ptr;
    const ProjectItem* node = reinterpret_cast<const ProjectItem*>(ptr);

    if (row != -1) { return false; }

    auto parent_node = reinterpret_cast<ProjectItem*>(parent.internalPointer());
    Q_ASSERT(parent_node);

    if (parent_node->is_folder_) {
        return true;
    } else if (parent_node->parent_ == node->parent_) {
        return false;
    }

    auto result = AbstractTreeModel::canDropMimeData(data, action, row, column, parent);
    LOG_F(INFO, "result: {}", result);
    return result;
}

Qt::ItemFlags ProjectModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    auto result = QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    return result;
}

ProjectItemMetadata ProjectModel::getMetadata(const QString& path)
{
    QSqlQuery query;
    query.prepare("SELECT object_name, size, last_modified FROM file_metadata WHERE path = :path");
    query.bindValue(":path", path);
    if (query.exec() && query.next()) {
        ProjectItemMetadata metadata;
        metadata.object_name = query.value(0).toString();
        metadata.size = query.value(1).toLongLong();
        metadata.lastModified = query.value(2).toDateTime();
        return metadata;
    }
    return {}; // Return an empty metadata struct if not found
}

void ProjectModel::insertMetadata(const QString& path, const ProjectItemMetadata& metadata)
{
    QSqlQuery query;
    query.prepare("REPLACE INTO file_metadata (path, object_name, size, last_modified) VALUES (:path, :object_name, :size, :last_modified)");
    query.bindValue(":path", path);
    query.bindValue(":object_name", metadata.object_name);
    query.bindValue(":size", metadata.size);
    query.bindValue(":last_modified", metadata.lastModified);
    if (!query.exec()) {
        qWarning() << "Insert error:" << query.lastError().text();
    }
}

QMimeData* ProjectModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* mimeData = new QMimeData;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    const ProjectItem* node = static_cast<ProjectItem*>(indexes[0].internalPointer());

    stream << QCoreApplication::applicationPid();
    stream << reinterpret_cast<qlonglong>(node);

    mimeData->setData("application/x-arclight-projectitem", data);
    return mimeData;
}

QStringList ProjectModel::mimeTypes() const
{
    return QStringList() << "application/x-arclight-projectitem";
}

void ProjectModel::setupDatabase()
{
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(QDir(path_).filePath(".arclight_meta.db"));
    if (db_.open()) {
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS file_metadata ("
                   "path TEXT PRIMARY KEY, "
                   "object_name TEXT, "
                   "size INTEGER, "
                   "last_modified DATETIME)");
    } else {
        qWarning() << "Open database" << db_.lastError().text();
    }
}

Qt::DropActions ProjectModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions ProjectModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

// == ProjectProxyModel =======================================================
// ============================================================================

ProjectProxyModel::ProjectProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

bool ProjectProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    auto item = static_cast<ProjectItem*>(index.internalPointer());
    if (item->res_.type == nw::ResourceType::git || item->res_.type == nw::ResourceType::gic) {
        return false;
    }

    if (filter_.isEmpty()) { return true; }
    auto data = index.data(Qt::DisplayRole);
    return has_match(filter_.toStdString().c_str(), data.toString().toStdString().c_str());
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
    return lhs->name_.compare(rhs->name_, Qt::CaseInsensitive) < 0;
}

void ProjectProxyModel::onFilterChanged(QString filter)
{
    filter_ = std::move(filter);
    invalidateFilter();
}

// == ProjectView ==========================================================
// ============================================================================

ProjectView::ProjectView(nw::StaticDirectory* module, QWidget* parent)
    : ArclightTreeView(parent)
    , module_{module}
{
    setHeaderHidden(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(this, &QTreeView::doubleClicked, this, &ProjectView::onDoubleClicked);
}

ProjectView::~ProjectView()
{
    delete model_;
}

void ProjectView::activateModel()
{
    proxy_ = new ProjectProxyModel(this);
    proxy_->setRecursiveFilteringEnabled(true);
    proxy_->setSourceModel(model_);
    setModel(proxy_);
    proxy_->sort(0);
}

AbstractTreeModel* ProjectView::loadModel()
{
    model_ = new ProjectModel(module_);
    model_->loadRootItems();
    return model_;
}

void ProjectView::onDoubleClicked(const QModelIndex& index)
{
    if (!index.isValid()) { return; }
    QModelIndex sourceIndex = proxy_->mapToSource(index);
    if (!sourceIndex.isValid()) { return; }

    emit itemDoubleClicked(static_cast<ProjectItem*>(sourceIndex.internalPointer()));
}
