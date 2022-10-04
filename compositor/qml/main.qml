import QtQuick 2.0
import QtWayland.Compositor 1.0
import QtQuick.Window 2.2
import QtQml.Models 2.1

import com.embeddedcompositor.dbus 1.0
import com.embeddedcompositor.embeddedshell 1.0
import "DefaultTaskSwitcher"

WaylandCompositor {
    WaylandOutput {
        sizeFollowsWindow: true
        window: Window {
            id: window
            width: 1280
            height: 800
            visible: true
            property real initialSize: 0

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
                property ShellSurface surface
                function selectSurface(shellSurface, view)
                {
                    for(var i =0; i < children.length; i++)
                    {
                        var window = children[i];
                        if(window.shellSurface === shellSurface) {
                            surfaceItem = window;
                            break;
                        }
                    }
                }
            }
            Item {
                id:leftArea
                width: surfaceItem ? surfaceItem.margin:window.initialSize
                anchors.bottom: bottomArea.top
                anchors.left: parent.left
                anchors.top: topArea.bottom
                property Item surfaceItem
            }
            Item {
                id:rightArea
                width: surfaceItem ? surfaceItem.margin :window.initialSize
                anchors.bottom:bottomArea.top
                anchors.right: parent.right
                anchors.top: topArea.bottom
                property Item surfaceItem
            }
            Item {
                id: topArea
                height: surfaceItem ? surfaceItem.margin : window.initialSize
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                property Item surfaceItem
            }
            Item {
                id: bottomArea
                height: surfaceItem ? surfaceItem.margin : window.initialSize
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                property Item surfaceItem
            }

            TaskSwitcher {
                id: taskSwitcher
                anchors.fill:parent
                onSwitchTask: {
                    if(view != null) view.select();
                    centerArea.selectSurface(shellSurface, view);
                }
            }
        }
    }

    property var anchorMap: ({
         [EmbeddedShellTypes.Top]: topArea,
         [EmbeddedShellTypes.Bottom]: bottomArea,
         [EmbeddedShellTypes.Left]: leftArea,
         [EmbeddedShellTypes.Right]: rightArea,
         [EmbeddedShellTypes.Center]: centerArea,
         [EmbeddedShellTypes.Undefined]: limboArea,
    })

    Component {
        id: chromeComponent
        ShellSurfaceItem {
            id: shellSurfaceItem

            visible: parent.surfaceItem === shellSurfaceItem

            onSurfaceDestroyed:  destroy()
            onWidthChanged: handleResized()
            onHeightChanged: handleResized()
            Component.onCompleted: handleResized();

            property int margin: shellSurface.margin

            Connections {
                target: shellSurface
                function onAnchorChanged(){
                    var targetArea = anchorMap[shellSurface.anchor];

                    if(shellSurface.anchor === EmbeddedShellTypes.Center) {
                       taskSwitcher.addSurface(shellSurface, shellSurfaceItem);
                    }

                    shellSurfaceItem.parent = targetArea;
                    shellSurfaceItem.anchors.left = targetArea !== rightArea ? targetArea.left : undefined;
                    shellSurfaceItem.anchors.right = targetArea !== leftArea ? targetArea.right : undefined;
                    shellSurfaceItem.anchors.top = targetArea !== bottomArea ? targetArea.top : undefined;
                    shellSurfaceItem.anchors.bottom = targetArea !== topArea ? targetArea.bottom : undefined;
                    targetArea.surfaceItem = shellSurfaceItem;
                }

                function onCreateView(view){
                    taskSwitcher.createView(shellSurface, view);
                }
            }

            function handleResized() {
                console.log("handleResize anchor:"+shellSurface.anchor+" size "+width+"x"+height);
                if(width < 0 || height <0) return;
                shellSurface.sendConfigure(Qt.size(width, height));
            }
        }
    }

    Item {
        id: limboArea
        visible: false
    }

    EmbeddedShell {
        onSurfaceAdded: chromeComponent.createObject(limboArea, { "shellSurface": surface } );
    }
    TaskSwitcherInterface {
        onOpenRequested: taskSwitcher.open();
        onCloseRequested: taskSwitcher.close();
    }
}
