#include "projectview.h"
#include "proxymodels.h"

#include "nw/kernel/Resources.hpp"
#include "nw/kernel/Strings.hpp"
#include "nw/objects/Area.hpp"
#include "nw/objects/Module.hpp"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>

// == ProjectItem =============================================================
// ============================================================================

ProjectItem::ProjectItem(QString name, ProjectItemType type, ProjectItem* parent)
    : AbstractTreeItem(0, parent)
    , path_(std::move(name))
    , type_{type}
{
}

ProjectItem::ProjectItem(nw::Area* area, ProjectItemType type, ProjectItem* parent)
    : AbstractTreeItem(0, parent)
    , area_(area)
    , type_{type}
{
}

ProjectItem::ProjectItem(nw::Resource res, ProjectItemType type, ProjectItem* parent)
    : AbstractTreeItem(0, parent)
    , res_(res)
    , type_{type}
{
    path_ = QString::fromStdString(res_.resref.string());
}

QVariant ProjectItem::data(int column, int role) const
{
    if (column != 0) { return {}; }
    if (!path_.isEmpty()) {
        return path_;
    } else if (area_) {
        return QString::fromStdString(nw::kernel::strings().get(area_->name));
    }
    return {};
}

// == ProjectModel ============================================================
// ============================================================================

ProjectModel::ProjectModel(nw::Module* module, QString path, QObject* parent)
    : AbstractTreeModel(parent)
    , module_{module}
    , path_{std::move(path)}
{
}

int ProjectModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant ProjectModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) { return {}; }

    ProjectItem* node = static_cast<ProjectItem*>(index.internalPointer());
    if (role == Qt::DisplayRole) {
        return node->data(0);
    }

    return {};
}

void ProjectModel::loadRootItems()
{
    auto areas = new ProjectItem("Areas", ProjectItemType::category);
    addRootItem(areas);
    for (size_t i = 0; i < module_->area_count(); ++i) {
        auto it = new ProjectItem(module_->get_area(i), ProjectItemType::area, areas);
        areas->appendChild(it);
    }

    auto dialogs = new ProjectItem("Dialogs", ProjectItemType::category);
    addRootItem(dialogs);
    auto mod = nw::kernel::resman().module_container();

    auto dlg_getter = [dialogs](const nw::Resource& res) {
        auto it = new ProjectItem(res, ProjectItemType::dialog, dialogs);
        dialogs->appendChild(it);
    };

    mod->visit(dlg_getter, {nw::ResourceType::dlg});
}

// == ProjectView =============================================================
// ============================================================================

ProjectView::ProjectView(QWidget* parent)
    : QTreeView(parent)
{
    connect(this, &QTreeView::doubleClicked, this, &ProjectView::onDoubleClicked);
}

ProjectView::~ProjectView()
{
}

void ProjectView::load(nw::Module* module, QString path)
{
    module_ = module;
    path_ = std::move(path);
    model_ = new ProjectModel(module_, path_, this);
    model_->loadRootItems();

    filter_ = new FuzzyProxyModel(this);
    filter_->setRecursiveFilteringEnabled(true);
    filter_->setSourceModel(model_);
    filter_->sort(0);

    setModel(filter_);
    expandRecursively(filter_->index(0, 0));
}

void ProjectView::onDoubleClicked(const QModelIndex& index)
{
    if (!index.isValid()) { return; }
    QModelIndex sourceIndex = filter_->mapToSource(index);
    if (!sourceIndex.isValid()) { return; }

    emit doubleClicked(static_cast<ProjectItem*>(sourceIndex.internalPointer()));
}
