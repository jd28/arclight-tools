#ifndef ARCLIGHTVIEW_H
#define ARCLIGHTVIEW_H

#include <QWidget>

/// Arclight View is an abstraction for any widget that is placed in the main window tab.
class ArclightView : public QWidget {
    Q_OBJECT

public:
    ArclightView(QWidget* parent = nullptr);
    virtual ~ArclightView() = default;

    /// Is view read only
    bool read_only() const;

private:
    bool read_only_ = false;
};

#endif // ARCLIGHTVIEW_H
