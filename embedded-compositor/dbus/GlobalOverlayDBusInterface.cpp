// SPDX-License-Identifier: GPL-3.0-only

#include "GlobalOverlayDBusInterface.hpp"
#include "globaloverlay_adaptor.h"


GlobalOverlayDBusInterface::GlobalOverlayDBusInterface(QObject *parent)
    : DBusInterface(QStringLiteral("/globaloverlay"), parent)
{
    new GlobaloverlayAdaptor(this);
}

void GlobalOverlayDBusInterface::Show(const QString &message)
{
    emit showRequested(message);
}

void GlobalOverlayDBusInterface::Hide()
{
    emit hideRequested();
}
