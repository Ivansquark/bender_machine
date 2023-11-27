#include "main.h"

#include "adc.h"
#include "basic_timer.h"
#include "gpio.h"
#include "led.h"
#include "rcc.h"
#include "tirq.h"

#include <stdlib.h>

/* Global Variable */
uint32_t SystemCoreClock = 144000000;
//------------- objects in static memory (.data section) ----------------------
Rcc rcc(8);
//-----------------------------------------------------------------------------

int main(void) {

    while (1) {

    }
}

