import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    id: window
    width: menu.visible ? menu.width : bar.width
    height: 700
    visible: true
    title: qsTr("Hello World")
    color: "transparent"

    Rectangle {
        id: menu
        color: "#d02020ff"
        height: parent.height
        width: 400
        property bool open: false
        x: open ? 0 : width
        Behavior on x {
            PropertyAnimation {}
        }

        visible: x != width

        border {
            width:1
            color:"white"
        }
    }

    Rectangle {
        id: bar
        color: "#3030b0"
        width: 64
        height: parent.height
        anchors.right: parent.right
        MouseArea {
            anchors.fill: parent
            onClicked: {
                menu.open= !menu.open
            }
        }
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
    }

}
