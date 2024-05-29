#ifndef ABSTRACTTREEMODEL_H
#define ABSTRACTTREEMODEL_H

#include <QAbstractItemModel>

#include <functional>

class AbstractTreeItem {
public:
    explicit AbstractTreeItem(int row, AbstractTreeItem* parent = 0)
        : row_(row)
        , parent_(parent)
    {
    }

    virtual ~AbstractTreeItem();

    virtual void appendChild(AbstractTreeItem* child)
    {
        child->row_ = childCount();
        children_.append(child);
    }

    AbstractTreeItem* child(int row) { return children_[row]; }
    int childCount() const { return int(children_.size()); }
    virtual QVariant data(int column, int role = Qt::DisplayRole) const = 0;
    int row() const { return row_; }
    AbstractTreeItem* parent() { return parent_; }
    void refreshChildRows();
    virtual void removeChild(AbstractTreeItem* item);

    int row_;
    QList<AbstractTreeItem*> children_;
    AbstractTreeItem* parent_;
};

class AbstractTreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit AbstractTreeModel(QObject* parent = nullptr);
    ~AbstractTreeModel();

    // Basic functionality:
    QModelIndex index(int row, int column,
        const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    void addRootItem(AbstractTreeItem* item);
    void addRow(AbstractTreeItem* item, QModelIndex parent = QModelIndex());
    virtual void loadRootItems() = 0;
    void deleteRow(AbstractTreeItem* item, QModelIndex parent);
    void deleteAllMatchingRows(std::function<bool(AbstractTreeItem*)> matcher, AbstractTreeItem* cursor = nullptr);
    AbstractTreeItem* root() { return root_items_[0]; }
    AbstractTreeItem* root() const { return root_items_[0]; }

private:
    QList<AbstractTreeItem*> root_items_;
};

#endif // ABSTRACTTREEMODEL_H
