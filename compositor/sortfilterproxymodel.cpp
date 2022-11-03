#include "sortfilterproxymodel.h"
#include <QDebug>

SortFilterProxyModel::SortFilterProxyModel() {
  setDynamicSortFilter(true);
  QSortFilterProxyModel::sort(0);
  connect(this, &QSortFilterProxyModel::rowsInserted, this,
          &SortFilterProxyModel::countChanged);
  connect(this, &QSortFilterProxyModel::rowsRemoved, this,
          &SortFilterProxyModel::countChanged);
  connect(this, &QSortFilterProxyModel::rowsInserted,
          [=](QModelIndex p, int f, int l) {
            auto index = this->index(f, 0, p);
            qDebug() << "inserted!" << f << l << data(index);
          });
}

void SortFilterProxyModel::setSourceModel(QAbstractItemModel *newSourceModel) {
  if (sourceModel() == newSourceModel)
    return;
  QSortFilterProxyModel::setSourceModel(newSourceModel);
  connect(newSourceModel, &QAbstractItemModel::rowsInserted,
          [=](QModelIndex i, int f, int l) {
            auto index = sourceModel()->index(f, 0, i);
            qDebug() << "source inserted!" << f << l
                     << sourceModel()->data(index);
          });

  if (newSourceModel && newSourceModel->roleNames().isEmpty()) {
    // QTBUG-57971
    connect(newSourceModel, &QAbstractItemModel::rowsInserted, this,
            &SortFilterProxyModel::initRoles);
  }

  invalidate();
  emit sourceModelChanged(newSourceModel);
}

void SortFilterProxyModel::initRoles() {
  disconnect(sourceModel(), &QAbstractItemModel::rowsInserted, this,
             &SortFilterProxyModel::initRoles);
  resetInternalData();
}

QHash<int, QByteArray> SortFilterProxyModel::roleNames() const {
  return sourceModel() != nullptr ? sourceModel()->roleNames()
                                  : QHash<int, QByteArray>();
}

const QString &SortFilterProxyModel::sortFunction() const {
  return m_sortFunction;
}

void SortFilterProxyModel::setSortFunction(const QString &newSortFunction) {
  if (m_sortFunction == newSortFunction)
    return;
  m_sortFunction = newSortFunction;
  invalidate();
  emit sortFunctionChanged(m_sortFunction);
}

const QString &SortFilterProxyModel::acceptFunction() const {
  return m_acceptFunction;
}

void SortFilterProxyModel::setAcceptFunction(const QString &newAcceptFunction) {
  if (m_acceptFunction == newAcceptFunction)
    return;
  m_acceptFunction = newAcceptFunction;
  invalidate();
  emit acceptFunctionChanged(m_acceptFunction);
}

int SortFilterProxyModel::count() const { return rowCount(); }

bool SortFilterProxyModel::lessThan(const QModelIndex &left,
                                    const QModelIndex &right) const {
  QVariant leftData = sourceModel()->data(left);
  QVariant rightData = sourceModel()->data(right);
  QVariant returnedValue;
  QMetaObject::invokeMethod(const_cast<SortFilterProxyModel *>(this),
                            m_sortFunction.toUtf8().constData(),
                            Q_RETURN_ARG(QVariant, returnedValue),
                            Q_ARG(QVariant, leftData),
                            Q_ARG(QVariant, rightData));
  return returnedValue.toBool();
}

bool SortFilterProxyModel::filterAcceptsRow(
    int sourceRow, const QModelIndex &sourceParent) const {

  if (m_acceptFunction.isEmpty()) {
    return true;
  }

  QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
  QObject *item = sourceModel()->data(index0).value<QObject *>();
  bool accepts = false;
  QVariant returnedValue;
  QMetaObject::invokeMethod(const_cast<SortFilterProxyModel *>(this),
                            m_acceptFunction.toUtf8().constData(),
                            Q_RETURN_ARG(QVariant, returnedValue),
                            Q_ARG(QVariant, QVariant::fromValue(item)));
  accepts = returnedValue.toBool();
  return accepts;
}
