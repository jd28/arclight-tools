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
    void readSettings();
    void writeSettings();

    void closeEvent(QCloseEvent* event) override;

public slots:
    void onActionAbout();
    void onActionAboutQt();
    void onActionExit();
    void onActionOpen();
    void onActionOpenFolder();
    void onActionRecent();

private:
    Ui::MainWindow* ui;

    QStringList recentFiles_;
    QList<QAction*> recentActions_;
};

#endif // MAINWINDOW_H
