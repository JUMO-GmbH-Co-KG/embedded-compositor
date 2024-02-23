// SPDX-License-Identifier: GPL-3.0-only

import QtQuick 2.15
import QtQuick.VirtualKeyboard 2.0

Item {
    id: keyboardContainer

    property int contentYTranslate: 0
    property int keyboardMargin: 50

    // Cannot be a Binding since it would cause a feedback loop between the
    // translation and the cursor rect changing in response to the translation.
    function updateTranslate() {
        const im = Qt.inputMethod;
        if (im.visible) {
            const cursorRect = im.cursorRectangle;

            const overlap = -contentYTranslate + cursorRect.y + cursorRect.height - inputPanel.y + keyboardMargin;
            contentYTranslate = Math.min(0, -overlap);
        } else {
            contentYTranslate = 0;
        }
    }

    Connections {
        target: Qt.inputMethod
        function onCursorRectangleChanged() {
            keyboardContainer.updateTranslate();
        }
    }

    InputPanel {
        id: inputPanel
        visible: y < parent.height
        y: active ? parent.height - height : parent.height
        anchors.left: parent.left
        anchors.right: parent.right
        Behavior on y {
            NumberAnimation { }
        }
        onYChanged: Qt.callLater(keyboardContainer.updateTranslate)
        onHeightChanged: Qt.callLater(keyboardContainer.updateTranslate)
        onVisibleChanged: keyboardContainer.updateTranslate()
    }
}
