#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <nw/formats/Image.hpp>
#include <nw/resources/Container.hpp>

#include <QImage>
#include <QMainWindow>

#include <string>

struct Payload {
    nw::Image image;
    QImage qimage;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void loadIcons();
    void open(const QString& path);

public slots:
    void onActionOpen();

private:
    Ui::MainWindow* ui;
    std::unique_ptr<nw::Container> container_;
    std::vector<nw::Image> images_;
    std::vector<std::string> labels_;
};

#endif // MAINWINDOW_H
