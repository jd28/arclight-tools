#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void onActionAbout();
    void onActionAboutQt();
    void onActionOpen();
    void onActionOpenFolder();

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H