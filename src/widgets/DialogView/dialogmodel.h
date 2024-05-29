#ifndef DIALOGMODEL_H
#define DIALOGMODEL_H

#include "../AbstractTreeModel.hpp"

#include "nw/i18n/Language.hpp"

#include <QColor>
#include <QFont>

namespace nw {

struct Dialog;
struct DialogPtr;

} // namespace nw

class DialogModel;

struct DialogItem : public AbstractTreeItem {
    DialogItem(nw::DialogPtr* ptr, int row, DialogModel* model, AbstractTreeItem* parent = nullptr);

    QVariant data(int column, int role = Qt::DisplayRole) const override;

    nw::DialogPtr* ptr_ = nullptr;
    DialogModel* model_ = nullptr;
};

class DialogModel : public AbstractTreeModel {
    Q_OBJECT

public:
    explicit DialogModel(nw::Dialog* dialog, QObject* parent = nullptr);
    ~DialogModel();

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    nw::Dialog* dialog();
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    DialogItem* index_to_node(QModelIndex index);
    DialogItem* index_to_node(QModelIndex index) const;
    void loadRootItems() override;
    QMimeData* mimeData(const QModelIndexList& indexes) const override;
    QStringList mimeTypes() const override;
    void setColors(QColor entry, QColor reply, QColor link);
    void setFont(const QFont& font);
    void setLanguage(nw::LanguageID lang, bool feminine);
    Qt::DropActions supportedDropActions() const override;
    Qt::DropActions supportedDragActions() const override;

    nw::LanguageID lang_ = nw::LanguageID::english;
    bool feminine_ = false;

private:
    std::unique_ptr<nw::Dialog> dialog_;
    QFont font_;
    QColor entry_ = Qt::blue;
    QColor reply_ = Qt::green;
    QColor link_ = Qt::gray;
};

#endif // DIALOGMODEL_H
