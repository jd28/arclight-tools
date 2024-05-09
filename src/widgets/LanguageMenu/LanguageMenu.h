#pragma once

#include <nw/i18n/Language.hpp>

#include <QActionGroup>
#include <QMenu>

class LanguageMenu : public QMenu {
    Q_OBJECT
public:
    LanguageMenu(nw::LanguageID lang, bool feminine = false, QWidget* parent = nullptr);
    LanguageMenu(QWidget* parent = nullptr);

signals:
    void languageChanged(nw::LanguageID language, bool feminine);

private slots:
    void onLanguageToggled(bool toggled);
    void onFeminineChecked(bool checked);

private:
    void createActions();

    QActionGroup* languageActions = nullptr;
    QAction* actionFeminine_ = nullptr;
    nw::LanguageID lang_ = nw::LanguageID::english;
    bool feminine_ = false;
};
