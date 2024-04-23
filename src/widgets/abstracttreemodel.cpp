#include "AbstractTreeModel.hpp"

AbstractTreeItem::~AbstractTreeItem()
{
    qDeleteAll(children_);
}

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
        return root_items_.size();
    }

    AbstractTreeItem* node = static_cast<AbstractTreeItem*>(parent.internalPointer());
    return node->childCount();
}

void AbstractTreeModel::addRootItem(AbstractTreeItem* item)
{
    root_items_.push_back(item);
}
