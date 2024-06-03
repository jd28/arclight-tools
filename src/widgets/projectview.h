#ifndef FILESTYSTEMMODEL_H
#define FILESTYSTEMMODEL_H

#include "AbstractTreeModel.hpp"

#include "arclighttreeview.h"
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

class ProjectProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    ProjectProxyModel(QObject* parent = nullptr);

protected:
    virtual bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;
    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

public slots:
    void onFilterChanged(QString filter);

public:
    QString filter_;
};

// == ProjectView =============================================================
// ============================================================================

class ProjectView : public ArclightTreeView {
    Q_OBJECT
public:
    ProjectView(nw::StaticDirectory* module, QWidget* parent = nullptr);
    ~ProjectView();

    void activateModel();
    AbstractTreeModel* loadModel();

public slots:
    void onDoubleClicked(const QModelIndex& index);

signals:
    void itemDoubleClicked(ProjectItem*);

public:
    nw::StaticDirectory* module_ = nullptr;
    ProjectModel* model_;
    ProjectProxyModel* proxy_;
};

#endif // FILESTYSTEMMODEL_H
