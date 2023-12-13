#include "state_machine.h"

StateMachine* StateMachine::pThis = nullptr;

StateMachine::StateMachine() {
    pThis = this;
    stepX.stop();
    stepY.stop();
    udp.WaitForReply = false;
    // TODO: start timer to send NEED_CALIBRATION
    timerCalStart(500);
    // udp.reply.currentReply = Protocol::Replies::NEED_CALIBRATION;
    // udp.MustSend = true;
    //stepY.startPlus();
}

void StateMachine::handler() {
    //--------------  Calibration timer handler -------------------------------
    if (IsCalTimeout) {
        IsCalTimeout = false;
        udp.reply.currentReply = Protocol::Replies::NEED_CALIBRATION;
        udp.MustSend = true;
    }
    //--------------  receive handler  ----------------------------------------
    if (udp.WaitForReply) {
        if (IsTimeout) {
            udp.WaitForReply = false;
            udp.MustResend = true;
            IsTimeout = false;
        }
    }
    // udp handler parse reply
    if (udp.IsDataReceived) {
        udp.IsDataReceived = false;
        if (udp.receiveCommand.currentCommand == Protocol::Commands::REPLY_PC) {
            udp.WaitForReply = false;
            udp.MustResend = false;
            // udp.MustSend = false;
            timerReplyStop();
        }
        // if (udp.WaitForReply) {
        //     // check timer if not send another time
        //     // if(timeout)
        // }
        else {
            // parse data
            udp.sendReply();
            parse();
            // udp.MustSendReply = true;
            timerReplyStop();
        }
    } else if (udp.IsDataSetReceived) {
        udp.IsDataSetReceived = false;
        // parse data
        udp.sendReply();
        // udp.MustSendReply = true;
        timerReplyStop();
        stepX.coeff = Udp::pThis->receiveCommandSet.coefX;
        stepY.coeff = Udp::pThis->receiveCommandSet.coefY;
        stepX.deviation = Udp::pThis->receiveCommandSet.deviationX;
        stepY.deviation = Udp::pThis->receiveCommandSet.deviationY;
    }
    // if (udp.MustSendReply) {
    //     udp.sendReply();
    //     udp.MustSendReply = false;
    // }
    //--------------  move handler  -------------------------------------------
    if (IsCalibrated) {
        stepX.handler();
        stepY.handler();
    } else {
        calibrationHandler();
    }
    //--------------  tablo handler  ------------------------------------------
    if ((stepX.currentState == StepX::STOP) &&
        (stepY.currentState == StepY::STOP)) {
        // TODO: set tablo

    } else {

        // TODO: reset tablo
    }
    //--------------  send handler  -------------------------------------------
    if (!udp.WaitForReply) {
        if (udp.MustSend) {
            udp.MustSend = false;
            udp.MustResend = false;
            // send last reply
            if (IsMustReplySettings) {
                IsMustReplySettings = false;
                udp.sendDataSet(udp.replySet);
            } else {
                udp.sendData(udp.reply);
            }
            udp.WaitForReply = true;
            timerReplyStart(100);
        }
    }
    //--------------  resend handler  -----------------------------------------
    if (udp.MustResend) {
        udp.MustSend = false;
        udp.MustResend = false;
        udp.sendData(udp.reply);
        udp.WaitForReply = true;
        timerReplyStart(100);
    }
}
//------------------- parse received data -------------------------------------
void StateMachine::parse() {
    switch (udp.receiveCommand.currentCommand) {
    case Protocol::SEND_START_X:
        stepX.currentState = StepX::START_MOVING;
        stepX.stopValue = udp.receiveCommand.val;
        break;
    case Protocol::SEND_START_Y:
        stepY.currentState = StepY::START_MOVING;
        stepY.stopValue = udp.receiveCommand.val;
        break;
    case Protocol::SEND_STOP:
        stepY.stop();
        stepY.currentState = StepY::STOP;
        stepX.stop();
        stepX.currentState = StepX::STOP;
        break;
    case Protocol::SEND_NEW_VAL_X:
        stepX.stopValue = udp.receiveCommand.val;
        udp.MustSend = true;
        udp.reply.currentReply = Protocol::Replies::VAL_X;
        udp.reply.val = stepX.stopValue;
        break;
    case Protocol::SEND_NEW_VAL_Y:
        stepY.stopValue = udp.receiveCommand.val;
        udp.MustSend = true;
        udp.reply.currentReply = Protocol::Replies::VAL_Y;
        udp.reply.val = stepY.stopValue;
        break;
    case Protocol::SEND_Y_PLUS:
        stepY.stopValue = udp.receiveCommand.val;
        stepY.currentState = StepY::START_MOVING;
        break;
    case Protocol::SEND_Y_MINUS:
        stepY.stopValue = udp.receiveCommand.val;
        stepY.currentState = StepY::START_MOVING;
        break;
    case Protocol::SEND_X_PLUS:
        stepX.stopValue = udp.receiveCommand.val;
        stepX.currentState = StepX::START_MOVING;
        break;
    case Protocol::SEND_X_MINUS:
        stepX.stopValue = udp.receiveCommand.val;
        stepX.currentState = StepX::START_MOVING;
        break;
    case Protocol::SEND_CALIBRATION_START:
        if (!IsCalibrated) {
            currentCalibrationState = CalibrationStates::CAL_X_START;
        }
        break;
    case Protocol::SEND_CALIBRATION_STOP:
        if (!IsCalibrated) {
            currentCalibrationState = CalibrationStates::NONE;
            stepX.stop();
            stepY.stop();
        }
        break;
    case Protocol::SEND_GET_SETTINGS:
        udp.MustSend = true;
        udp.replySet.currentReply = Protocol::SETTINGS;
        udp.replySet.coefY = stepY.coeff;
        udp.replySet.coefX = stepX.coeff;
        udp.replySet.deviationY = stepY.deviation;
        udp.replySet.deviationX = stepX.deviation;
        IsMustReplySettings = true;
        break;
    default:
        break;
    }
}

//---------------------- Calibration ------------------------------------------
void StateMachine::calibrationHandler() {
    switch (currentCalibrationState) {
    case NONE:
        break;
    case CAL_X_START:
        udp.MustSend = true;
        udp.reply.currentReply = Protocol::Replies::CALIBRATION_START;
        stepX.startMinus();
        currentCalibrationState = CAL_X;
        // TODO: check speed
        break;
    case CAL_X:
        if (stepX.getLimitMinus()) {
            stepX.stop();
            stepX.currentValue = 0;
            stepX.clearPwmCounter();
            currentCalibrationState = CAL_X_DEVIATION_START;
            // currentCalibrationState = CAL_X_STOP;
        }
        break;
    case CAL_X_DEVIATION_START:
        stepX.startPlus();
        stepX.stopValue = stepX.deviation;
        currentCalibrationState = CAL_X_DEVIATION;
        break;
    case CAL_X_DEVIATION:
        stepX.currentValue = stepX.getCounterPWM() / stepX.coeff;
        if (stepX.currentValue >= stepX.stopValue) {
            stepX.stop();
            stepX.currentValue = 0;
            stepX.clearPwmCounter();
            currentCalibrationState = CAL_X_STOP;
        }
        break;
    case CAL_X_STOP:
        udp.MustSend = true;
        udp.reply.currentReply = Protocol::Replies::CALIBRATION_X_STOP;
        udp.reply.val = stepX.currentValue;
        currentCalibrationState = CAL_Y_START;
        break;
    case CAL_Y_START:
        stepY.startMinus();
        currentCalibrationState = CAL_Y;
        break;
    case CAL_Y:
        if (stepY.getLimitMinus()) {
            stepY.stop();
            stepY.currentValue = 0;
            stepY.clearPwmCounter();
            currentCalibrationState = CAL_Y_DEVIATION_START;
            // currentCalibrationState = CAL_Y_STOP;
        }
        break;
    case CAL_Y_DEVIATION_START:
        stepY.startPlus();
        stepY.stopValue = stepY.deviation;
        currentCalibrationState = CAL_Y_DEVIATION;
        break;
    case CAL_Y_DEVIATION:
        stepY.currentValue = stepY.getCounterPWM() / stepY.coeff;
        if (stepY.currentValue >= stepY.stopValue) {
            stepY.stop();
            stepY.currentValue = 0;
            stepY.clearPwmCounter();
            currentCalibrationState = CAL_Y_STOP;
        }
        break;
    case CAL_Y_STOP:
        udp.MustSend = true;
        udp.reply.currentReply = Protocol::Replies::CALIBRATION_Y_STOP;
        udp.reply.val = stepY.currentValue;
        IsCalibrated = true;
        currentCalibrationState = NONE;
        break;
    default:
        break;
    }
}

//---------------------- timers -----------------------------------------------
void StateMachine::timerReplyStart(uint32_t ms) {
    timerReplyCounterMax = ms;
    timerReplyCounter = 0;
    TimerReplyStart = true;
}
void StateMachine::timerReplyStop() {
    timerReplyCounter = 0;
    TimerReplyStart = false;
}

void StateMachine::timerReplyTimeout() {
    IsTimeout = true;
    timerReplyStop();
}
void StateMachine::timerCalStart(uint32_t ms) {
    timerCalCounterMax = ms;
    timerCalCounter = 0;
    TimerCalStart = true;
}
void StateMachine::timerCalStop() {
    timerCalCounter = 0;
    TimerCalStart = false;
}

void StateMachine::timerCalTimeout() {
    IsCalTimeout = true;
    timerCalStop();
}

void StateMachine::interruptHandler() {
    if (StateMachine::pThis->TimerReplyStart) {
        if (StateMachine::pThis->timerReplyCounter >=
            StateMachine::pThis->timerReplyCounterMax) {
            StateMachine::pThis->timerReplyTimeout();
            StateMachine::pThis->timerReplyCounter = 0;
            StateMachine::pThis->TimerReplyStart = false;
        } else {
            StateMachine::pThis->timerReplyCounter++;
        }
    }
    if (StateMachine::pThis->TimerCalStart) {
        if (StateMachine::pThis->timerCalCounter >=
            StateMachine::pThis->timerCalCounterMax) {
            StateMachine::pThis->timerCalTimeout();
            StateMachine::pThis->timerCalCounter = 0;
            StateMachine::pThis->TimerCalStart = false;
        } else {
            StateMachine::pThis->timerCalCounter++;
        }
    }
}
