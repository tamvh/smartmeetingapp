#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H
#include <QString>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>

class CommonFunction
{
public:
    CommonFunction();
    static QString generateQRCode(const QString &url, const QString &_datetime, int _roomId);
    static int parseMsgType(const QString& msg);
    static bool parseMsgSensors(const QString &msg, QString &devId, QString &gatewayId,
                                  bool &havepersion, int &temperature, int &humidity, QString &area_id);
    static QVariant initJson(QString &devId, QString &gatewayId,
                            int &temperature, int &humidity, QString &area_id);
    static int getAreaId(const QString& data);
    static QString getCurrentTimeInServer(const QString& data);
};

#endif // COMMONFUNCTION_H
