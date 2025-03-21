// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "DBusInterface.hpp"
#include "TaskSwitcherEntry.hpp"

#include <QObject>
#include <QString>
#include <QList>
#include <QAbstractListModel>


class TaskSwitcherDBusInterface : public DBusInterface
{
    Q_OBJECT

public:
    explicit TaskSwitcherDBusInterface(QObject *parent = nullptr);
    virtual ~TaskSwitcherDBusInterface() override = default;

    Q_PROPERTY(QString currentView READ currentView WRITE setCurrentView NOTIFY
                   currentViewChanged)
    Q_PROPERTY(QList<TaskSwitcherEntry> views READ views NOTIFY viewsChanged)
    Q_PROPERTY(QAbstractListModel *viewModel READ viewModel WRITE setViewModel
                   NOTIFY viewModelChanged)

    // DBus
    void Open();
    void Close();

    QString currentView() const;
    void setCurrentView(QString newCurrentView);

    QList<TaskSwitcherEntry> views() const;

    QAbstractListModel *viewModel() const;
    void setViewModel(QAbstractListModel *newViewModel);

signals:
    void openRequested();
    void closeRequested();
    void currentViewChanged(QString CurrentView);
    void viewsChanged();

    void viewModelChanged(QAbstractListModel *viewModel);

private:
    void onViewsInserted(const QModelIndex &parent, int first, int last);
    void onViewsAboutToBeRemoved(const QModelIndex &parent, int first, int last);

    QString m_currentView;
    QAbstractListModel *m_viewModel = nullptr;
};
