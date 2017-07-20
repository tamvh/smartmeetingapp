#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QVariant>

class MeetingFeed;
class WSClient;
class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = 0);
    ~MainController();
    Q_INVOKABLE int initialize();
    Q_INVOKABLE void uninitialize();

    MeetingFeed* meetingFeed() const;

signals:
    void meetingChanged(const QVariant& newMeetingData);
    void roomInfoChanged(const QVariant& newData);
    void updateBegin();
    void updateDone();
    void currentEventChanged(const QVariant& event);
    void currentEventAvailable();
    void genQRCode(const QString& qrcode);
    void refreshSensors(const QVariant &msg);  
    void currentTimeInServer(const QString &data);
protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void onMeetingChanged(const QVariant& newMeetingData);
    void onNotify(const QString& message);
    void roomInformation(const QString &data);
    void currentTime(const QVariant &data);
private:
    void checkMeetingHappen(bool sort = false);

private:
    MeetingFeed *_meetingFeed;
    WSClient* m_wsClient;
    QVariant _meetingInfo;
    QVariant _lastMeetingInfo;
    QVariant _lastCurrent;
    int _timerCheckEventHappen;
    QList<QDateTime> _dtFrees;
    QString _lastDate;
    QString _currTimeInServer;
    int _lightFlag;
    int m_area_id;
};

#endif // MAINCONTROLLER_H
