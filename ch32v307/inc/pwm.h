#ifndef PWM_H
#define PWM_H

#include "gpios.h"
#include "main.h"

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

    inline void startY() { TIM1->CTLR1 |= TIM_CEN; }
    inline void stopY() { TIM1->CTLR1 &= ~TIM_CEN; }
    inline void startX() { TIM9->CTLR1 |= TIM_CEN; }
    inline void stopX() { TIM9->CTLR1 &= TIM_CEN; }

  private:
    void init();
    Gpios::Out<Gpios::PE, 8, Gpios::InitModeOut::ALTERNATE_PUSH_PULL> pwmY;
    Gpios::Out<Gpios::PD, 12, Gpios::InitModeOut::ALTERNATE_PUSH_PULL> pwmX;
};

#endif // PWM_H
