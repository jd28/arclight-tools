#include "TlkView.hpp"
#include "ui_TlkView.h"

TlkView::TlkView(nw::Tlk* tlk, QWidget* parent)
    : QFrame(parent)
    , ui_{std::make_unique<Ui::TlkView>()}
    , tlk_{tlk}
    , model_{new TlkModel(tlk, this)}
{
    ui_->setupUi(this);
    ui_->tableView->setModel(model_);
}

TlkView::~TlkView()
{
}
