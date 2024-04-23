#include "fontchooserdialog.h"
#include "ui_fontchooserdialog.h"

#include <QColorDialog>
#include <QFontDialog>

FontChooserDialog::FontChooserDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::FontChooserDialog)
{
    ui->setupUi(this);
    update();

    connect(ui->fontButton, &QPushButton::clicked, this, &FontChooserDialog::onFontButtonClicked);
    connect(ui->entryColorButton, &QPushButton::clicked, this, &FontChooserDialog::onEntryColorButtonClicked);
    connect(ui->replyColorButton, &QPushButton::clicked, this, &FontChooserDialog::onReplyColorButtonClicked);
    connect(ui->linkColorButton, &QPushButton::clicked, this, &FontChooserDialog::onLinkColorButtonClicked);
}

QDialogButtonBox* FontChooserDialog::buttonBox()
{
    return ui->buttonBox;
}

void FontChooserDialog::update()
{
    ui->fontFamily->setText(font.family());
    ui->fontSize->setValue(font.pointSize());

    QString qss = QString("background-color: %1").arg(entry_color.name());
    ui->entryColorButton->setStyleSheet(qss);
    qss = QString("background-color: %1").arg(reply_color.name());
    ui->replyColorButton->setStyleSheet(qss);
    qss = QString("background-color: %1").arg(link_color.name());
    ui->linkColorButton->setStyleSheet(qss);
}

void FontChooserDialog::onFontButtonClicked()
{
    bool ok = false;
    QFont f = QFontDialog::getFont(&ok, font);

    if (ok) {
        font = f;
        ui->fontFamily->setText(font.family());
        ui->fontSize->setValue(font.pointSize());
    }
}

void FontChooserDialog::onEntryColorButtonClicked()
{
    QColor col = QColorDialog::getColor(entry_color);
    if (col.isValid()) {
        entry_color = col;
        QString qss = QString("background-color: %1").arg(entry_color.name());
        ui->entryColorButton->setStyleSheet(qss);
    }
}

void FontChooserDialog::onReplyColorButtonClicked()
{
    QColor col = QColorDialog::getColor(reply_color);
    if (col.isValid()) {
        reply_color = col;
        QString qss = QString("background-color: %1").arg(reply_color.name());
        ui->replyColorButton->setStyleSheet(qss);
    }
}

void FontChooserDialog::onLinkColorButtonClicked()
{
    QColor col = QColorDialog::getColor(link_color);
    if (col.isValid()) {
        link_color = col;
        QString qss = QString("background-color: %1").arg(link_color.name());
        ui->linkColorButton->setStyleSheet(qss);
    }
}

FontChooserDialog::~FontChooserDialog()
{
    delete ui;
}
