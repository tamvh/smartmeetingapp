#include "settingscontroller.h"
#include "appsetting.h"
#include "utils.h"

#include <QJsonValue>
#include <QJsonArray>

SettingsController::SettingsController(QObject *parent) : QObject(parent)
{

}

QVariant SettingsController::getRoomList()
{
    QString url = QString("%1%2").arg(AppSetting::getInstance()->serverUrl()).arg("room?cm=getlist");

    QNetworkReply* reply = Utils::getInstance()->syncGet(url);

    if (reply->error() == QNetworkReply::NoError) {
        QString response = reply->readAll();
//        qDebug() << "Get room list res: " << response;
        QJsonValue jdt;
        int perr = Utils::parseServerResponse(response, jdt);
        if (0 == perr) {
            return jdt.toArray();
        }
    } else {

    }

    delete reply;

    return QVariant();
}

void SettingsController::testRed()
{
    Utils::turnOnRed();
}

void SettingsController::testGreen()
{
    Utils::turnOnGreen();
}

void SettingsController::testBlue()
{
    Utils::turnOnBlue();
}

void SettingsController::testOff()
{
    Utils::turnOff();
}

void SettingsController::testCrash()
{
    QObject* test = Q_NULLPTR;
    QString string = test->objectName();
}
