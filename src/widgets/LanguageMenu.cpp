#include "LanguageMenu.hpp"

#include <nw/log.hpp>
#include <nw/util/macros.hpp>

LanguageMenu::LanguageMenu(QWidget* parent)
    : QMenu(tr("&Lang"), parent)
    , languageActions{new QActionGroup(parent)}
{
    QAction* act = nullptr;

#define CREATE_LANG_ACTION(lang, id)                                             \
    do {                                                                         \
        act = new QAction(tr(ROLLNW_STRINGIFY(lang)), languageActions);           \
        this->addAction(act);                                                    \
        act->setCheckable(true);                                                 \
        act->setData(static_cast<int>(id));                                      \
        connect(act, &QAction::toggled, this, &LanguageMenu::onLanguageToggled); \
    } while (0)

    CREATE_LANG_ACTION(English, nw::LanguageID::english);
    act->setChecked(true);
    CREATE_LANG_ACTION(French, nw::LanguageID::french);
    CREATE_LANG_ACTION(German, nw::LanguageID::german);
    CREATE_LANG_ACTION(Italian, nw::LanguageID::italian);
    CREATE_LANG_ACTION(Spanish, nw::LanguageID::spanish);
    CREATE_LANG_ACTION(Polish, nw::LanguageID::polish);

#undef CREATE_LANG_ACTION
}

void LanguageMenu::onLanguageToggled(bool toggle)
{
    if (!toggle) { return; }
    auto act = reinterpret_cast<QAction*>(this->sender());
    LOG_F(INFO, "Language changed: {}", act->data().toInt());
    emit languageChanged(static_cast<nw::LanguageID>(act->data().toInt()));
}
