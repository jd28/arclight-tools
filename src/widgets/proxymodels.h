#ifndef PROXYMODELS_H
#define PROXYMODELS_H

#include <QSortFilterProxyModel>

class FuzzyProxyModel : public QSortFilterProxyModel {
public:
    FuzzyProxyModel(QObject* parent = nullptr);

    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

public slots:
    void onFilterChanged(QString filter);

private:
    QString filter_;
    bool filter_empty_ = true;
};

#endif // PROXYMODELS_H
