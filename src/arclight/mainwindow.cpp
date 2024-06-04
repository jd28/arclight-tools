#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "AreaView/areaview.h"
#include "CreatureView/creatureview.h"
#include "DialogView/dialogmodel.h"
#include "DialogView/dialogview.h"
#include "LanguageMenu/LanguageMenu.h"
#include "explorerview.h"
#include "widgets/AbstractTreeModel.hpp"
#include "widgets/ArclightView.h"
#include "widgets/QtWaitingSpinner/waitingspinnerwidget.h"
#include "widgets/arclighttreeview.h"
#include "widgets/arealistview.h"
#include "widgets/projectview.h"

#include "nw/formats/Dialog.hpp"
#include "nw/kernel/Objects.hpp"
#include "nw/kernel/Resources.hpp"
#include "nw/log.hpp"
#include "nw/objects/Creature.hpp"
#include "nw/serialization/Gff.hpp"

#include <QDir>
#include <QFileDialog>
#include <QPluginLoader>
#include <QTreeView>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto menu = new LanguageMenu(this);
    ui->menubar->addMenu(menu);
    menu->setVisible(true);
    spinner_ = new WaitingSpinnerWidget{this};
    spinner_->setColor(Qt::white);

    loadCallbacks();

    ui->projectComboBox->addItem("Project", 0);
    ui->projectComboBox->addItem("Explorer", 1);

    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::onActionClose);
    connect(ui->actionCloseProject, &QAction::triggered, this, &MainWindow::onActionCloseProject);
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
            nw::Gff gff(nw::kernel::resman().demand(res));
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
            nw::Gff gff(nw::kernel::resman().demand(res));
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

    type_to_view_.emplace(nw::ResourceType::are,
        [this](nw::Resource res) -> ArclightView* {
            auto area = nw::kernel::objects().make_area(res.resref);
            area->instantiate();
            auto tv = new AreaView(area, this);
            return tv;
        });
}

// == Slots ===================================================================
// ============================================================================

void MainWindow::onActionClose(bool checked)
{
    Q_UNUSED(checked);
    onTabCloseRequested(ui->tabWidget->currentIndex());
}

void MainWindow::onActionCloseProject(bool checked)
{
    Q_UNUSED(checked);
    while (ui->tabWidget->currentIndex() != -1) {
        onTabCloseRequested(ui->tabWidget->currentIndex());
        // If when closing a tab that's not saved, user decides to cancel close,
        // abort everything else.
        if (close_project_cancelled_) { return; }
    }

    ui->projectComboBox->setCurrentIndex(0);
    ui->projectComboBox->setEnabled(false);
    ui->filter->clear();
    ui->filter->setEnabled(false);

    ui->placeHolder->setVisible(true);
    for (auto widget : project_treeviews_) {
        widget->setVisible(false);
    }

    for (auto widget : project_treeviews_) {
        ui->projectLayout->removeWidget(widget);
        delete widget;
    }
    project_treeviews_.clear();
    nw::kernel::unload_module();
}

void MainWindow::loadTreeviews()
{
    auto result = mod_load_watcher_->result();
    delete mod_load_watcher_;
    module_ = result[0];
    module_container_ = dynamic_cast<nw::StaticDirectory*>(nw::kernel::resman().module_container());
    QFileInfo fi{module_path_};

    ui->placeHolder->setHidden(true);

    auto project_view = new ProjectView(module_container_, this);
    project_view->setHidden(false);
    project_treeviews_.push_back(project_view);
    ui->projectLayout->addWidget(project_view);

    auto explorer_view = new ExplorerView(this);
    explorer_view->setHidden(true);
    project_treeviews_.push_back(explorer_view);
    ui->projectLayout->addWidget(explorer_view);

    treeview_load_watcher_ = new QFutureWatcher<QList<AbstractTreeModel*>>(this);
    connect(treeview_load_watcher_, &QFutureWatcher<QList<AbstractTreeModel*>>::finished, this, &MainWindow::onTreeviewsLoaded);

    treeview_load_future_ = QtConcurrent::run([views = this->project_treeviews_] {
        QList<AbstractTreeModel*> result;
        for (auto it : views) {
            result.push_back(it->loadModel());
        }
        return result;
    });
    treeview_load_watcher_->setFuture(treeview_load_future_);
}

void MainWindow::onActionOpen(bool checked)
{
    Q_UNUSED(checked);

    auto path = QFileDialog::getOpenFileName(this, "Open Project", "", "Module (*.ifo *.ifo.json)");
    if (path.isEmpty()) { return; }

    QFileInfo fi(path);
    module_path_ = fi.absolutePath();

    spinner_->start();
    mod_load_watcher_ = new QFutureWatcher<QList<nw::Module*>>(this);
    connect(mod_load_watcher_, &QFutureWatcher<QList<nw::Module*>>::finished, this, &MainWindow::loadTreeviews);

    mod_load_future_ = QtConcurrent::run([path = module_path_.toStdString()] {
        return QList<nw::Module*>{nw::kernel::load_module(path, false)};
    });
    mod_load_watcher_->setFuture(mod_load_future_);
}

void MainWindow::onProjectDoubleClicked(ProjectItem* item)
{
    if (!item || item->is_folder_) { return; }

    auto it = type_to_view_.find(item->res_.type);
    if (it == std::end(type_to_view_)) { return; }

    auto view = it->second(item->res_);
    auto idx = ui->tabWidget->addTab(view, QString::fromStdString(item->res_.filename()));
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->setCurrentIndex(idx);
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

void MainWindow::onTreeviewsLoaded()
{

    auto result = treeview_load_watcher_->result();
    delete treeview_load_watcher_;

    Q_UNUSED(result);

    for (auto it : project_treeviews_) {
        it->activateModel();
    }

    auto project_view = static_cast<ProjectView*>(project_treeviews_[0]);
    connect(ui->filter, &QLineEdit::textChanged, project_view->proxy_, &ProjectProxyModel::onFilterChanged);
    connect(project_view, &ProjectView::itemDoubleClicked, this, &MainWindow::onProjectDoubleClicked);

    auto explorer_view = static_cast<ExplorerView*>(project_treeviews_[1]);
    connect(ui->filter, &QLineEdit::textChanged, explorer_view->proxy_, &FuzzyProxyModel::onFilterChanged);

    ui->projectComboBox->setEnabled(true);
    ui->projectComboBox->setCurrentIndex(0);
    ui->filter->setEnabled(true);

    spinner_->stop();
}
