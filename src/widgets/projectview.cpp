#include "projectview.h"
#include "ui_projectview.h"

#include "projectmodel.h"

ProjectView::ProjectView(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ProjectView)
{
    ui->setupUi(this);

    connect(ui->projectTree, &QTreeView::doubleClicked, this, &ProjectView::onDoubleClicked);
}

ProjectView::~ProjectView()
{
    delete ui;
}

void ProjectView::load(nw::Module* module, QString path)
{
    module_ = module;
    path_ = std::move(path);
    model_ = new ProjectModel(module_, path_, this);
    model_->loadRootItems();
    filter_ = new ProjectSortFilterProxyModel(this);
    filter_->setRecursiveFilteringEnabled(true);
    filter_->setSourceModel(model_);
    filter_->sort(0);
    ui->projectTree->setModel(filter_);
    ui->projectTree->expandRecursively(filter_->index(0, 0));
}

void ProjectView::onDoubleClicked(const QModelIndex& index)
{
    if (!index.isValid()) { return; }
    QModelIndex sourceIndex = filter_->mapToSource(index);
    if (!sourceIndex.isValid()) { return; }

    emit doubleClicked(static_cast<ProjectItem*>(sourceIndex.internalPointer()));
}
