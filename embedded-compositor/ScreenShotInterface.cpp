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


ScreenShotInterface::ScreenShotInterface(QQmlApplicationEngine *engine, QObject *parent)
    : DBusInterface(QStringLiteral("/screenshot"), parent),
    m_pEngine(engine),
    m_ImageQuality(50),
    m_pImageFormat("PNG"),
    m_FileExtension(QStringLiteral(".png")),
    m_BaseFileName(QStringLiteral("screenshot_"))
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

QQuickWindow* ScreenShotInterface::retieveQWindow() const
{
    if (m_pEngine)
    {
        for (auto* pRootObject : m_pEngine->rootObjects())
        {
            if (pRootObject)
            {
                QQuickWindow* pQmlQuickWindow = qobject_cast<QQuickWindow*>(
                    pRootObject->findChild<QObject*>("compositorWindow") );
                if (pQmlQuickWindow)
                {
                    return pQmlQuickWindow;
                }
            }
        }
    }

    qDebug() << "Could not retrieve QML window object !";

    return nullptr;
}

QString ScreenShotInterface::ScreenShot(const QString& storePath)
{
    QString fileSavePath;
    QQuickWindow* pQuickWindow = retieveQWindow();
    if (pQuickWindow)
    {
        QImage capturedWindowImage = pQuickWindow->grabWindow();
        fileSavePath = storePath + "/" + generateFilename();
        if (false == capturedWindowImage.save(fileSavePath, m_pImageFormat, m_ImageQuality))
        {
            qDebug() << "Could not save Screenshot !";
            return QString();
        }
    }

    return fileSavePath;
}
