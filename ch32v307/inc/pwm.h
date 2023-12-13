#ifndef PWM_H
#define PWM_H

#include "gpios.h"
#include "main.h"
//#include "step.h"

class PwmY {
  public:
    PwmY();
    static PwmY* pThis;
    static uint32_t counter;

    void slow();
    void fast();

    enum Direction {
        UP,
        DOWN
    };
    static Direction currentDirection;
    inline void start() { TIM9->CTLR1 |= TIM_CEN; }
    inline void stop() { TIM9->CTLR1 &= ~TIM_CEN; }
    inline bool isStopped() { return !(TIM9->CTLR1 & TIM_CEN); }

  private:
    void init();
    Gpios::Out<Gpios::PD, 12, Gpios::InitModeOut::ALTERNATE_PUSH_PULL> pwmY;
};

class PwmX {
  public:
    PwmX();
    static PwmX* pThis;
    static uint32_t counter;

    void slow();
    void fast();

    enum Direction {
        PLUS,
        MINUS
    };
    static Direction currentDirection;

    inline void start() { TIM1->CTLR1 |= TIM_CEN; }
    inline void stop() { TIM1->CTLR1 &= ~TIM_CEN; }
    inline bool isStopped() { return !(TIM1->CTLR1 & TIM_CEN); }

  private:
    void init();
    Gpios::Out<Gpios::PE, 8, Gpios::InitModeOut::ALTERNATE_PUSH_PULL> pwmX;
};

#endif // PWM_H
