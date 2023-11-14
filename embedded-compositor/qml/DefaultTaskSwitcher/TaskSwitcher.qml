// SPDX-License-Identifier: GPL-3.0-only

import QtQuick 2.0
import QtWayland.Compositor 1.0
import de.EmbeddedCompositor.embeddedshell 1.0
import QtGraphicalEffects 1.12
import de.EmbeddedCompositor.utility 1.0

Rectangle {
    id: taskSwitcher
    color: Qt.rgba(0.0,0.0,0.0,0.8)
    anchors.fill: parent

    signal switchTask(shellSurface: ShellSurface, view: SurfaceView);

    property ListModel surfaceModel

    function open()
    {
        state = "open"
    }

    function close()
    {
        state = "closed"
    }

    SortFilterProxyModel {
        id: sorted
        sourceModel: surfaceModel
        sortFunction: "sort"
        function sort(a,b) {
            var ai = a.view ? a.view.sortIndex : a.surface.sortIndex
            var bi = b.view ? b.view.sortIndex : b.surface.sortIndex
            return ai<bi;
        }
    }

    ListView {
        id: listView
        z:100
        model: sorted

        anchors.centerIn: parent
        width: contentWidth < parent.width ? contentWidth : parent.width
        height: 200

        delegate: taskSwitchComponent
        spacing: 5
        orientation: ListView.Horizontal
    }

    MouseArea {
        anchors.fill: parent
        onClicked: taskSwitcher.close()
    }

    Component {
        id: taskSwitchComponent
        Rectangle {
            id: taskSwitchComponentRoot
            color: ListView.isCurrentItem ? "white" : "darkgray"
            property alias shellSurface: sItem.shellSurface
            property SurfaceView view: model.data.view
            width: 1280 / 4
            height: 800 / 4

            function select()
            {
                console.log("switch task",shellSurface, view);
                ListView.view.currentIndex = model.index
                taskSwitcher.switchTask(shellSurface, view);
                taskSwitcher.close()
            }

            ShellSurfaceItem {
                anchors.centerIn: parent
                scale: parent.width / sItem.width
                id: sItem
                visible: taskSwitcher.visible
                shellSurface: model.data.surface
                bufferLocked: !taskSwitchComponentRoot.ListView.isCurrentItem
            }

            FastBlur {
                anchors.fill:parent
                source: sItem
                radius: 64
                cached: sItem.bufferLocked
            }

            Text {
                id: text
                anchors.centerIn: parent
                text: (view?
                           ("<"+view.label+"> "+view.sortIndex+" view id "+view.uuid)
                         : "NOT VIEW "+shellSurface.sortIndex+" "+JSON.stringify(shellSurface,null,2)) + "\nsurf:"+shellSurface.uuid
                color:"white"
            }

            MouseArea {
                anchors.fill: parent
                onClicked:taskSwitchComponentRoot.select()
            }
        }
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
}
