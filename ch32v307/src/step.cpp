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

bool StepY::isStopped() {
    return pwm.isYstopped();
}

void StepY::handler() {
    currentValue = pwm.counterY / coeff;
    switch (currentState) {
    case STOP:
        break;
    case START_MOVING:
        //if (currentValue < stopValue) {
        if (pwm.counterY < stopValue * coeff) {
            currentState = MOVING_PLUS;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::START_Y;
            Udp::pThis->reply.val = currentValue;
            startPlus();
            // TODO: block all
        //} else if (currentValue > stopValue) {
        } else if (pwm.counterY > stopValue * coeff) {
            currentState = MOVING_MINUS;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::START_Y;
            Udp::pThis->reply.val = currentValue;
            startMinus();
            // TODO: block all
        } else {
            currentState = STOP;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_Y;
            Udp::pThis->reply.val = currentValue;
        }
        break;
    case MOVING_PLUS:
        //if (currentValue >= stopValue) {
        if (pwm.counterY >= stopValue * coeff) {
            stop();
            currentState = STOP;
            // send signal about final place
            currentValue = pwm.counterY / coeff;
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
        //if (currentValue <= stopValue) {
        if (pwm.counterX <= stopValue * coeff) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
            currentValue = pwm.counterY / coeff;
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

bool StepX::isStopped() {
    return pwm.isXstopped();
}

void StepX::handler() {
    // currentValue = (pwm.counterX) / coeff;
    currentValue = (pwm.counterX) / coeff;
    switch (currentState) {
    case STOP:
        break;
    case START_MOVING:
        // if (currentValue < stopValue) {
        if (pwm.counterX < stopValue * coeff) {
            currentState = MOVING_PLUS;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::START_X;
            Udp::pThis->reply.val = currentValue;
            startPlus();
            // TODO: block all
            //} else if (currentValue > stopValue) {
        } else if (pwm.counterX > stopValue * coeff) {
            currentState = MOVING_MINUS;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::START_X;
            Udp::pThis->reply.val = currentValue;
            startMinus();
            // TODO: block all
        } else {
            currentState = STOP;
            // TODO: send signal about final place
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_X;
            Udp::pThis->reply.val = currentValue;
        }
        break;
    case MOVING_PLUS:
        if (pwm.counterX >= stopValue * coeff) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
            currentValue = (pwm.counterX) / coeff;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_X;
            Udp::pThis->reply.val = currentValue;
        } else {
            // send currentValue
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
            //if (getLimitMinus()) {
            //    stop();
            //    currentState = STOP;
            //    // send signal about final place
            //    Udp::pThis->MustSend = true;
            //    Udp::pThis->reply.currentReply = Protocol::LIMIT_X_MINUS;
            //    Udp::pThis->reply.val = currentValue;
            //}
        }
        break;
    case MOVING_MINUS:
        // if (currentValue <= stopValue) {
        if (pwm.counterX <= stopValue * coeff) {
            // if (pwm.counterX * coeff >= stopValue) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
            currentValue = (pwm.counterX) / coeff;
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
            //if (getLimitPlus()) {
            //    stop();
            //    currentState = STOP;
            //    // send signal about final place
            //    Udp::pThis->MustSend = true;
            //    Udp::pThis->reply.currentReply = Protocol::LIMIT_X_PLUS;
            //    Udp::pThis->reply.val = currentValue;
            //}

        }
        break;
    default:
        break;
    }
}
