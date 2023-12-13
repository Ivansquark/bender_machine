#ifndef TOUCH_H
#define TOUCH_H

#include <QFile>
#include <QObject>
#include <QSocketNotifier>
class Touch : public QObject {
    Q_OBJECT

    QString fileName = "/dev/input/by-path/platform-3f204000.spi-cs-1-event";
    QSocketNotifier *notifier;
    int fd;
  public:
    explicit Touch(QObject* parent = nullptr);
  public slots:
    void handle_readNotification(int socket);

  signals:
  private:
    bool IsPressed = false;
    uint16_t x = 0;
    uint16_t y = 0;
};

#endif // TOUCH_H
