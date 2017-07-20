#include "wsclient.h"
#include <QtCore/QDebug>
WSClient::WSClient(const QUrl &url, QObject *parent):
    QObject(parent),
    m_url(url),
    isConnected(false)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &WSClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WSClient::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WSClient::onTextMessageReceived);
    connect(&m_reconnTimer, SIGNAL(timeout()), this, SLOT(onReconnect()) );
    m_webSocket.open(QUrl(m_url));
}

WSClient::~WSClient() {
    qDebug() << "close websocket client!";
}

void WSClient::sendMsg(const QString &msg) {
    qint64 sent = m_webSocket.sendTextMessage(msg);
    qDebug() << "Sent: " << sent;
}

void WSClient::onConnected() {
    qDebug() << "onConnected";
   if( m_reconnTimer.isActive() )
   {
       qDebug() << "stopTimer...";
       m_reconnTimer.stop();
   }
   isConnected = true;
}

void WSClient::onTextMessageReceived(QString message)
{
    emit textMessageReceived(message);
}

void WSClient::onDisconnected()
{
    qDebug() << "onDisconnected";
    isConnected = false;

    if( !m_reconnTimer.isActive() )
    {
        qDebug() << "start retry connection...";
        m_reconnTimer.start(5000);
    }
}

void WSClient::onReconnect()
{
    qDebug() << "onReconnect";
    m_webSocket.open(QUrl(m_url));
}


