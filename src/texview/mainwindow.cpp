#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "nw/log.hpp"

#include "texuregallerymodel.h"

#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QtLogging>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onActionAbout);
    QObject::connect(ui->actionAbout_Qt, &QAction::triggered, this, &MainWindow::onActionAboutQt);
    QObject::connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onActionOpen);
    QObject::connect(ui->actionOpen_Folder, &QAction::triggered, this, &MainWindow::onActionOpenFolder);
}

// -- Methods -----------------------------------------------------------------

void MainWindow::open(const QString& path)
{
    if (!QFileInfo::exists(path)) {
        return;
    }
    ui->imageGallery->setModel(new TexureGalleryModel(path));
}

void MainWindow::restoreWindow()
{
    QSettings settings("jmd", "texview");
    auto geom = settings.value("Window/geometry");
    if (!geom.isNull()) {
        restoreGeometry(geom.toByteArray());
    }
}

// -- Overrides ---------------------------------------------------------------

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    QMainWindow::closeEvent(event);
}

// -- Slots -------------------------------------------------------------------
void MainWindow::onActionAbout()
{
    QMessageBox::about(nullptr, "texview",
        R"end(<h3>About texview</h3><br>
              A texture gallery viewer for NWN textures.<br><br>

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

void MainWindow::onActionOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, "Open Container", "", "Container (*.erf *.hak *.key *.zip)");
    if (fn.isEmpty()) {
        return;
    }
    open(fn);
}

void MainWindow::onActionOpenFolder()
{
    QString fn = QFileDialog::getExistingDirectory(this, "Open Folder");
    if (fn.isEmpty()) {
        return;
    }
    open(fn);
}

void MainWindow::writeSettings()
{
    QSettings settings("jmd", "texview");
    settings.setValue("Window/geometry", saveGeometry());
}

MainWindow::~MainWindow()
{
    delete ui;
}
