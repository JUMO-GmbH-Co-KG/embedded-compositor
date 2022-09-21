import QtQuick 2.0
import QtWayland.Compositor 1.0
import QtQuick.Window 2.2
import QtQml.Models 2.1

import com.embeddedcompositor.dbus 1.0
import com.embeddedcompositor.embeddedshell 1.0

WaylandCompositor {
    WaylandOutput {
        sizeFollowsWindow: true
        window: Window {
            id: window
            width: 1280
            height: 800
            visible: true
            property real initialSize: 100

            Item {
                Keys.onPressed:
                    (event)=> {
                        if (event.key == Qt.Key_Super_L || event.key == Qt.Key_Super_R) {
                            if(taskSwitcher.state == "open") taskSwitcher.close();
                            else taskSwitcher.open();
                        }
                }
                focus: true
            }

/*
            Rectangle {
                id: spinner
                width: 10
                height: 10
                color: "lime"
                anchors.centerIn: parent
                z:1000

                RotationAnimation {
                    target: spinner
                    property: "rotation"
                    from: 0; to: 360
                    loops: Animation.Infinite
                    running: true;
                    duration: 4000
                }
            }

            MouseArea {
                z:1000
                anchors.centerIn: parent
                width:100
                height:100
                onClicked: {
                    dbusInterface.onBar.connect( function(){ console.log("back in qml") }  )
                    dbusInterface.onFoo();
                }
            }
*/
            Rectangle {
                id: centerArea
                anchors.top: topArea.bottom
                anchors.left: leftArea.right
                anchors.right: rightArea.left
                anchors.bottom: bottomArea.top
                border {
                    width: 1
                    color:"red"
                }
                property Item surfaceItem
            }
            Rectangle {
                id:leftArea
                width: surfaceItem ? surfaceItem.margin:window.initialSize
                anchors.bottom: bottomArea.top
                anchors.left: parent.left
                anchors.top: topArea.bottom
                border {
                    width:1
                    color:"lime"
                }
                color: "transparent"
                property Item surfaceItem
            }
            Rectangle {
                id:rightArea
                width: surfaceItem ? surfaceItem.margin :window.initialSize
                anchors.bottom:bottomArea.top
                anchors.right: parent.right
                anchors.top: topArea.bottom
                border {
                    width:1
                    color:"lime"
                }
                color: "transparent"
                property Item surfaceItem
            }
            Rectangle {
                id: topArea
                height: surfaceItem ? surfaceItem.margin : window.initialSize
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                border {
                    width: 1
                    color:"lime"
                }
                color: "transparent"
                property Item surfaceItem
            }
            Rectangle {
                id: bottomArea
                height: surfaceItem ? surfaceItem.margin : window.initialSize
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                border {
                    width:1
                    color:"lime"
                }
                color: "transparent"
                property Item surfaceItem
            }


            Rectangle {
                id: taskSwitcher
                color: Qt.rgba(0.0,0.0,0.0,0.8)
                anchors.fill: parent

                function open()
                {
                    state = "open"
                }

                function close()
                {
                    state = "closed"
                }

                state: "closed"
                states: [
                State {
                        name: "open"
                        PropertyChanges {
                            target: taskSwitcher
                            visible: true
                            opacity: 1
                        }
                    },
                    State {
                        name: "closed"
                        PropertyChanges {
                            target: taskSwitcher
                            visible: false
                            opacity: 0
                        }
                    }

                ]

                transitions: [
                    Transition {
                        from:"closed"
                        to:"open"
                        reversible: true

                        SequentialAnimation {
                            PropertyAction { property: "visible" }
                            PropertyAnimation {
                                property: "opacity"
                                duration: 200
                            }
                        }
                    }
                ]

                Flickable {
                    id: taskSwitcherFlick
                    anchors.centerIn: parent
                    width: contentWidth < parent.width ? contentWidth : parent.width
                    height: contentHeight
                    flickableDirection: Flickable.HorizontalFlick
                    contentHeight: taskSwitchContainer.height
                    contentWidth: taskSwitchContainer.width
                    Row {
                        id: taskSwitchContainer
                        height: 400
                        spacing: 10
                    }
                }
            }
        }
    }

    Component {
        id: chromeComponent
        ShellSurfaceItem {
            id: shellSurfaceItem

            opacity: 0.2
//            visible: false

            onSurfaceDestroyed: {
                destroy()
            }
            Component.onCompleted: {
                console.log(shellSurface)
            }

            onWidthChanged: handleResized()
            onHeightChanged: handleResized()
            property int margin: shellSurface.margin

            function reanchor(anchor) {
                var targetArea = centerArea;
                switch(shellSurface.anchor){

                case EmbeddedShellTypes.Top:
                    targetArea = topArea;
                    break;

                case EmbeddedShellTypes.Bottom:
                    targetArea = bottomArea;
                    break;
                case EmbeddedShellTypes.Left:
                    targetArea = leftArea;
                    break;
                case EmbeddedShellTypes.Right:
                    targetArea = rightArea;
                    break;
                case EmbeddedShellTypes.Center:
                    targetArea = centerArea;
                    var taskSwitchItem = taskSwitchComponent.createObject(taskSwitchContainer, {"shellSurface": shellSurface});
                    break;
                default:

                }

                shellSurfaceItem.parent = targetArea;
                shellSurfaceItem.anchors.left = targetArea != rightArea ? targetArea.left : undefined;
                shellSurfaceItem.anchors.right = targetArea != leftArea ? targetArea.right : undefined;
                shellSurfaceItem.anchors.top = targetArea != bottomArea ? targetArea.top : undefined;
                shellSurfaceItem.anchors.bottom = targetArea != topArea ? targetArea.bottom : undefined;
                targetArea.surfaceItem = shellSurfaceItem;
            }

            function createView(view)
            {
                console.log("compositor: create view! ", view);
                var viewItem = viewComponent.createObject(taskSwitchContainer, {view: view, surface: shellSurface});
            }

            onShellSurfaceChanged: {
                console.log(shellSurface)
                shellSurface.anchorChanged.connect(reanchor);
                shellSurface.createView.connect(createView);
            }

            function handleResized() {
                console.log("handleResize anchor:"+shellSurface.anchor+" size "+width+"x"+height);
                if(width < 0 || height <0) return;
                shellSurface.sendConfigure(Qt.size(width, height));
            }
        }
    }

    Component {
        id: taskSwitchComponent
        Rectangle {
            id: taskSwitchComponentRoot
            color: "lime"
            property alias shellSurface: sItem.shellSurface
            width: 1280 / 4
            height: 800 / 4
            ShellSurfaceItem {
                anchors.centerIn: parent
                scale: parent.width / sItem.width
                id: sItem
                onSurfaceDestroyed: {
                    taskSwitchComponentRoot.destroy()
                }
                visible: false
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("switch task");
                    taskSwitcher.close()
                }
            }
        }
    }

    Component {
        id: viewComponent
        Rectangle {
            property var surface
            width: 1280/4
            height: 800/4
            id: viewChrome
            property SurfaceView view
            color:"gray"
            border {
                width:1
                color: "white"
            }
            Text {
                anchors.centerIn: parent
                text: "<"+view.label+">"
                color:"white"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("selecting");
                    view.select();
                    taskSwitcher.close();
                }
            }
        }
    }

    EmbeddedShell {
        Component.onCompleted: console.log("INITIALIZED!");
        onSurfaceAdded: {
            console.log("QML: shell surface created: "+surface+" "+surface.anchor);
            var item = chromeComponent.createObject(centerArea, { "shellSurface": surface } );
            item.handleResized();


        }
    }
    TaskSwitcherInterface {
        onOpenRequested: taskSwitcher.open();
        onCloseRequested: taskSwitcher.close();
    }
}
