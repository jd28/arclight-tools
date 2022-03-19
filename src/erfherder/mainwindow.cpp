#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "widgets/LanguageMenu.hpp"

#include <nw/log.hpp>
#include <nw/resources/Erf.hpp>

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include <filesystem>
namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    readSettings();

    ui_->setupUi(this);
    // auto la = new LanguageMenu(ui_->menubar);
    // la->setTitle(tr("&Language"));
    // ui_->menubar->insertMenu(ui_->menuView->menuAction(), la);

    for (int i = 0; i < 10; ++i) {
        QAction* act = new QAction(this);
        if (i < recentFiles_.size()) {
            ui_->menuRecent->addAction(act);
            act->setData(recentFiles_[i]);
            act->setText(QString::fromStdString(
                fmt::format("&{} - {}", i + 1, recentFiles_[i].toStdString())));
        } else {
            ui_->menuRecent->addAction(act);
            act->setVisible(false);
        }
        recentActions_.append(act);
        QObject::connect(act, &QAction::triggered, this, &MainWindow::onActionRecent);
    }

    QObject::connect(ui_->actionNew, &QAction::triggered, this, &MainWindow::onActionNew);
    QObject::connect(ui_->actionOpen, &QAction::triggered, this, &MainWindow::onActionOpen);
    QObject::connect(ui_->actionClose, &QAction::triggered, this, &MainWindow::onActionClose);
    QObject::connect(ui_->actionSave, &QAction::triggered, this, &MainWindow::onActionSave);
    QObject::connect(ui_->actionSave_As, &QAction::triggered, this, &MainWindow::onActionSaveAs);

    QObject::connect(ui_->actionImport, &QAction::triggered, this, &MainWindow::onActionImport);
    QObject::connect(ui_->actionExport, &QAction::triggered, this, &MainWindow::onActionExport);
    QObject::connect(ui_->actionExport_All, &QAction::triggered, this, &MainWindow::onActionExportAll);
    QObject::connect(ui_->actionAbout, &QAction::triggered, this, &MainWindow::onActionAbout);
    QObject::connect(ui_->actionAbout_Qt_2, &QAction::triggered, this, &MainWindow::onActionAboutQt);

    QObject::connect(ui_->containerTabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequested);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

ContainerWidget* MainWindow::current()
{
    return reinterpret_cast<ContainerWidget*>(ui_->containerTabWidget->currentWidget());
}

void MainWindow::open(const QString& path)
{
    if (!QFileInfo(path).exists()) { return; }
    std::string p = path.toStdString();
    for (int i = 0; i < ui_->containerTabWidget->count(); ++i) {
        auto cw = reinterpret_cast<ContainerWidget*>(ui_->containerTabWidget->widget(i));
        if (!cw) { continue; }
        if (p == cw->container()->path()) {
            ui_->containerTabWidget->setCurrentIndex(i);
            currentContainer_ = cw;
            return;
        }
    }

    nw::Container* c = new nw::Erf(p);
    currentContainer_ = new ContainerWidget(c, 2);
    int idx = ui_->containerTabWidget->addTab(currentContainer_, QString::fromStdString(c->name()));
    ui_->containerTabWidget->setTabsClosable(true);
    ui_->containerTabWidget->setCurrentIndex(idx);

    if (recentFiles_.contains(path)) {
        recentFiles_.removeOne(path);
    } else if (recentFiles_.size() >= 10) {
        recentFiles_.pop_back();
    }
    recentFiles_.insert(0, path);

    for (int i = 0; i < recentFiles_.size(); ++i) {
        recentActions_[i]->setData(recentFiles_[i]);
        recentActions_[i]->setText(QString::fromStdString(
            fmt::format("&{} - {}", i + 1, recentFiles_[i].toStdString())));
        recentActions_[i]->setVisible(true);
    }

    ui_->actionImport->setEnabled(true);
    ui_->actionMerge->setEnabled(true);
    ui_->actionExport->setEnabled(true);
    ui_->actionExport_All->setEnabled(true);
}

void MainWindow::restoreWindow()
{
    auto geom = settings_.value("Window/geometry");
    if (!geom.isNull()) {
        restoreGeometry(geom.toByteArray());
    }
}

void MainWindow::onActionNew()
{
    nw::Container* c = new nw::Erf();
    currentContainer_ = new ContainerWidget(c, 2);
    int idx = ui_->containerTabWidget->addTab(currentContainer_, "untitled");
    ui_->containerTabWidget->setTabsClosable(true);
    ui_->containerTabWidget->setCurrentIndex(idx);
    ui_->actionImport->setEnabled(true);
    ui_->actionMerge->setEnabled(true);
    ui_->actionExport->setEnabled(true);
    ui_->actionExport_All->setEnabled(true);
}

void MainWindow::onActionOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, "Open Erf", "", "Erf (*.erf *.mod *.hak *.nwm *.sav)");
    if (fn.isEmpty()) { return; }
    open(fn);
}

void MainWindow::onActionRecent()
{
    QAction* act = reinterpret_cast<QAction*>(sender());
    open(act->data().toString());
}

void MainWindow::onActionImport()
{
    auto files = QFileDialog::getOpenFileNames(this, "Import...");
    if (files.isEmpty()) { return; }
    current()->model()->addFiles(files);
    auto name = current()->container()->name();
    ui_->containerTabWidget->setTabText(ui_->containerTabWidget->currentIndex(),
        QString::fromStdString(name.empty() ? "untitled*" : name + "*"));
}

void MainWindow::onActionMerge()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Merge Files...", "", "Erf (*.erf *.mod *.hak *.nwm *.sav)");
    if (files.isEmpty()) { return; }
    current()->model()->mergeFiles(files);
}

void MainWindow::onActionExport()
{
    if (!current()) { return; }
    auto path = QFileDialog::getExistingDirectory(this, "Export To...");
    if (path.isEmpty()) { return; }
    auto select = current()->table()->selectionModel()->selectedRows();
    for (const auto& s : select) {
        auto idx = current()->model()->index(s.row(), 0);
        auto fname = idx.data(Qt::DisplayRole).toString().toStdString();
        current()->container()->extract_by_glob(fname, path.toStdString());
    }
}

void MainWindow::onActionExportAll()
{
    if (!current()) { return; }
    auto path = QFileDialog::getExistingDirectory(this, "Export To...");
    if (path.isEmpty()) { return; }
    auto c = current()->container();
    if (!c) { return; }
    c->extract(std::regex(".*"), path.toStdString());
}

void MainWindow::onActionSave()
{
    if (!current()) { return; }
    if (current()->container()->name().empty()) {
        onActionSaveAs();
    } else if (auto e = dynamic_cast<nw::Erf*>(current()->container())) {
        e->save();
        e->reload();
        ui_->containerTabWidget->setTabText(ui_->containerTabWidget->currentIndex(),
            QString::fromStdString(e->name()));
    }
}

void MainWindow::onActionSaveAs()
{
    if (!current()) { return; }
    QString fn = QFileDialog::getSaveFileName(this, "Save As", "", "Erf (*.erf *.mod *.hak *.nwm *.sav)");
    if (fn.isEmpty()) { return; }
    if (auto e = dynamic_cast<nw::Erf*>(current()->container())) {
        e->save_as(fs::u8path(fn.toStdString()));
        onTabCloseRequested(ui_->containerTabWidget->currentIndex());
        open(fn);
    }
}

void MainWindow::onActionClose()
{
    auto index = ui_->containerTabWidget->currentIndex();
    onTabCloseRequested(index);
}

void MainWindow::onActionAbout()
{
    QMessageBox::about(nullptr, "erfherder",
        R"end(<h3>About erfherder</h3><br>
              A utility for creating/modifying ERF files.<br><br>

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

void MainWindow::onTabCloseRequested(int index)
{
    auto cw = reinterpret_cast<ContainerWidget*>(ui_->containerTabWidget->widget(index));
    ui_->containerTabWidget->removeTab(index);
    delete cw;
    auto w = ui_->containerTabWidget->currentWidget();
    currentContainer_ = reinterpret_cast<ContainerWidget*>(w);
    if (!currentContainer_) {
        ui_->actionImport->setEnabled(false);
        ui_->actionMerge->setEnabled(false);
        ui_->actionExport->setEnabled(false);
        ui_->actionExport_All->setEnabled(false);
    }
}

void MainWindow::readSettings()
{
    int size = settings_.beginReadArray("Recent Files");
    for (int i = 0; i < size; ++i) {
        settings_.setArrayIndex(i);
        recentFiles_.append(settings_.value("file").toString());
    }
    settings_.endArray();
}

void MainWindow::writeSettings()
{
    settings_.beginWriteArray("Recent Files", static_cast<int>(recentFiles_.size()));
    int i = 0;
    for (const auto& f : recentFiles_) {
        settings_.setArrayIndex(i++);
        settings_.setValue("file", f);
    }
    settings_.endArray();
    settings_.setValue("Window/geometry", saveGeometry());
}
