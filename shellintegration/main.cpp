// SPDX-License-Identifier: LGPL-3.0-only

#include "embeddedshellintegration.h"
#include <QCoreApplication>
#include <QtWaylandClient/private/qwaylandshellintegrationplugin_p.h>

class EmbeddedShellIntegrationPlugin
    : public QtWaylandClient::QWaylandShellIntegrationPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QWaylandShellIntegrationFactoryInterface_iid FILE
                    "embedded-shell.json")

public:
  QtWaylandClient::QWaylandShellIntegration *
  create(const QString &key, const QStringList &paramList) override;
};

QtWaylandClient::QWaylandShellIntegration *
EmbeddedShellIntegrationPlugin::create(const QString &key,
                                       const QStringList &paramList) {
  auto v = new EmbeddedShellIntegration();
  // there is a queued connection in QWaylandClientExtension
  // that will not fire unless we process the event loop
  while (!v->isActive()) {
    QCoreApplication::processEvents();
  }
  Q_UNUSED(key)
  Q_UNUSED(paramList)
  return v;
}

#include "main.moc"
