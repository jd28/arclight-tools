#ifndef FILESTYSTEMMODEL_H
#define FILESTYSTEMMODEL_H

#include "AbstractTreeModel.hpp"

#include "arclighttreeview.h"
#include "nw/resources/StaticDirectory.hpp"
#include "proxymodels.h"

#include "nw/resources/ResourceType.hpp"

#include <QDateTime>
#include <QFileSystemWatcher>
#include <QSqlDatabase>
#include <QTreeView>

struct ProjectItemMetadata {
    QString object_name;
    qint64 size;
    QDateTime lastModified;
};

// == ProjectItem =============================================================
// ============================================================================

class ProjectItem : public AbstractTreeItem {
public:
    ProjectItem(const QString& name, const QString& path, nw::StaticDirectory* module, ProjectItem* parent = nullptr);
    ProjectItem(const QString& name, const QString& path, nw::Resource res, nw::StaticDirectory* module, ProjectItem* parent = nullptr);
    virtual QVariant data(int column, int role = Qt::DisplayRole) const override;

    nw::StaticDirectory* module_ = nullptr;
    QString path_;
    QString name_;
    nw::Resource res_;
    bool is_folder_ = false;
};

// == ProjectModel ============================================================
// ============================================================================

class ProjectModel : public AbstractTreeModel {
    Q_OBJECT
public:
    explicit ProjectModel(nw::StaticDirectory* module, QObject* parent = nullptr);

    virtual bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
    ProjectItemMetadata getMetadata(const QString& path);
    void insertMetadata(const QString& path, const ProjectItemMetadata& metadata);
    virtual void loadRootItems() override;
    QMimeData* mimeData(const QModelIndexList& indexes) const override;
    QStringList mimeTypes() const override;
    void setupDatabase();
    Qt::DropActions supportedDropActions() const override;
    Qt::DropActions supportedDragActions() const override;
    void walkDirectory(const QString& path, ProjectItem* parent = nullptr);

    QFileSystemWatcher watcher_;
    nw::StaticDirectory* module_ = nullptr;
    QString path_;
    QSqlDatabase db_;
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
