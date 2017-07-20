#include "appsetting.h"
#include "settingscontroller.h"
#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


AppSetting* AppSetting::_instance = Q_NULLPTR;

AppSetting::AppSetting(QObject *parent) : QObject(parent)
{
    _settings = new QSettings();
    load();
}

AppSetting::~AppSetting()
{
    delete _settings;
}

void AppSetting::load()
{
    _roomId = _settings->value("settings/roomid", -1).toInt();
    _roomName = _settings->value("settings/roomname", "Not set").toString();
    _serverUrl = _settings->value("settings/serverurl", "https://gbc.zing.vn/mrm/api/").toString();
    _bookroomUrl = _settings->value("settings/bookroomurl", "https://gbc.zing.vn/mrm/#/event/mobile").toString();
    _updateInterval = _settings->value("settings/updateinterval", 10000).toInt();
    _updateRoomInterval = _settings->value("settings/roomupdateinterval", 30000).toInt();
    _useLastUpdate = _settings->value("settings/uselastupdate", true).toBool();
}

void AppSetting::save()
{
    _settings->setValue("settings/roomid", _roomId);
    _settings->setValue("settings/roomname", _roomName);
    _settings->setValue("settings/serverurl", _serverUrl);
    _settings->setValue("settings/bookroomurl", _bookroomUrl);
    _settings->setValue("settings/updateinterval", _updateInterval);
    _settings->setValue("settings/roomupdateinterval", _updateRoomInterval);
    _settings->setValue("settings/uselastupdate", _useLastUpdate);

    _settings->sync();
}

int AppSetting::roomId() const
{
    return _roomId;
}

QString AppSetting::roomName() const
{
    return _roomName;
}

int AppSetting::checkHappenInterval() const
{
    return 30000;
}

int AppSetting::checkEventInterval() const
{
    return 10000;
}

void AppSetting::setRoomName(const QString &roomName)
{
    _roomName = roomName;
}

void AppSetting::setRoomId(int roomId)
{
    _roomId = roomId;
}

QString AppSetting::serverUrl() const
{
    return _serverUrl;
    // For test only
    //return "https://gbc.zing.vn/mrm/api/";
}

void AppSetting::setServerUrl(const QString &serverUrl)
{
    _serverUrl = serverUrl;
}

QString AppSetting::bookRoomUrl() const {
    return _bookroomUrl;
}

void AppSetting::setBookRoomUrl(const QString &bookRoomUrl) {
    _bookroomUrl = bookRoomUrl;
}

int AppSetting::getUpdateInterval() const
{
    return _updateInterval;
}

void AppSetting::setUpdateInterval(int updateInterval)
{
    _updateInterval = updateInterval;
}

bool AppSetting::getUseLastUpdate() const
{
    return _useLastUpdate;
}

void AppSetting::setUseLastUpdate(bool useLastUpdate)
{
    _useLastUpdate = useLastUpdate;
}

int AppSetting::getUpdateRoomInterval() const
{
    return _updateRoomInterval;
}

void AppSetting::setUpdateRoomInterval(int updateRoomInterval)
{
    _updateRoomInterval = updateRoomInterval;
}
