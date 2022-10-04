import QtQuick 2.15
import EmbeddedShell 1.0

Window {
    id: window
    height: 48
    width: 100
    visible: true
    title: qsTr("Hello World")
    color: "grey"
    anchor: Window.Anchor.Bottom
    margin: 48
    Rectangle {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            margins: 4
        }

        width: 100
        color: "plum"
        MouseArea {
            anchors.fill:parent
            onClicked: {
                dbusClient.openTaskSwitcher();
            }
        }
        Text {
            anchors.centerIn: parent
            text: "⊞"
            font.pixelSize: 48
        }
    }

}
