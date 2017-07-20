#ifndef MEETINGFEED_H
#define MEETINGFEED_H

#include <QObject>
#include <QString>
#include <QVariant>

//class QNetworkAccessManager;
//class QEventLoop;
class MeetingFeed : public QObject
{
    Q_OBJECT
public:
    explicit MeetingFeed(int feedInterval = 5000, int feedRoomInterval = 60000,  QObject *parent = 0);
    int initialize();
    void uninitialize();

signals:
    void meetingChanged(const QVariant& newData);
    void roomInfoChanged(const QVariant& newData);
    void updateMeetingBegin();
    void updateMeetingDone(bool success);
    void roomInformation(const QString& data);
    void currentTime(const QVariant& data);
protected:
    void timerEvent(QTimerEvent *event);

private:
    int checkLastModified();
    int processLastModified(const QString& response);
    void checkMeetingData();
    void getCurrentTime();
    QString getCurrTimeInServer();
    int processMeetingData(const QString& response);
    void checkRoomInfoData();
    int processRoomInfoData(const QString& response);

private:
    QString _url;
    QString _meetingData;
    QString _lastModified;
    QString _roomInfo;

    QString _currDate;
    QString _updateUrl;
    QString _lastModifiedUrl;
    QString _roomInfoUrl;
    QString _currentTimeUrl;
    QVariant _currentTime;
    int _timerUpdate;
    int _feedInterval;
    int _timerRoomUpdate;
    int _feedRoomInterval;

//    QNetworkAccessManager* _networkMgr;
//    QEventLoop* _eventLoop;
};

#endif // MEETINGFEED_H
