// SPDX-License-Identifier: GPL-3.0-only

#include "embeddedshellextension.h"
#include "TaskSwitcherDBusInterface.hpp"
#include "taskswitcher_adaptor.h"


TaskSwitcherDBusInterface::TaskSwitcherDBusInterface(QObject *parent)
    : DBusInterface(QStringLiteral("/taskswitcher"), parent)
{
    new TaskswitcherAdaptor(this);
}

void TaskSwitcherDBusInterface::Open()
{
    emit openRequested();
}

void TaskSwitcherDBusInterface::Close()
{
    emit closeRequested();
}

QString TaskSwitcherDBusInterface::currentView() const
{
    return m_currentView;
}

void TaskSwitcherDBusInterface::setCurrentView(QString newCurrentView)
{
    qDebug() << "new current view " << newCurrentView;
    if (m_currentView == newCurrentView)
        return;
    m_currentView = newCurrentView;
    emit currentViewChanged(m_currentView);
}

const QList<TaskSwitcherEntry> &TaskSwitcherDBusInterface::views() const
{
    return m_views;
}

QDBusArgument &operator<<(QDBusArgument &argument,
                          const TaskSwitcherEntry &entry)
{
    argument.beginStructure();
    argument << entry.uuid << entry.appId << entry.appLabel << entry.appIcon << entry.label << entry.icon << entry.pid << entry.sortIndex << entry.args;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument,
                                TaskSwitcherEntry &entry)
{
    argument.beginStructure();
    argument >> entry.uuid >> entry.appId >> entry.appLabel >> entry.appIcon >> entry.label >> entry.icon >> entry.pid >> entry.sortIndex >> entry.args;
    argument.endStructure();
    return argument;
}

QAbstractListModel *TaskSwitcherDBusInterface::viewModel() const
{
    return m_viewModel;
}

void TaskSwitcherDBusInterface::setViewModel(QAbstractListModel *newViewModel)
{
    if (m_viewModel == newViewModel)
        return;
    connect(newViewModel, &QAbstractItemModel::rowsInserted, this,
            &TaskSwitcherDBusInterface::publishViews);
    connect(newViewModel, &QAbstractItemModel::rowsRemoved, this,
            &TaskSwitcherDBusInterface::publishViews);
    m_viewModel = newViewModel;
    emit viewModelChanged(m_viewModel);
}

void TaskSwitcherDBusInterface::publishViews()
{
    QList<TaskSwitcherEntry> list;
    for (auto i = 0; i < m_viewModel->rowCount(); i++)
    {
        auto d = m_viewModel->index(i).data().toMap();
        auto surface = d["surface"].value<EmbeddedShellSurface *>();
        auto view = d["view"].value<EmbeddedShellSurfaceView *>();

        list.append({
            view ? view->getUuid() : surface->getUuid(),
            view ? view->appId() : QString(),
            view ? view->appLabel() : QString(),
            view ? view->appIcon() : QString(),
            view ? view->label() : "surface",
            view ? view->icon() : QString(),
            uint32_t(surface->getClientPid()),
            view ? uint32_t(view->sortIndex()) : 0,
            QVariantMap(), //args
        });
    }
    m_views = list;
    emit viewsChanged(m_views);
}
