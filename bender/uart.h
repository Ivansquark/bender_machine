#ifndef UART_H
#define UART_H

//#include <wiringPi.h>
#include <cmath>
//#include "bcm2835.h"

#include <QEventLoop>
#include <QTimer>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>

class Uart: public QObject {
    Q_OBJECT
public:
    explicit Uart(QObject* parent = nullptr);
    virtual ~Uart();
signals:
    void sendText(const QString&);

private slots:
    void bytesHandle(qint64 bytes);
    void read_from_com();
public slots:
    void initSlot();    
private:
    void write_to_com(const QByteArray& data);

    QSerialPort* com{nullptr};
    QByteArray receivedData;
    QTimer* timerMeasure{nullptr};

    float Amperage = 0;
    bool Written = false;
    bool flag =false;

};

#endif // UART_H
