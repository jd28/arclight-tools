#ifndef FILESTYSTEMMODEL_H
#define FILESTYSTEMMODEL_H

#include "AbstractTreeModel.hpp"

#include "nw/resources/StaticDirectory.hpp"
#include "proxymodels.h"

#include "nw/resources/ResourceType.hpp"

#include <QTreeView>

// == ProjectItem =============================================================
// ============================================================================

class ProjectItem : public AbstractTreeItem {
public:
    ProjectItem(const QString& path, nw::StaticDirectory* module, ProjectItem* parent = nullptr);
    virtual QVariant data(int column, int role = Qt::DisplayRole) const override;

    nw::StaticDirectory* module_ = nullptr;
    QString path_;
    QString basename_;
    bool is_folder_ = false;
    nw::Resource res_;
};

// == ProjectModel ============================================================
// ============================================================================

class ProjectModel : public AbstractTreeModel {
    Q_OBJECT
public:
    explicit ProjectModel(nw::StaticDirectory* module, QObject* parent = nullptr);

    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual void loadRootItems() override;
    void walkDirectory(const QString& path, ProjectItem* parent = nullptr);

    nw::StaticDirectory* module_ = nullptr;
    QString path_;
};

// == FileSystemProxtModel ====================================================
// ============================================================================

class ProjectProxyModel : public FuzzyProxyModel {
    Q_OBJECT
public:
    ProjectProxyModel(QObject* parent = nullptr);

    virtual bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;
};

// == ProjectView =============================================================
// ============================================================================

class ProjectView : public QTreeView {
public:
    ProjectView(nw::StaticDirectory* module, QWidget* parent = nullptr);

    nw::StaticDirectory* module_ = nullptr;
    ProjectModel* model_;
    ProjectProxyModel* proxy_;
};

#endif // FILESTYSTEMMODEL_H
