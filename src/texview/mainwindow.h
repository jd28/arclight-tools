#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void open(const QString& path);
    void restoreWindow();
    void writeSettings();

    void closeEvent(QCloseEvent* event) override;

public slots:
    void onActionAbout();
    void onActionAboutQt();
    void onActionOpen();
    void onActionOpenFolder();

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
