#pragma once

#include <nw/resources/Container.hpp>

#include <QAbstractTableModel>
#include <QDropEvent>
#include <QSortFilterProxyModel>
#include <QString>
#include <QTimer>

#include <filesystem>

class ContainerSortFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit ContainerSortFilterProxyModel(QObject* parent);

public slots:
    void onFilterUpdated(const QString& filter);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    std::string filter_;
    bool filter_empty_ = true;
};

class ContainerModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit ContainerModel(nw::Container* container, QObject* parent = nullptr);

    void addFile(const nw::Resource& res, const std::filesystem::path& file);
    void addFile(const nw::Resource& res, const nw::ResourceData& bytes);
    void addFiles(const QStringList& files);
    void mergeFiles(const QStringList& files);
    void setColumnCount(int cols);

    // QAbstractTableModel overrides
    virtual bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QMimeData* mimeData(const QModelIndexList& indexes) const override;
    virtual QStringList mimeTypes() const override;
    virtual bool removeRows(int row, int count, const QModelIndex& index) override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual Qt::DropActions supportedDropActions() const override;

private:
    nw::Container* container_ = nullptr;
    std::vector<nw::ResourceDescriptor> resources_;
    QString path_;
    int cols_ = 1;
};
