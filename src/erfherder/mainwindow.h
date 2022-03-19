#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "widgets/ContainerWidget.hpp"

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
    ContainerWidget* current();
    void restoreWindow();

public slots:
    void onActionNew();
    void onActionOpen();
    void onActionRecent();
    void onActionSave();
    void onActionSaveAs();
    void onActionClose();

    void onActionImport();
    void onActionMerge();
    void onActionExport();
    void onActionExportAll();
    void onTabCloseRequested(int index);

    void onActionAbout();
    void onActionAboutQt();

    void writeSettings();

private:
    Ui::MainWindow* ui_;
    ContainerWidget* currentContainer_;
    QSettings settings_;
    QStringList recentFiles_;
    QList<QAction*> recentActions_;

    void readSettings();
};

#endif // MAINWINDOW_H
