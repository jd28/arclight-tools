#pragma once

#include "TlkModel.hpp"

#include <nw/i18n/Tlk.hpp>

#include <QFrame>

#include <memory>

namespace Ui {
class TlkView;
}

class TlkView : public QFrame {
    Q_OBJECT

public:
    explicit TlkView(nw::Tlk* tlk, QWidget* parent = nullptr);
    ~TlkView();

private:
    std::unique_ptr<Ui::TlkView> ui_;
    nw::Tlk* tlk_ = nullptr;
    TlkModel* model_ = nullptr;
};
