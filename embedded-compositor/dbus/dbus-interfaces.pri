DEPENDPATH  += $$PWD/
INCLUDEPATH += $$PWD/

HEADERS += \
    $$PWD/CompositorScreenDBusInterface.hpp \
    $$PWD/GlobalOverlayDBusInterface.hpp \
    $$PWD/NotificationModel.hpp \
    $$PWD/ScreenShotDBusInterface.hpp \
    $$PWD/TaskSwitcherDBusInterface.hpp \
    $$PWD/TaskSwitcherEntry.hpp \
    $$PWD/DBusInterface.hpp

SOURCES += \
    $$PWD/CompositorScreenDBusInterface.cpp \
    $$PWD/GlobalOverlayDBusInterface.cpp \
    $$PWD/NotificationModel.cpp \
    $$PWD/ScreenShotDBusInterface.cpp \
    $$PWD/TaskSwitcherDBusInterface.cpp \
    $$PWD/DBusInterface.cpp
