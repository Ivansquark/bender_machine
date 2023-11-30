#include "step.h"

StepY* StepY::pThis = nullptr;

StepY::StepY() { pThis = this; }

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
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::START_Y;
            Udp::pThis->reply.val = currentValue;
        } else if (currentValue > stopValue) {
            startMinus();
            currentState = MOVING_MINUS;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::START_Y;
            Udp::pThis->reply.val = currentValue;
        } else {
            currentState = STOP;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_Y;
            Udp::pThis->reply.val = currentValue;
        }
        break;
    case MOVING_PLUS:
        if (currentValue >= stopValue) {
            stop();
            currentState = STOP;
            // send signal about final place
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_Y;
            Udp::pThis->reply.val = currentValue;
        } else {
            // TODO: send currentValue
            if (previousValue != currentValue) {
                Udp::pThis->MustSend = true;
                Udp::pThis->reply.currentReply = Protocol::CURRENT_Y;
                Udp::pThis->reply.val = currentValue;
            }
            previousValue = currentValue;
            // TODO: check limit switch
            if (getLimitPlus()) {
                stop();
                currentState = STOP;
                // send signal about final place
                Udp::pThis->MustSend = true;
                Udp::pThis->reply.currentReply = Protocol::LIMIT_Y_PLUS;
                Udp::pThis->reply.val = currentValue;
            }
        }
        break;
    case MOVING_MINUS:
        if (currentValue <= stopValue) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_Y;
            Udp::pThis->reply.val = currentValue;
        } else {
            // TODO: send currentValue
            if (previousValue != currentValue) {
                Udp::pThis->MustSend = true;
                Udp::pThis->reply.currentReply = Protocol::CURRENT_Y;
                Udp::pThis->reply.val = currentValue;
            }
            previousValue = currentValue;
            // TODO: check limit switch
            if (getLimitMinus()) {
                stop();
                currentState = STOP;
                // send signal about final place
                Udp::pThis->MustSend = true;
                Udp::pThis->reply.currentReply = Protocol::LIMIT_Y_MINUS;
                Udp::pThis->reply.val = currentValue;
            }
        }
        break;
    default:
        break;
    }
}

StepX* StepX::pThis = nullptr;

StepX::StepX() { pThis = this; }

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
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::START_X;
            Udp::pThis->reply.val = currentValue;
        } else if (currentValue > stopValue) {
            startMinus();
            currentState = MOVING_MINUS;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::START_X;
            Udp::pThis->reply.val = currentValue;
        } else {
            currentState = STOP;
            // TODO: send signal about final place
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_X;
            Udp::pThis->reply.val = currentValue;
        }
        break;
    case MOVING_PLUS:
        if (currentValue >= stopValue) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_X;
            Udp::pThis->reply.val = currentValue;
        } else {
            // TODO: send currentValue
            if (previousValue != currentValue) {
                Udp::pThis->MustSend = true;
                Udp::pThis->reply.currentReply = Protocol::CURRENT_X;
                Udp::pThis->reply.val = currentValue;
            }
            previousValue = currentValue;
            // TODO: check limit switch
            if (getLimitPlus()) {
                stop();
                currentState = STOP;
                // send signal about final place
                Udp::pThis->MustSend = true;
                Udp::pThis->reply.currentReply = Protocol::LIMIT_X_PLUS;
                Udp::pThis->reply.val = currentValue;
            }
        }
        break;
    case MOVING_MINUS:
        if (currentValue <= stopValue) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_X;
            Udp::pThis->reply.val = currentValue;
        } else {
            // TODO: send currentValue
            if (previousValue != currentValue) {
                Udp::pThis->MustSend = true;
                Udp::pThis->reply.currentReply = Protocol::CURRENT_X;
                Udp::pThis->reply.val = currentValue;
            }
            previousValue = currentValue;
            // TODO: check limit switch
            if (getLimitMinus()) {
                stop();
                currentState = STOP;
                // send signal about final place
                Udp::pThis->MustSend = true;
                Udp::pThis->reply.currentReply = Protocol::LIMIT_X_MINUS;
                Udp::pThis->reply.val = currentValue;
            }
        }
        break;
    default:
        break;
    }
}
