#include "LanguageMenu.h"

#include <nw/util/macros.hpp>

LanguageMenu::LanguageMenu(nw::LanguageID lang, bool feminine, QWidget* parent)
    : QMenu(tr("&Language"), parent)
    , languageActions{new QActionGroup(parent)}
    , lang_{lang}
    , feminine_{feminine}
{
    createActions();
}

LanguageMenu::LanguageMenu(QWidget* parent)
    : QMenu(tr("&Language"), parent)
    , languageActions{new QActionGroup(parent)}
{
    createActions();
}

void LanguageMenu::createActions()
{
    QAction* act = nullptr;

#define CREATE_LANG_ACTION(lang, id)                                             \
    do {                                                                         \
        act = new QAction(tr(ROLLNW_STRINGIFY(lang)), languageActions);          \
        this->addAction(act);                                                    \
        act->setCheckable(true);                                                 \
        act->setChecked(id == lang_);                                            \
        act->setData(static_cast<int>(id));                                      \
        connect(act, &QAction::toggled, this, &LanguageMenu::onLanguageToggled); \
    } while (0)

    CREATE_LANG_ACTION(English, nw::LanguageID::english);
    CREATE_LANG_ACTION(French, nw::LanguageID::french);
    CREATE_LANG_ACTION(German, nw::LanguageID::german);
    CREATE_LANG_ACTION(Italian, nw::LanguageID::italian);
    CREATE_LANG_ACTION(Spanish, nw::LanguageID::spanish);
    CREATE_LANG_ACTION(Polish, nw::LanguageID::polish);

#undef CREATE_LANG_ACTION

    this->addSeparator();
    actionFeminine_ = new ::QAction("Feminine", this);
    actionFeminine_->setCheckable(true);
    actionFeminine_->setChecked(false);
    this->addAction(actionFeminine_);

    auto lang = static_cast<nw::LanguageID>(languageActions->checkedAction()->data().toInt());
    bool has_feminine = nw::Language::has_feminine(lang);

    actionFeminine_->setEnabled(has_feminine);
    if (!has_feminine && actionFeminine_->isChecked()) {
        actionFeminine_->setChecked(false);
    }

    connect(actionFeminine_, &QAction::toggled, this, &LanguageMenu::onFeminineChecked);
}

void LanguageMenu::onLanguageToggled(bool toggle)
{
    if (!toggle) { return; }
    auto act = reinterpret_cast<QAction*>(this->sender());
    auto lang = static_cast<nw::LanguageID>(act->data().toInt());

    bool has_feminine = nw::Language::has_feminine(lang);
    actionFeminine_->setEnabled(has_feminine);
    if (!has_feminine && actionFeminine_->isChecked()) {
        actionFeminine_->setChecked(false);
    }

    emit languageChanged(lang, feminine_);
}

void LanguageMenu::onFeminineChecked(bool checked)
{
    feminine_ = actionFeminine_->isChecked();
    emit languageChanged(lang_, feminine_);
}
