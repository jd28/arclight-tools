#ifndef STRREFTEXTEDIT_H
#define STRREFTEXTEDIT_H

#include "nw/i18n/LocString.hpp"

#include <QWidget>

namespace Ui {
class StrrefTextEdit;
}

class StrrefTextEdit : public QWidget {
    Q_OBJECT

public:
    explicit StrrefTextEdit(QWidget* parent = nullptr);
    ~StrrefTextEdit();

    const nw::LocString& locstring() const;
    void setLocstring(const nw::LocString& newLocstring);
    void updateTextEdit();

public slots:
    void onFeminineToggled(bool value);
    void onLanguageChanged(int index);
    void onStrrefChanged(int value);
    void onStrrefShowToggled(bool value);
    void onTextEditChanged();

private:
    Ui::StrrefTextEdit* ui;
    nw::LocString locstring_;
    nw::LanguageID lang_ = nw::LanguageID::english;
};

#endif // STRREFTEXTEDIT_H
