#include "step.h"

StepY* StepY::pThis = nullptr;

StepY::StepY() { pThis = this; }

void StepY::startPlus() {
    pwm.currentDirection = PwmY::Direction::UP;
    enableOn();
    dirOn();
    pwm.start();
    Buzzer::pThis->setMustBuzz(true);
}

void StepY::startMinus() {
    pwm.currentDirection = PwmY::Direction::DOWN;
    enableOn();
    dirOff();
    pwm.start();
    Buzzer::pThis->setMustBuzz(true);
}

void StepY::stop() {
    pwm.stop();
    Buzzer::pThis->setMustBuzz(false);
}

bool StepY::isStopped() { return pwm.isStopped(); }

void StepY::fast_slow(Dir dir) {
    if (dir == Dir::PLUS) {
        if (stopValue - startValue < NUM_STEPS_FOR_SPEED_CHANGE) {
            pwm.slow();
            return;
        }
        if ((currentValue > (startValue + NUM_STEPS_FOR_SPEED_CHANGE)) &&
            (currentValue < (stopValue - NUM_STEPS_FOR_SPEED_CHANGE))) {
            pwm.fast();
        } else {
            pwm.slow();
        }
    } else {
        if (startValue - stopValue < NUM_STEPS_FOR_SPEED_CHANGE) {
            pwm.slow();
            return;
        }
        if ((currentValue < (startValue - NUM_STEPS_FOR_SPEED_CHANGE)) &&
            (currentValue > (stopValue + NUM_STEPS_FOR_SPEED_CHANGE))) {
            pwm.fast();
        } else {
            pwm.slow();
        }
    }
}

void StepY::handler() {
    currentValue = pwm.counter / coeff;
    switch (currentState) {
    case STOP:
        break;
    case START_MOVING:
        // if (currentValue < stopValue) {
        if (pwm.counter < (uint32_t)((float)stopValue * coeff)) {
            currentState = MOVING_PLUS;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::START_Y;
            Udp::pThis->reply.val = currentValue;
            startValue = currentValue;
            startPlus();
            pwm.slow();
            // TODO: block all
            //} else if (currentValue > stopValue) {
        } else if (pwm.counter > (uint32_t)((float)stopValue * coeff)) {
            currentState = MOVING_MINUS;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::START_Y;
            Udp::pThis->reply.val = currentValue;
            startValue = currentValue;
            startMinus();
            pwm.slow();
            // TODO: block all
        } else {
            currentState = STOP;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_Y;
            Udp::pThis->reply.val = currentValue;
        }
        break;
    case MOVING_PLUS:
        // if (currentValue >= stopValue) {
        if (pwm.counter >= (uint32_t)((float)stopValue * coeff)) {
            stop();
            currentState = STOP;
            // send signal about final place
            currentValue = (uint32_t)((float)pwm.counter / coeff);
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_Y;
            Udp::pThis->reply.val = currentValue;
        } else {
            // check for fast or low speed
            fast_slow(Dir::PLUS);
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
        // if (currentValue <= stopValue) {
        if (pwm.counter <= (uint32_t)((float)stopValue * coeff)) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
            currentValue = (uint32_t)((float)pwm.counter / coeff);
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_Y;
            Udp::pThis->reply.val = currentValue;
        } else {
            fast_slow(Dir::MINUS);
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
    pwm.currentDirection = PwmX::Direction::PLUS;
    enableOn();
    dirOn();
    pwm.start();
    Buzzer::pThis->setMustBuzz(true);
}

void StepX::startMinus() {
    pwm.currentDirection = PwmX::Direction::MINUS;
    enableOn();
    dirOff();
    pwm.start();
    Buzzer::pThis->setMustBuzz(true);
}

void StepX::stop() {
    pwm.stop();
    Buzzer::pThis->setMustBuzz(false);
}

bool StepX::isStopped() { return pwm.isStopped(); }

void StepX::fast_slow(Dir dir) {
    if (dir == Dir::PLUS) {
        if (stopValue - startValue < NUM_STEPS_FOR_SPEED_CHANGE) {
            pwm.slow();
            return;
        }
        if ((currentValue > (startValue + NUM_STEPS_FOR_SPEED_CHANGE)) &&
            (currentValue < (stopValue - NUM_STEPS_FOR_SPEED_CHANGE))) {
            pwm.fast();
        } else {
            pwm.slow();
        }
    } else {
        if (startValue - stopValue < NUM_STEPS_FOR_SPEED_CHANGE) {
            pwm.slow();
            return;
        }
        if ((currentValue < (startValue - NUM_STEPS_FOR_SPEED_CHANGE)) &&
            (currentValue > (stopValue + NUM_STEPS_FOR_SPEED_CHANGE))) {
            pwm.fast();
        } else {
            pwm.slow();
        }
    }
}

void StepX::handler() {
    // currentValue = (pwm.counterX) / coeff;
    currentValue = (uint32_t)((float)(pwm.counter) / coeff);
    switch (currentState) {
    case STOP:
        break;
    case START_MOVING:
        // if (currentValue < stopValue) {
        if (pwm.counter < (uint32_t)((float)stopValue * coeff)) {
            currentState = MOVING_PLUS;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::START_X;
            Udp::pThis->reply.val = currentValue;
            startValue = currentValue;
            startPlus();
            // TODO: block all
            //} else if (currentValue > stopValue) {
        } else if (pwm.counter > (uint32_t)((float)stopValue * coeff)) {
            currentState = MOVING_MINUS;
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::START_X;
            Udp::pThis->reply.val = currentValue;
            startValue = currentValue;
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
        if (pwm.counter >= (uint32_t)((float)stopValue * coeff)) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
            currentValue = (uint32_t)((float)(pwm.counter) / coeff);
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_X;
            Udp::pThis->reply.val = currentValue;
        } else {
            fast_slow(Dir::PLUS);
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
            // if (getLimitMinus()) {
            //     stop();
            //     currentState = STOP;
            //     // send signal about final place
            //     Udp::pThis->MustSend = true;
            //     Udp::pThis->reply.currentReply = Protocol::LIMIT_X_MINUS;
            //     Udp::pThis->reply.val = currentValue;
            // }
        }
        break;
    case MOVING_MINUS:
        // if (currentValue <= stopValue) {
        if (pwm.counter <= (uint32_t)((float)stopValue * coeff)) {
            // if (pwm.counterX * coeff >= stopValue) {
            stop();
            currentState = STOP;
            // TODO: send signal about final place
            currentValue = (uint32_t)((float)(pwm.counter) / coeff);
            Udp::pThis->MustSend = true;
            Udp::pThis->reply.currentReply = Protocol::STOP_X;
            Udp::pThis->reply.val = currentValue;
        } else {
            fast_slow(Dir::MINUS);
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
            // if (getLimitPlus()) {
            //     stop();
            //     currentState = STOP;
            //     // send signal about final place
            //     Udp::pThis->MustSend = true;
            //     Udp::pThis->reply.currentReply = Protocol::LIMIT_X_PLUS;
            //     Udp::pThis->reply.val = currentValue;
            // }
        }
        break;
    default:
        break;
    }
}
