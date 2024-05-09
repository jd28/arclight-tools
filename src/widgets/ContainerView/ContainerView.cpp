#include "ContainerView.hpp"

#include "ui_ContainerView.h"

#include <nw/log.hpp>
#include <nw/resources/Erf.hpp>

#include <QMessageBox>

namespace fs = std::filesystem;

ContainerView::ContainerView(nw::Container* container, int columns, QWidget* parent)
    : QWidget(parent)
    , ui_{new Ui::ContainerViewUI}
    , proxy_{std::make_unique<ContainerSortFilterProxyModel>(this)}
    , model_{std::make_unique<ContainerModel>(container, this)}
    , container_{container}
{
    ui_->setupUi(this);

    QObject::connect(ui_->filterEdit, &QLineEdit::textChanged,
        proxy_.get(), &ContainerSortFilterProxyModel::onFilterUpdated);

    model_->setColumnCount(columns);
    proxy_->setSourceModel(model_.get());

    if (dynamic_cast<nw::Erf*>(container)) {
        ui_->containerView->setAcceptDrops(true);
        ui_->containerView->setDropIndicatorShown(true);
    }
    ui_->containerView->setDragEnabled(true);
    ui_->containerView->setDragDropMode(QAbstractItemView::DragDrop);

    ui_->containerView->setModel(proxy_.get());
    ui_->containerView->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
    ui_->containerView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

ContainerView::~ContainerView()
{
    delete ui_;
}

nw::Container* ContainerView::container()
{
    return container_.get();
}

ContainerModel* ContainerView::model()
{
    return model_.get();
}

ContainerSortFilterProxyModel* ContainerView::proxy()
{
    return proxy_.get();
}

QTableView* ContainerView::table()
{
    return ui_->containerView;
}
