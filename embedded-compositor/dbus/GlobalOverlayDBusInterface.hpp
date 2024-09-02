// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "DBusInterface.hpp"

#include <QObject>
#include <QString>


class GlobalOverlayDBusInterface : public DBusInterface
{
    Q_OBJECT

public:
    explicit GlobalOverlayDBusInterface(QObject *parent = nullptr);
    virtual ~GlobalOverlayDBusInterface() override = default;

    // DBus
    void Show(const QString &message);
    void Hide();

signals:
    void showRequested(const QString &message);
    void hideRequested();
};
