#include "touch.h"

#include <QDebug>
#include <fcntl.h>
#include <unistd.h>

Touch::Touch(QObject* parent) : QObject{parent} { init(); }

void Touch::handle_readNotification(int /*socket*/) {
    uint8_t buf[16] = {0};
    while (read(fd, &buf, sizeof(buf)) > 0) {
        // time
        //uint32_t x0 =
        //    (buf[3] << 24) + (buf[2] << 16) + (buf[1] << 8) + (buf[0]);
        //uint32_t x1 =
        //    (buf[7] << 24) + (buf[6] << 16) + (buf[5] << 8) + (buf[4]);
        // code 0x14A0001 - 1pressed,2released, 0x3-Ycoord, 0x10003 -Xcoord
        //  0x180003 - 252? 0-0-?
        uint32_t x2 =
            (buf[11] << 24) + (buf[10] << 16) + (buf[9] << 8) + (buf[8]);
        // coord
        uint32_t x3 =
            (buf[15] << 24) + (buf[14] << 16) + (buf[13] << 8) + (buf[12]);
        if (x2 == 0x14A0001 && x3 == 1) {
            //qDebug() << "Pressed";
            IsPressed = true;
        } else if (x2 == 0x14A0001 && x3 == 0) {
            IsPressed = false;
            emit sendScreenCoord(IsPressed, getX(), getY());
            //qDebug() << "released";
        }
        if (IsPressed) {
            if (x2 == 0x3) {
                y = x3;
            } else if (x2 == 0x10003) {
                x = x3;
            }
            //qDebug() << "X = " << getX() << "\t Y = " << getY();
            emit sendScreenCoord(IsPressed, getX(), getY());
        }
        // qDebug() << x0 << x1 << x2 << x3;
    }
    // CheckPressed = false;
    //qDebug() << "";
}

// void Touch::timeoutPressed() {
//     if (CheckPressed) {
//         IsPressed = false;
//     }
//     CheckPressed = true;
// }

void Touch::init() {
    auto file = new QFile();
    file->setFileName(fileName);
    if (!file->exists()) {
        qWarning("file does not exist");
        return;
    }

    fd = open(fileName.toUtf8().data(), O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        qWarning("can not open file");
        return;
    }

    notifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);

    connect(notifier, &QSocketNotifier::activated, this,
            &Touch::handle_readNotification);
    //-----------------------  timer  -----------------------------------------
    // connect(timPressed.get(), &QTimer::timeout, this,
    // &Touch::timeoutPressed); timPressed->start(10);
}

uint16_t Touch::getX() {
    // 0-70 1023-4000
    if (X_MIN >= x)
        return 0;
    else
        return (SCREEN_X_MAX * (x - X_MIN)) / (X_MAX - X_MIN);
}

uint16_t Touch::getY() {
    if (Y_MIN >= y)
        return SCREEN_Y_MAX - 1;
    else
        return (SCREEN_Y_MAX - 1) - (SCREEN_Y_MAX * (y - Y_MIN)) / (Y_MAX - Y_MIN);
}
