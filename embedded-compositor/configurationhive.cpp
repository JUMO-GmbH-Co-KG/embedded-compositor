#include "configurationhive.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QMetaProperty>

ConfigurationHive::ConfigurationHive()
{
}

void ConfigurationHive::classBegin()
{
}

void ConfigurationHive::componentComplete()
{

  QCommandLineParser commandLineParser;
  const auto propertyCount = metaObject()->propertyCount();

  for (int i = 0; i < propertyCount; ++i)
  {
    QMetaProperty metaProperty = metaObject()->property(i);

    if (metaProperty.name() == QString("objectName"))
    {
      continue;
    }

    qDebug() << metaProperty.name() << metaProperty.typeName() << metaProperty.read(this);

    QCommandLineOption commandLineOption(metaProperty.name(), QString(), metaProperty.name(), metaProperty.read(this).toString());
    commandLineParser.addOption(commandLineOption);

    auto environmentVariableName = QStringLiteral("COMPOSITOR_%1").arg(metaProperty.name());
    auto environmentVariableValue = qgetenv(environmentVariableName.toLocal8Bit());

    if (!environmentVariableValue.isNull())
    {
      if (!metaProperty.write(this, QString::fromUtf8(environmentVariableValue)))
      {
        qWarning() << "Cannot set environment variable" << environmentVariableName
                   << "with value" << environmentVariableValue << "to property" << metaProperty.name()
                   << "of type" << metaProperty.typeName() << "!";
      }
    }
  }

  commandLineParser.process(QCoreApplication::arguments());
  const auto optionNames = commandLineParser.optionNames();

  for (const auto &optionName : optionNames)
  {
    if (commandLineParser.isSet(optionName))
    {
      auto index = metaObject()->indexOfProperty(optionName.toLocal8Bit());
      auto metaProperty = metaObject()->property(index);
      if (metaProperty.isValid())
      {
        const auto value = commandLineParser.value(optionName);
        if (!metaProperty.write(this, commandLineParser.value(optionName)))
        {
          qWarning() << "Cannot set command line option" << optionName
                     << "with value" << value << "to property" << metaProperty.name()
                     << "of type" << metaProperty.typeName() << "!";
        }
      }
      else
      {
        qWarning() << "Invalid option" << optionName << "!";
      }
    }
  }
}
