#ifndef BUZZER_H
#define BUZZER_H

#include "gpios.h"
#include "main.h"

class Buzzer {
  public:
    Buzzer();
    static Buzzer* pThis;

    void start();
    void stop();

    void handler();
    void setMustBuzz(bool state) { MustBuzz = state; }
    bool setMustBuzz() { return MustBuzz; }

  private:
    void init();
    bool MustBuzz = false;
    Gpios::Out<Gpios::PA, 6, Gpios::InitModeOut::PUSH_PULL> bzz;
};

#endif // BUZZER_H
