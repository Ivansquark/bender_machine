#include "step.h"

StepY* StepY::pThis = nullptr;

StepY::StepY() {
    pThis = this;
    init();
}

void StepY::startPlus() {
    pwm.currentDirectionY = Pwm::DirectionY::UP;
    enableOn();
    dirOn();
    pwm.startY();
    Buzzer::pThis->setMustBuzz(true);
}

void StepY::startMinus() {
    pwm.currentDirectionY = Pwm::DirectionY::DOWN;
    enableOn();
    dirOff();
    pwm.startY();
    Buzzer::pThis->setMustBuzz(true);
}

void StepY::stop() {
    pwm.stopY();
    Buzzer::pThis->setMustBuzz(false);
}

void StepY::handler() {
    switch (currentState) {
    case STOP:
        break;
    case START_MOVING:
        if (currentValue < stopValue) {
            startPlus();
            currentState = MOVING_PLUS;
        } else if (currentValue > stopValue) {
            startMinus();
            currentState = MOVING_MINUS;
        } else {
            currentState = STOP;
            // TODO: send signal about final place
        }
        break;
    case MOVING_PLUS:
        if (currentValue >= stopValue) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
        } else {
            // TODO: send currentValue
            // TODO: check limit switch
        }
        break;
    case MOVING_MINUS:
        if (currentValue <= stopValue) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
        } else {
            // TODO: send currentValue
            // TODO: check limit switch
        }
        break;
    default:
        break;
    }
}

StepX* StepX::pThis = nullptr;

StepX::StepX() {
    pThis = this;
    init();
}

void StepX::startPlus() {
    pwm.currentDirectionX = Pwm::DirectionX::PLUS;
    enableOn();
    dirOn();
    pwm.startX();
    Buzzer::pThis->setMustBuzz(true);
}

void StepX::startMinus() {
    pwm.currentDirectionX = Pwm::DirectionX::MINUS;
    enableOn();
    dirOff();
    pwm.startX();
    Buzzer::pThis->setMustBuzz(true);
}

void StepX::stop() {
    pwm.stopX();
    Buzzer::pThis->setMustBuzz(false);
}

void StepX::handler() {
    switch (currentState) {
    case STOP:
        break;
    case START_MOVING:
        if (currentValue < stopValue) {
            startPlus();
            currentState = MOVING_PLUS;
        } else if (currentValue > stopValue) {
            startMinus();
            currentState = MOVING_MINUS;
        } else {
            currentState = STOP;
            // TODO: send signal about final place
        }
        break;
    case MOVING_PLUS:
        if (currentValue >= stopValue) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
        } else {
            // TODO: send currentValue
            // TODO: check limit switch
        }
        break;
    case MOVING_MINUS:
        if (currentValue <= stopValue) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
        } else {
            // TODO: send currentValue
            // TODO: check limit switch
        }
        break;
    default:
        break;
    }
}
