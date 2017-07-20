QT += qml quick quickcontrols2 network websockets

CONFIG += c++11

SOURCES += main.cpp \
    maincontroller.cpp \
    meetingfeed.cpp \
    settingscontroller.cpp \
    appsetting.cpp \
    utils.cpp \
    ledcontroller.cpp \
    commonfunction.cpp \
    qqrencode.cpp \
    configsetting.cpp \
    colorimageprovider.cpp \
    wsclient.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    maincontroller.h \
    meetingfeed.h \
    settingscontroller.h \
    appsetting.h \
    utils.h \
    ledcontroller.h \
    version.h \
    commonfunction.h \
    qqrencode.h \
    qqrencode_p.h \
    configsetting.h \
    colorimageprovider.h \
    wsclient.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/src/com/vng/gbc/smr/MyActivity.java \
    android/src/com/vng/gbc/smr/BootUpReceiver.java

android {
    DEFINES += AUTO_HIDE_NAVI
    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
#        LIBS+= -L/Users/baohavan/Dev/android/libraries/output/armeabi-v7a/lib -lssl -lcrypto
        #DEFINES += USE_BREAKPAD
        #include(qBreakpad/qBreakpad.pri)
    }
    contains(ANDROID_TARGET_ARCH,x86) {
        LIBS+= -L/Users/baohavan/Dev/android/libraries/output/x86/lib -lssl -lcrypto
    }
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS =
}

include(qrencode-3.4.4/qrencode.pri)
INCLUDEPATH += $$PWD/qrencode-3.4.4
DEFINES += HAVE_CONFIG_H
