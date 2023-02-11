#pragma once

#include <nw/legacy/Tlk.hpp>

#include <QAbstractTableModel>

class TlkModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit TlkModel(nw::Tlk* tlk, QObject* parent = nullptr);

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    // Qt::ItemFlags flags(const QModelIndex& index) const override;
    // bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
    // Qt::DropActions supportedDropActions() const override;
    // QMimeData* mimeData(const QModelIndexList& indexes) const override;
    // bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
    // virtual QStringList mimeTypes() const override;

private:
    nw::Tlk* tlk_ = nullptr;
};
