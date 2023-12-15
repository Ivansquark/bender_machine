#ifndef TOUCH_H
#define TOUCH_H

#include <QFile>
#include <QObject>
#include <QSocketNotifier>
//#include <QTimer>
#include <memory.h>
class Touch : public QObject {
    Q_OBJECT

    QString fileName = "/dev/input/by-path/platform-3f204000.spi-cs-1-event";
    QSocketNotifier* notifier;
    int fd;

  public:
    explicit Touch(QObject* parent = nullptr);
    bool getIsPressed();
  public slots:
    void handle_readNotification(int socket);
  private slots:
    //void timeoutPressed();

  signals:
    void sendScreenCoord(bool WasPressed, uint16_t x, uint16_t y);
  private:
    void init();
    bool IsPressed = false;
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t getX();
    uint16_t getY();
    static constexpr uint16_t X_MIN = 70;
    static constexpr uint16_t X_MAX = 4000;
    static constexpr uint16_t Y_MIN = 70;
    static constexpr uint16_t Y_MAX = 3850;
    static constexpr uint16_t SCREEN_X_MAX = 1024;
    static constexpr uint16_t SCREEN_Y_MAX = 600;

};

#endif // TOUCH_H
