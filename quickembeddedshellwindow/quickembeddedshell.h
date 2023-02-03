// SPDX-License-Identifier: LGPL-3.0-only

#ifndef QUICKEMBEDDEDSHELL_H
#define QUICKEMBEDDEDSHELL_H

#include <QQmlExtensionPlugin>

class QuickEmbeddedShell : public QQmlExtensionPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
  void registerTypes(const char *uri) override;
};

#endif // QUICKEMBEDDEDSHELL_H
