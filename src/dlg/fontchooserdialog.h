#ifndef FONTCHOOSERDIALOG_H
#define FONTCHOOSERDIALOG_H

#include <QDialog>

class QDialogButtonBox;

namespace Ui {
class FontChooserDialog;
}

class FontChooserDialog : public QDialog {
    Q_OBJECT

public:
    explicit FontChooserDialog(QWidget* parent = nullptr);
    ~FontChooserDialog();

    QColor entry_color = Qt::red;
    QColor reply_color = Qt::blue;
    QColor link_color = Qt::gray;
    QFont font;

    QDialogButtonBox* buttonBox();
    void update();

public slots:
    void onEntryColorButtonClicked();
    void onFontButtonClicked();
    void onReplyColorButtonClicked();
    void onLinkColorButtonClicked();

private:
    Ui::FontChooserDialog* ui;
};

#endif // FONTCHOOSERDIALOG_H
