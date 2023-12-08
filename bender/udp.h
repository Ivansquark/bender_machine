#ifndef UDP_H
#define UDP_H

#include <QUdpSocket>
#include <QObject>

class Udp : public QObject {
    Q_OBJECT
  public:
    Udp();

    void sendDataToUdp(const QByteArray& arr);
  signals:
    void sendData(const QByteArray& arr);
  private slots:
    void readDatagrammGet();

  private:
    QUdpSocket* udpSocketSend{nullptr};
    QUdpSocket* udpSocketGet{nullptr};
    QHostAddress* localAddr{nullptr};
    QHostAddress* destAddr{nullptr};
    static constexpr uint16_t MCU_PORT = 4000;
    static constexpr uint16_t LOCAL_PORT = 4001;
    static constexpr const char* IP_MCU = "192.168.1.5";
    std::unique_ptr<QByteArray> bArr = std::make_unique<QByteArray>();

};

#endif // UDP_H
