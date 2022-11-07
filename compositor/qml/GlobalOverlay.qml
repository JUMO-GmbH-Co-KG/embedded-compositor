import QtQuick 2.0
import QtWayland.Compositor 1.0
import QtQuick.Window 2.2
import QtQml.Models 2.1

import com.embeddedcompositor.dbus 1.0
import com.embeddedcompositor.embeddedshell 1.0
import "DefaultTaskSwitcher"
import "Notifications"

Rectangle {
    id: globalOverlay
    anchors.fill: parent
    color: "darkblue"
    function show(message) {
        messageText.text = message
        visible = true;
    }

    function hide() {
        visible = false
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {}
    }

    Text {
        id: messageText
        anchors.centerIn: parent
        color: "white"
        font.pixelSize: 32
        text: "please wait..."
    }
}
