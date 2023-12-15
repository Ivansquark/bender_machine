#include "udp.h"
#include "protocol.h"

Udp::Udp() {
    //_______________ UDP ____________________________________________________
    udpSocketSend = new QUdpSocket();
    udpSocketGet = new QUdpSocket();
    //udpSocketSend->connectToHost(localAddr->toString(), LOCAL_PORT);
    // udpSocketSend->connectToHost(QHostAddress("192.168.3.100"), UDP_PORT);
    // udpSocketSend->bind(QHostAddress("193.168.0.100"), 44444);
    //udpSocketGet->connectToHost(QHostAddress("192.168.1.5"), MCU_PORT);
    udpSocketGet->bind(QHostAddress::Any, LOCAL_PORT);
    //udpSocketGet->bind(QHostAddress::Any, LOCAL_PORT);
    connect(udpSocketGet, &QUdpSocket::readyRead, this,
            &Udp::readDatagrammGet);
}

void Udp::readDatagrammGet() {
    QHostAddress senderIP = QHostAddress::Any;

    uint16_t sizeDatagram = udpSocketGet->pendingDatagramSize();
    char arr[256];
    udpSocketGet->readDatagram(arr, sizeDatagram, &senderIP);
    if(arr[0] != Protocol::FROM) {
        return;
    }
    //QByteArray bArr;
    //qDebug() << bArr->data();
//    QString data;
    for (int i = 0; i < sizeDatagram; i++) {
        bArr->append(arr[i]);
    }
    // emit signal
    emit sendData(*bArr);
    bArr->clear();
}
void Udp::sendDataToUdp(const QByteArray& bytes) {
    udpSocketSend->writeDatagram(bytes, QHostAddress(IP_MCU), MCU_PORT);
    udpSocketSend->waitForBytesWritten(10000);
    //qDebug() << bytes;
}
