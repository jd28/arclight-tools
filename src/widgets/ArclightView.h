#ifndef ARCLIGHTVIEW_H
#define ARCLIGHTVIEW_H

#include <QWidget>

class ArclightPluginInterface;

/// Arclight View is an abstraction for any widget that is placed in the main window tab.
class ArclightView : public QWidget {
    Q_OBJECT

public:
    ArclightView(ArclightPluginInterface* plugin, QWidget* parent = nullptr);
    ArclightPluginInterface* plugin() const;

    /// Is view read only
    bool read_only() const;

private:
    ArclightPluginInterface* plugin_ = nullptr;
    bool read_only_ = false;
};

#endif // ARCLIGHTVIEW_H
