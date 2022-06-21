import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    id: window
    height: menu.open ? menu.height: bar.height
    width: 100
    visible: true
    title: qsTr("Hello World")
    color: "transparent"

    Rectangle {
        id: menu
        property bool open: false
        color: "#d02020ef"
        height: 200
        width: parent.width
        visible: open
        y: open ? 0 : -height
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

        Text {
            color: "white"
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 24
            Timer {
                interval: 1000
                running: true
                repeat: true
                onTriggered: {
                    var d = new Date();
                    var f = [d.getHours(), d.getMinutes(), d.getSeconds()];
                    parent.text = f.map((v) => v<10?"0"+v:""+v).join(":");
                }
            }
        }
    }
}
