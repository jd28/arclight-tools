#ifndef ABSTRACTTREEMODEL_H
#define ABSTRACTTREEMODEL_H

#include <QAbstractItemModel>

class AbstractTreeItem {
public:
    explicit AbstractTreeItem(int row, AbstractTreeItem* parent = 0)
        : row_(row)
        , parent_(parent)
    {
    }
    virtual ~AbstractTreeItem();

    void appendChild(AbstractTreeItem* child) { children_.append(child); }
    AbstractTreeItem* child(int row) { return children_[row]; }
    int childCount() const { return children_.size(); }
    virtual QVariant data(int column) const = 0;
    int row() const { return row_; }
    AbstractTreeItem* parent() { return parent_; }

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
    virtual void loadRootItems() = 0;

private:
    QList<AbstractTreeItem*> root_items_;
};

#endif // ABSTRACTTREEMODEL_H
