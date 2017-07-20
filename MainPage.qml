import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Material.impl 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

import com.vng.gbc.smr 1.0

Page {
    anchors.fill: parent
    property int globalPadding: 10
    property string _datetime: qsTr("")
    property string _roomid: qsTr("")
    property string url: qsTr("")
    property string starttime: qsTr("")
    property string endtime: qsTr("")
    property string currentTimeInServer: qsTr("")
    property int roomId: 0

    Connections {
        target: MainController
        onMeetingChanged: {
//            console.log("Meeting change ", JSON.stringify(newMeetingData))
            for (var i = 0; i < newMeetingData.length; i++) {
//                console.log("start event: ", newMeetingData[i].startEvent)
//                console.log("end event: ", newMeetingData[i].endEvent)
            }

            clock.updateTime()
            eventModel.clear()
            eventModel.append(newMeetingData)
        }

        onCurrentEventChanged: {
            //currentEvent.event = event
            //currentEvent.available = false
            console.log("onCurrentEventChanged ", event)
            clock.updateTime()
            currentEvent.reloadCurrent(event)
        }

        onCurrentEventAvailable: {
            //currentEvent.available = true
            console.log("onCurrentEventAvailable")
            clock.updateTime()
            currentEvent.reloadAvailable()
        }

        onRoomInfoChanged: {
//            console.log("onRoomInfoChanged: " + JSON.stringify(newData))
            lblTemperature.value = newData.temperature.toString()
            lblHumidity.value = newData.humidity.toString()
        }

        onRefreshSensors: {
//            console.log("onRefreshSensors: " + JSON.stringify(msg));

            if(parseInt(msg.humidity.toString()) > 0) {
                console.log("humidity: " + parseInt(msg.humidity.toString()));
                lblHumidity.value = msg.humidity.toString()
            }
            if(parseInt(msg.temperature.toString()) > 0) {
                console.log("temperature: " + parseInt(msg.temperature.toString()));
                lblTemperature.value = msg.temperature.toString()
            }
        }

        onCurrentTimeInServer: {
//            console.log("onCurrentTimeInServer: " + data);
            var _now = new Date(data);
            currentTimeInServer = data;
            txtFullDate.text = _now.toLocaleDateString(Qt.locale("vi_VN"), "dddd, d Tháng M, yyyy")
            currentEvent.clock = _now.toLocaleTimeString(Qt.locale(), "HH:mm");
        }

    }

    Timer {
        id: clock
        interval: 60000
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            updateTime();
//            currentEvent.event = eventModel.get(0)
//            console.log("current", now)
//            if (now.getSeconds() % 10 < 4) {
//                currentEvent.available = true
//                console.log("set to available")
//            } else {
//                currentEvent.available = false
//                console.log("set to unavailable")
//            }
        }

        function updateTime() {
            var now = new Date();
            //txtFullDate.text = now.toLocaleDateString(Qt.locale("vi_VN"), "dddd, d Tháng M, yyyy")
            //currentEvent.clock = now.toLocaleTimeString(Qt.locale(), "HH:mm");
        }
    }

    Timer {
        property bool userInteractive: false
        interval: 60000
        running: true
        repeat: true
        onTriggered: {
            listViewEvents.positionViewAtBeginning();
        }
    }

    // Main column
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Title section
        Image {
            id: titleRow
            Layout.preferredHeight: 70
            Layout.fillWidth: true

            source: "qrc:/images/background/BG_top.png"

            RowLayout {
                spacing: 0
                anchors.fill: parent

                Label {
                    Layout.preferredWidth: parent.width*2/5
                    text: AppSetting.roomName()
                    font.family: montserrat.name
                    Layout.leftMargin: 30
                    font.pixelSize: 30
                    font.bold: true
                    color: "#B1B1B1"
                    Layout.alignment: Qt.AlignHCenter

                    Button {
                        id: secret
                        property bool allow: false
                        opacity: 0.0
                        text: 'xxxxxx'
                        onClicked: {
                            allow = true
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 20
                    Label {
                        font.family: awesome.family
                        font.pointSize: 20
                        color: "#B1B1B1"
                        renderType: Text.NativeRendering
                        text: awesome.loaded ? awesome.icons.fa_calendar : ""
                    }

                    Label {
                        id: txtFullDate
                        Layout.fillWidth: true
                        font.family: montserrat.name
                        color: "#B1B1B1"
                        font.pointSize: 20
                        text: "No data"

                        Button {
                            opacity: 0.0
                            text: 'xxxxxx'
                            onClicked: {
                                if (secret.allow)
                                    stackView.push(settingPage)
                                secret.allow = false
                            }
                        }
                    }

                    // Environment condition
                    RowLayout {
                        spacing: 0
                        RowLayout {
                            spacing: 10
                            Layout.fillWidth: true
                            Label {
                                font.family: awesome.family
                                font.pointSize: 20
                                color: "#B1B1B1"
                                renderType: Text.NativeRendering
                                text: awesome.loaded ? awesome.icons.fa_thermometer_half : ""
                            }

                            Label {
                                property string value: "--"
                                id: lblTemperature
                                Layout.fillWidth: true
                                font.family: montserrat.name
                                color: "#B1B1B1"
                                font.pointSize: 20
                                text: value + "°C"
                            }
                        }

                        RowLayout {
                            spacing: 10
                            Layout.fillWidth: true
                            Label {
                                font.family: awesome.family
                                font.pointSize: 20
                                color: "#B1B1B1"
                                renderType: Text.NativeRendering
                                text: awesome.loaded ? awesome.icons.fa_tint : ""
                            }

                            Label {
                                property string value: "--"
                                id: lblHumidity
                                Layout.fillWidth: true
                                font.family: montserrat.name
                                color: "#B1B1B1"
                                font.pointSize: 20
                                text: value + "%"
                            }
                        }
                    }
                }
            }
        }

        // Content section
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            // Current meeting
            CurrentEvent {
                id: currentEvent
                Layout.fillHeight:  true
                Layout.preferredWidth: parent.width*2/5
            }

            // Next meeting
            // Event listview
            ListModel {
                id: eventModel2
                ListElement {
                    title: "Game Backend and Cloud (GBC) 02/2017 Game Backend and Cloud (GBC) 02/2017"
                    available: false
                    startEvent: "1970-01-01 14:00:00.000"
                    endEvent: "1970-01-01 14:30:00.000"
                    organizer: "baohv"
                }
                ListElement {
                    title: "Họp team GBC"
                    available: false
                    startEvent: "14:00"
                    stopEvent: "14:30"
                    organizer: "baohv"
                }
                ListElement {
                    title: "Họp team GBC"
                    available: false
                    startEvent: "14:00"
                    stopEvent: "14:30"
                    organizer: "baohv"
                }
                ListElement {
                    title: "Hop team gbc"
                    available: true
                    startEvent: "15:00"
                    stopEvent: "15:30"
                }
                ListElement {
                    title: "Họp team GBC"
                    available: false
                    startEvent: "14:00"
                    stopEvent: "14:30"
                    organizer: "baohv"
                }
                ListElement {
                    title: "Họp team GBC"
                    available: false
                    startEvent: "14:00"
                    stopEvent: "14:30"
                    organizer: "baohv"
                }
                ListElement {
                    title: "Họp team GBC"
                    available: false
                    startEvent: "14:00"
                    stopEvent: "14:30"
                    organizer: "baohv"
                }
            }

            ListModel {
                id: eventModel
            }

            ListView {
                id: listViewEvents
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                model: eventModel
                delegate: Component {
                    id: delegateComponent
                    Loader {
                        source: available ? "EventAvailable.qml" : "EventDetail.qml"
                    }
                }

                Label {
                    visible: eventModel.count > 0 ? false : true
                    text: "Không có dữ liệu"
                    anchors.centerIn: parent
                    font.family: montserrat.name
                    font.pixelSize: 20
                    color: "#a5a6a7"
                }
            }
        }
    }

    Component.onCompleted: {
        MainController.initialize();
    }

    Component.onDestruction: {
        MainController.uninitialize();
    }

    Popup {
        id: qrdialog
        modal: true
        focus: true
        x: (stackView.width - width) / 2
        y: Math.abs(stackView.height -  qrdialog.height)/2

        function resetQRCode() {
            qrimage.setImage("")
            busyIndicatorQR.visible = true
        }

        function resetTimer() {
            qrTimer.restartTimerCounter();
        }
        Column {
            id: aboutColumn
            width: Math.max(parent.height, parent.width)
            height: parent.height
            Column {
                id: rowContent
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: globalPadding

                Label {
                    id: labelTitle
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 18
                    font.weight: Font.DemiBold
                    text: "XÁC NHẬN ĐẶT PHÒNG"
                }
                Rectangle {
                    border.width: 1
                    border.color: "#E2E6E7"
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: childrenRect.width + 2
                    height: childrenRect.height + 2
                    Image {
                        id: qrimage
                        visible: false
                        x: 1
                        y: 1
                        function setImage(url) {source=url;busyIndicatorQR.visible=false}
                    }
                    Component.onCompleted: {
                        qrimage.visible = true
                        roomId = AppSetting.roomId()
//                        url = AppSetting.serverUrl().replace("api/", "") + "#/event/mobile?"
                        url = AppSetting.bookRoomUrl()
                    }

                }

                Label {
                    id: labelQR
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Quét QR code để đặt phòng họp"
                    font.pixelSize: 15
                }

                Label {
                    id: lblstarttime
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Bắt đầu: " + starttime
                    font.pixelSize: 17
                    font.bold: true
                }

                Label {
                    id: lblPhong
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Tên phòng: " + AppSetting.roomName()
                    font.pixelSize: 17
                    font.bold: true
                }

                BusyIndicator {
                    id: busyIndicatorQR
                    width: 60
                    height: 60
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Label {
                        id: labelTimer
                        text: "Thời gian còn lại: "
                        font.pixelSize: 15
                    }

                    QrTimer {
                        id: qrTimer
                        parentId: qrdialog
                    }
                }
            }
            Row {
                width: parent.width
                Button {
                    text: "Hủy"
                    height: 60
                    width:  parent.width
                    ButtonStyle {
                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 25
                            border.width: control.activeFocus ? 2 : 1
                            border.color: "#FF9800"
                            radius: 4
                            gradient: Gradient {
                                GradientStop { position: 0 ; color: control.pressed ? "#FF9800" : "#FF9800" }
                                GradientStop { position: 1 ; color: control.pressed ? "#FF9800" : "#FF9800" }
                            }
                        }
                    }
                    onClicked: {
                        qrdialog.close();
                    }
                }
            }
        }
        onOpened: {
            qrTimer.parentId = 1;
            qrTimer.restartTimerCounter();
        }

        onClosed: {
            qrTimer.stopTimerCounter();
            resetQRCode()
        }
    }
}
