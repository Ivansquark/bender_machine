#ifndef INTERFACE_H
#define INTERFACE_H

#include "protocol.h"
#include "udp.h"

#include <QObject>
#include <QTimer>
#include <memory>

class Interface : public QObject {
    Q_OBJECT
  public:
    Interface(QObject* parent = nullptr);
    Protocol::Command currentCommand;
    Protocol::Reply currentReply;

    void sendData(const Protocol::Command& command);
  signals:
    void sendCurrentReply(const Protocol::Reply& reply);
  private slots:
    void replyTimeout();
  public slots:
    void receiveData(const QByteArray& arr);

  private:
    void sendData(const QByteArray& arr);
    void sendReply();
    std::unique_ptr<Udp> udp = std::make_unique<Udp>();
    QTimer timerReply;
    bool IsTimeout = false;
    bool IsNotReplied = false;
};

#endif // INTERFACE_H
