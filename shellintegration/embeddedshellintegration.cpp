// SPDX-License-Identifier: LGPL-3.0-only

#include "embeddedshellintegration.h"
#include "QtWaylandClient/private/qwaylandwindow_p.h"
#include "embeddedshell.h"
#include "embeddedshellsurface.h"
#include "embeddedshellsurfaceview.h"
#include "embeddedplatform.h"

EmbeddedShellIntegration::EmbeddedShellIntegration()
  : QtWaylandClient::QWaylandShellIntegrationTemplate<EmbeddedShellIntegration>(/*version*/ 1)
{
  connect(this, &QWaylandShellIntegrationTemplate::activeChanged, this, [this]
  {
    if (isActive())
    {
      m_shell.reset(new EmbeddedShell(this));
    }
    else
    {
      m_shell.reset();
    }
  });
}

QtWaylandClient::QWaylandShellSurface *
EmbeddedShellIntegration::createShellSurface(QtWaylandClient::QWaylandWindow *window)
{
  EmbeddedShellTypes::Anchor anchor = EmbeddedShellTypes::Anchor::Undefined;

  auto env_anchor = qgetenv("EMBEDDED_SHELL_ANCHOR");
  qDebug() << env_anchor;
  if (env_anchor == "center")
  {
    anchor = EmbeddedShellTypes::Anchor::Center;
  }
  else if (env_anchor == "left")
  {
    anchor = EmbeddedShellTypes::Anchor::Left;
  }
  else if (env_anchor == "right")
  {
    anchor = EmbeddedShellTypes::Anchor::Right;
  }
  else if (env_anchor == "top")
  {
    anchor = EmbeddedShellTypes::Anchor::Top;
  }
  else if (env_anchor == "bottom")
  {
    anchor = EmbeddedShellTypes::Anchor::Bottom;
  }
  else if (!env_anchor.isNull())
  {
    qWarning() << "unexpected value in EMBEDDED_SHELL_ANCHOR:" << env_anchor
               << "allowed values: center,left,right,top,bottom";
  }

  auto anchorProperty = window->window()->property("anchor");
  if (anchorProperty.isValid())
  {
    anchor = anchorProperty.value<EmbeddedShellTypes::Anchor>();
  }

  uint32_t margin = 0;

  auto env_margin = qgetenv("EMBEDDED_SHELL_MARGIN");
  if (!env_margin.isNull())
  {
    bool ok = false;
    margin = env_margin.toUInt(&ok);
    if (!ok)
    {
      qWarning() << "failed to read margin from EMBEDDED_SHELL_MARGIN:"
                 << env_margin << "is not an unsigned integer";
    }
  }

  anchorProperty = window->window()->property("margin");
  if (anchorProperty.isValid())
  {
    margin = anchorProperty.toUInt();
  }

  auto shellSurface = m_shell->createSurface(window, anchor, margin);

  if (shellSurface == nullptr)
  {
    return nullptr;
  }

  createDefaultView(shellSurface);

  m_windows.insert(window, shellSurface);
  emit EmbeddedPlatform::instance()->shellSurfaceCreated(shellSurface, window->window());
  return shellSurface->shellSurface();
}

void *
EmbeddedShellIntegration::nativeResourceForWindow(const QByteArray &resource, QWindow *window)
{
  if (resource == "embedded-shell-surface")
  {
    auto qww = static_cast<QtWaylandClient::QWaylandWindow *>(window->handle());
    auto found = m_windows.find(qww);
    if (found != m_windows.end())
    {
      return found.value();
    }

    qDebug() << Q_FUNC_INFO << " ... not found";
    return nullptr;
  }
  return QWaylandShellIntegration::nativeResourceForWindow(resource, window);
}

void EmbeddedShellIntegration::createDefaultView(EmbeddedShellSurface *shellSurface)
{
  const auto appLabel = qEnvironmentVariable("EMBEDDED_SHELL_APP_LABEL");
  const auto appIcon = qEnvironmentVariable("EMBEDDED_SHELL_APP_ICON");

  if (!appLabel.isEmpty() && !appIcon.isEmpty())
  {
    uint32_t sortIndex = 0;

    if (qEnvironmentVariableIsSet("EMBEDDED_SHELL_SORT_INDEX"))
    {
      bool ok = false;
      const auto sortIndexString = qgetenv("EMBEDDED_SHELL_SORT_INDEX");
      sortIndex = sortIndexString.toUInt(&ok);
      if (!ok)
      {
        qWarning() << "failed to read sort index from EMBEDDED_SHELL_SORT_INDEX:"
                   << sortIndexString << "is not an integer";
      }
    }

    const auto appId = qEnvironmentVariable("EMBEDDED_SHELL_APP_ID");

    auto view = shellSurface->createView(appLabel, appIcon, sortIndex, appId);

    connect(shellSurface, &EmbeddedShellSurface::viewCreated, view, [view](EmbeddedShellSurfaceView *manualView)
    {
      qDebug() << "Delete default view" << view << "upon first manual view creation of" << manualView;
      view->deleteLater();
    }, Qt::SingleShotConnection);

    qDebug() << "Create default view" << view << "based on environment variables" << appLabel << appIcon << sortIndex << appId;
  }
}
