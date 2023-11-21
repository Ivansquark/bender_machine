#ifndef FILEOPS_H
#define FILEOPS_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QObject>
#include <filesystem>
#include <fstream>

class Fileops : public QObject{
    Q_OBJECT
  public:
    Fileops();

    enum Pmode : uint8_t {
        Pmode1,
        Pmode2
    };
    Pmode currentPmode = Pmode::Pmode1;
    enum Pnum : uint8_t {
        P1 = 1,
        P2 = 2,
        P3 = 3,
        P4 = 4,
        P5 = 5,
        P6 = 6,
        P7 = 7,
        P8 = 8,
        P9 = 9,
        P10 = 10,
        P11 = 11,
        P12 = 12,
        P13 = 13,
        P14 = 14,
        P15 = 15,
        P16 = 16
    };
    Pnum currentPnum = P1;
  signals:
    void sendCurrentPmode(Pmode);
    void sendCurrentPnum(Pnum);
  private:
    void init();
};

#endif // FILEOPS_H
