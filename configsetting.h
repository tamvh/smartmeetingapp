#ifndef CONFIGSETTING_H
#define CONFIGSETTING_H

#include <QObject>

class ConfigSetting : public QObject
{
    Q_OBJECT
public:
    static ConfigSetting *instance();
    explicit ConfigSetting(QObject *parent = 0);
    ~ConfigSetting();

    void Load();
    void Save();
    void setDocServer(const QString& docsrv);
    void setDockVersion(const QString &ver);
    void setPrinter(const QString &prnName);
    void setAppName(const QString& appname);
    void setRecharge(const int recharge);
    void setRefreshTimer(const int refreshTimer);
    void setBillAlertTimer(const int alertTimer);
    void setTransProtocol(const QString transpro);
    void setShowThanhtoancunghoadon(const bool bshow);
    void setPaymentName(const QString& pmName);
    void setPaymentMethod(int pmMethod);
    void setPaymentMask(int pmMask);
    void setQrcodeRatio(const float ratio);
    void setFoodItemFontsize(const int fntsize);
    void setBillItemFontsize(const int fntsize);

    void setDeviceId(const QString& newDeviceId);

    QString transProtocol;  // http / https
    QString appName;        // "canteenvng", "vpos", ...
    QString printerName;    // "xpriner58"
    QString paymentName;    // "104.199.170.120", "payment.vng.com.vn"
    QString dockServer;     // "104.199.170.120", "payment.vng.com.vn" //104.155.232.21 (hard)
    QString dockVersion;    // DockServerService version; 1.0 is original, 2.0 support extent command

    QString deviceId;

    int     idRecharge;     // id recharge machine: 0..20
    int     paymentMethod;  // 1:zalo; 2:card; 4:cash
    int     paymentMask;    // zalo | card | cash
    int     foodRefreshTimer;// time to refresh food data in second
    int     billAlertTimer; // time waiting to alert user get print bill
    float   qrcodeRatio;    // qr-code image ratio

    int     foodItemFontsize;
    int     billItemFontsize;

    bool    showThanhtoancunghoadon; // hiển thị thanh toán cùng hóa đơn ra màn hình

    bool networkDevice;

private:
    static ConfigSetting *m_instance;

signals:

public slots:
};

#endif // CONFIGSETTING_H
