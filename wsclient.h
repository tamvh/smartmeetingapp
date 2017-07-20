#ifndef WSCLIENT_H
#define WSCLIENT_H
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QTimer>

class WSClient : public QObject
{
    Q_OBJECT
public:
    explicit WSClient(const QUrl &url, QObject *parent = Q_NULLPTR);
    ~WSClient();
    void sendMsg(const QString &msg);
signals:
    void reconnect();
    void textMessageReceived(QString message);
public slots:
    void onConnected();
    void onReconnect();
    void onTextMessageReceived(QString message);
    void onDisconnected();
private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool isConnected;
    QTimer m_reconnTimer;
};

#endif // WSCLIENT_H
