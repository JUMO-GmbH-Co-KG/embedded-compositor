import QtQuick 2.15
import EmbeddedShell 1.0

Window {
    id: window
    height: menu.visible ? menu.height: bar.height
    width: 100
    visible: true
    title: qsTr("Hello World")
    color: "transparent"
    anchor: Window.Anchor.Bottom

    Rectangle {
        id: menu
        property bool open: false
        color: "#d02020ef"
        height: 200
        width: parent.width
        visible: y != height
        y: open ? 0 : height
        Behavior on y { PropertyAnimation {  }}

        MouseArea {
            anchors.bottom: parent.bottom
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
    }

    Rectangle {
        id: bar
        color: menu.open ? "#1818ff" : "#181871"
        width: parent.width
        height: 32
        anchors.bottom: parent.bottom
        MouseArea {
            anchors.fill: parent
            onClicked: {
                menu.open= !menu.open
            }
        }

        border {
            width:1
            color:"white"
        }
    }
}
