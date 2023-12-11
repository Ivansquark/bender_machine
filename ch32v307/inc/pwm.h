#ifndef PWM_H
#define PWM_H

#include "gpios.h"
#include "main.h"
//#include "step.h"

class Pwm {
  public:
    Pwm();
    static Pwm* pThis;
    static uint32_t counterY;
    static uint32_t counterX;

    enum DirectionY {
        UP,
        DOWN
    };
    static DirectionY currentDirectionY;

    enum DirectionX {
        PLUS,
        MINUS
    };
    static DirectionX currentDirectionX;

    inline void startX() { TIM1->CTLR1 |= TIM_CEN; }
    inline void stopX() { TIM1->CTLR1 &= ~TIM_CEN; }
    inline bool isXstopped() { return !(TIM1->CTLR1 & TIM_CEN); }

    inline void startY() { TIM9->CTLR1 |= TIM_CEN; }
    inline void stopY() { TIM9->CTLR1 &= ~TIM_CEN; }
    inline bool isYstopped() { return !(TIM9->CTLR1 & TIM_CEN); }

  private:
    void init();
    Gpios::Out<Gpios::PE, 8, Gpios::InitModeOut::ALTERNATE_PUSH_PULL> pwmX;
    Gpios::Out<Gpios::PD, 12, Gpios::InitModeOut::ALTERNATE_PUSH_PULL> pwmY;
};

#endif // PWM_H
