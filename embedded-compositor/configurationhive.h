#ifndef CONFIGURATIONHIVE_H
#define CONFIGURATIONHIVE_H

#include <QObject>
#include <QQmlParserStatus>

class ConfigurationHive : public QObject, public QQmlParserStatus {
  Q_OBJECT
public:
  ConfigurationHive();

public:
  void classBegin() override;
  void componentComplete() override;
};

#endif // CONFIGURATIONHIVE_H
