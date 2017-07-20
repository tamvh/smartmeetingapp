#include "utils.h"

#include <QNetworkAccessManager>
#include <QSslConfiguration>
#include <QEventLoop>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QProcess>

Utils* Utils::_instance = Q_NULLPTR;

#define ANDROID_BIN_SH "/system/bin/sh"
#define ANDROID_BIN_SU "/system/xbin/su"

Utils::Utils(QObject *parent) : QObject(parent)
{
    _networkMgr = new QNetworkAccessManager();
}

Utils::~Utils()
{
    if (_networkMgr)
        delete _networkMgr;
}

QNetworkReply* Utils::syncPost(const QString &url, const QByteArray &data)
{
    QSslConfiguration conf;
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    // Setup url
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setSslConfiguration(conf);

    QEventLoop eventLoop;
    QNetworkReply* reply = _networkMgr->post(request, data);
    qDebug() << "Setup connection with reply";
    connect(reply,SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    qDebug() << "Request reply finish";

    return reply;
}

QNetworkReply *Utils::syncGet(const QString &url)
{
    QSslConfiguration conf;
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);

    // Setup url
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setSslConfiguration(conf);

    QEventLoop eventLoop;
    QNetworkReply* reply = _networkMgr->get(request);
    connect(reply,SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return reply;
}

int Utils::parseServerResponse(const QString &data, QJsonValue &dt)
{
    QJsonParseError perror;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(), &perror);
    if (QJsonParseError::NoError != perror.error) {
        qWarning() << "Have error while parse json response " << perror.errorString();
        return -1;
    }

    QJsonObject res = doc.object();
    if (res.isEmpty()) {
        qWarning() << "Response object empty ";
        return -1;
    }

    // Check error code
    int err = res["err"].toInt();
    if (err != 0) {
        qWarning() << "Response error from server " << err;
        return -1;
    }

    dt = res["dt"];

    return 0;
}

#define GPIO_PI14 "PI14"
#define GPIO_PI15 "PI15"

bool Utils::writeGPIO(const QString &deviceName, int value)
{
    QFile fgpio("/sys/class/gpio_sw/" + deviceName + "/data");
    if (fgpio.open(QFile::WriteOnly)) {
        qDebug() << "Open file success";
        if (fgpio.write((char*)&value, sizeof(value)) > 0) {
            qDebug() << "Write success";
        }
        fgpio.flush();
        fgpio.close();
    } else {
        qDebug() << "Open file failed";
    }

    return true;
}

bool Utils::turnOnRed()
{
    qDebug() << "Utils turn on red";
    QString cmd1 = QString("echo %1 > %2%3%4").arg(0).arg("/sys/class/gpio_sw/", GPIO_PI14, "/data");
    QString cmd2 = QString("echo %1 > %2%3%4").arg(0).arg("/sys/class/gpio_sw/", GPIO_PI15, "/data");

    QString cmd = cmd1 + " ; " + cmd2;
    return QProcess::startDetached("/system/bin/sh", QStringList() << "-c" << cmd);
}

bool Utils::turnOnGreen()
{
    QString cmd1 = QString("echo %1 > %2%3%4").arg(0).arg("/sys/class/gpio_sw/", GPIO_PI14, "/data");
    QString cmd2 = QString("echo %1 > %2%3%4").arg(1).arg("/sys/class/gpio_sw/", GPIO_PI15, "/data");
    QString cmd = cmd1 + " ; " + cmd2;
    return QProcess::startDetached("/system/bin/sh", QStringList() << "-c" << cmd);
}

bool Utils::turnOnBlue()
{
    QString cmd1 = QString("echo %1 > %2%3%4").arg(1).arg("/sys/class/gpio_sw/", GPIO_PI14, "/data");
    QString cmd2 = QString("echo %1 > %2%3%4").arg(0).arg("/sys/class/gpio_sw/", GPIO_PI15, "/data");
    QString cmd = cmd1 + " ; " + cmd2;
    return QProcess::startDetached("/system/bin/sh", QStringList() << "-c" << cmd);
}

bool Utils::turnOff()
{
    QString cmd1 = QString("echo %1 > %2%3%4").arg(1).arg("/sys/class/gpio_sw/", GPIO_PI14, "/data");
    QString cmd2 = QString("echo %1 > %2%3%4").arg(1).arg("/sys/class/gpio_sw/", GPIO_PI15, "/data");
    QString cmd = cmd1 + " ; " + cmd2;
    return QProcess::startDetached("/system/bin/sh", QStringList() << "-c" << cmd);
}

bool Utils::controlLight(int lightFlags)
{
    switch (lightFlags) {
    case Off:
        turnOff();
        break;
    case Red:
        turnOnRed();
        break;
    case Green:
        turnOnGreen();
        break;
    case Blue:
        turnOnBlue();
        break;
    default:
        qWarning() << "Unknown light command";
        break;
    }
    return true;
}

#ifdef AUTO_HIDE_NAVI
bool Utils::requestSudo()
{
    qDebug() << "Request sudo permission";
    int perr = QProcess::execute("su");

    qDebug() << "Return code " << perr;
    if (255 != perr) {
        qDebug() << "Request root success";
        return true;
    }

    qDebug() << "Request root failed";
    return false;
}

bool Utils::showNavigateBar()
{
    int perr = QProcess::execute("su", QStringList() << "-c" <<  "am startservice -n com.android.systemui/.SystemUIService");
    return true;
}

bool Utils::hideNavigateBar()
{
    qDebug() << "Hide navigate bar";
    int perr = QProcess::execute("su", QStringList() << "-c" << "service call activity 42 s16 com.android.systemui");
    return true;
}
#endif
