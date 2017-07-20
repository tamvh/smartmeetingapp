#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <QVariant>

class SettingsController : public QObject
{
    Q_OBJECT
public:
    explicit SettingsController(QObject *parent = 0);
public:
    static SettingsController* getInstance(){
      if (Q_NULLPTR == _instance) {
          _instance = new SettingsController();
      }
      return _instance;
    }

    Q_INVOKABLE QVariant getRoomList();

    Q_INVOKABLE void testRed();
    Q_INVOKABLE void testGreen();
    Q_INVOKABLE void testBlue();
    Q_INVOKABLE void testOff();
    Q_INVOKABLE void testCrash();

signals:

public slots:
private:
    static SettingsController* _instance;
};

#endif // SETTINGSCONTROLLER_H
