#include "pwm.h"

Pwm* Pwm::pThis = nullptr;
uint32_t Pwm::counterY = 0;
uint32_t Pwm::counterX = 0;
Pwm::DirectionX Pwm::currentDirectionX = Pwm::DirectionX::PLUS;
Pwm::DirectionY Pwm::currentDirectionY = Pwm::DirectionY::UP;

Pwm::Pwm() {
    pThis = this;
    init();
}

void Pwm::init() {
    // TODO: init 2 pwm E8-TIM1_CH1N_3 D12-TIM9_CH3N_2
    RCC->APB2PCENR |= RCC_TIM1EN;
    RCC->APB2PCENR |= RCC_TIM9EN;

    RCC->APB2PCENR |= RCC_AFIOEN;
    // 1:1 TIM1 CH1N - E8 - full remap
    AFIO->PCFR1 |= AFIO_PCFR1_TIM1_REMAP;

    TIM1->CHCTLR2 |= TIM_OC1PE; // Output compare 3 preload enable (comparator)
    TIM1->CHCTLR2 &= ~TIM_OC1M_0;
    TIM1->CHCTLR2 |= TIM_OC1M_1;
    TIM1->CHCTLR2 |= TIM_OC1M_2; // 1:1:0 pwm straight polarity
    TIM1->CHCTLR2 &= ~TIM_CC1S;
    TIM1->CCER |= TIM_CC1NE;  //| TIM_CCER_CC3NP; //signal on correponding pin
    TIM1->CCER &= ~TIM_CC1NP; // no inverting //active high
    TIM1->PSC = 8 - 1;        // TIM clk = 8000000  (clk APB2) => 1 MHz
    // auto reload register 1000000/1000 = 1000 Hz frequency
    TIM1->ATRLR = 1000 - 1;
    TIM1->CTLR1 |= TIM_ARPE;    // auto reload in register enable
    TIM1->DMAINTENR |= TIM_UIE; // update interrupt enable

    // TIM1->CH1CVR = 500; // duty cycle 50%
    TIM1->CH2CVR = TIM1->ATRLR / 2; // duty cycle 50%
    // OC and OCN outputs are enabled if their respective enable bits are set
    // (CCxE, CCxNE in TIMx_CCER register).
    TIM1->BDTR |= TIM_MOE; // Main output enable
    // TIM1->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM1_UP_IRQn); // irq enable

    // 1:1 TIM9 CH2 - D11 - full remap
    AFIO->PCFR2 |= AFIO_PCFR1_TIM2_REMAP;

    TIM9->CHCTLR2 |= TIM_OC3PE; // Output compare 2 preload enable (comparator)
    TIM9->CHCTLR2 &= ~TIM_OC3M_0;
    TIM9->CHCTLR2 |= TIM_OC3M_1;
    TIM9->CHCTLR2 |= TIM_OC3M_2; // 1:1:0 pwm straight polarity
    TIM9->CHCTLR2 &= ~TIM_CC3S;
    TIM9->CCER |= TIM_CC3NE;  // signal on correponding pin
    TIM9->CCER &= ~TIM_CC3NP; // no inverting //active high
    TIM9->PSC = 8 - 1;        // TIM clk = 8000000  (clk APB2) => 1 MHz
    // auto reload register 1000000/1000 = 1000 Hz frequency
    TIM9->ATRLR = 1000 - 1;
    TIM9->CTLR1 |= TIM_ARPE;    // auto reload in register enable
    TIM9->DMAINTENR |= TIM_UIE; // update interrupt enable

    TIM9->CH2CVR = TIM9->ATRLR / 2; // duty cycle 50%
    // OC and OCN outputs are enabled if their respective enable bits are set
    // (CCxE, CCxNE in TIMx_CCER register).
    TIM9->BDTR |= TIM_MOE; // Main output enable
    // TIM1->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM9_UP_IRQn); // irq enable
}
extern "C" 
__attribute__((interrupt)) void TIM1_UP_IRQHandler() {
    TIM1->INTFR &= ~TIM_UIF; // drop Update interrupt flag
    if (Pwm::currentDirectionY == Pwm::DirectionY::UP) {
        Pwm::counterY++;
    } else {
        if (Pwm::counterY > 0) {
            Pwm::counterY--;
        }
    }
}
extern "C" 
__attribute__((interrupt)) void TIM9_UP_IRQHandler() {
    TIM9->INTFR &= ~TIM_UIF; // drop Update interrupt flag
    if (Pwm::currentDirectionX == Pwm::DirectionX::PLUS) {
        Pwm::counterX++;
    } else {
        if (Pwm::counterX > 0) {
            Pwm::counterX--;
        }
    }
}
