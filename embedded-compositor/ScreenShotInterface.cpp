/*!
 * @file   ScreenShotInterface.cpp
 *
 * @date   10.07.2024
 *
 * @author Christian Happ
 *
*/

#include "ScreenShotInterface.hpp"
#include "screenshot_adaptor.h"

#include <QImage>
#include <QDateTime>
#include <QDebug>
#include <QQuickWindow>


ScreenShotInterface::ScreenShotInterface(QQmlApplicationEngine *engine, QObject *parent)
    : DBusInterface(QStringLiteral("/screenshot"), parent),
    m_pEngine(engine),
    m_ImageQuality(50),
    m_ImageFormat("PNG"),
    m_FileExtension(".png"),
    m_BaseFileName("screenshot_")
{
    new ScreenshotAdaptor(this);

    // register object by dbus service name
    componentComplete();
}

QString ScreenShotInterface::getISODate() const
{
    QDateTime dateTime;
    dateTime = QDateTime::currentDateTime();
    QString strDateTime = dateTime.toString(Qt::ISODate);

    return strDateTime;
}

QString ScreenShotInterface::generateFilename() const
{
    QString concateFileName = m_BaseFileName + getISODate() + m_FileExtension;

    return concateFileName;
}

QString ScreenShotInterface::ScreenShot(const QString storePath)
{
    QString fileSavePath = {};
    if (m_pEngine)
    {
        QObject *rootObject = m_pEngine->rootObjects().first();
        if (rootObject)
        {
            QQuickWindow* qmlQuickWindow = qobject_cast<QQuickWindow*>(
                rootObject->findChild<QObject*>("compositorWindow") );
            if (qmlQuickWindow)
            {
                QImage capturedWindowImage = qmlQuickWindow->grabWindow();   
                fileSavePath = storePath + "/" + generateFilename();
                if (false == capturedWindowImage.save(fileSavePath, m_ImageFormat, m_ImageQuality))
                {
                    qDebug() << "Could not save Screenshot !";
                    return {};
                }
            }
            else
            {
                qDebug() << "Could not retrieve QML window object !";
            }
        }
    }

    return fileSavePath;
}

