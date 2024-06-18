#include "vartabledialog.h"
#include "ui_vartabledialog.h"

VarTableDialog::VarTableDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::VarTableDialog)
{
    ui->setupUi(this);
    setWindowTitle("Variable Table Editor");
}

VarTableDialog::~VarTableDialog()
{
    delete ui;
}

const QList<VarTableItem*>& VarTableDialog::modified_variables() const
{
    return ui->widget->model()->modified_variables();
}

void VarTableDialog::setLocals(nw::LocalData* locals)
{
    ui->widget->setLocals(locals);
}
