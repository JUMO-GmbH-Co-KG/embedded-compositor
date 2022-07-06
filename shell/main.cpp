#include <QtWaylandClient/private/qwaylandshellintegrationplugin_p.h>
#include "embeddedshellintegration.h"
#include "qwayland-embedded-shell.h"
#include <QCoreApplication>

using namespace QtWaylandClient;
class EmbeddedShellIntegrationPlugin: public QWaylandShellIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QWaylandShellIntegrationFactoryInterface_iid FILE "embedded-shell.json")

public:
    QWaylandShellIntegration *create(const QString &key, const QStringList &paramList) override;
};


QWaylandShellIntegration *EmbeddedShellIntegrationPlugin::create(const QString &key, const QStringList &paramList)
{
    auto v = new EmbeddedShellIntegration();
    qDebug()<<__PRETTY_FUNCTION__<<key<<paramList<<v;
    // there is a queued connection in QWaylandClientExtension
    // that will not fire unless we process the event loop
    while(!v->isActive()) {
        QCoreApplication::processEvents();
    }
    Q_UNUSED(key)
    Q_UNUSED(paramList)
    return v;
}

#include "main.moc"
