#include "mainwindow.h"
#include "DialogView/dialogmodel.h"
#include "ui_mainwindow.h"

#include "AreaView/areaview.h"
#include "CreatureView/creatureview.h"
#include "DialogView/dialogview.h"
#include "LanguageMenu/LanguageMenu.h"
#include "widgets/ArclightView.h"
#include "widgets/projectmodel.h"

#include "nw/formats/Dialog.hpp"
#include "nw/kernel/Resources.hpp"
#include "nw/log.hpp"
#include "nw/objects/Creature.hpp"
#include "nw/serialization/Gff.hpp"

#include <QDir>
#include <QFileDialog>
#include <QPluginLoader>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto menu = new LanguageMenu(this);
    ui->menubar->addMenu(menu);
    menu->setVisible(true);

    loadCallbacks();

    connect(ui->project, &ProjectView::doubleClicked, this, &MainWindow::onProjectDoubleClicked);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onActionOpen);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadCallbacks()
{
    type_to_view_.emplace(nw::ResourceType::utc,
        [this](nw::Resource res) -> ArclightView* {
            auto data = nw::kernel::resman().demand(res);
            nw::Gff gff(std::move(data));
            if (!gff.valid()) {
                LOG_F(ERROR, "[utc] failed to open file: {}", res.filename());
                return nullptr;
            }
            auto utc = new nw::Creature();
            nw::deserialize(utc, gff.toplevel(), nw::SerializationProfile::blueprint);
            return new CreatureView(utc, this);
        });

    type_to_view_.emplace(nw::ResourceType::dlg,
        [this](nw::Resource res) -> ArclightView* {
            auto data = nw::kernel::resman().demand(res);
            nw::Gff gff(std::move(data));
            if (!gff.valid()) {
                LOG_F(ERROR, "[dlg] failed to open file: {}", res.filename());
                return nullptr;
            }
            auto dlg = new nw::Dialog(gff.toplevel());
            if (!dlg->valid()) {
                LOG_F(ERROR, "[dlg] invalid dialog: {}", res.filename());
                delete dlg;
                return nullptr;
            }

            auto tv = new DialogView(dlg, this);
            tv->setFont(QApplication::font());
            tv->selectFirst();

            return tv;
        });
}

// == Slots ===================================================================
// ============================================================================

void MainWindow::onActionOpen(bool checked)
{
    auto path = QFileDialog::getOpenFileName(this, "Open Project", "", "Module (*.ifo *.ifo.json)");
    if (path.isEmpty()) { return; }
    QFileInfo fi(path);
    module_ = nw::kernel::load_module(fi.absolutePath().toStdString());
    ui->project->load(module_, fi.absolutePath());
}

void MainWindow::onProjectDoubleClicked(ProjectItem* item)
{
    if (!item) { return; }

    if (item->type_ == ProjectItemType::area) {
        auto av = new AreaView(item->area_, ui->tabWidget);
        auto idx = ui->tabWidget->addTab(av, item->data(0).toString());
        ui->tabWidget->setTabsClosable(true);
        ui->tabWidget->setCurrentIndex(idx);
        av->load_model();
    } else if (item->type_ == ProjectItemType::dialog) {
        auto data = nw::kernel::resman().demand(item->res_);
        if (data.bytes.size() == 0) { return; }
        nw::Gff gff{std::move(data)};
        if (!gff.valid()) { return; }

        auto dlg = new nw::Dialog(gff.toplevel());
        if (!dlg->valid()) {
            delete dlg;
            return;
        }

        auto av = new DialogView(dlg, ui->tabWidget);
        auto idx = ui->tabWidget->addTab(av, item->data(0).toString());
        ui->tabWidget->setTabsClosable(true);
        ui->tabWidget->setCurrentIndex(idx);
        av->setFocus();
    }
}
