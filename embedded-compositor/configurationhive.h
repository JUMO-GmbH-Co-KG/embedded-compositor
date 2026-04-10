// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

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
