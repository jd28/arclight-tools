#include "tlkselector.h"
#include "ui_tlkselector.h"

TlkSelector::TlkSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TlkSelector)
{
    ui->setupUi(this);
}

TlkSelector::~TlkSelector()
{
    delete ui;
}
