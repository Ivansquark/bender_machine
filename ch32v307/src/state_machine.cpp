#include "state_machine.h"

StateMachine* StateMachine::pThis = nullptr;

StateMachine::StateMachine() { pThis = this; }

void StateMachine::handler() {
    //--------------  receive handler  ----------------------------------------
    // udp handler parse reply
    if (udp.IsDataReceived) {
        udp.IsDataReceived = false;
        if (udp.receiveCommand.currentCommand == Protocol::Commands::REPLY_PC) {
            udp.WaitForReply = false;
            udp.MustResend = false;
        }
        if (udp.WaitForReply) {
            // check timer if not send another time
            // if(timeout)
            if (IsTimeout) {
                udp.WaitForReply = false;
                udp.MustResend = true;
                IsTimeout = false;
            }
        } else {
            // parse data
            parse();
            udp.MustSendReply = true;
        }
    }
    if (udp.MustSendReply) {
        udp.sendReply();
        udp.MustSendReply = false;
    }
    //--------------  move handler  -------------------------------------------
    stepX.handler();
    stepY.handler();
    //--------------  tablo handler  ------------------------------------------
    if ((stepX.currentState == StepX::STOP) &&
        (stepY.currentState == StepY::STOP)) {
        // TODO: set tablo

    } else {

        // TODO: reset tablo
    }
    //--------------  send handler  -------------------------------------------
    if (udp.MustSend) {
        udp.MustSend = false;
        // send last reply
        udp.sendData(udp.reply);
        udp.WaitForReply = true;
        timerReplyStart(10);
    }
    //--------------  resend handler  -----------------------------------------
    if (udp.MustResend) {
        udp.MustResend = false;
        udp.sendData(udp.reply);
        udp.WaitForReply = true;
    }
    // send handler
}

void StateMachine::parse() {
    switch (udp.receiveCommand.currentCommand) {
    case Protocol::SEND_START_X:
        stepX.currentState = StepX::START_MOVING;
        break;
    case Protocol::SEND_START_Y:
        stepY.currentState = StepY::START_MOVING;
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
}
