#ifndef APPSETTING_H
#define APPSETTING_H

#include <QObject>
#include <QDebug>

class QSettings;
class AppSetting : public QObject
{
    Q_OBJECT
public:
    static AppSetting* getInstance(){
      if (Q_NULLPTR == _instance) {
          _instance = new AppSetting();
      }
      return _instance;
    }

    explicit AppSetting(QObject *parent = 0);
    ~AppSetting();

    void load();
    Q_INVOKABLE void save();
    Q_INVOKABLE int roomId() const;
    Q_INVOKABLE QString roomName() const;

    int checkHappenInterval() const;
    int checkEventInterval() const;

    Q_INVOKABLE void setRoomName(const QString &roomName);
    Q_INVOKABLE void setRoomId(int roomId);

    Q_INVOKABLE QString serverUrl() const;
    Q_INVOKABLE void setServerUrl(const QString &serverUrl);

    Q_INVOKABLE QString bookRoomUrl() const;
    Q_INVOKABLE void setBookRoomUrl(const QString &bookRoomUrl);

    Q_INVOKABLE int getUpdateInterval() const;
    Q_INVOKABLE void setUpdateInterval(int updateInterval);

    Q_INVOKABLE bool getUseLastUpdate() const;
    Q_INVOKABLE void setUseLastUpdate(bool useLastUpdate);

    Q_INVOKABLE int getUpdateRoomInterval() const;
    Q_INVOKABLE void setUpdateRoomInterval(int updateRoomInterval);

signals:

public slots:

private:
    static AppSetting* _instance;
    QSettings* _settings;
    int _roomId;
    QString _roomName;
    int _updateInterval;
    int _updateRoomInterval;
    bool _useLastUpdate;
    QString _serverUrl;
    QString _bookroomUrl;
};

#endif // APPSETTING_H
