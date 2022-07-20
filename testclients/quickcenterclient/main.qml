import QtQuick 2.15
import EmbeddedShell 1.0

Window {
    id: window
    visible: true
    title: qsTr("Hello from CenterCLient")
    anchor: Platform.Center
    color: "darkgray"

    onViewSelected: {
        console.log("view selected "+view);
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
                window.createView();
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
