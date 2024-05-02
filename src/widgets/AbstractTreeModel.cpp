#include "AbstractTreeModel.hpp"

#include "nw/log.hpp"

// == AbstractTreeItem ========================================================
// ============================================================================

AbstractTreeItem::~AbstractTreeItem()
{
    qDeleteAll(children_);
}

void AbstractTreeItem::refreshChildRows()
{
    for (int i = 0; i < childCount(); ++i) {
        children_[i]->row_ = i;
    }
}

void AbstractTreeItem::removeChild(AbstractTreeItem* item)
{
    children_.removeAll(item);
    refreshChildRows();
}

// == AbstractTreeModel =======================================================
// ============================================================================

AbstractTreeModel::AbstractTreeModel(QObject* parent)
    : QAbstractItemModel(parent)
{
}

AbstractTreeModel::~AbstractTreeModel()
{
    qDeleteAll(root_items_);
}

QModelIndex AbstractTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (row < 0 || column < 0 || column >= columnCount(parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        if (row >= root_items_.size()) {
            return QModelIndex();
        }
        return createIndex(row, column, root_items_[row]);
    }

    AbstractTreeItem* node = static_cast<AbstractTreeItem*>(parent.internalPointer());
    if (!node) { return QModelIndex(); }

    if (row >= node->childCount()) {
        return QModelIndex();
    }

    return createIndex(row, column, node->child(row));
}

QModelIndex AbstractTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid()) { return QModelIndex(); }

    AbstractTreeItem* node = static_cast<AbstractTreeItem*>(index.internalPointer());
    if (!node || !node->parent()) { return QModelIndex(); }

    return createIndex(node->parent()->row(), 0, node->parent());
}

int AbstractTreeModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid()) {
        return int(root_items_.size());
    }

    AbstractTreeItem* node = static_cast<AbstractTreeItem*>(parent.internalPointer());
    return node->childCount();
}

void AbstractTreeModel::addRow(AbstractTreeItem* item, QModelIndex parent)
{
    if (!parent.isValid()) { return; }
    auto parent_item = reinterpret_cast<AbstractTreeItem*>(parent.internalPointer());
    beginInsertRows(parent, item->row(), item->row());
    parent_item->appendChild(item);
    endInsertRows();
}

void AbstractTreeModel::deleteRow(AbstractTreeItem* item, QModelIndex parent)
{
    if (!parent.isValid()) { return; }
    auto parent_item = reinterpret_cast<AbstractTreeItem*>(parent.internalPointer());
    beginRemoveRows(parent, item->row(), item->row());
    parent_item->removeChild(item);
    endRemoveRows();
}

void AbstractTreeModel::deleteAllMatchingRows(std::function<bool(AbstractTreeItem*)> matcher, AbstractTreeItem* cursor)
{
    if (!cursor) {
        cursor = root();
    } else if (matcher(cursor)) {
        deleteRow(cursor, createIndex(cursor->row(), 0, cursor->parent_));
        delete cursor;
        return;
    }

    for (auto child : cursor->children_) {
        deleteAllMatchingRows(matcher, child);
    }
}

void AbstractTreeModel::addRootItem(AbstractTreeItem* item)
{
    root_items_.push_back(item);
}
