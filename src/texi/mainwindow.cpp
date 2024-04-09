#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "texuregallerymodel.h"

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

void MainWindow::open(const QString& path)
{
    if (!QFileInfo::exists(path)) {
        return;
    }
    ui->imageGallery->setModel(new TexureGalleryModel(path));
}

void MainWindow::onActionOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, "Open Container", "", "Container (*.erf *.hak *.key *.zip)");
    if (fn.isEmpty()) {
        return;
    }
    open(fn);
}

MainWindow::~MainWindow()
{
    delete ui;
}
