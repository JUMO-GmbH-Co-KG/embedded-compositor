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
    if (m_currentView == newCurrentView)
        return;

    qDebug() << "new current view" << newCurrentView;
    m_currentView = newCurrentView;
    emit currentViewChanged(m_currentView);
}

QList<TaskSwitcherEntry> TaskSwitcherDBusInterface::views() const
{
    QList<TaskSwitcherEntry> entries;

    if (m_viewModel) {
        entries.reserve(m_viewModel->rowCount());

        for (auto i = 0; i < m_viewModel->rowCount(); i++) {
            const auto &data = m_viewModel->index(i).data().toMap();

            auto *view = data.value(QStringLiteral("view")).value<EmbeddedShellSurfaceView *>();

            if (view) {
              entries.append({
                  view->uuid(),
                  view->parentUuid(),
                  view->label(),
                  view->icon(),
                  view->sortIndex(),
              });
            }
        }
    }

    return entries;
}

QDBusArgument &operator<<(QDBusArgument &argument, const TaskSwitcherEntry &entry)
{
    argument.beginStructure();
    argument << entry.uuid << entry.parentUuid << entry.label << entry.icon << entry.sortIndex;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, TaskSwitcherEntry &entry)
{
    argument.beginStructure();
    argument >> entry.uuid >> entry.parentUuid >> entry.label >> entry.icon >> entry.sortIndex;
    argument.endStructure();
    return argument;
}

QAbstractListModel *TaskSwitcherDBusInterface::viewModel() const
{
    return m_viewModel;
}

void TaskSwitcherDBusInterface::setViewModel(QAbstractListModel *newViewModel)
{
    if (m_viewModel == newViewModel) {
        return;
    }

    if (m_viewModel) {
        disconnect(m_viewModel, nullptr, this, nullptr);
    }

    m_viewModel = newViewModel;

    if (m_viewModel) {
        connect(m_viewModel, &QAbstractItemModel::rowsInserted, this, &TaskSwitcherDBusInterface::onViewsInserted);
        connect(m_viewModel, &QAbstractItemModel::rowsInserted, this, &TaskSwitcherDBusInterface::viewsChanged);
        connect(m_viewModel, &QAbstractItemModel::rowsRemoved, this, &TaskSwitcherDBusInterface::viewsChanged);
    }

    emit viewModelChanged(m_viewModel);

    emit viewsChanged();
}

void TaskSwitcherDBusInterface::onViewsInserted(const QModelIndex &parent, int first, int last)
{
    Q_ASSERT(!parent.isValid());

    for (int i = first; i <= last; ++i) {
        const auto &data = m_viewModel->index(i).data().toMap();

        if (auto *view = data.value(QStringLiteral("view")).value<EmbeddedShellSurfaceView *>()) {
            connect(view, &EmbeddedShellSurfaceView::labelChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
            connect(view, &EmbeddedShellSurfaceView::iconChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
            connect(view, &EmbeddedShellSurfaceView::sortIndexChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
            connect(view, &EmbeddedShellSurfaceView::customDataChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
        }
    }
}
