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
    bool TimerReplyStart = false;
    uint32_t timerReplyCounter = 0;
    uint32_t timerReplyCounterMax = 0;
    void timerReplyStart(uint32_t ms);
    void timerReplyStop();
    void timerReplyTimeout();
    bool IsTimeout = false;

    private:
    Udp udp;
    StepX stepX;
    StepY stepY;
    void parse();
};

#endif //STATE_MACHINE_H
