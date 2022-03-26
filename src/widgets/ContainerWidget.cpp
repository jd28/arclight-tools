#include "ContainerWidget.hpp"

#include "ui_ContainerWidget.h"

#include <nw/log.hpp>

#include <QEnterEvent>

ContainerWidget::ContainerWidget(nw::Container* container, int columns, QWidget* parent)
    : QWidget(parent)
    , ui_{new Ui::ContainerWidgetUI}
    , proxy_{std::make_unique<ContainerSortFilterProxyModel>(this)}
    , model_{std::make_unique<ContainerModel>(container, this)}
    , container_{container}
{
    ui_->setupUi(this);

    QObject::connect(ui_->filterEdit, &QLineEdit::textChanged,
        proxy_.get(), &ContainerSortFilterProxyModel::onFilterUpdated);

    model_->setColumnCount(columns);
    proxy_->setSourceModel(model_.get());

    ui_->containerView->setAcceptDrops(true);
    ui_->containerView->setDropIndicatorShown(true);
    ui_->containerView->setDragEnabled(true);
    ui_->containerView->setDragDropMode(QAbstractItemView::DragDrop);

    ui_->containerView->setModel(proxy_.get());
    ui_->containerView->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
    ui_->containerView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

ContainerWidget::~ContainerWidget()
{
    delete ui_;
}

nw::Container* ContainerWidget::container()
{
    return container_.get();
}

ContainerModel* ContainerWidget::model()
{
    return model_.get();
}

ContainerSortFilterProxyModel* ContainerWidget::proxy()
{
    return proxy_.get();
}

QTableView* ContainerWidget::table()
{
    return ui_->containerView;
}
