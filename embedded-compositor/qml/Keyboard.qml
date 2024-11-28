// SPDX-License-Identifier: GPL-3.0-only

import QtQuick 2.15
import QtQuick.VirtualKeyboard 2.0

Item {
    id: keyboardContainer

    property int contentYTranslate: 0
    property int keyboardMargin: 50

    rotation: parent.rotation

    state: "0"

    onRotationChanged: {
        state = rotation
    }

    // @TODO it is only when you switch from 90 to 270 or vice versa that the keyboard anchors get messy.
    // However, if you reboot the device, everything seems to be fine again.
    states: [
        State {
            name: "0"
            PropertyChanges {
                target: inputPanel
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: undefined
                anchors.bottom: undefined
                visible: y < keyboardContainer.height
                state: inputPanel.active ? "VISIBLE_0" : "HIDDEN_0"
            }
        },
        State {
            name: "90"
            PropertyChanges {
                target: inputPanel
                anchors.left: undefined
                anchors.right: undefined
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                visible: x < keyboardContainer.width
                state: inputPanel.active ? "VISIBLE_90" : "HIDDEN_90"
            }
        },
        State {
            name: "180"
            PropertyChanges {
                target: inputPanel
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: undefined
                anchors.bottom: undefined
                visible: y < keyboardContainer.height
                state: inputPanel.active ? "VISIBLE_180" : "HIDDEN_180"
            }
        },
        State {
            name: "270"
            PropertyChanges {
                target: inputPanel
                anchors.left: undefined
                anchors.right: undefined
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                visible: x < keyboardContainer.width
                state: inputPanel.active ? "VISIBLE_270" : "HIDDEN_270"
            }
        }
    ]

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
        //@TODO hides the keyboard on boot up and during rotation, but breaks the fade-out animation
        inputPanel.visible = Qt.inputMethod.visible;
    }

    Connections {
        target: Qt.inputMethod
        function onCursorRectangleChanged() {
            keyboardContainer.updateTranslate();
        }
    }

    InputPanel {
        id: inputPanel

        width: keyboardContainer.width
        height: keyboardContainer.height

        rotation: keyboardContainer.rotation
        state: "HIDDEN_0"

        onYChanged: {
            Qt.callLater(keyboardContainer.updateTranslate)
        }

        onXChanged: {
            Qt.callLater(keyboardContainer.updateTranslate)
        }

        onHeightChanged: {
            Qt.callLater(keyboardContainer.updateTranslate)
        }

        onWidthChanged: {
            Qt.callLater(keyboardContainer.updateTranslate)
        }

        onVisibleChanged: {
            keyboardContainer.updateTranslate()
        }

        states: [
            State {
                name: "VISIBLE_0"
                PropertyChanges {
                    target: inputPanel
                    y: keyboardContainer.height - inputPanel.height
                }
            },
            State {
                name: "HIDDEN_0"
                PropertyChanges {
                    target: inputPanel
                    y: keyboardContainer.height
                }
            },
            State {
                name: "VISIBLE_90"
                PropertyChanges {
                    target: inputPanel
                    x: keyboardContainer.width - inputPanel.width
                }
            },
            State {
                name: "HIDDEN_90"
                PropertyChanges {
                    target: inputPanel
                    x: -keyboardContainer.height
                }
            },
            State {
                name: "VISIBLE_180"
                PropertyChanges {
                    target: inputPanel
                    y: 0
                }
            },
            State {
                name: "HIDDEN_180"
                PropertyChanges {
                    target: inputPanel
                    y: -keyboardContainer.height
                }
            },
            State {
                name: "VISIBLE_270"
                PropertyChanges {
                    target: inputPanel
                    x: keyboardContainer.height - inputPanel.height
                }
            },
            State {
                name: "HIDDEN_270"
                PropertyChanges {
                    target: inputPanel
                    x: keyboardContainer.width
                }
            }
        ]

        transitions: [
             Transition {
                 NumberAnimation {
                     property: "y"
                 }
                 NumberAnimation {
                     property: "x"
                 }
             }
         ]
    }
}
