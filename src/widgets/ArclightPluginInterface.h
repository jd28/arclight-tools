#pragma once

#include "nw/resources/ResourceType.hpp"

#include <QList>
#include <QtPlugin>

class ArclightView;

class QMenu;
class QString;
class QWidget;

namespace nw {
struct Resource;
}

class ArclightPluginInterface {
public:
    virtual ~ArclightPluginInterface() = default;

    /// Gets the extensions supported by they plugin.
    virtual QList<QString> extensions() const = 0;

    /// Loads some entity by path
    virtual ArclightView* load_by_path(const QString& path) = 0;

    /// Loads some entity by resource, most likely from resman
    virtual ArclightView* load_by_resource(const nw::Resource& path) = 0;

    /// Gets plugins custom menus
    virtual QList<QMenu*> menus() const = 0;

    /// Gets plugin name
    virtual QString name() const = 0;

    /// Gets the resource types this plugin is able to handle.
    virtual QList<nw::ResourceType> resource_types() const = 0;

    /// Sets the current view
    virtual void set_current(ArclightView* view) = 0;
};

// [TODO] Change this.
#define ArclightPluginInterface_iid "com.github.jd28.ArclightPluginInterface"
Q_DECLARE_INTERFACE(ArclightPluginInterface, ArclightPluginInterface_iid);
