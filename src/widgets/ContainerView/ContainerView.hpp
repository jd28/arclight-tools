#pragma once

#include "ContainerModel.hpp"

#include <nw/resources/Container.hpp>

#include <QTableView>
#include <QWidget>

namespace Ui {
class ContainerViewUI;
}

class ContainerView : public QWidget {
public:
    ContainerView(nw::Container* container, int columns, QWidget* parent = nullptr);
    virtual ~ContainerView();

    nw::Container* container();
    ContainerModel* model();
    ContainerSortFilterProxyModel* proxy();
    QTableView* table();

private:
    Ui::ContainerViewUI* ui_;
    std::unique_ptr<ContainerSortFilterProxyModel> proxy_;
    std::unique_ptr<ContainerModel> model_;
    std::unique_ptr<nw::Container> container_;
};
