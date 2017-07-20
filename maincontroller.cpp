#include "maincontroller.h"
#include "meetingfeed.h"
#include "wsclient.h"

#include "appsetting.h"
#include "utils.h"
#include "commonfunction.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QTimerEvent>
#include <QDebug>
#include <QUuid>

#define DT_FORMAT "yyyy-MM-dd HH:mm:ss.z"
//#define CHECK_HAPPEN_INTERVAL 60000
//#define CHECK_EVENT_INTERVAL 10000
// Real url "https://gbc.zing.vn/mrm/api/event?cm=getlistbyroom"
//#define EVENT_URL "http://gbc.zing.vn/dev/mrm/api/event?cm=getlistbyroom"
#define SL_WEBSOCKET        "ws://gbcstaging.zing.vn/cloud/ntf/?session_id="
#define MAX_EVENT_VIEW 2

MainController::MainController(QObject *parent) : QObject(parent),
    _lightFlag(Utils::Off)
{
    QString deviceId = QString(QUuid::createUuid().toRfc4122().toHex());
    m_wsClient = new WSClient(QUrl(QString("%1%2").arg(SL_WEBSOCKET).arg(deviceId)));
    QObject::connect(m_wsClient, SIGNAL(textMessageReceived(QString)), this, SLOT(onNotify(const QString&)));
}

MainController::~MainController()
{

}

int MainController::initialize()
{
#ifdef Q_OS_ANDROID
    // Turn off light on start up
    Utils::getInstance()->turnOff();
#endif
    _meetingFeed = new MeetingFeed(AppSetting::getInstance()->getUpdateInterval(), AppSetting::getInstance()->getUpdateRoomInterval(), this);
    QObject::connect(_meetingFeed, SIGNAL(meetingChanged(QVariant)), this, SLOT(onMeetingChanged(QVariant)));
    QObject::connect(_meetingFeed, SIGNAL(updateMeetingDone(bool)), this, SIGNAL(updateDone()));
    QObject::connect(_meetingFeed, SIGNAL(updateMeetingBegin()), this, SIGNAL(updateBegin()));
    QObject::connect(_meetingFeed, SIGNAL(roomInfoChanged(QVariant)), this, SIGNAL(roomInfoChanged(QVariant)));
    QObject::connect(_meetingFeed, SIGNAL(roomInformation(QString)), this, SLOT(roomInformation(QString)));
    QObject::connect(_meetingFeed, SIGNAL(currentTime(QVariant)), this, SLOT(currentTime(QVariant)));

    _meetingFeed->initialize();

    _timerCheckEventHappen = startTimer(AppSetting::getInstance()->checkHappenInterval());
}

void MainController::roomInformation(const QString &data) {
//    qDebug() << "roomInformation: " << data;
    int area_id = CommonFunction::getAreaId(data);
    m_area_id = area_id;
}

void MainController::currentTime(const QVariant &data) {
//     qDebug() << "currentTime: " << data;
     QString _time = CommonFunction::getCurrentTimeInServer(data.toString());
     if(!_time.isEmpty()) {
        _currTimeInServer = _time;
        emit currentTimeInServer(_time);
     }
}

void MainController::uninitialize()
{
    killTimer(_timerCheckEventHappen);

    _meetingFeed->uninitialize();
    if (Q_NULLPTR != _meetingFeed)
        delete _meetingFeed;

#ifdef Q_OS_ANDROID
    // Turn off light on start up
    Utils::getInstance()->turnOff();
#endif
}

MeetingFeed *MainController::meetingFeed() const
{
    return _meetingFeed;
}

void MainController::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _timerCheckEventHappen) {
        checkMeetingHappen(false);
    }
}

void MainController::onMeetingChanged(const QVariant &newMeetingData)
{
    _meetingInfo = newMeetingData;
    qDebug() << "_meetingInfo: " << _meetingInfo.toJsonArray();
    //preProcessMeeting();
    checkMeetingHappen(true);
}

void MainController::onNotify(const QString &message) {
//    qDebug() << "message notify: " << message;
    int msgType = CommonFunction::parseMsgType(message);
    if (msgType == 10)
    {
        // update sensor data
        bool haveperson;
        int temperature = -1, humidity = -1;
        QString area_id = "";
        QString devId, gwId;

        if( CommonFunction::parseMsgSensors(message, devId, gwId, haveperson, temperature, humidity, area_id) )
        {
            //12
            if(area_id == QString::number(m_area_id)) {
                emit refreshSensors(CommonFunction::initJson(devId, gwId, temperature, humidity, area_id));
            }
        }
    }
}

void MainController::checkMeetingHappen(bool meetingUpdate)
{
    ///
    /// Check meeting happen now
    ///
    qDebug() << "Check happend event " << meetingUpdate;

    QJsonArray dt = _meetingInfo.toJsonArray();
    QJsonObject obj;
    QDateTime startEvent, endEvent;
    QDateTime now = QDateTime::currentDateTime();
    QDateTime nowTimeInServer = QDateTime::fromString(_currTimeInServer, "yyyy-MM-dd hh:mm:ss");

    qDebug() << "nowTime local: " << now;
    qDebug() << "nowTime in server: " << nowTimeInServer;

    int light_flag = Utils::Green;

    // Fill free event
    QDate currDate = nowTimeInServer.date();
    QString lastDate = currDate.toString("yyyy-MM-dd");
    if (lastDate != _lastDate) {
        _dtFrees.clear();
        for (int i = 8; i < 21; i++) {
            _dtFrees.append(QDateTime(currDate, QTime(i,0,0)));
            _dtFrees.append(QDateTime(currDate, QTime(i,30,0)));
        }
        _lastDate = lastDate;
    }

    int size;
    //if (meetingUpdate) {
        QJsonObject jobjFree;
        QDateTime dtStartFree;
        bool available;
        for (int i = 0; i < _dtFrees.size(); i++) {
            dtStartFree = _dtFrees.at(i);
            //if (dtStartFree >= now) {
                available = true;
                size = dt.size();
                for (int j = 0; j < size; j++) {
                    obj = dt.at(j).toObject();
                    startEvent = QDateTime::fromString(obj["startEvent"].toString(), DT_FORMAT);
                    endEvent = QDateTime::fromString(obj["endEvent"].toString(), DT_FORMAT);
                    if (_dtFrees.at(i) >= startEvent && _dtFrees.at(i) < endEvent) {
                        available = false;
                    }
                }

                if (available) {
                    jobjFree["startEvent"] = dtStartFree.toString(DT_FORMAT);
                    jobjFree["endEvent"] = dtStartFree.addSecs(60*30).toString(DT_FORMAT);
                    jobjFree["available"] = true;
                    jobjFree["title"] = "";
                    jobjFree["organizer"] = "";
                    dt.append(jobjFree);
                }
            //}
        }
        //_meetingInfo = dt;
    //}

    // Clear passed event
    size = dt.size();
    for (int i = size - 1; i >= 0; i--) {
        obj = dt.at(i).toObject();
        endEvent = QDateTime::fromString(obj["endEvent"].toString(), DT_FORMAT);
        if (endEvent < nowTimeInServer) {
            // Event is over
            /// TEST don't clear old event
            dt.removeAt(i);
            qDebug() << "Event " << obj["endEvent"].toString() << " is over";
        } else {
            qDebug() << "Event " << obj["endEvent"].toString() << " is OK";
            if (obj["available"].isNull())
                obj["available"] = false;
            dt.replace(i, obj);
        }
    }

    // Merge two event type

    ///
    /// Sort by start event time
    ///
    //if (meetingUpdate) {
        QJsonObject tmpObj;
        QDateTime tm, tmTmp;
        for (int i = 0; i < dt.size(); i++) {
            for (int j = 0; j < (dt.size() - 1) ; j++) {
                obj = dt.at(j).toObject();
                tmpObj = dt.at(j+1).toObject();
                tm = QDateTime::fromString(obj["startEvent"].toString(), DT_FORMAT);
                tmTmp = QDateTime::fromString(tmpObj["startEvent"].toString(), DT_FORMAT);
                if (tm > tmTmp) {
                    dt.replace(j, tmpObj);
                    dt.replace(j + 1, obj);
                }
            }
        }
    //}

    // Check happend event
    int currEventIdx = -1;
    bool firstUnavailableEvent = false;
    for (int i = 0; i < dt.size(); i++) {        
       obj = dt.at(i).toObject();
       startEvent = QDateTime::fromString(obj["startEvent"].toString(), DT_FORMAT);
       endEvent = QDateTime::fromString(obj["endEvent"].toString(), DT_FORMAT);

       if (obj["available"].toBool() == false) {
           // Check first event
           if (false == firstUnavailableEvent) {
               firstUnavailableEvent = true;
               if (firstUnavailableEvent) {
                   if (startEvent <= nowTimeInServer.addSecs(60*60)) {
                       qDebug() << "Event will happen in one hour";
                       light_flag = Utils::Blue;
                   }
               }
           }

           if (startEvent <= nowTimeInServer && endEvent >= nowTimeInServer) {
               obj["happenning"] = true;
               light_flag = Utils::Red;
               currEventIdx = i;
           }
           else
               obj["happenning"] = false;
           dt.replace(i, obj);
       }
    }

    if (currEventIdx != -1) {
        obj = dt.at(currEventIdx).toObject();
        if (obj != _lastCurrent) {
            emit currentEventChanged(obj);
            _lastCurrent = obj;
        } else {
            qDebug() << "Already set current";
        }
        dt.removeAt(currEventIdx);
    } else {
        if (!_lastCurrent.isNull()) {
            emit currentEventAvailable();
            _lastCurrent = QVariant();
        } else {
            qDebug() << "Already set available";
        }
    }

    qDebug() << "New meeting size " << dt.size();

   // Fill empty event
//   size = dt.size();
//   for (int i = 0; i < MAX_EVENT_VIEW - size; i++) {
//       QJsonObject jevent;
//       jevent["empty"] = true;
//       jevent["startEvent"] = "1970-01-01 00:00:00.0";
//       jevent["endEvent"] = "1970-01-01 00:00:00.0";
//       jevent["title"] = "";
//       jevent["comment"] = "";
//       jevent["organizer"] = "";
//       jevent["happenning"] = false;
//       dt.append(jevent);

//       qDebug() << "Append empty event, new size " << dt.size();
//   }

   if (_lastMeetingInfo != dt) {
//       qDebug() << "Meeting changed by happen " << _lastMeetingInfo;
       _lastMeetingInfo = dt;
       emit meetingChanged(_lastMeetingInfo);
   }
#ifdef Q_OS_ANDROID
    if (_lightFlag != light_flag) {
        Utils::controlLight(light_flag);
        _lightFlag = light_flag;
    }
#endif
}
