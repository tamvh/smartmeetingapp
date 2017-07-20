#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <QObject>

class LedController : public QObject
{
    Q_OBJECT
public:
    explicit LedController(QObject *parent = 0);

signals:

public slots:
};

#endif // LEDCONTROLLER_H
