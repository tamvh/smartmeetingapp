import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0

import "controls" as Awesome

Item {
    width: listViewEvents.width
    height: detailColumn.height + 40

    Rectangle {
        anchors.fill: parent
        color: "#F2F2F2"
    }

    Rectangle {
        height: 1
        anchors.bottom: parent.bottom
        width: parent.width
        color: "#D6D6D6"
    }

    ColumnLayout {
        id: detailColumn
        anchors.left: parent.left
        anchors.right: parent.right
        //anchors.fill: parent
        anchors.margins: 20
        anchors.verticalCenter: parent.verticalCenter

        Label {
            text: title != undefined ? title : ""
            color: "#6e6864"
            font.family: montserrat.name
            font.pointSize: 24
            font.weight: Font.DemiBold

            Layout.fillWidth: true
            elide: Label.ElideRight
            wrapMode: Label.WordWrap
            maximumLineCount: 2
        }

        RowLayout {
            spacing: 10

            RowLayout {
                spacing: 10
                Label {
                    font.family: awesome.family
                    font.pointSize: 16
                    renderType: Text.NativeRendering
                    text: awesome.loaded ? awesome.icons.fa_clock_o : ""
                }
                Label {
                    text: startEvent != undefined && endEvent != undefined ? (new Date(Date.parse(startEvent))).toLocaleTimeString(Qt.locale(), "HH:mm") + " - " + (new Date(Date.parse(endEvent))).toLocaleTimeString(Qt.locale(), "HH:mm") : ""
                    font.family: montserrat.name
                    font.pointSize: 16
                }
            }

            RowLayout {
                spacing: 10
                Label {
                    font.family: awesome.family
                    font.pointSize: 16
                    renderType: Text.NativeRendering
                    text: awesome.loaded ? awesome.icons.fa_user_circle_o : ""
                }
                Label {
                    text: organizer != undefined ? organizer : ""
                    font.family: montserrat.name
                    font.pointSize: 16
                }
            }
        }
    }
}
