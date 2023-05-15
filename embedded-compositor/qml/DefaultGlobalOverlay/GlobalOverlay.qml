// SPDX-License-Identifier: GPL-3.0-only
import QtQuick 2.0

Rectangle {
    id: globalOverlay
    anchors.fill: parent
    color: "#000030"
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
