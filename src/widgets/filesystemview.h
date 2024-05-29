#ifndef FILESTYSTEMMODEL_H
#define FILESTYSTEMMODEL_H

#include "AbstractTreeModel.hpp"

#include "proxymodels.h"

#include "nw/resources/ResourceType.hpp"

#include <QTreeView>

// == FileSystemItem ==========================================================
// ============================================================================

class FileSystemItem : public AbstractTreeItem {
public:
    FileSystemItem(const QString& path, FileSystemItem* parent = nullptr);
    virtual QVariant data(int column, int role = Qt::DisplayRole) const override;

    QString path_;
    QString basename_;
    bool is_folder_ = false;
    nw::ResourceType::type restype_ = nw::ResourceType::invalid;
};

// == FileSystemModel =========================================================
// ============================================================================

class FileSystemModel : public AbstractTreeModel {
    Q_OBJECT
public:
    explicit FileSystemModel(QString path, QObject* parent = nullptr);

    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual void loadRootItems() override;
    void walkDirectory(const QString& path, FileSystemItem* parent = nullptr);

    QString path_;
};

// == FileSystemProxtModel ====================================================
// ============================================================================

class FileSystemProxyModel : public FuzzyProxyModel {
    Q_OBJECT
public:
    FileSystemProxyModel(QObject* parent = nullptr);

    virtual bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;
};

// == FileSystemView ==========================================================
// ============================================================================

class FileSystemView : public QTreeView {
public:
    FileSystemView(QString path, QWidget* parent = nullptr);

    QString path_;
    FileSystemModel* model_;
    FileSystemProxyModel* proxy_;
};

#endif // FILESTYSTEMMODEL_H
