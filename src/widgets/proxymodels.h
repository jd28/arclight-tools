#ifndef PROXYMODELS_H
#define PROXYMODELS_H

#include <QSortFilterProxyModel>

class FuzzyProxyModel : public QSortFilterProxyModel {
public:
    FuzzyProxyModel(QObject* parent = nullptr);

    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

public slots:
    void onFilterChanged(QString filter);

public:
    QString filter_;
};

#endif // PROXYMODELS_H
