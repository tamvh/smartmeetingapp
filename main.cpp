#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "colorimageprovider.h"
#include "maincontroller.h"
#include "settingscontroller.h"
#include "appsetting.h"
#include "configsetting.h"
#include "utils.h"

static QObject *main_controller_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new MainController();
}

static QObject *settings_controller_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new SettingsController();
}

static QObject *app_setting_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return AppSetting::getInstance();
}

#ifdef USE_BREAKPAD
#include "QBreakpadHandler.h"
#endif

int main(int argc, char *argv[])
{
#ifdef AUTO_HIDE_NAVI
    Utils::hideNavigateBar();
#endif

#ifdef USE_BREAKPAD
    QBreakpadInstance.setDumpPath("/mnt/sdcard/smr/crash");
    //QBreakpadInstance.setUploadUrl(QUrl("http://192.168.6.111/crash_upload"));
    //QBreakpadInstance.sendDumps();
#endif
    ConfigSetting conf;
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("GBC - VNG");
    QCoreApplication::setOrganizationDomain("com.vng.gbc.smr");
    QCoreApplication::setApplicationName("Smart Meeeting Room");
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    QObject::connect(&engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

    qmlRegisterSingletonType<MainController>("com.vng.gbc.smr", 1, 0, "MainController", main_controller_provider);
    qmlRegisterSingletonType<SettingsController>("com.vng.gbc.smr", 1, 0, "SettingsController", settings_controller_provider);
    qmlRegisterSingletonType<AppSetting>("com.vng.gbc.smr", 1, 0, "AppSetting", app_setting_provider);
    engine.addImageProvider("colors", new ColorImageProvider(4.0));
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    int ret = app.exec();

#ifdef AUTO_HIDE_NAVI
    Utils::showNavigateBar();
#endif

    return ret;
}
