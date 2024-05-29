#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H

#include <QWidget>

class ProjectItem;
class ProjectModel;
class FuzzyProxyModel;

namespace nw {
struct Module;
}

namespace Ui {
class ProjectView;
}

class ProjectView : public QWidget {
    Q_OBJECT

public:
    explicit ProjectView(QWidget* parent = nullptr);
    ~ProjectView();

    void load(nw::Module* module, QString path);

public slots:
    void onDoubleClicked(const QModelIndex& index);

signals:
    void doubleClicked(ProjectItem*);

private:
    Ui::ProjectView* ui = nullptr;
    nw::Module* module_ = nullptr;
    QString path_;
    ProjectModel* model_ = nullptr;
    FuzzyProxyModel* filter_ = nullptr;
};

#endif // PROJECTVIEW_H
