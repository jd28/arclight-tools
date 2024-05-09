#include "ArclightView.h"

ArclightView::ArclightView(ArclightPluginInterface* plugin, QWidget* parent)
    : QWidget(parent)
    , plugin_{plugin}
{
}

ArclightPluginInterface* ArclightView::plugin() const
{
    return plugin_;
}
