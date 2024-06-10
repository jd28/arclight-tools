#ifndef STRREFLINEEDIT_H
#define STRREFLINEEDIT_H

#include "nw/i18n/LocString.hpp"

#include <QLineEdit>

class StrrefLineEdit : public QLineEdit {
    Q_OBJECT
public:
    StrrefLineEdit(nw::LocString value, QWidget* parent = nullptr);
    StrrefLineEdit(QWidget* parent = nullptr);
    void setLocString(nw::LocString value);

private slots:
    void onActionTriggered();

signals:
    void editClicked(nw::LocString);

private:
    nw::LocString value_;
};

#endif // STRREFLINEEDIT_H
