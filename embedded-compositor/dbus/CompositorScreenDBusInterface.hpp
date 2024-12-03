// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "DBusInterface.hpp"

#include <QObject>
#include <QString>


class CompositorScreenDBusInterface : public DBusInterface
{
    Q_OBJECT

    Q_PROPERTY(QString orientation READ orientation WRITE setOrientation NOTIFY
                   orientationChanged)
    Q_PROPERTY(bool screenSaverActive READ screenSaverActive WRITE
                   setScreenSaverActive NOTIFY screenSaverActiveChanged)
    Q_PROPERTY(bool screenSaverEnabled READ screenSaverEnabled WRITE
                   setScreenSaverEnabled NOTIFY screenSaverEnabledChanged)
    Q_PROPERTY(
        int screenSaverTimeoutSeconds READ screenSaverTimeoutSeconds WRITE
            setScreenSaverTimeoutSeconds NOTIFY screenSaverTimeoutSecondsChanged)
    Q_PROPERTY(bool fullScreen READ fullScreen WRITE setFullScreen
                   NOTIFY fullScreenChanged)

public:
    explicit CompositorScreenDBusInterface(QObject *parent = nullptr);
    virtual ~CompositorScreenDBusInterface() override = default;

    QString orientation() const;
    void setOrientation(const QString &orientation);

    bool screenSaverActive() const;
    void setScreenSaverActive(bool newScreenSaverActive);
    void ShowScreenSaver();

    bool screenSaverEnabled() const;
    void setScreenSaverEnabled(bool newScreenSaverEnabled);

    int screenSaverTimeoutSeconds() const;
    void setScreenSaverTimeoutSeconds(int newScreenSaverTimeoutSeconds);
    bool fullScreen() const;
    void setFullScreen(bool fullScreen);

signals:
    void orientationChanged(const QString &orientation);
    void screenSaverActiveChanged(bool screenSaverActive);
    void showScreenSaver();
    void screenSaverEnabledChanged(bool screenSaverEnabled);
    void screenSaverTimeoutSecondsChanged(int screenSaverTimeoutSeconds);
    void fullScreenChanged(bool fullScreen);

private:
    QString m_orientation = QStringLiteral("0");
    bool m_fullScreen;
    bool m_screenSaverActive;
    bool m_screenSaverEnabled;
    int m_screenSaverTimeoutSeconds;
};
