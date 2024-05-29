#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H

#include "AbstractTreeModel.hpp"

#include "nw/resources/Resource.hpp"

#include <QTreeView>
#include <QWidget>

class ProjectItem;
class ProjectModel;
class FuzzyProxyModel;

namespace nw {
struct Area;
struct Module;
}

// == ProjectItem =============================================================
// ============================================================================

enum struct ProjectItemType {
    category,
    area,
    dialog,
};

class ProjectItem : public AbstractTreeItem {
public:
    ProjectItem(nw::Area* area, ProjectItemType type, ProjectItem* parent = nullptr);
    ProjectItem(QString name, ProjectItemType type, ProjectItem* parent = nullptr);
    ProjectItem(nw::Resource res, ProjectItemType type, ProjectItem* parent);

    virtual QVariant data(int column, int role = Qt::DisplayRole) const override;

    nw::Area* area_ = nullptr;
    QString path_;
    nw::Resource res_;
    ProjectItemType type_;
};

// == ProjectModel ============================================================
// ============================================================================

class ProjectModel : public AbstractTreeModel {
    Q_OBJECT

public:
    explicit ProjectModel(nw::Module* module, QString path, QObject* parent = nullptr);

    // Header:
    // QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // QAbstractItemModel interface
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

    // AbstractTreeModel interface
    virtual void loadRootItems() override;

private:
    nw::Module* module_ = nullptr;
    QString path_;
};

// == ProjectView =============================================================
// ============================================================================

class ProjectView : public QTreeView {
    Q_OBJECT

public:
    explicit ProjectView(QWidget* parent = nullptr);
    ~ProjectView();

    void load(nw::Module* module, QString path);

    nw::Module* module_ = nullptr;
    QString path_;
    ProjectModel* model_ = nullptr;
    FuzzyProxyModel* filter_ = nullptr;

public slots:
    void onDoubleClicked(const QModelIndex& index);

signals:
    void doubleClicked(ProjectItem*);
};

#endif // PROJECTVIEW_H
