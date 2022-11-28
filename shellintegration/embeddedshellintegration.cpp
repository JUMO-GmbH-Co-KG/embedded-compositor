// SPDX-License-Identifier: LGPL-3.0-only

#include "embeddedshellintegration.h"
#include "QtWaylandClient/private/qwaylandwindow_p.h"
#include "embeddedshell.h"
#include "embeddedshellsurface.h"

bool EmbeddedShellIntegration::isActive() const { return m_shell->isActive(); }

EmbeddedShellIntegration::EmbeddedShellIntegration()
    : m_shell(new EmbeddedShell()) {}

QtWaylandClient::QWaylandShellSurface *
EmbeddedShellIntegration::createShellSurface(
    QtWaylandClient::QWaylandWindow *window) {

  EmbeddedShellTypes::Anchor anchor = EmbeddedShellTypes::Anchor::Undefined;

  auto env_anchor = qgetenv("EMBEDDED_SHELL_ANCHOR");
  qDebug() << env_anchor;
  if (env_anchor == "center")
    anchor = EmbeddedShellTypes::Anchor::Center;
  else if (env_anchor == "left")
    anchor = EmbeddedShellTypes::Anchor::Left;
  else if (env_anchor == "right")
    anchor = EmbeddedShellTypes::Anchor::Right;
  else if (env_anchor == "top")
    anchor = EmbeddedShellTypes::Anchor::Top;
  else if (env_anchor == "bottom")
    anchor = EmbeddedShellTypes::Anchor::Bottom;
  else if (!env_anchor.isNull())
    qWarning() << "unexpected value in EMBEDDED_SHELL_ANCHOR:" << env_anchor
               << "allowed values: center,left,right,top,bottom";

  auto prop = window->window()->property("anchor");
  if (prop.isValid())
    anchor = prop.value<EmbeddedShellTypes::Anchor>();

  uint32_t margin = 0;

  auto env_margin = qgetenv("EMBEDDED_SHELL_MARGIN");
  if (!env_margin.isNull()) {
    bool ok = false;
    margin = env_margin.toUInt(&ok);
    if (!ok) {
      qWarning() << "failed to read margin from EMBEDDED_SHELL_MARGIN:"
                 << env_margin << "is not an unsigned integer";
    }
  }

  prop = window->window()->property("margin");
  if (prop.isValid())
    margin = prop.toUInt();

  int32_t sort_index = 0;

  auto env_sort_index = qgetenv("EMBEDDED_SHELL_SORT_INDEX");
  if (!env_sort_index.isNull()) {
    bool ok = false;
    sort_index = env_sort_index.toInt(&ok);
    if (!ok) {
      qWarning() << "failed to read sort index from EMBEDDED_SORT_INDEX:"
                 << env_sort_index << "is not an integer";
    }
  }

  prop = window->window()->property("sortIndex");
  if (prop.isValid())
    sort_index = prop.toInt();

  auto ess = m_shell->createSurface(window, anchor, margin, sort_index);

  if (ess == nullptr) {
    return nullptr;
  }

  m_windows.insert(window, ess);
  emit EmbeddedPlatform::instance()->shellSurfaceCreated(ess, window->window());
  return ess->shellSurface();
}

bool EmbeddedShellIntegration::initialize(
    QtWaylandClient::QWaylandDisplay *display) {
  QWaylandShellIntegration::initialize(display);
  return m_shell->isActive();
}

void *
EmbeddedShellIntegration::nativeResourceForWindow(const QByteArray &resource,
                                                  QWindow *window) {
  if (resource == "embedded-shell-surface") {
    auto qww = static_cast<QtWaylandClient::QWaylandWindow *>(window->handle());
    auto found = m_windows.find(qww);
    if (found != m_windows.end()) {
      return found.value();
    }
    qDebug() << __PRETTY_FUNCTION__ << " ... not found";
    return nullptr;
  }
  return QWaylandShellIntegration::nativeResourceForWindow(resource, window);
}
