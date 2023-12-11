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

void Interface::sendData(const Protocol::Command& command) {
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

void Interface::sendDataSettings(const Protocol::CommandSet& data) {
    // if udp
    currentCommandSet = data;
    QByteArray arr;
    arr.append(Protocol::TO);
    arr.append((uint8_t)currentCommandSet.currentCommand);
    arr.append((uint8_t)(currentCommandSet.coefY >> 24));
    arr.append((uint8_t)(currentCommandSet.coefY >> 16));
    arr.append((uint8_t)(currentCommandSet.coefY >> 8));
    arr.append((uint8_t)(currentCommandSet.coefY));
    arr.append((uint8_t)(currentCommandSet.coefX >> 24));
    arr.append((uint8_t)(currentCommandSet.coefX >> 16));
    arr.append((uint8_t)(currentCommandSet.coefX >> 8));
    arr.append((uint8_t)(currentCommandSet.coefX));
    arr.append((uint8_t)(currentCommandSet.deviationY >> 24));
    arr.append((uint8_t)(currentCommandSet.deviationY >> 16));
    arr.append((uint8_t)(currentCommandSet.deviationY >> 8));
    arr.append((uint8_t)(currentCommandSet.deviationY));
    arr.append((uint8_t)(currentCommandSet.deviationX >> 24));
    arr.append((uint8_t)(currentCommandSet.deviationX >> 16));
    arr.append((uint8_t)(currentCommandSet.deviationX >> 8));
    arr.append((uint8_t)(currentCommandSet.deviationX));
    udp->sendDataToUdp(arr);
    timerReply.start(100);
    // if usb ...
}

void Interface::replyTimeout() {
    IsNotReplied = true;
    // send another time
    sendData(currentCommand);
}

void Interface::receiveData(const QByteArray& arr) {
    // parse data
    if (arr[1] == Protocol::Replies::SETTINGS) {
        currentReplySet.currentReply = (Protocol::Replies)arr[1];
        currentReplySet.coefY =
            (uint32_t)(((uint8_t)arr[2] << 24) | ((uint8_t)arr[3] << 16) |
                       ((uint8_t)arr[4] << 8) | (uint8_t)arr[5]);
        currentReplySet.coefX =
            (uint32_t)(((uint8_t)arr[6] << 24) | ((uint8_t)arr[7] << 16) |
                       ((uint8_t)arr[8] << 8) | (uint8_t)arr[9]);
        currentReplySet.deviationY =
            (uint32_t)(((uint8_t)arr[10] << 24) | ((uint8_t)arr[11] << 16) |
                       ((uint8_t)arr[12] << 8) | (uint8_t)arr[13]);
        currentReplySet.deviationX =
            (uint32_t)(((uint8_t)arr[14] << 24) | ((uint8_t)arr[15] << 16) |
                       ((uint8_t)arr[16] << 8) | (uint8_t)arr[17]);
        sendReply();
        emit sendCurrentReplySet(currentReplySet);

    } else if (arr[1] == Protocol::Replies::REPLY_CONTROLLER) {
        IsNotReplied = false;
        timerReply.stop();
    } else {
        currentReply.currentReply = (Protocol::Replies)arr[1];
        currentReply.val =
            (uint32_t)(((uint8_t)arr[2] << 24) | ((uint8_t)arr[3] << 16) |
                       ((uint8_t)arr[4] << 8) | (uint8_t)arr[5]);
        if (currentReply.val > 99999) {
            qDebug() << (uint8_t)arr[2];
        }
        sendReply();
        emit sendCurrentReply(currentReply);
    }
}

void Interface::sendReply() {
    QByteArray arr;
    arr.append(Protocol::TO);
    arr.append(Protocol::Commands::REPLY_PC);
    for (int i = 0; i < 4; i++) {
        arr.append((char)0);
    }
    udp->sendDataToUdp(arr);
}
