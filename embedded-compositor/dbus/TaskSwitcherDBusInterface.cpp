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

QList<TaskSwitcherEntry> TaskSwitcherDBusInterface::views() const
{
    QList<TaskSwitcherEntry> entries;

    if (m_viewModel) {
        entries.reserve(m_viewModel->rowCount());

        for (auto i = 0; i < m_viewModel->rowCount(); i++) {
            const auto &data = m_viewModel->index(i).data().toMap();

            auto *surface = data.value(QStringLiteral("surface")).value<EmbeddedShellSurface *>();
            auto *view = data.value(QStringLiteral("view")).value<EmbeddedShellSurfaceView *>();

            entries.append({
                view ? view->getUuid() : surface->getUuid(),
                view ? view->appId() : QString(),
                view ? view->appLabel() : QString(),
                view ? view->appIcon() : QString(),
                view ? view->label() : QStringLiteral("surface"),
                view ? view->icon() : QString(),
                uint32_t(surface->getClientPid()),
                view ? uint32_t(view->sortIndex()) : 0,
                QVariantMap(), //args
            });
        }
    }

    return entries;
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
    if (m_viewModel == newViewModel) {
        return;
    }

    if (m_viewModel) {
        disconnect(m_viewModel, nullptr, this, nullptr);
    }

    m_viewModel = newViewModel;

    if (m_viewModel) {
        connect(m_viewModel, &QAbstractItemModel::rowsInserted, this,
                &TaskSwitcherDBusInterface::onViewsInserted);
        connect(m_viewModel, &QAbstractItemModel::rowsAboutToBeRemoved, this,
                &TaskSwitcherDBusInterface::onViewsAboutToBeRemoved);
    }

    Q_EMIT viewModelChanged(m_viewModel);

    Q_EMIT viewsChanged();
}

void TaskSwitcherDBusInterface::onViewsInserted(const QModelIndex &parent, int first, int last)
{
    Q_ASSERT(!parent.isValid());

    for (int i = first; i <= last; ++i) {
        const auto &data = m_viewModel->index(i).data().toMap();

        if (auto *view = data.value(QStringLiteral("view")).value<EmbeddedShellSurfaceView *>()) {
            connect(view, &EmbeddedShellSurfaceView::appIdChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
            connect(view, &EmbeddedShellSurfaceView::appLabelChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
            connect(view, &EmbeddedShellSurfaceView::appIconChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
            connect(view, &EmbeddedShellSurfaceView::labelChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
            connect(view, &EmbeddedShellSurfaceView::iconChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
        }
    }
}

void TaskSwitcherDBusInterface::onViewsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    Q_ASSERT(!parent.isValid());

    for (int i = first; i <= last; ++i) {
        const auto &data = m_viewModel->index(i).data().toMap();

        if (auto *view = data.value(QStringLiteral("view")).value<EmbeddedShellSurfaceView *>()) {
            disconnect(view, &EmbeddedShellSurfaceView::appIdChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
            disconnect(view, &EmbeddedShellSurfaceView::appLabelChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
            disconnect(view, &EmbeddedShellSurfaceView::appIconChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
            disconnect(view, &EmbeddedShellSurfaceView::labelChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
            disconnect(view, &EmbeddedShellSurfaceView::iconChanged, this, &TaskSwitcherDBusInterface::viewsChanged);
        }
    }
}
