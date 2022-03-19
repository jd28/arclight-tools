#ifndef TLKSELECTOR_H
#define TLKSELECTOR_H

#include <QWidget>

namespace Ui {
class TlkSelector;
}

class TlkSelector : public QWidget
{
    Q_OBJECT

public:
    explicit TlkSelector(QWidget *parent = nullptr);
    ~TlkSelector();

private:
    Ui::TlkSelector *ui;
};

#endif // TLKSELECTOR_H
