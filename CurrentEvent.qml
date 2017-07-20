import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import "controls" as Awesome

Item {
    id: root

    clip: true;
    property bool available: true
    property string clock : "--:--"
    property variant event: undefined

    function reloadCurrent(newEvent) {
        if (newEvent !== undefined) {
            console.log("new event ", newEvent.title)
            event = newEvent
            available = false

            titleLoader.sourceComponent = titleComponent
            eventDetailLoader.sourceComponent = eventDetailComponent
        }
    }

    function reloadAvailable() {
        titleLoader.sourceComponent = undefined;
        eventDetailLoader.sourceComponent = undefined;

        available = true
        event = undefined
    }

    Rectangle {
        anchors.fill: parent
        color: available? "#9BC832" : "#F77711"
    }

    Item {
        anchors.fill: parent
        anchors.bottomMargin: 30

        Rectangle {
            anchors.centerIn: parent
            width: parent.width*8.5/6
            height: width
            radius: width/2
            color: available? "#9ECB36" : "#F87C0F"
        }

        Rectangle {
            id:clockStatus
            anchors.centerIn: parent
            width: parent.width
            height: width
            radius: width/2
            color: available? "#A5D140" : "#F9830D"
        }

        Rectangle {
            id: clockRound
            anchors.centerIn: parent
            width: parent.width*4/6
            height: width
            radius: width/2
            color: available? "#ABD647" : "#FB8B0B"

            Label {
                anchors.centerIn: parent
                text: clock
                font.family: montserrat.name
                font.pixelSize: 100
                font.weight: Font.Medium
                color: "white"
            }
        }

        // Room status region
        Item {
            y: clockStatus.y
            height: clockRound.y - clockStatus.y
            width: parent.width

            Label {
                id: roomStatus
                text: available? "ĐANG TRỐNG" : "ĐANG HỌP"
                anchors.centerIn: parent
                font.family: montserrat.name
                font.pixelSize: 40
                font.weight: Font.Light
                color: "white"               
            }
        }

        // Event title
        Component {
            id: titleComponent
            Item {
                y: clockRound.y + clockRound.height
                height: clockRound.y - clockStatus.y
                width: root.width

                Label {
                    id: txtTitle
                    width: root.width - 40
                    //text: "Interview - Huynh Phuong - Big data - 17-PMA-NA"
                    text: event.title
                    anchors.centerIn: parent
                    //anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: montserrat.name
                    font.pixelSize: 22
                    elide: Label.ElideRight
                    wrapMode: Label.WordWrap
                    maximumLineCount: 4
                    color: "white"
                    opacity: 0.75
                }

    //            Awesome.Marquee {
    //                width: clockRound.width
    //            }
            }
        }

        Loader {
            id: titleLoader
            //sourceComponent: available ? undefined : titleComponent
        }

        Component {
            id: eventDetailComponent

            Item {
                y: clockStatus.y + clockStatus.height
                width: root.width

                RowLayout {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20
                    opacity: 0.65


                    RowLayout {
                        spacing: 10
                        Layout.fillWidth: true
                        Label {
                            font.family: awesome.family
                            font.pointSize: 16
                            color: "white"
                            renderType: Text.NativeRendering
                            text: awesome.loaded ? awesome.icons.fa_calendar : ""
                        }

                        Label {
                            Layout.fillWidth: true
                            font.family: montserrat.name
                            color: "white"
                            font.pointSize: 16
                            //text: "14:00 - 15:00"
                            text: (new Date(Date.parse(event.startEvent))).toLocaleTimeString(Qt.locale(), "HH:mm") + " - " + (new Date(Date.parse(event.endEvent))).toLocaleTimeString(Qt.locale(), "HH:mm");
                        }
                    }

                    RowLayout {
                        spacing: 10
                        Layout.fillWidth: true

                        Label {
                            font.family: awesome.family
                            font.pointSize: 16
                            color: "white"
                            renderType: Text.NativeRendering
                            text: awesome.loaded ? awesome.icons.fa_user_circle_o : ""
                        }

                        Label {
                            Layout.fillWidth: true
                            font.family: montserrat.name
                            color: "white"
                            font.pointSize: 16
                            text: event.organizer
                        }
                    }
                }
            }
        }

        Loader {
            id: eventDetailLoader           
        }
    }
}
