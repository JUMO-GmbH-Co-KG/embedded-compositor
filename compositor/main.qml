import QtQuick 2.0
import QtWayland.Compositor 1.0
import QtQuick.Window 2.2
import QtQml.Models 2.1

import com.embeddedcompositor.embeddedshell 1.0

WaylandCompositor {
    WaylandOutput {
        sizeFollowsWindow: true
        window: Window {
            id: window
            width: 800
            height: 480
            visible: true
            property real initialSize: 0
            Rectangle {
                id: centerArea
                anchors.top: topArea.bottom
                anchors.left: leftArea.right
                anchors.right: rightArea.left
                anchors.bottom: bottomArea.top
                border {
                    width:4
                    color:"red"
                }
                property Item surfaceItem
                Row {
                    z:1000
                    id: viewsContainer
                    height:100
                    anchors.bottom: parent.bottom
                }
            }
            Rectangle {
                id:leftArea
                width: surfaceItem ? surfaceItem.initialWidth :window.initialSize
                anchors.bottom: bottomArea.top
                anchors.left: parent.left
                anchors.top: topArea.bottom
                border {
                    width:1
                    color:"green"
                }
                color: "darkgreen"
                property Item surfaceItem
            }
            Rectangle {
                id:rightArea
                width: surfaceItem ? surfaceItem.initialWidth :window.initialSize
                anchors.bottom:bottomArea.top
                anchors.right: parent.right
                anchors.top: topArea.bottom
                border {
                    width:1
                    color:"green"
                }
                color: "darkgreen"
                property Item surfaceItem
            }
            Rectangle {
                id: topArea
                height: surfaceItem ? surfaceItem.initialHeight : window.initialSize
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                border {
                    width: 1
                    color:"green"
                }
                color: "darkgreen"
                property Item surfaceItem
            }
            Rectangle {
                id: bottomArea
                height: surfaceItem ? surfaceItem.initialHeight : window.initialSize
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                border {
                    width:1
                    color:"green"
                }
                color: "darkgreen"
                property Item surfaceItem
            }
        }
    }

    Component {
        id: chromeComponent
        ShellSurfaceItem {
            id: shellSurfaceItem

            onSurfaceDestroyed: {
                destroy()
            }
            Component.onCompleted: {
                console.log(shellSurface)
            }

            onWidthChanged: handleResized()
            onHeightChanged: handleResized()
            property int initialHeight: -1
            property int initialWidth: -1

            function reanchor(anchor) {
                console.log("reanchor!! " + shellSurface.anchor);
                var targetArea = centerArea;
                if(shellSurface.anchor === 1) targetArea = topArea;
                if(shellSurface.anchor === 2) targetArea = bottomArea;
                if(shellSurface.anchor === 3) targetArea = leftArea;
                if(shellSurface.anchor === 4) targetArea = rightArea;
                if(shellSurface.anchor === 5) targetArea = centerArea;

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
                var viewItem = viewComponent.createObject(viewsContainer, {view: view});
            }

            onShellSurfaceChanged: {
                console.log(shellSurface)
                shellSurface.anchorChanged.connect(reanchor)
                shellSurface.createView.connect(createView)
            }

            function handleResized() {
                console.log(shellSurface.anchor+": "+width+"x"+height);
                if(width < 0 || height <0) return;
                if(initialWidth < 0) initialWidth = width;
                if(initialHeight < 0) initialHeight = height;
                shellSurface.sendConfigure(Qt.size(width, height));
            }
        }
    }
    Component {
        id: viewComponent
        Rectangle {
            width:100
            height: 100
            id: viewChrome
            property SurfaceView view
            color:Qt.rgba(0,0.5,0,0.5);
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
}
