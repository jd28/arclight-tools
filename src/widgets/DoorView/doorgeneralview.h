#ifndef DOORGENERALVIEW_H
#define DOORGENERALVIEW_H

#include <QWidget>

namespace nw {
struct Door;
struct Resref;
}

namespace Ui {
class DoorGeneralView;
}

class DoorGeneralView : public QWidget {
    Q_OBJECT

public:
    explicit DoorGeneralView(nw::Door* obj, QWidget* parent = nullptr);
    ~DoorGeneralView();

signals:
    void appearanceChanged();

private slots:
    void onAppearanceChanged(int value);
    void onGenericChanged(int value);
    void onVariablesClicked();

private:
    Ui::DoorGeneralView* ui = nullptr;
    nw::Door* obj_ = nullptr;
};

#endif // DOORGENERALVIEW_H
