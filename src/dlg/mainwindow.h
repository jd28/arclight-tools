#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "nw/i18n/Language.hpp"

#include <QMainWindow>

class FontChooserDialog;
class DialogView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    DialogView* current();
    void open(const QString& path);
    void restoreWindow();
    void readSettings();
    void setModifiedTabName(bool modified);
    void updateRecent(const QString& path);
    void writeSettings();

    void closeEvent(QCloseEvent* event);

public slots:
    void onActionAbout();
    void onActionAboutQt();
    void onActionAdd();
    void onActionClose();
    void onActionCopy();
    void onActionCut();
    void onActionDelete();
    void onActionNew();
    void onActionOpen();
    void onActionLangauge();
    void onActionLangaugeFeminine();
    void onActionPaste();
    void onActionPasteAsLink();
    void onActionRecent();
    void onActionSave();
    void onActionSaveAs();
    void onActionFont();
    void onDialogDataChanged(bool changed);
    void onTabCloseRequested(int index);
    void onFontAccepted();

signals:
    void languageChanged(nw::LanguageID lang, bool feminine);

private:
    Ui::MainWindow* ui;
    QStringList recentFiles_;
    QList<QAction*> recentActions_;
    QColor entry_color_ = Qt::red;
    QColor reply_color_ = Qt::blue;
    QColor link_color_ = Qt::gray;
    QFont font_;
    FontChooserDialog* font_dialog_ = nullptr;
    nw::LanguageID lang_ = nw::LanguageID::english;
    bool feminine_ = false;
};

#endif // MAINWINDOW_H
