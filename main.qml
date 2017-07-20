import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Extras 1.4

import "controls" as Awesome

ApplicationWindow {
    id: window
    visible: true
    visibility: Qt.platform.os === "android" ? "FullScreen" : "Windowed"
    width: 1024
    height: 600
    property int globalPadding: 10
    title: qsTr("Smart Meetting Room")

    FontLoader {
        id: montserrat;
        source: "qrc:///resource/Montserrat-Regular.ttf"
    }

    FontLoader {
        source: "qrc:///resource/Montserrat-Bold.ttf"
    }

    FontLoader {
        source: "qrc:///resource/Montserrat-Medium.ttf"
    }

    FontLoader {
        source: "qrc:///resource/Montserrat-Light.ttf"
    }

    font.family: montserrat.name

    FontAwesome {
        id: awesome
        resource: "qrc:///resource/fontawesome-webfont.ttf"
    }

    SettingsPage {
        id: settingPage
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: MainPage {
            id: mainPage
        }
    }




}
