#pragma once

#include <nw/i18n/Language.hpp>

#include <QActionGroup>
#include <QMenu>

class LanguageMenu : public QMenu {
    Q_OBJECT
public:
    LanguageMenu(QWidget* parent = nullptr);

signals:
    void languageChanged(nw::LanguageID newLanguage);

private slots:
    void onLanguageToggled(bool toggled);

private:
    QActionGroup* languageActions = nullptr;
};
