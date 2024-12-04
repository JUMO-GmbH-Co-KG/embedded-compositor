// SPDX-License-Identifier: GPL-3.0-only

import QtQuick 2.15
import QtWayland.Compositor 1.15
import QtQuick.Window 2.2
import QtQml.Models 2.1

import de.EmbeddedCompositor.dbus 1.0
import de.EmbeddedCompositor.embeddedshell 1.0
import de.EmbeddedCompositor.configuration 1.0

import "DefaultTaskSwitcher"
import "Notifications"

WaylandCompositor {
    objectName: "compositor"

    TextInputManager { }

    XdgOutputManagerV1 {
        WaylandOutput {
            objectName: "output"
            id: waylandOutput
            transform: {
                if(rootTransformItem.state == "90") return WaylandOutput.Transform90;
                else  if(rootTransformItem.state == "180") return WaylandOutput.Transform180;
                else  if(rootTransformItem.state == "270") return WaylandOutput.Transform270;
                else  return WaylandOutput.TransformNormal;
            }
            physicalSize: Qt.size(configuration.screenPhysicalWidth,configuration.screenPhysicalHeight)
            manufacturer: "JUMO"
            model: "variTron"

            XdgOutputV1 {
                id: xdgOutput
                name: "main screen"
                logicalPosition: waylandOutput.position
                logicalSize: {
                    if(waylandOutput.transform == WaylandOutput.Transform90
                            || waylandOutput.transform == WaylandOutput.Transform270)
                    return Qt.size(window.height,window.width);
                    return Qt.size(window.width,window.height);
                }
            }
            window: window
        }
    }

    Window {
        id: window
        objectName: "compositorWindow"
        // the window tracks the physical screen dimensions
        width: configuration.screenWidth
        height: configuration.screenHeight
        visible: true
        property real initialSize: 0

        RootTransformer {
            id: rootTransformItem
            state: dbusScreenInterface.orientation
            fullScreen: dbusScreenInterface.fullScreen

            Item {
                Keys.onPressed:
                    (event)=> {
                        if (event.key === Qt.Key_Super_L || event.key === Qt.Key_Super_R) {
                            if(taskSwitcherLoader.item.state === "open") taskSwitcherLoader.item.close();
                            else taskSwitcherLoader.item.open();
                        } else if(event.key === Qt.Key_F1) {
                            rootTransformItem.state = "0"
                        } else if(event.key === Qt.Key_F2) {
                            rootTransformItem.state = "90"
                        } else if(event.key === Qt.Key_F3) {
                            rootTransformItem.state = "180"
                        } else if(event.key === Qt.Key_F4) {
                            rootTransformItem.state = "270"
                        } else {
                            screenSaverController.reset();
                        }
                    }
                focus: true
            }

            Rectangle {
                id: centerArea
                objectName: "centerArea"
                anchors.top: !rootTransformItem.fullScreen ? topArea.bottom : rootTransformItem.top
                anchors.left: !rootTransformItem.fullScreen ? leftArea.right : rootTransformItem.left
                anchors.right: !rootTransformItem.fullScreen ? rightArea.left : rootTransformItem.right
                anchors.bottom: !rootTransformItem.fullScreen ? bottomArea.top : rootTransformItem.bottom

                transform: Translate {
                    y: keyboardLoader.contentYTranslate
                }

                border {
                    width: 1
                    color:"red"
                }
                property Item surfaceItem
                onSurfaceItemChanged: {
                    if(surfaceItem == null) {
                        if(centerApplicationViewModel.count > 0) {
                            var entry = centerApplicationViewModel.get(0);
                            selectSurface(entry.data.surface, entry.data.view);
                        } else {
                            console.log("last surface closed...");
                        }
                    } else {
                        taskSwitcherInterface.currentView = surfaceItem.uuid
                    }
                }
                property ShellSurface surface
                function selectSurface(shellSurface, view)
                {
                    for(var i =0; i < children.length; i++)
                    {
                        var window = children[i];
                        if(window.shellSurface === shellSurface) {
                            window.currentView = view;
                            surfaceItem = window;
                            taskSwitcherInterface.currentView = surfaceItem.uuid;
                            break;
                        }
                    }
                }
            }
            Item {
                id:leftArea
                objectName: "leftArea"
                width: surfaceItem ? surfaceItem.margin:window.initialSize
                anchors.bottom: bottomArea.top
                anchors.left: parent.left
                anchors.top: topArea.bottom
                property Item surfaceItem
                visible: !rootTransformItem.fullScreen
            }
            Item {
                id:rightArea
                objectName: "rightArea"
                width: surfaceItem ? surfaceItem.margin :window.initialSize
                anchors.bottom:bottomArea.top
                anchors.right: parent.right
                anchors.top: topArea.bottom
                property Item surfaceItem
                visible: !rootTransformItem.fullScreen
            }
            Item {
                id: topArea
                objectName: "topArea"
                height: surfaceItem ? surfaceItem.margin : window.initialSize
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                property Item surfaceItem
                visible: !rootTransformItem.fullScreen
            }
            Item {
                id: bottomArea
                objectName: "bottomArea"
                height: surfaceItem ? surfaceItem.margin : window.initialSize
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                property Item surfaceItem
                visible: !rootTransformItem.fullScreen
            }

            Loader {
                id:taskSwitcherLoader
                anchors.fill:parent
                source: configuration.taskSwitcherUrl
                active: true
                onLoaded: {
                    item.surfaceModel = centerApplicationViewModel
                    item.switchTask.connect(doSwitch)
                }

                function doSwitch(shellSurface, view)
                {
                    centerArea.selectSurface(shellSurface, view);
                }
            }

            Loader {
                id: keyboardLoader
                readonly property int contentYTranslate: item ? item.contentYTranslate : 0
                source: "Keyboard.qml"
                anchors.fill: parent
                rotation: rootTransformItem.rotation

                onStatusChanged: {
                    if (status === Loader.Error) {
                        console.warn("Failed to load virtual keyboard:", sourceComponent.errorString());
                    }
                }
            }

            Notifications {
                anchors.fill: parent
            }

            Loader {
                id: globalOverlayLoader
                anchors.fill:parent
                source: configuration.globalOverlayUrl
                active: true
            }

            ScreenSaverController {
                id: screenSaverController
                timeoutSeconds: dbusScreenInterface.screenSaverTimeoutSeconds
                screenSaverUrl: configuration.screenSaverUrl
                screenSaverEnabled: dbusScreenInterface.screenSaverEnabled
                mouseHoverSupport: configuration.screenSaverMouseHoverSupport
            }
        }
        Item {
            id: limboArea
            objectName: "limboArea"
            visible: false
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

    ListModel {
        id: centerApplicationViewModel
        property var surfaces: ({});

        function addSurface(shellSurface, shellSurfaceItem)
        {
            if(surfaces.hasOwnProperty(shellSurface))
            {
                console.debug("application model: duplicate key! "+shellSurface.uuid);
                return;
            }

            surfaces[shellSurface] = {
                views: []
            };
            append({data: {view: null, surface: shellSurface}});
            shellSurfaceItem.surfaceDestroyed.connect(function(){
                removeSurface(shellSurface);
            });

            shellSurface.anchorChanged.connect(function(){
                if(shellSurface.anchor !== EmbeddedShellTypes.Center) removeSurface(shellSurface);
            });
        }

        function removeSurface(shellSurface)
        {
            for(var i = count - 1; i >= 0; i--)
            {
                if(get(i).data.surface === shellSurface)
                {
                    remove(i);
                }
            }
            delete surfaces[shellSurface]
        }

        function createView(shellSurface, view)
        {
            var entry = surfaces[shellSurface];
            console.log("compositor: create view! "+ view +" have entry "+entry);
            if(entry.views.length === 0)
            {
                console.log("setting first view!");
                entry.views.push(view);
                append({data: {view: view, surface: shellSurface}});
                for(var i = 0; i<count; i++)
                {
                    if(get(i).data.surface === shellSurface)
                    {
                        remove(i);
                        break;
                    }
                }
                if (centerArea.surfaceItem.shellSurface === shellSurface)
                    centerArea.selectSurface(shellSurface, view);
                return;
            }

            append({data: {view: view, surface: shellSurface}});
        }

        function findByUuid(uuid)
        {
            for(var i = 0; i<count; i++)
            {
                var entry = get(i).data;
                if (entry.surface.uuid === uuid || entry.view !== null && entry.view.uuid === uuid)
                {
                    return entry
                }
            }
        }

    }

    Component {
        id: chromeComponent
        ShellSurfaceItem {
            id: shellSurfaceItem

            visible: parent.surfaceItem === shellSurfaceItem

            onSurfaceDestroyed:  destroy()
            onWidthChanged: handleResized()
            onHeightChanged: handleResized()
            Component.onCompleted: {
                handleAnchor();
            }

            property int margin: shellSurface.margin
            property string uuid: currentView ? currentView.uuid : shellSurface.uuid
            property var currentView: null

            onCurrentViewChanged: if(currentView != null) currentView.select();


            Connections {
                target: shellSurface
                function onAnchorChanged() {
                    handleAnchor();
                }

                function onCreateView(view) {
                    centerApplicationViewModel.createView(shellSurface, view);
                }
            }

            function handleAnchor() {
                var targetArea = anchorMap[shellSurface.anchor];

                if(shellSurface.anchor === EmbeddedShellTypes.Center) {
                    centerApplicationViewModel.addSurface(shellSurface, shellSurfaceItem);
                } else if(shellSurface.anchor === EmbeddedShellTypes.Undefined) {
                    shellSurfaceItem.parent = limboArea;
                    console.log("undefined -> noop");
                    return;
                }

                shellSurfaceItem.parent = targetArea;
                shellSurfaceItem.anchors.left = targetArea !== rightArea ? targetArea.left : undefined;
                shellSurfaceItem.anchors.right = targetArea !== leftArea ? targetArea.right : undefined;
                shellSurfaceItem.anchors.top = targetArea !== bottomArea ? targetArea.top : undefined;
                shellSurfaceItem.anchors.bottom = targetArea !== topArea ? targetArea.bottom : undefined;

                if(shellSurface.anchor !== EmbeddedShellTypes.Undefined) {
                    targetArea.surfaceItem = shellSurfaceItem;
                }
            }

            function handleResized() {
                if(width <= 0 || height <=0) return;
                shellSurface.sendConfigure(Qt.size(width, height));
            }
        }
    }


    EmbeddedShell {
        onSurfaceAdded: chromeComponent.createObject(limboArea, { "shellSurface": surface } );
    }
    TaskSwitcherInterface {
        id: taskSwitcherInterface
        onOpenRequested: taskSwitcherLoader.item.open();
        onCloseRequested: taskSwitcherLoader.item.close();
        viewModel: centerApplicationViewModel
        onCurrentViewChanged: {
            var entry = centerApplicationViewModel.findByUuid(currentView);
            centerArea.selectSurface(entry.surface, entry.view);
        }
    }
    GlobalOverlayInterface {
        onShowRequested: (message) => {
            globalOverlayLoader.item.show(message);
        }
        onHideRequested: {
            if(globalOverlayLoader.active)
                globalOverlayLoader.item.hide()
        }
    }

    CompositorScreenInterface {
        id: dbusScreenInterface
        screenSaverActive: screenSaverController.screenSaverActive
        onShowScreenSaver: screenSaverController.showScreenSaver();
        screenSaverEnabled: configuration.screenSaverEnabled
        screenSaverTimeoutSeconds: configuration.screenSaverTimeoutSeconds
        orientation: configuration.screenOrientation
        fullScreen: configuration.fullScreen
    }

    ConfigurationHive {
        id: configuration
        property url taskSwitcherUrl: "DefaultTaskSwitcher/TaskSwitcher.qml"
        property url globalOverlayUrl: "DefaultGlobalOverlay/GlobalOverlay.qml"
        property url screenSaverUrl: "DefaultScreenSaver/ScreenSaver.qml"
        property url notificationUrl: "Notifications/Notification.qml"
        property int screenSaverTimeoutSeconds: 6
        property bool screenSaverMouseHoverSupport: false
        property bool screenSaverEnabled: false
        // size in pixels as seen by the OS before compositor controlled rotation
        property int screenWidth: (Qt.application.primaryScreen || Qt.application.screens[0]).width
        property int screenHeight: (Qt.application.primaryScreen || Qt.application.screens[0]).height
        // size in millimeters, as seen by the OS before compositor controlled rotation.
        // e.g. size of a common 10.1" display, seen by the OS in default widescreen
        property real screenPhysicalWidth: 219
        property real screenPhysicalHeight: 137
        // compositor controlled rotation, i.e. "0", "90", "180", "270"
        property string screenOrientation: "0"
        property bool fullScreen: false
    }
}
