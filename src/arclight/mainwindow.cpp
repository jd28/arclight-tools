#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "widgets/AbstractTreeModel.hpp"
#include "widgets/ArclightView.h"
#include "widgets/AreaView/areaview.h"
#include "widgets/CreatureView/creatureview.h"
#include "widgets/DialogView/dialogmodel.h"
#include "widgets/DialogView/dialogview.h"
#include "widgets/PlaceableView/placeableview.h"
#include "widgets/QtWaitingSpinner/waitingspinnerwidget.h"
#include "widgets/arclighttreeview.h"
#include "widgets/arealistview.h"
#include "widgets/explorerview.h"
#include "widgets/projectview.h"

#include "nw/formats/Dialog.hpp"
#include "nw/kernel/Objects.hpp"
#include "nw/kernel/Resources.hpp"
#include "nw/log.hpp"
#include "nw/objects/Creature.hpp"
#include "nw/serialization/Gff.hpp"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QPluginLoader>
#include <QTreeView>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    readSettings();

    ui->setupUi(this);
    spinner_ = new WaitingSpinnerWidget{this};
    spinner_->setColor(Qt::white);

    loadCallbacks();

    ui->projectComboBox->addItem("Project", 0);
    ui->projectComboBox->addItem("Explorer", 1);

    for (int i = 0; i < 10; ++i) {
        QAction* act = new QAction(this);
        if (i < recentProjects_.size()) {
            ui->menuRecentProjects->addAction(act);
            act->setData(recentProjects_[i]);
            act->setText(QString::fromStdString(
                fmt::format("&{} - {}", i + 1, recentProjects_[i].toStdString())));
        } else {
            ui->menuRecentProjects->addAction(act);
            act->setVisible(false);
        }
        recentActions_.append(act);
        connect(act, &QAction::triggered, this, &MainWindow::onActionRecent);
    }

    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onActionAbout);
    connect(ui->actionAboutQt, &QAction::triggered, this, &MainWindow::onActionAboutQt);
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

// == Methods =================================================================
// ============================================================================

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    QMainWindow::closeEvent(event);
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

    type_to_view_.emplace(nw::ResourceType::utp,
        [this](nw::Resource res) -> ArclightView* {
            nw::Gff gff(nw::kernel::resman().demand(res));
            if (!gff.valid()) {
                LOG_F(ERROR, "[utc] failed to open file: {}", res.filename());
                return nullptr;
            }
            auto obj = new nw::Placeable();
            nw::deserialize(obj, gff.toplevel(), nw::SerializationProfile::blueprint);
            return new PlaceableView(obj, this);
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

void MainWindow::loadTreeviews()
{
    auto result = mod_load_watcher_->result();
    delete mod_load_watcher_;
    module_ = result[0];
    module_container_ = dynamic_cast<nw::StaticDirectory*>(nw::kernel::resman().module_container());

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

void MainWindow::open(const QString& path)
{
    QFileInfo fi(path);
    module_path_ = fi.absolutePath();
    auto abspath = fi.absoluteFilePath();

    if (recentProjects_.contains(abspath)) {
        recentProjects_.removeOne(abspath);
    } else if (recentProjects_.size() >= 10) {
        recentProjects_.pop_back();
    }
    recentProjects_.insert(0, abspath);

    for (int i = 0; i < recentProjects_.size(); ++i) {
        recentActions_[i]->setData(recentProjects_[i]);
        recentActions_[i]->setText(QString::fromStdString(
            fmt::format("&{} - {}", i + 1, recentProjects_[i].toStdString())));
        recentActions_[i]->setVisible(true);
    }

    spinner_->start();
    mod_load_watcher_ = new QFutureWatcher<QList<nw::Module*>>(this);
    connect(mod_load_watcher_, &QFutureWatcher<QList<nw::Module*>>::finished, this, &MainWindow::loadTreeviews);

    mod_load_future_ = QtConcurrent::run([path = module_path_.toStdString()] {
        return QList<nw::Module*>{nw::kernel::load_module(path, false)};
    });
    mod_load_watcher_->setFuture(mod_load_future_);
}

void MainWindow::readSettings()
{
    QSettings settings("jmd", "arclight");
    int size = settings.beginReadArray("Recent Projects");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        recentProjects_.append(settings.value("file").toString());
    }
    settings.endArray();
}

void MainWindow::writeSettings()
{
    QSettings settings("jmd", "arclight");
    settings.beginWriteArray("Recent Projects", static_cast<int>(recentProjects_.size()));
    int i = 0;
    for (const auto& f : recentProjects_) {
        settings.setArrayIndex(i++);
        settings.setValue("file", f);
    }
    settings.endArray();
}

// == Slots ===================================================================
// ============================================================================

void MainWindow::onActionAbout()
{
    QMessageBox::about(nullptr, "Arclight",
        R"end(<h3>About Arclight</h3><br>
              A utility for working with Nevewinter Nights modules<br><br>

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.)end");
}

void MainWindow::onActionAboutQt()
{
    QMessageBox::aboutQt(this);
}

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

void MainWindow::onActionOpen(bool checked)
{
    Q_UNUSED(checked);

    auto path = QFileDialog::getOpenFileName(this, "Open Project", "", "Module (*.ifo *.ifo.json)");
    if (path.isEmpty()) { return; }
    open(path);
}

void MainWindow::onActionRecent()
{
    QAction* act = reinterpret_cast<QAction*>(sender());
    open(act->data().toString());
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
