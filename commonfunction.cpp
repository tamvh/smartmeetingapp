#include "commonfunction.h"
#include <QDebug>
CommonFunction::CommonFunction()
{

}

QString CommonFunction::generateQRCode(const QString &url, const QString &_datetime, int _roomId)
{
    return NULL;
}


int CommonFunction::parseMsgType(const QString& msg) {
    QJsonObject jsonMain;
    QJsonDocument jsonDoc;

    jsonDoc = QJsonDocument::fromJson(QByteArray(msg.toUtf8()));
    jsonMain = jsonDoc.object();

    return jsonMain["msg_type"].toInt();
}
bool CommonFunction::parseMsgSensors(const QString &msg, QString &devId, QString &gatewayId,
                              bool &havepersion, int &temperature, int &humidity, QString &area_id) {
    QJsonDocument jdoc = QJsonDocument::fromJson(QByteArray(msg.toUtf8()));
    QJsonObject jso = jdoc.object();

    if( jso.contains("dt"))
    {
        QJsonObject jdt = jso["dt"].toObject();

        if( jdt.contains("device_id")) {
            devId = jdt["device_id"].toString();
        }

        if( jdt.contains("area_id") ) {
            area_id = jdt["area_id"].toString();
        }

        if( jdt.contains("gateway_id")) {
            gatewayId = jdt["gateway_id"].toString();
        }

        int sensor = -1;
        if( jdt.contains("sensor_data")) {
            sensor = jdt["sensor_data"].toInt();
        }

        QString cmd;
        if( jdt.contains("command")) {
            cmd = jdt["command"].toString();
        }

        if( cmd.compare("humidity_sensor") == 0 ) {
            humidity = sensor;
        }

        if( cmd.compare("temperature_sensor") == 0 ) {
            temperature = sensor;
        }        

        return true;
    }

    return false;
}

QVariant CommonFunction::initJson(QString &devId, QString &gatewayId,
                            int &temperature, int &humidity, QString &area_id) {
    QJsonObject jso;
    jso["devId"] = devId;
    jso["gatewayId"] = gatewayId;
    jso["temperature"] = temperature;
    jso["humidity"] = humidity;
    jso["area_id"] = area_id;
//    QJsonDocument doc(jso);
    return jso;
//    return doc.toJson(QJsonDocument::Compact);
}

int CommonFunction::getAreaId(const QString &data) {
    QJsonDocument jdoc = QJsonDocument::fromJson(QByteArray(data.toUtf8()));
    QJsonObject jso = jdoc.object();
    int area_id = 0;
    if(jso.contains("dt")) {
        QJsonObject jdt = jso["dt"].toObject();
        if( jdt.contains("area_id") ) {
            area_id = jdt["area_id"].toInt();
        }
    }
    return area_id;
}

QString CommonFunction::getCurrentTimeInServer(const QString &data) {
    QJsonDocument jdoc = QJsonDocument::fromJson(QByteArray(data.toUtf8()));
    QJsonObject jso = jdoc.object();
    QString _time = "";
    if(jso.contains("dt")) {
        _time = jso["dt"].toString();
    }
    return _time;
}
