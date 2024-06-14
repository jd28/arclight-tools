#ifndef PLACEABLEPROPERTIES_H
#define PLACEABLEPROPERTIES_H

#include "../propertiesview.h"

#include <QMap>
#include <QWidget>

#include <functional>

namespace nw {
struct Placeable;
}

class QCompleter;
class QtProperty;
class QtIntPropertyManager;
class QtStringPropertyManager;

class PlaceableProperties : public PropertiesView {
    Q_OBJECT
public:
    explicit PlaceableProperties(QWidget* parent = nullptr);
    void setObject(nw::Placeable* obj);

public slots:
    void onPropertyChanged(QtProperty* prop);

private:
    QtProperty* lock_locked_prop_ = nullptr;
    QtProperty* lock_lockable_prop_ = nullptr;
    QtProperty* lock_remove_key_prop_ = nullptr;
    QtProperty* lock_key_required_prop_ = nullptr;
    QtProperty* lock_key_name_prop_ = nullptr;
    QtProperty* lock_lock_dc_prop_ = nullptr;
    QtProperty* lock_unlock_dc_prop_ = nullptr;

    QtProperty* trap_type_ = nullptr;
    QtProperty* trap_is_trapped_ = nullptr;
    QtProperty* trap_detectable_ = nullptr;
    QtProperty* trap_detect_dc_ = nullptr;
    QtProperty* trap_disarmable_ = nullptr;
    QtProperty* trap_disarm_dc_ = nullptr;
    QtProperty* trap_one_shot_ = nullptr;

    nw::Placeable* obj_ = nullptr;
    QCompleter* script_completer_ = nullptr;
    QMap<QtProperty*, std::function<void(QtProperty*)>> prop_func_map_;

    void basicsLoad();
    void conversationLoad();
    void locksLoad();
    void locksUpdate();
    void savesLoad();
    void scriptsLoad();
    void trapsLoad();
    void trapsUpdate();
};

#endif // PLACEABLEPROPERTIES_H
