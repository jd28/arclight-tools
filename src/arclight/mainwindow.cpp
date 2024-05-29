#include "mainwindow.h"
#include "DialogView/dialogmodel.h"
#include "ui_mainwindow.h"

#include "AreaView/areaview.h"
#include "CreatureView/creatureview.h"
#include "DialogView/dialogview.h"
#include "LanguageMenu/LanguageMenu.h"
#include "widgets/ArclightView.h"
#include "widgets/filesystemview.h"
#include "widgets/projectview.h"

#include "nw/formats/Dialog.hpp"
#include "nw/kernel/Resources.hpp"
#include "nw/log.hpp"
#include "nw/objects/Creature.hpp"
#include "nw/serialization/Gff.hpp"

#include <QDir>
#include <QFileDialog>
#include <QPluginLoader>
#include <QTreeView>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto menu = new LanguageMenu(this);
    ui->menubar->addMenu(menu);
    menu->setVisible(true);

    loadCallbacks();

    ui->projectComboBox->addItem("Project", 0);
    ui->projectComboBox->addItem("File System", 1);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onActionOpen);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequested);
    connect(ui->projectComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onProjectViewChanged);
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

    ui->placeHolder->setHidden(true);

    auto project_view = new ProjectView(this);
    module_ = nw::kernel::load_module(fi.absolutePath().toStdString());
    project_view->load(module_, fi.absolutePath());

    project_treeviews_.push_back(project_view);
    ui->projectLayout->addWidget(project_view);
    connect(project_view, &ProjectView::doubleClicked, this, &MainWindow::onProjectDoubleClicked);
    connect(ui->filter, &QLineEdit::textChanged, project_view->filter_, &FuzzyProxyModel::onFilterChanged);

    auto filesystem_view = new FileSystemView(fi.absolutePath(), this);
    filesystem_view->setHidden(true);
    project_treeviews_.push_back(filesystem_view);
    ui->projectLayout->addWidget(filesystem_view);
    connect(ui->filter, &QLineEdit::textChanged, filesystem_view->proxy_, &FuzzyProxyModel::onFilterChanged);

    ui->projectComboBox->setEnabled(true);
    ui->projectComboBox->setCurrentIndex(0);
    ui->filter->setEnabled(true);
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
        auto rd = nw::kernel::resman().demand(item->res_);
        if (rd.bytes.size() == 0) { return; }
        nw::Gff gff{std::move(rd)};
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

void MainWindow::onProjectViewChanged(int index)
{
    auto variant = ui->projectComboBox->itemData(index);
    if (variant.isNull()) { return; }
    int view = variant.toInt();
    for (int i = 0; i < project_treeviews_.size(); ++i) {
        project_treeviews_[i]->setHidden(view != i);
    }
    ui->filter->clear();
}

void MainWindow::onTabCloseRequested(int index)
{
    auto cw = reinterpret_cast<ArclightView*>(ui->tabWidget->widget(index));
    ui->tabWidget->removeTab(index);
    delete cw;
}
