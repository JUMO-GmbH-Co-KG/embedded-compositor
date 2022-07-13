import QtQuick 2.15
import EmbeddedShell 1.0

Window {
    id: window
    width: menu.visible ? menu.width : bar.width
    height: 700
    visible: true
    title: qsTr("Hello from LeftCLient")
    anchor: Window.Anchor.Left
    color: "transparent"
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
                onClicked: popup.show();
                Rectangle {
                    anchors.fill: parent
                }

                Text {
                    text: "show"
                    anchors.centerIn: parent
                }
            }
            MouseArea {
                width:32
                height:32
                onClicked: popup.hide();
                Rectangle {
                    anchors.fill: parent
                }

                Text {
                    text: "hide"
                    anchors.centerIn: parent
                }
            }
        }

    }
    Window {
        id: popup
        width:48
        height:48
        color:"red"
    }
}
