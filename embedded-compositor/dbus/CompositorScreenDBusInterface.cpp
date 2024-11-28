// SPDX-License-Identifier: GPL-3.0-only

#include "CompositorScreenDBusInterface.hpp"
#include "screen_adaptor.h"


CompositorScreenDBusInterface::CompositorScreenDBusInterface(QObject *parent)
    : DBusInterface(QStringLiteral("/screen"), parent)
{
    new ScreenAdaptor(this);
}

QString CompositorScreenDBusInterface::orientation() const
{
    return m_orientation;
}

void CompositorScreenDBusInterface::setOrientation(const QString &orientation)
{
    if (m_orientation != orientation)
    {
        m_orientation = orientation;
        emit orientationChanged(orientation);
    }
}

bool CompositorScreenDBusInterface::screenSaverActive() const
{
    return m_screenSaverActive;
}

void CompositorScreenDBusInterface::setScreenSaverActive(
    bool newScreenSaverActive)
{
    if (m_screenSaverActive == newScreenSaverActive)
        return;
    m_screenSaverActive = newScreenSaverActive;
    emit screenSaverActiveChanged(m_screenSaverActive);
}

void CompositorScreenDBusInterface::ShowScreenSaver()
{
    emit showScreenSaver();
}

bool CompositorScreenDBusInterface::screenSaverEnabled() const
{
    return m_screenSaverEnabled;
}

void CompositorScreenDBusInterface::setScreenSaverEnabled(
    bool newScreenSaverEnabled)
{
    if (m_screenSaverEnabled == newScreenSaverEnabled)
        return;
    m_screenSaverEnabled = newScreenSaverEnabled;
    emit screenSaverEnabledChanged(m_screenSaverEnabled);
}

int CompositorScreenDBusInterface::screenSaverTimeoutSeconds() const
{
    return m_screenSaverTimeoutSeconds;
}

void CompositorScreenDBusInterface::setScreenSaverTimeoutSeconds(
    int newScreenSaverTimeoutSeconds)
{
    if (m_screenSaverTimeoutSeconds == newScreenSaverTimeoutSeconds)
        return;
    m_screenSaverTimeoutSeconds = newScreenSaverTimeoutSeconds;
    emit screenSaverTimeoutSecondsChanged(m_screenSaverTimeoutSeconds);
}

void CompositorScreenDBusInterface::setFullScreen(bool value)
{
    if (m_fullScreen == value)
        return;
    m_fullScreen = value;
    emit fullScreenChanged(m_fullScreen);
}

bool CompositorScreenDBusInterface::fullScreen() const
{
    return m_fullScreen;
}
