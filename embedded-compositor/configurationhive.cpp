#include "configurationhive.h"
#include "qurl.h"
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QMetaProperty>
#include <QSettings>
#include <QUrl>

ConfigurationHive::ConfigurationHive() {}

void ConfigurationHive::classBegin() {}

void ConfigurationHive::componentComplete() {

  QCommandLineParser parser;
  for (int i = 0; i < metaObject()->propertyCount(); ++i) {
    QMetaProperty metaProperty = metaObject()->property(i);
    if (metaProperty.name() == QString("objectName"))
      continue;

    qDebug() << metaProperty.name() << metaProperty.type()
             << metaProperty.read(this);

    QCommandLineOption opt(metaProperty.name(), QString(), metaProperty.name(),
                           metaProperty.read(this).toString());
    parser.addOption(opt);

    auto envName = QString("COMPOSITOR_%1").arg(metaProperty.name());
    auto env = qgetenv(envName.toLocal8Bit());

    if (!env.isNull()) {
      QVariant value;
      if (metaProperty.type() == QVariant::String) {
        value = QString(env);
      } else if (metaProperty.type() == QVariant::Int) {
        value = env.toInt();
      } else if (metaProperty.type() == QVariant::Double) {
        value = env.toDouble();
      } else if (metaProperty.type() == QVariant::Bool) {
        value = (env == "true" || env == "1");
      } else if (metaProperty.type() == QVariant::Url) {
        value = QUrl(env);
      }

      if (!value.isNull()) {
        metaProperty.write(this, value);
      }
    }
  }
  parser.process(QCoreApplication::arguments());
  for (auto optName : parser.optionNames()) {
    if (parser.isSet(optName)) {
      auto i = metaObject()->indexOfProperty(optName.toLocal8Bit());
      metaObject()->property(i).write(this, parser.value(optName));
    }
  }
}
