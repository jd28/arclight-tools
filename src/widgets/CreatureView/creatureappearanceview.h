#ifndef CREATUREAPPEARANCEVIEW_H
#define CREATUREAPPEARANCEVIEW_H

#include <QWidget>

namespace nw {
struct Creature;
}

namespace Ui {
class CreatureAppearanceView;
}

class CreatureAppearanceView : public QWidget {
    Q_OBJECT

public:
    explicit CreatureAppearanceView(nw::Creature* creature, QWidget* parent = nullptr);
    ~CreatureAppearanceView();

public slots:
    void onAppearanceChange(int index);

signals:
    emit void dataChanged();

private:
    Ui::CreatureAppearanceView* ui;
    nw::Creature* creature_ = nullptr;
};

#endif // CREATUREAPPEARANCEVIEW_H
