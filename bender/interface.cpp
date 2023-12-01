#include "interface.h"

Interface::Interface(QObject* parent) : QObject(parent) {
    connect(udp.get(), &Udp::sendData, this, &Interface::receiveData);
    // -----------------  reply timer  -------------------------------
    timerReply.setSingleShot(true);
    connect(&timerReply, &QTimer::timeout, this, &Interface::replyTimeout);
}

void Interface::sendData(const QByteArray& arr) {
    // if udp
    udp->sendDataToUdp(arr);
    // if usb ...
}

void Interface::getData(const Protocol::Command& command) {
    // if udp
    currentCommand = command;
    QByteArray arr;
    arr.append(Protocol::TO);
    arr.append((uint8_t)currentCommand.currentCommand);
    arr.append((uint8_t)(currentCommand.val >> 24));
    arr.append((uint8_t)(currentCommand.val >> 16));
    arr.append((uint8_t)(currentCommand.val >> 8));
    arr.append((uint8_t)(currentCommand.val));
    udp->sendDataToUdp(arr);
    timerReply.start(100);
    // if usb ...
}


void Interface::replyTimeout() {
    IsNotReplied = true;
    // send another time
    getData(currentCommand);
}

void Interface::receiveData(const QByteArray& arr) {
    // parse data
    currentReply.currentReply = (Protocol::Replies)arr[1];
    currentReply.val = (arr[2] << 24) | (arr[3] << 16) | (arr[3] << 8) | arr[4];
    if (currentReply.currentReply == Protocol::Replies::REPLY_CONTROLLER) {
        IsNotReplied = false;
        timerReply.stop();
    } else {
        sendReply();
        emit sendCurrentReply(currentReply);
    }
}

void Interface::sendReply() {
    QByteArray arr;
    arr.append(Protocol::TO);
    arr.append(Protocol::Commands::REPLY_PC);
    for(int i = 0; i < 4; i++) {
        arr.append((char)0);
    }
    udp->sendDataToUdp(arr);
}
