#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "nw/resources/Resource.hpp"

#include <absl/container/flat_hash_map.h>

#include <QMainWindow>

#include <functional>

class ArclightView;
class PluginInfoDialog;
class AreaListItem;

class QTreeView;

namespace nw {
struct Module;
struct StaticDirectory;
}

namespace Ui {
class MainWindow;
}

using ResourceCallback = std::function<ArclightView*(nw::Resource)>;
using ExtensionCallback = std::function<ArclightView*(const std::string&)>;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadCallbacks();

public slots:
    void onActionOpen(bool checked = false);
    void onProjectDoubleClicked(AreaListItem* item);
    void onTabCloseRequested(int index);
    void onProjectViewChanged(int index);

private:
    Ui::MainWindow *ui;
    nw::Module* module_ = nullptr;
    nw::StaticDirectory* module_container_ = nullptr;
    absl::flat_hash_map<QString, ExtensionCallback> ext_to_view_;
    absl::flat_hash_map<nw::ResourceType::type, ResourceCallback> type_to_view_;
    QList<QTreeView*> project_treeviews_;
};

#endif // MAINWINDOW_H
