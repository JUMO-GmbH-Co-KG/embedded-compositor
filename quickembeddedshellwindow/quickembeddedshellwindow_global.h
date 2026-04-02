// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <QtCore/qglobal.h>

#if defined(EMBEDDEDSHELLWINDOW_LIBRARY)
#  define EMBEDDEDSHELLWINDOW_EXPORT Q_DECL_EXPORT
#else
#  define EMBEDDEDSHELLWINDOW_EXPORT Q_DECL_IMPORT
#endif
