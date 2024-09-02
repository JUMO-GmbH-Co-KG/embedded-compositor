// SPDX-License-Identifier: GPL-3.0-only

/*!
 * @file   ScreenShotDBusInterface.hpp
 *
 * @class  ScreenShotDBusInterface
 *
 * @brief  This class takes a screenshot of the current QML view.
 *         The image is stored locally with the current timestamp in the filename.
 *         The image format is PNG.
 *
 * @date   10.07.24
 *
 * @author Christian Happ
 *
*/

#pragma once

#include "DBusInterface.hpp"

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QString>


class ScreenShotDBusInterface : public DBusInterface
{
    Q_OBJECT

public:
    explicit ScreenShotDBusInterface(QQmlApplicationEngine *pEngine,  QObject *parent = 0);
    virtual ~ScreenShotDBusInterface() = default;

    QString ScreenShot(const QString& storePath);

private:
    QString getISODate() const;
    QString generateFilename() const;
    QQuickWindow* retieveQWindow() const;

    QQmlApplicationEngine* m_pEngine;

    const int m_ImageQuality;
    const char* m_pImageFormat;
    const QString m_FileExtension;
    const QString m_BaseFileName;
};


