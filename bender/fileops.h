#ifndef FILEOPS_H
#define FILEOPS_H

#include <QByteArray>
#include <QDebug>
#include <QObject>
#include <filesystem>
#include <fstream>

class Fileops : public QObject {
    Q_OBJECT
  public:
    Fileops();
    ~Fileops();

    enum Pmode : uint8_t {
        Pmode1,
        Pmode2
    };
    void setCurrentPmode(Pmode mode) { currentPmode = mode; }
    Pmode getCurrentPmode() { return currentPmode; }
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
    void setCurrentPnum(Pnum num) { currentPnum = num; }
    Pnum getCurrentPnum() { return currentPnum; }
    Pnum currentPnum = P1;

    // set values
    struct YX {
        uint32_t Y = 0;
        uint32_t X = 0;
    };
    int getIndex(Pnum num, Pmode mode);
    void setFilePnum(Pnum num, Pmode mode);
    void setFileValues(Pnum num, Pmode mode, YX yx);
    YX getFileValues(Pnum num, Pmode mode);
  signals:
    void sendCurrentPmode(Pmode);
    void sendCurrentPnum(Pnum);

  private:
    void init();
};

#endif // FILEOPS_H
