#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <nw/formats/Image.hpp>
#include <nw/resources/Erf.hpp>

#include <QFileDialog>
#include <QImage>
#include <QtLogging>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onActionOpen);
}

void MainWindow::loadIcons()
{
    if (!container_) {
        return;
    }

    auto cb = [this](const nw::Resource& resource) {
        if (resource.type == nw::ResourceType::plt) {
            return;
        }
        if (nw::ResourceType::check_category(nw::ResourceType::texture, resource.type)) {
            images_.emplace_back(container_->demand(resource));
            labels_.push_back(resource.filename());
        }
    };

    container_->visit(cb);

    for (size_t i = 0; i < images_.size(); ++i) {
        if (!images_[i].valid()) {
            continue;
        }
        QImage qi(images_[i].data(), images_[i].width(), images_[i].height(),
            images_[i].channels() == 4 ? QImage::Format_RGBA8888 : QImage::Format_RGB888);

        ui->imageGallery->addItem(new QListWidgetItem(QIcon(QPixmap::fromImage(qi)),
            QString::fromStdString(labels_[i])));
    }
}

void MainWindow::open(const QString& path)
{
    if (!QFileInfo::exists(path)) {
        return;
    }
    std::string p = path.toStdString();

    ui->imageGallery->clear();
    container_ = std::make_unique<nw::Erf>(p);
    loadIcons();
}

void MainWindow::onActionOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, "Open Erf", "", "Erf (*.erf *.mod *.hak *.nwm *.sav)");
    if (fn.isEmpty()) {
        return;
    }
    open(fn);
}

MainWindow::~MainWindow()
{
    delete ui;
}
