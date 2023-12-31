#ifndef STEP_H
#define STEP_H

#include "buzzer.h"
#include "gpios.h"
#include "main.h"
#include "my_udp.h"
#include "protocol.h"
#include "pwm.h"

class StepY {
  public:
    StepY();
    static StepY* pThis;

    enum State {
        STOP,
        START_MOVING,
        MOVING_PLUS,
        MOVING_MINUS
    };
    State currentState = State::STOP;

    void startPlus();
    void startMinus();
    void stop();
    bool isStopped();
    inline uint32_t getCounterPWM() { return pwm.counter; }
    inline void setCounterPWM(uint32_t val) { pwm.counter = val; }
    inline void clearPwmCounter() { pwm.counter = 0; }

    uint32_t startValue = 0;
    uint32_t stopValue = 0;
    uint32_t currentValue = 0;
    uint32_t previousValue = 0;

    void handler();

    inline bool getLimitPlus() { return konc.getState(); }
    inline bool getLimitMinus() { return zerro.getState(); }
    inline bool getLimitOtstup() { return otstup.getState(); }

    float coeff = COEFF_PWM_TO_MM;
    static constexpr float COEFF_PWM_TO_MM = 119.617F;
    uint32_t deviation = DEVIATION;
    static constexpr uint32_t DEVIATION = 50;

    inline void pwm_slow() { pwm.slow(); }
    inline void pwm_fast() { pwm.fast(); }
    inline void pwm_max() { pwm.max(); }

  private:
    // void init();
    //
    enum Dir {
        PLUS,
        MINUS
    };
    void fast_slow(Dir dir);

    inline void enableOn() { en.setLow(); }
    inline void enableOff() { en.setHigh(); }
    inline void dirOn() { dir.setHigh(); }
    inline void dirOff() { dir.setLow(); }

    PwmY pwm;
    // Gpios::Out<Gpios::PD, 10, Gpios::InitModeOut::PUSH_PULL> en;
    // Gpios::Out<Gpios::PD, 8, Gpios::InitModeOut::PUSH_PULL> dir;
    Gpios::Out<Gpios::PD, 10, Gpios::InitModeOut::PUSH_PULL> dir;
    Gpios::Out<Gpios::PD, 8, Gpios::InitModeOut::PUSH_PULL> en;
    Gpios::Out<Gpios::PD, 11, Gpios::InitModeOut::PUSH_PULL> block;
    Gpios::Out<Gpios::PD, 13, Gpios::InitModeOut::PUSH_PULL> tablo;
    Gpios::Out<Gpios::PD, 15, Gpios::InitModeOut::PUSH_PULL> motor_power;
    Gpios::In<Gpios::PB, 14, Gpios::InitModeIn::FLOATING> err;
    Gpios::In<Gpios::PA, 2, Gpios::InitModeIn::FLOATING> block_up;
    Gpios::In<Gpios::PA, 0, Gpios::InitModeIn::FLOATING> up_ok;
    Gpios::In<Gpios::PC, 1, Gpios::InitModeIn::FLOATING> konc;
    Gpios::In<Gpios::PC, 3, Gpios::InitModeIn::FLOATING> otstup;
    Gpios::In<Gpios::PA, 1, Gpios::InitModeIn::FLOATING> zerro;
    Gpios::In<Gpios::PC, 0, Gpios::InitModeIn::FLOATING> rezerv1;
    Gpios::In<Gpios::PC, 2, Gpios::InitModeIn::FLOATING> rezerv2;

    static constexpr uint32_t NUM_STEPS_FOR_SPEED_CHANGE = 50;
};

class StepX {
  public:
    StepX();
    static StepX* pThis;

    enum State {
        STOP,
        START_MOVING,
        MOVING_PLUS,
        MOVING_MINUS
    };
    State currentState = State::STOP;

    void startPlus();
    void startMinus();
    void stop();
    bool isStopped();
    inline uint32_t getCounterPWM() { return pwm.counter; }
    inline void setCounterPWM(uint32_t val) { pwm.counter = val; }
    inline void clearPwmCounter() { pwm.counter = 0; }

    uint32_t startValue = 0;
    uint32_t stopValue = 0;
    uint32_t currentValue = 0;
    uint32_t previousValue = 0;

    void handler();

    inline bool getLimitPlus() { return konc.getState(); }
    inline bool getLimitMinus() { return zerro.getState(); }
    inline bool getLimitOtstup() { return otstup.getState(); }

    float coeff = COEFF_PWM_TO_MM;
    static constexpr float COEFF_PWM_TO_MM = 100.0F;
    uint32_t deviation = COEFF_PWM_TO_MM;
    static constexpr uint32_t DEVIATION = 10;

    inline void pwm_slow() { pwm.slow(); }
    inline void pwm_fast() { pwm.fast(); }
    inline void pwm_max() { pwm.max(); }
  private:
    void init();

    enum Dir {
        PLUS,
        MINUS
    };
    void fast_slow(Dir dir);

    inline void enableOn() { en.setLow(); }
    inline void enableOff() { en.setHigh(); }
    inline void dirOn() { dir.setHigh(); }
    inline void dirOff() { dir.setLow(); }

    PwmX pwm;
    // Gpios::Out<Gpios::PE, 10, Gpios::InitModeOut::PUSH_PULL> en;
    // Gpios::Out<Gpios::PE, 12, Gpios::InitModeOut::PUSH_PULL> dir;
    Gpios::Out<Gpios::PE, 10, Gpios::InitModeOut::PUSH_PULL> dir;
    Gpios::Out<Gpios::PE, 12, Gpios::InitModeOut::PUSH_PULL> en;
    Gpios::In<Gpios::PE, 14, Gpios::InitModeIn::FLOATING> err;
    Gpios::In<Gpios::PE, 2, Gpios::InitModeIn::FLOATING> zerro;
    Gpios::In<Gpios::PE, 4, Gpios::InitModeIn::FLOATING> otstup;
    Gpios::In<Gpios::PE, 6, Gpios::InitModeIn::FLOATING> konc;
    static constexpr uint32_t NUM_STEPS_FOR_SPEED_CHANGE = 50;
};
#endif // STEPH
