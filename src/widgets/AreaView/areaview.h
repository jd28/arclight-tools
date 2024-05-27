#ifndef AREAVIEW_H
#define AREAVIEW_H

#include "../ArclightView.h"

class BasicTileArea;

namespace nw {
struct Area;
}

namespace Ui {
class AreaView;
}

class AreaView : public ArclightView {
    Q_OBJECT

public:
    void load_model();
    explicit AreaView(nw::Area* area, QWidget* parent = nullptr);
    ~AreaView();

private:
    Ui::AreaView* ui;
    nw::Area* area_ = nullptr;
    BasicTileArea* area_model_;
};

#endif // AREAVIEW_H
