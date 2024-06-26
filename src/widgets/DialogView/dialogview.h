#ifndef DIALOGVIEW_H
#define DIALOGVIEW_H

#include "../ArclightView.h"

#include "nw/i18n/Language.hpp"

#include <QAbstractItemModel>
#include <QWidget>

struct DialogItem;
class DialogModel;
class ArclightPluginInterface;

class QAudioOutput;
class QCompleter;
class QItemSelection;
class QMediaPlayer;
class QMenu;

namespace Ui {
class DialogView;
}

namespace nw {
struct Dialog;
struct DialogPtr;
}

class DialogView : public ArclightView {
    Q_OBJECT

public:
    explicit DialogView(nw::Dialog* dialog, QWidget* parent = nullptr);
    explicit DialogView(QString path, QWidget* parent = nullptr);
    ~DialogView();

    void loadItem(DialogItem* item);
    bool modified() const;
    QString name() const;
    const QString& path() const { return path_; }
    void selectFirst();
    void setColors(QColor entry, QColor reply, QColor link);
    void setFont(const QFont& font);
    void setModel(DialogModel* model);
    void setModified(bool modified);

    void setupUi();

public slots:
    void onAbortScriptChanged(const QString& value);
    void onActionAnimationChanged(int index);
    void onActionParamCellChanged(int row, int col);
    void onActionParamAddClicked();
    void onActionParamDelClicked();
    void onActionSoundChanged(const QString& value);
    void onActionSoundClicked();
    void onActionScriptChanged(const QString& value);
    void onCommentTextChanged();
    void onConditionParamCellChanged(int row, int col);
    void onConditionParamAddClicked();
    void onConditionParamDelClicked();
    void onConditionScriptChanged(const QString& value);
    void onCustomContextMenu(const QPoint& point);
    void onDialogAddNode();
    void onDialogCopyNode();
    void onDialogCutNode();
    void onDialogDeleteNode();
    void onDialogPasteNode();
    void onDialogPasteLinkNode();
    void onDialogSave();
    void onDialogSaveAs();
    void onDialogTextChanged();
    void onEndScriptChanged(const QString& value);
    void onLanguageChanged(nw::LanguageID lang, bool feminine);
    void onNoZoomChanged(int state);
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void onSpeakersListAddClicked();
    void onSpeakerTableCellChanged(int row, int col);
    void onSpeakersListDelClicked();

signals:
    void dataChanged(bool changed);

private:
    Ui::DialogView* ui;
    QString path_;
    DialogModel* model_ = nullptr;
    DialogItem* current_item_ = nullptr;
    nw::DialogPtr* last_copy_or_cut_ = nullptr;
    bool last_edit_was_cut_ = false;
    QCompleter* script_completer_ = nullptr;
    QCompleter* sound_completer_ = nullptr;
    QMenu* context_menu_ = nullptr;
    QFont font_;
    QColor entry_;
    QColor reply_;
    QColor link_;
    QModelIndex context_index_;
    QAction* add_ctx_action_ = nullptr;
    QAction* copy_ctx_action_ = nullptr;
    QAction* cut_ctx_action_ = nullptr;
    QAction* paste_ctx_action_ = nullptr;
    QAction* paste_link_ctx_action_ = nullptr;
    QAction* delete_ctx_action_ = nullptr;
    nw::LanguageID lang_ = nw::LanguageID::english;
    bool feminine_ = false;
    QMediaPlayer* player_ = nullptr;
    QAudioOutput* output_ = nullptr;
    bool modified_ = false;
};

#endif // DIALOGVIEW_H
