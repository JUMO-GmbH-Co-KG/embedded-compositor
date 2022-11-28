// SPDX-License-Identifier: LGPL-3.0-only

#ifndef QUICKEMBEDDEDSHELL_H
#define QUICKEMBEDDEDSHELL_H

#include <QQmlExtensionPlugin>

class QuickEmbeddedShell : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};


namespace EmbeddedShell
{
    Q_NAMESPACE         // required for meta object creation
}

#endif // QUICKEMBEDDEDSHELL_H
