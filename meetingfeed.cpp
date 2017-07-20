#include "meetingfeed.h"
#include "appsetting.h"
#include "utils.h"
#include "commonfunction.h"

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QTimerEvent>
#include <QDebug>

MeetingFeed::MeetingFeed(int feedInterval, int feedRoomInterval, QObject *parent) : QObject(parent),
    _timerUpdate(0),
    _feedInterval(feedInterval),
    _timerRoomUpdate(0),
    _feedRoomInterval(feedRoomInterval)
{

}

int MeetingFeed::initialize()
{
    qDebug() << "Init meeting feed settings: "
             << "Server url: " << AppSetting::getInstance()->serverUrl()
             << "Update interval: " << AppSetting::getInstance()->getUpdateInterval()
             << "Update room interval: " << AppSetting::getInstance()->getUpdateRoomInterval();
    _updateUrl = QString("%1%2").arg(AppSetting::getInstance()->serverUrl(), "event?cm=getlistbyroom");
    _lastModifiedUrl = QString("%1%2").arg(AppSetting::getInstance()->serverUrl(), "room?cm=getlastmodify");
    _roomInfoUrl = QString("%1%2").arg(AppSetting::getInstance()->serverUrl(), "room?cm=getinfo");
    _currentTimeUrl = AppSetting::getInstance()->serverUrl() + "util?cm=getcurrtime";
     getCurrentTime();
    _timerUpdate = startTimer(_feedInterval);
    if ( _timerUpdate <= 0) {
        qWarning() << "Can't create feed interval";
        return -1;
    }

    _timerRoomUpdate = startTimer(_feedRoomInterval);
    if (_timerRoomUpdate <= 0) {
        qWarning() << "Can't create feed room interval";
        return -1;
    }

//    _networkMgr = new QNetworkAccessManager(this);
//    _eventLoop = new QEventLoop(this);

    //QObject::connect(_networkMgr, SIGNAL(finished(QNetworkReply*)), _eventLoop, SLOT(quit()));

    checkRoomInfoData();
    checkMeetingData();

    return 0;
}

void MeetingFeed::uninitialize()
{
    if (_timerUpdate)
        killTimer(_timerUpdate);

    if (_timerRoomUpdate)
        killTimer(_timerRoomUpdate);


//    if (_eventLoop)
//        delete _eventLoop;

//    if (_networkMgr)
//        delete _networkMgr;
}

void MeetingFeed::timerEvent(QTimerEvent *event)
{
    if (_timerUpdate == event->timerId()) {
//        qDebug() << "Timer check event feed " << QTime::currentTime();
        getCurrentTime();
        checkMeetingData();
    } else if (event->timerId() == _timerRoomUpdate) {
        qDebug() << "Timer check room info change" << QTime::currentTime();
        checkRoomInfoData();
    }
}

int MeetingFeed::checkLastModified()
{
    int ret = -1;
    // Setup data post
    QJsonObject jdata;
    jdata["id"] = AppSetting::getInstance()->roomId();

    qDebug() << "Check last update data: " << jdata;

    QNetworkReply *reply = Utils::getInstance()->syncPost(_lastModifiedUrl, QJsonDocument(jdata).toJson());

    if (Q_NULLPTR == reply) {
        qWarning() << "Check last update null reply";
        return -1;
    }

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Request success";
        QString response = QString(reply->readAll());
//        qDebug() << "Last modified data: " << response;

        ret = processLastModified(response);
    }

    delete reply;

    return ret;
}

int MeetingFeed::processLastModified(const QString &response)
{
    QJsonValue jdt;
    int perr = Utils::parseServerResponse(response, jdt);
    if (0 == perr) {
        QString dt = jdt.toString();
        if (!jdt.isString()) {
            qWarning() << "Reponse dt not string";
            return -1;
        }

        if (_lastModified != dt) {
            qDebug() << "Last modified has changed";
            _lastModified = dt;
            return 1;
        }

        qDebug() << "Last modified not changed data is " << dt;
        return 0;
    }

    qDebug() << "Have error while get last modified data";
    return -2;
}

void MeetingFeed::getCurrentTime() {
    QNetworkReply *reply = Utils::getInstance()->syncGet(_currentTimeUrl);

    if (Q_NULLPTR == reply) {
        qWarning() << "Check current time null reply";
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Request success";
        QVariant response = reply->readAll();
        _currentTime = response;
//        qDebug() << "current time: " << response.toString();
        emit currentTime(response);
    } else {
        qDebug() << "Have error while get current time " << reply->errorString();
    }

    delete reply;
}

QString MeetingFeed::getCurrTimeInServer() {
    QString _time = CommonFunction::getCurrentTimeInServer(_currentTime.toString());
     if(!_time.isEmpty()) {
        return  _time;
     }
    return "";
}

void MeetingFeed::checkMeetingData()
{
    emit updateMeetingBegin();

    QDateTime now = QDateTime::fromString(this->getCurrTimeInServer(), "yyyy-MM-dd hh:mm:ss"); //QDateTime::currentDateTime();
    qDebug() << "QDateTime::" << now;
    QString currDate = now.toString("yyyy-MM-dd");

    if (AppSetting::getInstance()->getUseLastUpdate()) {
//        qDebug() << "Use last update api";
        int ilastupdate = checkLastModified();
        if (ilastupdate < 0) {
            emit updateMeetingDone(false);
            return;
        } else if (ilastupdate == 0) {
            if (currDate == _currDate) {
                emit updateMeetingDone(true);
                return;
            }
            qDebug() << "Force change by date";
            _currDate = currDate;
        } else {
            _currDate = currDate;
        }
    } else {
        qDebug() << "Don't use last update api";
    }

    // Setup data post
    QJsonObject jdata;
    QDateTime begin, end;
    begin = end = now;
    begin.setTime(QTime(0,0));
    end.setTime(QTime(23,59,59,999));
    jdata.insert("date1", begin.toString("yyyy-MM-dd HH:mm:ss"));
    // Test date
    //jdata.insert("date1", "2017-03-02 00:00:00");

    jdata.insert("date2", end.toString("yyyy-MM-dd HH:mm:ss"));
    // Test date
    //jdata.insert("date2", "2017-03-17 23:59:59");
    jdata.insert("room_id", AppSetting::getInstance()->roomId());

    qDebug() << "Request date " << jdata;

    QNetworkReply *reply = Utils::getInstance()->syncPost(_updateUrl, QJsonDocument(jdata).toJson());

    if (Q_NULLPTR == reply) {
        qWarning() << "Check meeting data null reply";
    }

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Request success";
        QString response = QString(reply->readAll());
        qDebug() << "Meeting data: " << response;

        if (processMeetingData(response) > 0) {
            qDebug() << "Meeting data has changed";
        }

        emit updateMeetingDone(true);
    } else {
        qWarning() << "Request error: " << reply->errorString();
        emit updateMeetingDone(false);
    }

    delete reply;
}

int MeetingFeed::processMeetingData(const QString& response)
{
    QJsonValue jdt;
    int perr = Utils::parseServerResponse(response, jdt);
    if (0 == perr) {
        QJsonArray dt = jdt.toArray();
        if (!jdt.isArray()) {
            qWarning() << "Reponse dt not array";
            return -1;
        }

        if (_meetingData != response) {
            emit meetingChanged(dt);
            _meetingData = response;
            return 1;
        }

        return 0;
    }

    qDebug() << "Data not changed";
    return -2;
}

void MeetingFeed::checkRoomInfoData()
{
    // Setup data post
    QJsonObject jdata;
    jdata["id"] = AppSetting::getInstance()->roomId();

//    qDebug() << "Check room info data: " << jdata;

    QNetworkReply *reply = Utils::getInstance()->syncPost(_roomInfoUrl, QJsonDocument(jdata).toJson());

    if (Q_NULLPTR == reply) {
        qWarning() << "Check room info null reply";
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Request success";
        QString response = QString(reply->readAll());
        qDebug() << "Room info data: " << response;
        emit roomInformation(response);

        if (processRoomInfoData(response) > 0) {
            qDebug() << "Room info has changed";
        }
    } else {
        qDebug() << "Have error while get room info " << reply->errorString();
    }

    delete reply;
}

int MeetingFeed::processRoomInfoData(const QString &response)
{
    QJsonValue jdt;
    int perr = Utils::parseServerResponse(response, jdt);
    if (0 == perr) {
        QJsonObject dt = jdt.toObject();
        if (!jdt.isObject()) {
            qWarning() << "Reponse dt not object";
            return -1;
        }

        if (_roomInfo != response) {
            qDebug() << "Room info has changed";
            _roomInfo = response;
            emit roomInfoChanged(dt);
            return 1;
        }

//        qDebug() << "Room info not changed data is " << dt;
        return 0;
    }

    qDebug() << "Have error while get room info";
    return -2;
}


