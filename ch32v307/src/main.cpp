#include "main.h"

#include "basic_timer.h"
#include "eth.h"
#include "gpio.h"
#include "led.h"
#include "my_udp.h"
#include "rcc.h"
#include "state_machine.h"
#include "tirq.h"

#include <stdlib.h>

/* Global Variable */
uint32_t SystemCoreClock = 144000000;
//------------- objects in static memory (.data section) ----------------------
Rcc rcc(24);
Eth eth(192, 168, 1, 5);
StateMachine stateMachine;
//-----------------------------------------------------------------------------

int main(void) {
    ObserverForInterrupt<TIM6_IRQn, StateMachine> obs;
    BasicTimer6::Instance().start(1);
    while (1) {
        eth.rx_handler();
        stateMachine.handler();
    }
}
