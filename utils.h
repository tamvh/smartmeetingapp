#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QNetworkReply>

class QNetworkAccessManager;
class Utils : public QObject
{
    Q_OBJECT
public:
    enum LightFlags {
        Off,
        Red,
        Green,
        Blue
    };

    static Utils* getInstance(){
      if (Q_NULLPTR == _instance) {
          _instance = new Utils();
      }
      return _instance;
    }

    explicit Utils(QObject *parent = 0);
    ~Utils();

    QNetworkReply* syncPost(const QString& url, const QByteArray& data);
    QNetworkReply* syncGet(const QString& url);
    static int parseServerResponse(const QString& data, QJsonValue& dt);

    static bool writeGPIO(const QString& deviceName, int value);
    static bool turnOnRed();
    static bool turnOnGreen();
    static bool turnOnBlue();
    static bool turnOff();

    static bool controlLight(int lightFlags);

#ifdef AUTO_HIDE_NAVI
    static bool requestSudo();
    static bool showNavigateBar();
    static bool hideNavigateBar();
#endif

signals:

public slots:

private:
    QNetworkAccessManager* _networkMgr;
    static Utils* _instance;
};

#endif // UTILS_H
