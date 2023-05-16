import QtQuick 2.15

Rectangle {
    id: screenSaverRoot
    color: "black"

    Rectangle {
        id: ball
        width:100
        height:100
        color: "plum"
        radius: width / 2
        x:(screenSaverRoot.width - width)/2
        y:(screenSaverRoot.height - height)/2
    }

    onWidthChanged: {
        tx = width - ball.width
        animx.restart();
    }

    onHeightChanged: {
        ty = height - ball.height
        animy.restart();
    }
    Component.onCompleted: {
        animx.restart();
        animy.restart();
    }

    property real tx
    property real ty

    SequentialAnimation {
        id: animx
        loops: Animation.Infinite
        PropertyAnimation {
            id: a1
            target: ball
            property: "x"
            to: tx
            duration: tx * 5
        }
        PropertyAnimation {
            target: ball
            property: "x"
            to: 0
            duration: tx * 5
        }
    }

    SequentialAnimation {
        id: animy
        loops: Animation.Infinite
        PropertyAnimation {
            target: ball
            property: "y"
            to: ty
            easing.type: Easing.OutBounce
            duration: ty * 5
        }
        PropertyAnimation {
            target: ball
            property: "y"
            to: 0
            easing.type: Easing.OutQuad
            duration: ty * 2
        }
    }

}
