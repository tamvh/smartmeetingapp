import QtQuick 2.0

import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0

import com.vng.gbc.smr 1.0

Page {
    header: ToolBar {
        Material.foreground: "white"
        Material.primary: "#f78c37"

        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: stackView.depth > 1 ? "images/back.png" : "images/drawer.png"
                }
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop()
                    }
                }
            }

            Label {
                id: titleLabel
                text: "Settings"
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/menu.png"
                }
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Settings"
                        onTriggered: settingsPopup.open()
                    }
                    MenuItem {
                        text: "About"
                        onTriggered: aboutDialog.open()
                    }
                    MenuItem {
                        text: "Quit"
                        onTriggered: Qt.quit()
                    }
                }
            }
        }
    }

    Pane {
        id: settingsPane
        property bool changed: false

        anchors.fill: parent
            ColumnLayout {
            id: settingsColumn
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter

            RowLayout {
                spacing: 10

                Label {
                    id: lblRestart
                    text: "Restart required"
                    color: "#e41e25"
                    visible: settingsPane.changed === true ? true : false
                    Layout.fillWidth: true
                }
            }

            // Row setting server url
            RowLayout {
                spacing: 10

                Label {
                    text: "Server:"
                }

                TextField {
                    id: txtServerUrl
                    Layout.fillWidth: true;
                    Component.onCompleted: {
                        text = AppSetting.serverUrl()
                    }

                    onTextChanged: {
                        settingsPane.changed = true;
                    }
                }

                Button {
                    text: "Save"
                    onClicked: {
                        AppSetting.setServerUrl(txtServerUrl.text);
                        AppSetting.save();
                        console.log("already save");
                    }
                }
            }

            RowLayout {
                spacing: 10

                Label {
                    text: "Book room url:"
                }

                TextField {
                    id: txtBookRoomUrl
                    Layout.fillWidth: true;
                    Component.onCompleted: {
                        text = AppSetting.bookRoomUrl()
                    }

                    onTextChanged: {
                        settingsPane.changed = true;
                    }
                }

                Button {
                    text: "Save"
                    onClicked: {
                        AppSetting.setBookRoomUrl(txtBookRoomUrl.text);
                        AppSetting.save();
                        console.log("already save");
                    }
                }
            }

            RowLayout {
                id: roomSettings
                spacing: 10

                Label {
                    text: "Room:"
                }

                ComboBox {
                    id: roomCombobox
                    property int styleIndex: -1
                    textRole: "name"
                    model: ListModel {
                        id: roomModel
                    }

                    function loadRoomList() {
                        roomModel.clear();
                        roomModel.append(SettingsController.getRoomList())
                        var roomId = AppSetting.roomId();
                        for (var i = 0; i < roomModel.count; i++) {
                            if (roomModel.get(i).id === roomId) {
                                currentIndex = i;
                                settingsPane.changed = false;
                                return;
                            }
                        }
                    }

                    Component.onCompleted: {
                        loadRoomList();
                    }

                    onCurrentIndexChanged: {
                        settingsPane.changed = true;
                    }

                    Layout.fillWidth: true;
                }

                Button {
                    text: "Update"
                    onClicked: {
                        roomCombobox.loadRoomList();
                    }
                }
            }

            // Row setting use last update
            RowLayout {
                spacing: 10

                Label {
                    text: "Use api last update:"
                }

                CheckBox {
                    id: cbLastUpdate
                    Component.onCompleted: {
                        checked = AppSetting.getUseLastUpdate();
                        settingsPane.changed = false;
                    }
                    onCheckedChanged: {
                        settingsPane.changed = true;
                    }
                }
            }

            // Row setting update interval
            RowLayout {
                spacing: 10

                Label {
                    text: "Update interval:"
                }

                TextField {
                    id: txtUpdateInterval
                    Layout.fillWidth: true;

                    Component.onCompleted: {
                        text = AppSetting.getUpdateInterval()
                    }

                    onTextChanged: {
                        settingsPane.changed = true;
                    }
                }

                Label {
                    text: "ms"
                }
            }

            // Row setting room info interval
            RowLayout {
                spacing: 10

                Label {
                    text: "Update room interval:"
                }

                TextField {
                    id: txtRoomInfoInterval
                    Layout.fillWidth: true;

                    Component.onCompleted: {
                        text = AppSetting.getUpdateRoomInterval()
                    }

                    onTextChanged: {
                        settingsPane.changed = true;
                    }
                }

                Label {
                    text: "ms"
                }
            }

            // Row save settings button
            RowLayout {
                spacing: 10

                Button {
                    text: "Save"
                    onClicked:  {
                        if (true === settingsPane.changed) {
                            AppSetting.setRoomId(roomModel.get(roomCombobox.currentIndex).id);
                            AppSetting.setRoomName(roomModel.get(roomCombobox.currentIndex).name);
                            AppSetting.setServerUrl(txtServerUrl.text);
                            AppSetting.setUpdateInterval(parseInt(txtUpdateInterval.text));
                            AppSetting.setUpdateRoomInterval(parseInt(txtRoomInfoInterval.text));
                            AppSetting.setUseLastUpdate(cbLastUpdate.checked);

                            AppSetting.save();
                        }
                    }
                }
            }

            RowLayout {
                spacing: 10

                Label {
                    text: "Test LED"
                }

                Button {
                    text: "Red"
                    onClicked:  {
                        console.log("Turn on red");
                        SettingsController.testRed();
                    }
                }

                Button {
                    text: "Green"
                    onClicked:  {
                        SettingsController.testGreen();
                    }
                }

                Button {
                    text: "Blue"
                    onClicked:  {
                        SettingsController.testBlue();
                    }
                }

                Button {
                    text: "Off"
                    onClicked:  {
                        SettingsController.testOff();
                    }
                }

                Button {
                    text: "Test crash"
                    onClicked:  {
                        SettingsController.testCrash();
                    }
                }
            }
        }
    }
}
