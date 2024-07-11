/*!
 * @file   ScreenShotInterface.hpp
 *
 * @class  ScreenShotInterface
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

#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>
#include "dbusinterface.h"

class ScreenShotInterface : public DBusInterface
{
    Q_OBJECT

public:
    explicit ScreenShotInterface(QQmlApplicationEngine *pEngine,  QObject *parent = 0);
    virtual ~ScreenShotInterface() = default;

    QString ScreenShot(const QString storePath);

private:
    QString getISODate() const;
    QString generateFilename() const;

    QQmlApplicationEngine* m_pEngine;

    const int m_ImageQuality;
    const char* m_ImageFormat;
    const QString m_FileExtension;
    const QString m_BaseFileName;
};


