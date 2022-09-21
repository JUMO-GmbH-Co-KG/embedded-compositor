import QtQuick 2.15
import EmbeddedShell 1.0

Window {
    id: window
    visible: true
    title: qsTr("Hello from CenterCLient")
    anchor: EmbeddedShellTypes.Center
    color: "darkgray"
    width: 200
    height:200

    Column {
        spacing: 8
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
            property int idx:0

            onClicked: {
                var names = ["List view", "Tree view", "FPS test view" ];
                var view = window.createView(names[idx++]);
                console.log("client: view", view);
                view.selected.connect(function(){
                    console.log("view "+ view.label +" was selected");
                    viewLabel.text = view.label;
                })
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
    Text {
        id: viewLabel
        anchors.centerIn: parent
        text: "view?"
    }

    Rectangle {
        id: spinner
        width: 100
        height: 100
        color: "blue"
        anchors.centerIn: parent

        RotationAnimation {
            target: spinner
            property: "rotation"
            from: 0; to: 360
            loops: Animation.Infinite
            running: true;
            duration: 4000
        }
    }

    property int frameCounter: 0
    onFrameSwapped: frameCounter++
    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            console.log(window.frameCounter+"fps (client)");
            window.frameCounter = 0;
        }
    }



}
