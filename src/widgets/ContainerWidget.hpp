#pragma once

#include "ContainerModel.hpp"

#include <nw/resources/Container.hpp>

#include <QTableView>
#include <QWidget>

namespace Ui {
class ContainerWidgetUI;
}

class ContainerWidget : public QWidget {
public:
    ContainerWidget(nw::Container* container, int columns, QWidget* parent = nullptr);
    virtual ~ContainerWidget();

    nw::Container* container();
    ContainerModel* model();
    ContainerSortFilterProxyModel* proxy();
    QTableView* table();

private:
    Ui::ContainerWidgetUI* ui_;
    std::unique_ptr<ContainerSortFilterProxyModel> proxy_;
    std::unique_ptr<ContainerModel> model_;
    std::unique_ptr<nw::Container> container_;
};
