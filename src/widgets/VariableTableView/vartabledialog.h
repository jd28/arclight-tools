#ifndef VARTABLEDIALOG_H
#define VARTABLEDIALOG_H

#include "variabletableview.h"

#include <QDialog>

namespace nw {
struct LocalData;
}

namespace Ui {
class VarTableDialog;
}

class VarTableDialog : public QDialog {
    Q_OBJECT

public:
    explicit VarTableDialog(QWidget* parent = nullptr);
    ~VarTableDialog();

    const QList<VarTableItem*>& modified_variables() const;
    void setLocals(nw::LocalData* locals);

private:
    Ui::VarTableDialog* ui;
};

#endif // VARTABLEDIALOG_H
