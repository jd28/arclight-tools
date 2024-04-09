#ifndef TEXUREGALLERYMODEL_H
#define TEXUREGALLERYMODEL_H

#include "nw/resources/Container.hpp"

#include "absl/container/flat_hash_map.h"

#include <QAbstractListModel>
#include <QPixmap>

#include <string>

class TexureGalleryModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit TexureGalleryModel(const QString& path, QObject* parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    std::unique_ptr<nw::Container> container_;
    std::vector<nw::Resource> labels_;
    mutable absl::flat_hash_map<nw::Resource, QPixmap> cache_;
};

#endif // TEXUREGALLERYMODEL_H
