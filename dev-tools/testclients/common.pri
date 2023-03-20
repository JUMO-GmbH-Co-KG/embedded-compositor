use_system_bus {
    message(using system dbus)
    DEFINES+="USE_SYSTEM_BUS=1"
} else {
    message(using session dbus)
}

target.path = /usr/share/embeddedcompositor-examples/
INSTALLS += target
