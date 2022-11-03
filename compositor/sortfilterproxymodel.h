#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class SortFilterProxyModel : public QSortFilterProxyModel {
  Q_OBJECT
  Q_PROPERTY(QAbstractItemModel *sourceModel READ sourceModel WRITE
                 setSourceModel NOTIFY sourceModelChanged)

  Q_PROPERTY(QString sortFunction READ sortFunction WRITE setSortFunction NOTIFY
                 sortFunctionChanged)
  Q_PROPERTY(QString acceptFunction READ acceptFunction WRITE setAcceptFunction
                 NOTIFY acceptFunctionChanged)
  Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
  SortFilterProxyModel();
  void setSourceModel(QAbstractItemModel *newSourceModel) override;
  const QString &sortFunction() const;
  void setSortFunction(const QString &newSortFunction);

  const QString &acceptFunction() const;
  void setAcceptFunction(const QString &newAcceptFunction);

  int count() const;
  bool hasChildren(const QModelIndex &) const override { return false; }
  QModelIndex parent(const QModelIndex &) const override {
    return QModelIndex();
  }
  QHash<int, QByteArray> roleNames() const override;
signals:
  void sourceModelChanged(QAbstractItemModel *sourceModel);
  void sortFunctionChanged(const QString &sortFunction);
  void acceptFunctionChanged(const QString &acceptFunction);
  void countChanged();

protected:
  bool lessThan(const QModelIndex &left,
                const QModelIndex &right) const override;

  bool filterAcceptsRow(int sourceRow,
                        const QModelIndex &sourceParent) const override;

private slots:
  void initRoles();

private:
  QString m_sortFunction;
  QString m_acceptFunction;
};

#endif // SORTFILTERPROXYMODEL_H
