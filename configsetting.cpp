#include <QMutex>
#include <QSettings>
#include <QDebug>
#include "configsetting.h"

ConfigSetting* ConfigSetting::m_instance = NULL;

ConfigSetting::ConfigSetting(QObject *parent) : QObject(parent)
{
    Load();
}

ConfigSetting::~ConfigSetting()
{
    //Save();
}

ConfigSetting *ConfigSetting::instance()
{
    static QMutex mutex;

    if (m_instance == NULL)
    {
        mutex.lock();

        if (m_instance == NULL) {
            m_instance = new ConfigSetting();
        }

        mutex.unlock();
    }

    return (m_instance);
}

#include <QStandardPaths>
void ConfigSetting::Load()
{
    QSettings sets("vng", "vpos");

    appName         = sets.value(QString("option/appname"), QString("vpos")).toString();
    paymentName     = sets.value(QString("option/payment"), QString("gbc.zing.vn/vpos")).toString();
    transProtocol   = sets.value(QString("option/transferprotocol"), QString("https")).toString();
    printerName     = sets.value(QString("option/printer"), QString("xpriner58")).toString();

    dockServer      = sets.value(QString("option/dock"), QString("192.168.2.1")).toString();
    dockVersion     = sets.value(QString("option/dockversion"), QString("1.0")).toString();
    networkDevice   = sets.value(QString("option/networkdevice"), true).toBool();
    idRecharge      = sets.value(QString("option/machine"), 0).toInt();
    paymentMethod   = sets.value(QString("option/paymentmethod"), 1).toInt();
    paymentMask     = sets.value(QString("option/paymentmask"), 1).toInt();

    foodRefreshTimer= sets.value(QString("option/foodrefreshtimer"), 60).toInt();
    billAlertTimer  = sets.value(QString("option/billalerttimer"), 5).toInt();
    qrcodeRatio     = sets.value(QString("option/qrcoderatio"), 4.0).toFloat();
    showThanhtoancunghoadon = sets.value(QString("option/showthanhtoancunghoadon"), false).toBool();

    foodItemFontsize = sets.value(QString("option/fooditemfontsize"), 16).toInt();
    billItemFontsize = sets.value(QString("option/billitemfontsize"), 16).toInt();

    deviceId = sets.value(QString("option/deviceid"), QString("112233445566")).toString();
}

void ConfigSetting::Save()
{
    qDebug() << "Save data";
    QSettings sets("vng", "vpos");

    sets.setValue(QString("option/printer"), printerName);
    sets.sync();
    sets.setValue(QString("option/payment"), paymentName);
    sets.sync();
    sets.setValue(QString("option/dock"), dockServer);
    sets.sync();
    sets.setValue(QString("option/dockversion"), dockVersion);
    sets.sync();
    sets.setValue(QString("option/networkdevice"), networkDevice);
    sets.sync();
    sets.setValue(QString("option/machine"), idRecharge);
    sets.sync();
    sets.setValue(QString("option/paymentmethod"), paymentMethod);
    sets.sync();
    sets.setValue(QString("option/paymentmask"), paymentMask);
    sets.sync();
    sets.setValue(QString("option/foodrefreshtimer"), foodRefreshTimer);
    sets.sync();
    sets.setValue(QString("option/billalerttimer"), billAlertTimer);
    sets.sync();
    sets.setValue(QString("option/showthanhtoancunghoadon"), showThanhtoancunghoadon);
    sets.sync();
    sets.setValue(QString("option/fooditemfontsize"), foodItemFontsize);
    sets.sync();
    sets.setValue(QString("option/billitemfontsize"), billItemFontsize);
    sets.sync();
}

void ConfigSetting::setDocServer(const QString& docsrv)
{
    if( dockServer.compare(docsrv) ) {
        dockServer = docsrv;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/dock"), dockServer);
        sets.sync();
    }
}

void ConfigSetting::setDockVersion(const QString &ver)
{
    if (dockVersion != ver) {
        dockVersion = ver;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/dockversion"), dockVersion);
        sets.sync();
    }
}

void ConfigSetting::setPrinter(const QString &prnName)
{
    printerName = prnName;

    QSettings sets("vng", "vpos");
    sets.setValue(QString("option/printer"), printerName);
    sets.sync();
}

void ConfigSetting::setAppName(const QString &appname)
{
    if( appName.compare(appname) ) {
        appName = appname;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/appname"), appName);
        sets.sync();
    }
}

void ConfigSetting::setRecharge(const int recharge)
{
    idRecharge = recharge;

    QSettings sets("vng", "vpos");
    sets.setValue(QString("option/machine"), idRecharge);
    sets.sync();
}

void ConfigSetting::setRefreshTimer(const int refreshTimer)
{
    foodRefreshTimer = refreshTimer;

    QSettings sets("vng", "vpos");
    sets.setValue(QString("option/foodrefreshtimer"), foodRefreshTimer);
    sets.sync();
}

void ConfigSetting::setBillAlertTimer(const int alertTimer)
{
    billAlertTimer = alertTimer;

    QSettings sets("vng", "vpos");
    sets.setValue(QString("option/billalerttimer"), billAlertTimer);
    sets.sync();
}

void ConfigSetting::setTransProtocol(const QString transpro)
{
    if( transProtocol.compare(transpro) ) {
        transProtocol = transpro;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/transferprotocol"), transProtocol);
        sets.sync();
    }
}

void ConfigSetting::setShowThanhtoancunghoadon(const bool bshow)
{
    if( showThanhtoancunghoadon != bshow ) {
        showThanhtoancunghoadon = bshow;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/showthanhtoancunghoadon"), showThanhtoancunghoadon);
        sets.sync();
    }
}

void ConfigSetting::setPaymentName(const QString& pmName)
{
    if( paymentName.compare(pmName) ) {
        paymentName = pmName;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/payment"), paymentName);
        sets.sync();
   }
}

void ConfigSetting::setPaymentMethod(int pmMethod)
{
    if( paymentMethod != pmMethod ) {
        paymentMethod = pmMethod;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/paymentmethod"), paymentMethod);
        sets.sync();
   }
}

void ConfigSetting::setPaymentMask(int pmMask)
{
    paymentMask = pmMask;

    QSettings sets("vng", "vpos");
    sets.setValue(QString("option/paymentmask"), paymentMask);
    sets.sync();
}

void ConfigSetting::setQrcodeRatio(const float ratio)
{
    if (ratio != qrcodeRatio) {
        qrcodeRatio = ratio;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/qrcoderatio"), qrcodeRatio);
        sets.sync();
    }
}

void ConfigSetting::setFoodItemFontsize(const int fntsize)
{
    if (foodItemFontsize != fntsize) {
        foodItemFontsize = fntsize;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/fooditemfontsize"), foodItemFontsize);
        sets.sync();
    }
}

void ConfigSetting::setBillItemFontsize(const int fntsize)
{
    if (billItemFontsize != fntsize) {
        billItemFontsize = fntsize;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/billitemfontsize"), billItemFontsize);
        sets.sync();
    }
}

void ConfigSetting::setDeviceId(const QString &newDeviceId)
{
    deviceId = newDeviceId;
    QSettings sets("vng", "vpos");
    sets.setValue(QString("option/deviceid"), deviceId);
    sets.sync();
}
