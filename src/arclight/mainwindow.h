#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "nw/resources/Resource.hpp"

#include <absl/container/flat_hash_map.h>

#include <QMainWindow>

#include <functional>

class ArclightView;
class PluginInfoDialog;
class ProjectItem;

namespace nw {
struct Module;
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
    void onProjectDoubleClicked(ProjectItem* item);

private:
    Ui::MainWindow *ui;
    nw::Module* module_ = nullptr;
    absl::flat_hash_map<QString, ExtensionCallback> ext_to_view_;
    absl::flat_hash_map<nw::ResourceType::type, ResourceCallback> type_to_view_;
};

#endif // MAINWINDOW_H
