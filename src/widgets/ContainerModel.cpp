#include "ContainerModel.hpp"

#include "util/restypeicons.h"

extern "C" {
#include <fzy/match.h>
}

#include <nw/log.hpp>
#include <nw/resources/Erf.hpp>
#include <nw/resources/ResourceType.hpp>
#include <nw/util/string.hpp>

#include <QFileInfo>
#include <QMimeData>
#include <QUrl>

#include <algorithm>

namespace fs = std::filesystem;

// ----------------------------------------------------------------------------
// -- ContainerSortFilterProxyModel -------------------------------------------

ContainerSortFilterProxyModel::ContainerSortFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
    , filter_empty_{true}
{
}

void ContainerSortFilterProxyModel::onFilterUpdated(const QString& filter)
{
    if (filter.size() == 0) {
        filter_ = std::string{};
        filter_empty_ = true;
    } else {
        filter_ = filter.toLower().toStdString();
        filter_empty_ = false;
    }

    invalidateFilter();
}

bool ContainerSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    if (filter_empty_) { return true; }

    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    auto data = index.data(Qt::DisplayRole);
    return has_match(filter_.c_str(), data.toString().toStdString().c_str());
}

// ----------------------------------------------------------------------------
// -- ContainerModel ----------------------------------------------------------

ContainerModel::ContainerModel(nw::Container* container, QObject* parent)
    : QAbstractTableModel(parent)
    , container_{container}
{
    resources_ = container_->all();
    std::sort(std::begin(resources_), std::end(resources_), [](const auto& a, const auto& b) {
        return a.name.filename() < b.name.filename();
    });
}

void ContainerModel::addFiles(const QStringList& files)
{
    std::filesystem::path p;
    nw::Resource r;
    beginResetModel();
    if (auto e = dynamic_cast<nw::Erf*>(container_)) {
        for (const auto& f : files) {
            p = fs::u8path(f.toStdString());
            r = nw::Resource::from_path(p);
            if (!r.valid()) {
                // Put message eventually.
                continue;
            }
            e->add(p);
        }
    }
    resources_ = container_->all();
    endResetModel();
}

void ContainerModel::mergeFiles(const QStringList& files)
{
    beginResetModel();
    if (auto e = dynamic_cast<nw::Erf*>(container_)) {
    }
    resources_ = container_->all();
    endResetModel();
}

QVariant ContainerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!container_) { return {}; }
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return {};

    if (section == 0) {
        return "Resource";
    } else {
        return "Size (bytes)";
    }
}

int ContainerModel::rowCount(const QModelIndex& parent) const
{
    return !parent.isValid() ? static_cast<int>(container_->size()) : 0;
}

int ContainerModel::columnCount(const QModelIndex&) const
{
    return cols_;
}

void ContainerModel::setColumnCount(int cols)
{
    cols_ = cols;
}

QVariant ContainerModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    size_t row = static_cast<size_t>(index.row());

    if (role == Qt::DecorationRole && index.column() == 0) {
        return restypeToIcon(resources_[row].name.type);
    } else if (role == Qt::DisplayRole) {
        switch (index.column()) {
        default:
            return {};
        case 0:
            return QString::fromStdString(resources_[row].name.filename());
        case 1:
            return static_cast<int>(resources_[row].size);
        }
    }
    return {};
}

Qt::ItemFlags ContainerModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

    if (index.isValid()) {
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    } else {
        return Qt::ItemIsDropEnabled | defaultFlags;
    }
}

bool ContainerModel::canDropMimeData(const QMimeData* mime, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    if (!mime->hasUrls()) { return false; }
    for (const auto& url : mime->urls()) {
        QFileInfo fn{url.fileName()};
        QFileInfo path{url.toLocalFile()};
        if (path.absolutePath().toStdString() == container_->working_directory()) {
            return false;
        }
        if (path.isDir()) { return false; }
        if (nw::ResourceType::from_extension(fn.completeSuffix().toStdString()) == nw::ResourceType::invalid) {
            return false;
        }
    }
    return true;
}

bool ContainerModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
    if (!canDropMimeData(data, action, row, column, parent))
        return false;

    if (action == Qt::IgnoreAction)
        return true;

    QStringList files;
    for (const auto& url : data->urls()) {
        files.append(url.toLocalFile());
    }

    addFiles(files);

    return true;
}

Qt::DropActions ContainerModel::supportedDropActions() const
{
    return Qt::CopyAction;
}

QMimeData* ContainerModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* md = new QMimeData;
    QList<QUrl> urls;
    for (const auto& idx : indexes) {
        size_t row = static_cast<size_t>(idx.row());
        auto name = resources_[row].name.filename();
        container_->extract_by_glob(name, container_->working_directory());
        auto fn = (container_->working_directory() / name).u8string();
        auto url = QUrl::fromLocalFile(QString::fromStdString(fn));
        if (!urls.contains(url)) {
            urls << url;
        }
    }

    md->setUrls(std::move(urls));
    return md;
}

QStringList ContainerModel::mimeTypes() const
{
    return QStringList() << "text/uri-list";
}
