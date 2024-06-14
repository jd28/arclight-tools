#ifndef PLACEABLEGENERALVIEW_H
#define PLACEABLEGENERALVIEW_H

#include <QWidget>

namespace nw {
struct Placeable;
struct Resref;
}

namespace Ui {
class PlaceableGeneralView;
}

class PlaceableGeneralView : public QWidget {
    Q_OBJECT

public:
    explicit PlaceableGeneralView(nw::Placeable* obj, QWidget* parent = nullptr);
    ~PlaceableGeneralView();

signals:
    void appearanceChanged();

private slots:
    void onAppearanceChanged(int value);

private:
    Ui::PlaceableGeneralView* ui = nullptr;
    nw::Placeable* obj_ = nullptr;
};

#endif // PLACEABLEGENERALVIEW_H
