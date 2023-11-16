#include "uart.h"

Uart::Uart(QObject* parent):QObject(parent) {

}

Uart::~Uart() {
    delete com;
    delete timerMeasure;
}

void Uart::bytesHandle(qint64 bytes) {
    qDebug()<<"bytesHandle="<<bytes;
    Written=true;
}

void Uart::initSlot() {
    com = new QSerialPort(this);
    com->setPortName("/dev/ttyAMA1"); ///dev/ttyS0 - miniUart
    com->setBaudRate(QSerialPort::Baud9600);
    com->setParity(QSerialPort::NoParity);
    com->setDataBits(QSerialPort::Data8);
    com->setStopBits(QSerialPort::OneStop);
    //com->setFlowControl(QSerialPort::NoFlowControl);
    qDebug()<<"UART thread: "<< QThread::currentThreadId();
    if(com->open(QIODevice::ReadWrite)){
        emit sendText("UART started");
        timerMeasure = new QTimer(this);
        connect(timerMeasure,&QTimer::timeout,this,[this]{
            QByteArray data;
            for(int i =0; i<11;i++){
                //data.append(measureRequest[i]);
            }
            write_to_com(data);
            data.clear();
            QThread::msleep(20);
            //if(flag) {
            //    setForm(1); flag = false;
            //} else {
            //    setForm(0); flag = true;
            //}

        });
        connect(com,&QSerialPort::bytesWritten,this, &Uart::bytesHandle);
        connect(com,&QSerialPort::readyRead,this,&Uart::read_from_com);
        //wiringPiSetup();
        //pinMode(29,OUTPUT);
    } else {
        emit sendText("unable to start UART");
    }
}

void Uart::write_to_com(const QByteArray &data) {
    if(com->isOpen()) {
        // pin rts on
        //digitalWrite(29,HIGH); // (pin,value)
        com->write(data);
        qDebug()<<"write_to_com"<<data;
        //Written = false;
        com->waitForBytesWritten(10000);
        QThread::msleep(11);
        QThread::usleep(150);
        //digitalWrite(29,LOW); // (pin,value)
    }
}

void Uart::read_from_com() {
    if(com->isOpen()) {
        qint64 count = com->bytesAvailable();
        qDebug()<<"read from com bytes"<<count;
        if(count>14) {
            QByteArray receivedData = com->readAll();
        }
    }
}
