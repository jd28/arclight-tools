#include "creatureappearanceview.h"
#include "ui_creatureappearanceview.h"

#include "nw/kernel/Strings.hpp"
#include "nw/kernel/TwoDACache.hpp"
#include "nw/objects/Creature.hpp"

#include <QAbstractItemView>

CreatureAppearanceView::CreatureAppearanceView(nw::Creature* creature, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::CreatureAppearanceView)
    , creature_{creature}
{
    ui->setupUi(this);

    auto appearance_2da = nw::kernel::twodas().get("appearance");
    if (!appearance_2da) {
        throw std::runtime_error("Unable to load appearance.2da");
    }

    int idx = 0;
    std::string temp;
    for (size_t i = 0; i < appearance_2da->rows(); ++i) {
        int32_t strref;
        if (!appearance_2da->get_to(i, "STRING_REF", strref)) { continue; }
        if (!appearance_2da->get_to(i, "RACE", temp)) { continue; }

        auto string = nw::kernel::strings().get(static_cast<uint32_t>(strref));
        if (string.empty()) { continue; }

        if (temp.size() <= 1) {
            string = fmt::format("(Dynamic) {}", string);
        }

        ui->appearance->addItem(QString::fromStdString(string), int(i));
        if (creature_->appearance.id == i) {
            ui->appearance->setCurrentIndex(idx);
        }
        ++idx;
    }
    ui->appearance->model()->sort(0, Qt::AscendingOrder);

    auto wingmodel_2da = nw::kernel::twodas().get("wingmodel");
    if (!wingmodel_2da) {
        throw std::runtime_error("Unable to load wingmodel.2da");
    }

    ui->wings->addItem("-- None --", 0);
    idx = 1;
    for (size_t i = 1; i < wingmodel_2da->rows(); ++i) {
        if (!wingmodel_2da->get_to(i, "LABEL", temp) || temp.empty()) { continue; }
        ui->wings->addItem(QString::fromStdString(temp), int(i));
        if (creature_->appearance.wings == uint32_t(idx)) {
            ui->wings->setCurrentIndex(idx);
        }
        ++idx;
    }
    ui->wings->model()->sort(0, Qt::AscendingOrder);

    auto tailmodel_2da = nw::kernel::twodas().get("tailmodel");
    if (!tailmodel_2da) {
        throw std::runtime_error("Unable to load tailmodel.2da");
    }

    ui->tails->addItem("-- None --", 0);
    idx = 1;
    for (size_t i = 1; i < tailmodel_2da->rows(); ++i) {
        if (!tailmodel_2da->get_to(i, "LABEL", temp) || temp.empty()) { continue; }
        ui->tails->addItem(QString::fromStdString(temp), int(i));
        if (creature_->appearance.tail == uint32_t(idx)) {
            ui->tails->setCurrentIndex(idx);
        }
        ++idx;
    }
    ui->tails->model()->sort(0, Qt::AscendingOrder);

    ui->appearance->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(ui->appearance, &QComboBox::currentIndexChanged, this, &CreatureAppearanceView::onAppearanceChange);
}

CreatureAppearanceView::~CreatureAppearanceView()
{
    delete ui;
}

// == public slots ============================================================
// ============================================================================

void CreatureAppearanceView::onAppearanceChange(int index)
{
    creature_->appearance.id = static_cast<uint16_t>(ui->appearance->currentData(Qt::UserRole).toInt());
    emit dataChanged();
}
