// SPDX-License-Identifier: LGPL-3.0-only

import QtQuick 2.15
import EmbeddedShell 1.0 as EmbeddedShell

EmbeddedShell.Window {
    id: window
    width: menu.visible ? menu.width : bar.width
    visible: true
    title: qsTr("Hello from LeftCLient")
    anchor: EmbeddedShell.Window.Anchor.Left
    color: "transparent"
    margin: 64
    Rectangle {
        id: menu
        color: "#d02020ff"
        height: parent.height
        property bool open: false
        width: 200
        visible: x != -width
        x: open ? 0 : -width
        Behavior on x {
            PropertyAnimation {}
        }

        border {
            width:1
            color:"white"
        }
    }

    Rectangle {
        id: bar
        color: "#3030b0"
        width:48
        height: parent.height
        MouseArea {
            anchors.fill: parent
            onClicked: {
                menu.open= !menu.open
            }
        }

        Column {
            spacing: 8
            anchors.centerIn: parent
            MouseArea {
                width:32
                height:32
                onClicked: Qt.quit()
                Rectangle {
                    anchors.fill: parent
                }

                Text {
                    text: "quit"
                    anchors.centerIn: parent
                }
            }

            MouseArea {
                width:32
                height:32
                onClicked: {
                    var view =  window.createView("New View", 0);
                    console.log("view:"+view);
                }
                Rectangle {
                    anchors.fill: parent
                }

                Text {
                    text: "createView"
                    anchors.centerIn: parent
                }
            }
        }

    }
}
