import QtQuick 2.3
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

Item {
    id: root

    property alias textColor: marqueeText.color
    property alias boldFont: marqueeText.font.bold
    property alias fontSize: marqueeText.font.pixelSize
    property alias fontFamily: marqueeText.font.family
    property alias text: marqueeText.text
    property alias scrollDuration: textAnimation.duration
    property bool enableScrolling

    height: marqueeText.height
    clip: true

//    Label {
//        id: marqueeText

//        verticalAlignment: Text.AlignVCenter
//        wrapMode: Text.NoWrap
//        x: textAnimation.running ? root.width : 0

//        NumberAnimation on x {
//            id: textAnimation
//            from: root.width/2
//            to: -(marqueeText.width - root.width/2);
//            duration: marqueeText.text.length * 300
//            loops: Animation.Infinite
//            running: marqueeText.width > root.width;
//        }
//    }

    Label {
        id: marqueeText
        text: "Interview - Huynh Phuong - Big data - 17-PMA-NA"
        x: textAnimation.running ? root.width : 0
        //anchors.horizontalCenter: parent.horizontalCenter
        //horizontalAlignment: Text.AlignHCenter
        width: lineCount <= 2 ? parent.width : 0
        font.family: montserrat.name
        font.pixelSize: 20
        //elide: Label.ElideRight
        wrapMode: Label.WordWrap
        maximumLineCount: 2
        color: "white"
        opacity: 0.6

        NumberAnimation on x {
            id: textAnimation
            from: root.width/2
            to: -(marqueeText.width - root.width/2);
            duration: marqueeText.text.length * 300
            loops: Animation.Infinite
            running: marqueeText.width > root.width;
        }
    }
}
