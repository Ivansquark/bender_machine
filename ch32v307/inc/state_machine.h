#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "step.h"
#include "my_udp.h"

class StateMachine {
    public:
    StateMachine();
    static StateMachine* pThis;
    void handler();
    //------------- interrupt -------------------------------------------------
    static void interruptHandler();
    //------------- timers ----------------------------------------------------
    bool TimerReplyStart = false;
    uint32_t timerReplyCounter = 0;
    uint32_t timerReplyCounterMax = 0;
    void timerReplyStart(uint32_t ms);
    void timerReplyStop();
    void timerReplyTimeout();
    bool IsTimeout = false;

    bool TimerCalStart = false;
    uint32_t timerCalCounter = 0;
    uint32_t timerCalCounterMax = 0;
    void timerCalStart(uint32_t ms);
    void timerCalStop();
    void timerCalTimeout();
    bool IsCalTimeout = false;


    bool IsCalibrated = false;

    bool IsMustReplySettings = false;

    enum CalibrationStates{
        NONE,
        CAL_X_START,
        CAL_X_TO_LIMIT_PLUS,
        CAL_X_TO_LIMIT_OTSTUP,
        CAL_X_DEVIATION,
        CAL_X_STOP,
        CAL_Y_START,
        CAL_Y_TO_LIMIT_PLUS,
        CAL_Y_TO_LIMIT_OTSTUP,
        CAL_Y_DEVIATION,
        CAL_Y_STOP
    };
    CalibrationStates currentCalibrationState = CalibrationStates::NONE;
    void calibrationHandler();

    private:
    Udp udp;
    StepX stepX;
    StepY stepY;
    void parse();
};

#endif //STATE_MACHINE_H
