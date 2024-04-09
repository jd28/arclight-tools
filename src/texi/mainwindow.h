#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <nw/formats/Image.hpp>
#include <nw/resources/Container.hpp>

#include <QImage>
#include <QMainWindow>

#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void open(const QString& path);

public slots:
    void onActionOpen();

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
