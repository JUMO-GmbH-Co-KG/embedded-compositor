// SPDX-License-Identifier: LGPL-3.0-only

#ifndef EMBEDDEDSHELL_H
#define EMBEDDEDSHELL_H

#include "embeddedshellsurface.h"
#include <QLoggingCategory>
#include <QObject>
#include <QtWaylandClient/private/qwaylandclientextension_p.h>

Q_DECLARE_LOGGING_CATEGORY(shellExt)

class EmbeddedShellPrivate;
namespace QtWayland {
class embedded_shell;
}

namespace QtWaylandClient {
class QWaylandWindow;
}

class EmbeddedShell {
public:
  explicit EmbeddedShell(QtWayland::embedded_shell *embeddedShell);
  EmbeddedShellSurface *createSurface(QtWaylandClient::QWaylandWindow *window,
                                      EmbeddedShellTypes::Anchor anchor,
                                      uint32_t margin, unsigned int sort_index);

private:
  QtWayland::embedded_shell *m_embeddedShell;
};

#endif // EMBEDDEDSHELL_H
