#include "screen.h"
#include "style.h"

Screen::Screen(QWidget* parent) : QDialog(parent) {
    setAttribute(Qt::WA_AcceptTouchEvents, true);
    init();
}

Screen::~Screen() {
    delete layVleftY;
    delete layVleftX;
    delete layVleftBut;
    delete layVright;
    delete layH;
}

void Screen::init() {
    setFixedSize(1024, 600);
    setStyleSheet(Style::BackgroundMain);
    // connect(butExit, &QPushButton::clicked, [this] { close(); });
    //labGetPosY->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    labGetPosY->setMaximumWidth(170);
    labGetPosX->setMaximumWidth(170);
    layVleftY->addWidget(labSetPosY);
    layVleftY->addWidget(labInfoY);
    layVleftY->addWidget(labPosY);
    layVleftY->addWidget(labGetPosY);
    layVleftX->addWidget(labSetPosX);
    layVleftX->addWidget(labInfoX);
    layVleftX->addWidget(labPosX);
    layVleftX->addWidget(labGetPosX);
    layVright->addWidget(programs);
    layVright->addStretch(0);
    layVright->addWidget(keyboard);
    //layVright->setStretch(1,1);
    // layVright->addWidget(butExit);
    layH->addLayout(layVleftY);
    layH->addLayout(layVleftX);
    layH->addLayout(layVleftBut);
    // control
    layVleft->addWidget(control);
    layH->addLayout(layVleft);
    // buttons + program
    layH->addLayout(layVright);

    // setLayout(layVleft);
    setLayout(layH);
    connect(keyboard, &Keyboard::onBut0Clicked, this, &Screen::onBut0Clicked);
    connect(keyboard, &Keyboard::onBut1Clicked, this, &Screen::onBut1Clicked);
    connect(keyboard, &Keyboard::onBut2Clicked, this, &Screen::onBut2Clicked);
    connect(keyboard, &Keyboard::onBut3Clicked, this, &Screen::onBut3Clicked);
    connect(keyboard, &Keyboard::onBut4Clicked, this, &Screen::onBut4Clicked);
    connect(keyboard, &Keyboard::onBut5Clicked, this, &Screen::onBut5Clicked);
    connect(keyboard, &Keyboard::onBut6Clicked, this, &Screen::onBut6Clicked);
    connect(keyboard, &Keyboard::onBut7Clicked, this, &Screen::onBut7Clicked);
    connect(keyboard, &Keyboard::onBut8Clicked, this, &Screen::onBut8Clicked);
    connect(keyboard, &Keyboard::onBut9Clicked, this, &Screen::onBut9Clicked);
    connect(keyboard, &Keyboard::onButXClicked, this, &Screen::onButXClicked);
    connect(keyboard, &Keyboard::onButYClicked, this, &Screen::onButYClicked);
    connect(keyboard, &Keyboard::onButEnterClicked, this,
            &Screen::onButEnterClicked);
    //-------------- text mem -------------------------------------------------
    labSetPosX->setStyleSheet(Style::TextTemp);
    labSetPosX->setText(*strTempValX);
    labSetPosY->setStyleSheet(Style::TextTemp);
    labSetPosY->setText(*strTempValY);
    labGetPosX->setStyleSheet(Style::TextCurrent);
    labGetPosY->setStyleSheet(Style::TextCurrent);
    //! TODO: check eeprom programs
    //-------------- programs -------------------------------------------------
    // get last values
    currentPmode = fileops.getCurrentPmode();
    currentPnum = fileops.getCurrentPnum();
    currentPauto = fileops.getCurrentPauto();
    getYX();
    programs->setCurrentButton(currentPnum);
    control->setPmode(currentPmode);
    control->setPauto(currentPauto);

    connect(programs, &Programs::sendCurrentButtonPressed,
            [this](Fileops::Pnum num) {
                currentPnum = num;
                fileops.setFilePnum(currentPnum, currentPmode);
                getYX();
            });

    //-------------- control --------------------------------------------------
    connect(control, &Control::onButStartPressed, this,
            &Screen::onButStartPressed);
    connect(control, &Control::onButStartReleased, this,
            &Screen::onButStartReleased);
    connect(control, &Control::sendCurrentModePressed,
            [this](Fileops::Pmode mode) {
                currentPmode = mode;
                fileops.setFilePnum(currentPnum, currentPmode);
                getYX();
            });
    connect(control, &Control::onButManual, [this] {
        currentPauto = Fileops::MANUAL;
        fileops.setFilePauto(Fileops::MANUAL);
    });
    connect(control, &Control::onButSemiAuto, [this] {
        currentPauto = Fileops::SEMI_AUTO;
        fileops.setFilePauto(Fileops::SEMI_AUTO);
    });
    connect(control, &Control::onButAuto, [this] {
        currentPauto = Fileops::AUTO;
        fileops.setFilePauto(Fileops::AUTO);
    });
    //----------------- BUT PLUS MINUS ----------------------------------------
    connect(control, &Control::onButPlusPressed, [this] {
        IsPlusPressed = true;
        IsMinusPressed = false;
        getYX();
        if (currentMoveXorY == MoveXorY::Y) {
            labSetPosY->setStyleSheet(Style::TextMove);
            // qDebug() << "currentY" << currentY;
            currentCommand.val = ++currentY;
            // qDebug() << "++currentY" << currentY << currentCommand.val;
            currentCommand.currentCommand = Protocol::Commands::SEND_Y_PLUS;
            interface->sendData(currentCommand);
            timerWaitForPress->stop();
            timerWaitForPress->start(TIME_TO_PRESS);
        } else if (currentMoveXorY == MoveXorY::X) {
            labSetPosX->setStyleSheet(Style::TextMove);
            currentCommand.val = ++currentX;
            currentCommand.currentCommand = Protocol::Commands::SEND_X_PLUS;
            interface->sendData(currentCommand);
            timerWaitForPress->stop();
            timerWaitForPress->start(TIME_TO_PRESS);
        }
    });
    connect(control, &Control::onButPlusReleased, [this] {
        IsPlusPressed = false;
        IsMinusPressed = false;
        timerForAutoSend->stop();
        timerWaitForPress->stop();
    });

    connect(control, &Control::onButMinusPressed, [this] {
        IsMinusPressed = true;
        IsPlusPressed = false;
        getYX();
        if (currentMoveXorY == MoveXorY::Y) {
            labSetPosY->setStyleSheet(Style::TextMove);
            if (currentY > 0) {
                currentCommand.val = --currentY;
            }
            currentCommand.currentCommand = Protocol::Commands::SEND_Y_MINUS;
            interface->sendData(currentCommand);
            timerWaitForPress->stop();
            timerWaitForPress->start(TIME_TO_PRESS);
        } else if (currentMoveXorY == MoveXorY::X) {
            labSetPosX->setStyleSheet(Style::TextMove);
            if (currentX > 0) {
                currentCommand.val = --currentX;
            }
            currentCommand.currentCommand = Protocol::Commands::SEND_X_MINUS;
            interface->sendData(currentCommand);
            timerWaitForPress->stop();
            timerWaitForPress->start(TIME_TO_PRESS);
        }
    });
    connect(control, &Control::onButMinusReleased, [this] {
        IsMinusPressed = false;
        IsPlusPressed = false;
        timerForAutoSend->stop();
        timerWaitForPress->stop();
    });
    //------------------- BUT CALIBRATION --------------------------------------
    connect(control, &Control::onButDashPressed, [this] {
        if (!IsCalibrated) {
            currentCommand.currentCommand =
                Protocol::Commands::SEND_CALIBRATION_START;
            interface->sendData(currentCommand);
            control->setButDashColor(true);
            //qDebug() << "onButDashPressed";
        }
    });
    connect(control, &Control::onButDashReleased, [this] {
        if (!IsCalibrated) {
            currentCommand.currentCommand =
                Protocol::Commands::SEND_CALIBRATION_STOP;
            interface->sendData(currentCommand);
            control->setButDashColor(false);
            //qDebug() << "onButDashReleased";
        }
    });
    //-------------- interface ------------------------------------------------
    connect(interface.get(), &Interface::sendCurrentReply, this,
            &Screen::getCurrentReply);
    //-------------- timers ---------------------------------------------------
    timerWaitForSaveInFile->setSingleShot(true);
    timerWaitForPress->setSingleShot(true);
    timerForAutoSend->setSingleShot(true);
    // connect(&moveTimer, &QTimer::timeout, this, &Screen::moveCycle);
    connect(timerWaitForPress.get(), &QTimer::timeout, [this] {
        // qDebug() << "Timer pressd timeout";
        if (IsPlusPressed || IsMinusPressed) {
            timerForAutoSend->start(TIME_TO_AUTO_SEND);
        }
    });
    connect(timerWaitForSaveInFile.get(), &QTimer::timeout, [this] {
        Fileops::YX yx{currentY, currentX};
        fileops.setFileValues(currentPnum, currentPmode, yx);
    });
    connect(timerForAutoSend.get(), &QTimer::timeout, [this] {
        // TODO: get X or Y
        if (currentMoveXorY == MoveXorY::X) {
            labSetPosX->setStyleSheet(Style::TextMove);
            if (IsPlusPressed) {
                // getYX();
                //if (WasXStopped) {
                    currentCommand.val = ++currentX;
                    currentCommand.currentCommand =
                        Protocol::Commands::SEND_X_PLUS;
                    interface->sendData(currentCommand);
                    timerForAutoSend->start(TIME_TO_AUTO_SEND);
                    WasXStopped = false;
                //}
            } else if (IsMinusPressed) {
                // getYX();
                //if (WasXStopped) {
                    if (currentX > 0) {
                        currentCommand.val = --currentX;
                    }
                    // qDebug() << "--currentY" << currentY <<
                    // currentCommand.val;
                    currentCommand.currentCommand =
                        Protocol::Commands::SEND_X_MINUS;
                    interface->sendData(currentCommand);
                    timerForAutoSend->start(TIME_TO_AUTO_SEND);
                    WasXStopped = false;
                //}
            }
        } else if (currentMoveXorY == MoveXorY::Y) {
            labSetPosY->setStyleSheet(Style::TextMove);
            if (IsPlusPressed) {
                //if (WasYStopped) {
                    currentCommand.val = ++currentY;
                    currentCommand.currentCommand =
                        Protocol::Commands::SEND_Y_PLUS;
                    interface->sendData(currentCommand);
                    timerForAutoSend->start(TIME_TO_AUTO_SEND);
                    WasYStopped = false;
                //}

            } else if (IsMinusPressed) {
                //if (WasYStopped) {
                    if (currentY > 0) {
                        currentCommand.val = --currentY;
                    }
                    currentCommand.currentCommand =
                        Protocol::Commands::SEND_Y_MINUS;
                    interface->sendData(currentCommand);
                    timerForAutoSend->start(TIME_TO_AUTO_SEND);
                    WasYStopped = false;
                //}
            }
        }
        timerWaitForSaveInFile->stop();
        timerWaitForSaveInFile->start(TIME_TO_SAVE_IN_FILE);
    });

    //-------------- disabling uncalibrated functional ------------------------
    setCalibrationState(true);

    //-------------- Settings -------------------------------------------------
    connect(set.get(), &Settings::sendGetData, [this]() {
        currentCommand.currentCommand = Protocol::Commands::SEND_GET_SETTINGS;
        interface->sendData(currentCommand);
    });
    connect(set.get(), &Settings::sendSetData,
            [this](bool IsNeedToWriteInFile, const Protocol::CommandSet& set) {
                if (IsNeedToWriteInFile) {
                    Fileops::Settings s{0, 0, 0, 0};
                    s.coefY = set.coefY;
                    s.coefX = set.coefX;
                    s.deviationY = set.deviationY;
                    s.deviationX = set.deviationX;
                    fileops.setSettings(s);
                }
                interface->sendDataSettings(set);
            });
    connect(interface.get(), &Interface::sendCurrentReplySet,
            [&](const Protocol::ReplySet& reply) { set->setData(reply); });
    // Fileops::Settings set{99, 99, 99, 99};
    // fileops.setSettings(set);
}

void Screen::getYX() {
    Fileops::YX yx{0, 0};
    yx = fileops.getFileValues(currentPnum, currentPmode);
    labSetPosY->setText(valToStr.valYToString(yx.Y));
    labSetPosY->setStyleSheet(Style::TextFinal);
    labSetPosX->setText(valToStr.valXToString(yx.X));
    labSetPosX->setStyleSheet(Style::TextFinal);
    currentY = yx.Y;
    currentX = yx.X;
}

void Screen::setCalibrationState(bool state) {
    programs->setEnabled(!state);
    keyboard->setEnabled(!state);
    labSetPosX->setVisible(!state);
    labSetPosY->setVisible(!state);
    control->setEnabledCalibration(state);
    labInfoX->setVisible(state);
    labInfoY->setVisible(state);
    IsCalibrated = !state;
    if(state) {
        control->setButDashColor(false);
    }
}

void Screen::setManualMoovement(bool state) {
    programs->setEnabled(!state);
    keyboard->setEnabled(!state);
    control->setManualMoovement(state);
}

void Screen::mousePressEvent(QMouseEvent* event) {
    //    qDebug() << "pos lab x , width, y, height" << labSetPosY->x()
    //             << labSetPosY->width() << labSetPosY->y() <<
    //             labSetPosY->height();
    //    qDebug() << "mouse pos x , pos y" << event->pos().x() <<
    //    event->pos().y();
    if (event->pos().x() >= labSetPosY->x() &&
        event->pos().x() <= labSetPosY->x() + labSetPosY->width() &&
        event->pos().y() >= labSetPosY->y() &&
        event->pos().y() <= labSetPosY->y() + labSetPosY->height()) {
        // qDebug() << "On label pressed";
        if (currentMoveXorY == MoveXorY::Y || currentMoveXorY == MoveXorY::X) {
            currentMoveXorY = MoveXorY::NONE;
            labSetPosY->setStyleSheet(Style::TextFinal);
            labSetPosX->setStyleSheet(Style::TextFinal);
            setManualMoovement(false);
        } else {
            currentMoveXorY = MoveXorY::Y;
            labSetPosY->setStyleSheet(Style::TextMove);
            setManualMoovement(true);
        }
    } else if (event->pos().x() >= labSetPosX->x() &&
               event->pos().x() <= labSetPosX->x() + labSetPosX->width() &&
               event->pos().y() >= labSetPosX->y() &&
               event->pos().y() <= labSetPosX->y() + labSetPosX->height()) {
        // qDebug() << "On label pressed";
        if (currentMoveXorY == MoveXorY::X || currentMoveXorY == MoveXorY::Y) {
            currentMoveXorY = MoveXorY::NONE;
            labSetPosX->setStyleSheet(Style::TextFinal);
            labSetPosY->setStyleSheet(Style::TextFinal);
            setManualMoovement(false);
        } else {
            currentMoveXorY = MoveXorY::X;
            labSetPosX->setStyleSheet(Style::TextMove);
            setManualMoovement(true);
        }
    }
    //---------------- Settings window ----------------------------------------
    if (event->pos().x() >= labPosY->x() &&
        event->pos().x() <= labPosY->x() + labPosY->width() &&
        event->pos().y() >= labPosY->y() &&
        event->pos().y() <= labPosY->y() + labPosY->height()) {

        if (IsCalibrated) return;
        set->setModal(true);
        set->show();
        //set->showFullScreen();
    }
    //    if (event->pos().x() >= control->butDash->x() &&
    //        event->pos().x() <= control->butDash->x() +
    //        control->butDash->width() && event->pos().y() >=
    //        control->butDash->y() && event->pos().y() <= control->butDash->y()
    //        + control->butDash->height()) { qDebug() << "Calibation pressed";
    //    }
}

void Screen::mouseReleaseEvent([[maybe_unused]] QMouseEvent* event) {
    //    if (event->pos().x() >= control->butDash->x() &&
    //        event->pos().x() <= control->butDash->x() +
    //        control->butDash->width() && event->pos().y() >=
    //        control->butDash->y() && event->pos().y() <= control->butDash->y()
    //        + control->butDash->height()) { qDebug() << "Calibation released";
    //    }
}

// void Screen::touchEvent(QTouchEvent* ev) {
//     switch (ev->type()) {
//     case QEvent::TouchBegin:
//         qDebug() << "TouchBegin";
//         break;
//     case QEvent::TouchEnd:
//         qDebug() << "TouchEnd";
//         break;
//     case QEvent::TouchUpdate: {
//         qDebug() << "TouchUpdate";
//         break;
//     }
//     }
// }

// bool Screen::event(QEvent* ev) {
//     qDebug() << "event" << ev->type();
//     switch (ev->type()) {
//     case QEvent::TouchBegin:
//         qDebug() << "TouchBegin";
//         break;
//     case QEvent::TouchEnd:
//         qDebug() << "TouchEnd";
//         break;
//     default:
//         return ev;
//     }
// }

void Screen::addSymbol(char sym) {
    if (currentXorY == XorY::X) {
        if (!symCountX) {
            if (sym == '0') return;
            x = sym - 0x30;
            *strTempValX = valToStr.valXToString(x);
            symCountX++;
        } else if (symCountX < 4) {
            x *= 10;
            x += (sym - 0x30);
            *strTempValX = valToStr.valXToString(x);
            symCountX++;
        } else
            return;
        labSetPosX->setStyleSheet(Style::TextTemp);
        labSetPosX->setText(*strTempValX);
    } else if (currentXorY == XorY::Y) {
        if (!symCountY) {
            if (sym == '0') return;
            y = sym - 0x30;
            *strTempValY = valToStr.valYToString(y);
            symCountY++;
        } else if (symCountY < 5) {
            y *= 10;
            y += (sym - 0x30);
            *strTempValY = valToStr.valYToString(y);
            symCountY++;
        } else
            return;
        labSetPosY->setStyleSheet(Style::TextTemp);
        labSetPosY->setText(*strTempValY);
    }
}

void Screen::onBut0Clicked() { addSymbol('0'); }

void Screen::onBut1Clicked() { addSymbol('1'); }

void Screen::onBut2Clicked() { addSymbol('2'); }

void Screen::onBut3Clicked() { addSymbol('3'); }

void Screen::onBut4Clicked() { addSymbol('4'); }

void Screen::onBut5Clicked() { addSymbol('5'); }

void Screen::onBut6Clicked() { addSymbol('6'); }

void Screen::onBut7Clicked() { addSymbol('7'); }

void Screen::onBut8Clicked() { addSymbol('8'); }

void Screen::onBut9Clicked() { addSymbol('9'); }

void Screen::onButXClicked() {
    if (currentXorY == XorY::X) {
        currentXorY = XorY::NONE;
        keyboard->setX_pressed(false);
        symCountX = 0;
        labSetPosX->setText("000.0");
        return;
    }
    currentXorY = XorY::X;
    keyboard->setY_pressed(false);
    keyboard->setX_pressed(true);
    x = 0;
    labSetPosX->setStyleSheet(Style::TextTemp);
    labSetPosX->setText("000.0");
    symCountX = 0;
}

void Screen::onButYClicked() {
    if (currentXorY == XorY::Y) {
        currentXorY = XorY::NONE;
        keyboard->setY_pressed(false);
        symCountY = 0;
        labSetPosY->setText("000.00");
        return;
    }
    currentXorY = XorY::Y;
    keyboard->setX_pressed(false);
    keyboard->setY_pressed(true);
    y = 0;
    labSetPosY->setStyleSheet(Style::TextTemp);
    labSetPosY->setText("000.00");
    symCountY = 0;
}

void Screen::onButEnterClicked() {
    if (currentXorY == XorY::X) {
        *strValX = *strTempValX;
        labSetPosX->setStyleSheet(Style::TextFinal);
        labSetPosX->setText(*strValX);
        Fileops::YX yx{currentY, currentX};
        currentX = yx.X = x;
        fileops.setFileValues(currentPnum, currentPmode, yx);
        currentCommand.currentCommand = Protocol::Commands::SEND_NEW_VAL_X;
        currentCommand.val = currentX;
        interface->sendData(currentCommand);
    } else if (currentXorY == XorY::Y) {
        *strValY = *strTempValY;
        labSetPosY->setStyleSheet(Style::TextFinal);
        labSetPosY->setText(*strValY);
        Fileops::YX yx{currentY, currentX};
        currentY = yx.Y = y;
        fileops.setFileValues(currentPnum, currentPmode, yx);
        currentCommand.currentCommand = Protocol::Commands::SEND_NEW_VAL_Y;
        currentCommand.val = currentY;
        interface->sendData(currentCommand);
    }
    keyboard->setX_pressed(false);
    keyboard->setY_pressed(false);
    currentXorY = XorY::NONE;
}

void Screen::onButStartPressed() {
    control->setStart(true);
    currentCommand.currentCommand = Protocol::SEND_START_X;
    getYX();
    currentCommand.val = currentX;
    interface->sendData(currentCommand);
}
void Screen::onButStartReleased() {
    control->setStart(false);
    if(currentPauto == Fileops::Pauto::MANUAL) {
        currentCommand.currentCommand = Protocol::SEND_STOP;
    } else if(currentPauto == Fileops::Pauto::SEMI_AUTO) {

    } else if(currentPauto == Fileops::Pauto::AUTO) {

    }
    interface->sendData(currentCommand);
}

void Screen::getCurrentReply(const Protocol::Reply& reply) {
    switch (reply.currentReply) {
    case Protocol::Replies::START_X:
        // qDebug() << "Get: Replies::START_X" << reply.val;
        break;
    case Protocol::Replies::START_Y:
        // qDebug() << "Get: Replies::START_Y" << reply.val;
        break;
    case Protocol::Replies::VAL_X:
        // qDebug() << "Get: Replies::VAL_X" << reply.val;
        break;
    case Protocol::Replies::VAL_Y:
        // qDebug() << "Get: Replies::VAL_Y" << reply.val;
        break;
    case Protocol::Replies::CURRENT_Y:
        // qDebug() << "Get: Replies::CURRENT_Y" << reply.val;
        labGetPosY->setText(valToStr.valYToString(reply.val));
        if (currentXorY == XorY::Y) {
            currentY = reply.val;
            labSetPosY->setStyleSheet(Style::TextFinal);
            labSetPosY->setText(valToStr.valYToString(currentY));
        }
        break;
    case Protocol::Replies::CURRENT_X:
        // qDebug() << "Get: Replies::CURRENT_X" << reply.val;
        labGetPosX->setText(valToStr.valXToString(reply.val));
        if (currentXorY == XorY::X) {
            currentX = reply.val;
            labSetPosX->setStyleSheet(Style::TextFinal);
            labSetPosX->setText(valToStr.valXToString(currentX));
        }
        break;
    case Protocol::Replies::LIMIT_Y_PLUS:
        // qDebug() << "Get: Replies::LIMIT_Y_PLUS" << reply.val;
        labGetPosY->setText(valToStr.valYToString(reply.val));
        //
        break;
    case Protocol::Replies::LIMIT_Y_MINUS:
        // qDebug() << "Get: Replies::LIMIT_Y_MINUS" << reply.val;
        labGetPosY->setText(valToStr.valYToString(reply.val));
        break;
    case Protocol::Replies::LIMIT_X_PLUS:
        // qDebug() << "Get: Replies::LIMIT_X_PLUS" << reply.val;
        labGetPosX->setText(valToStr.valXToString(reply.val));
        break;
    case Protocol::Replies::LIMIT_X_MINUS:
        // qDebug() << "Get: Replies::LIMIT_X_MINUS" << reply.val;
        labGetPosX->setText(valToStr.valXToString(reply.val));
        break;
    case Protocol::Replies::STOP_X:
        // qDebug() << "Get: Replies::STOP_X" << reply.val;
        labGetPosX->setText(valToStr.valXToString(reply.val));
        if (currentMoveXorY == MoveXorY::X) {
            currentX = reply.val;
            // labSetPosX->setStyleSheet(Style::TextFinal);
            labSetPosX->setText(valToStr.valXToString(currentX));
            timerWaitForSaveInFile->stop();
            timerWaitForSaveInFile->start(TIME_TO_SAVE_IN_FILE);
            WasXStopped = true;
        } else if (currentMoveXorY == MoveXorY::NONE) {
            currentCommand.currentCommand = Protocol::SEND_START_Y;
            currentCommand.val = currentY;
            interface->sendData(currentCommand);
        }
        break;
    case Protocol::Replies::STOP_Y:
        WasYStopped = true;
        // qDebug() << "Get: Replies::STOP_Y" << reply.val;
        labGetPosY->setText(valToStr.valYToString(reply.val));
        if (currentMoveXorY == MoveXorY::Y) {
            currentY = reply.val;
            // labSetPosY->setStyleSheet(Style::TextFinal);
            labSetPosY->setText(valToStr.valYToString(currentY));
            timerWaitForSaveInFile->stop();
            timerWaitForSaveInFile->start(TIME_TO_SAVE_IN_FILE);
        }
        control->setStart(false);
        break;
    case Protocol::Replies::CALIBRATION_START:
        // qDebug() << "CALIBRATION_START" << reply.val;
        labInfoY->setText("Calibration X started");
        break;
    case Protocol::Replies::CALIBRATION_X_STOP:
        // qDebug() << "Get: Replies::CALIBRATION_X_STOP" << reply.val;
        labInfoY->setText("Calibration Y started");
        break;
    case Protocol::Replies::CALIBRATION_Y_STOP:
        // qDebug() << "Get: Replies::CALIBRATION_Y_STOP" << reply.val;
        labInfoY->setText("Calibration OK");
        currentX = 0;
        currentY = 0;
        labGetPosX->setText(valToStr.valXToString(currentX));
        labGetPosY->setText(valToStr.valYToString(currentY));
        IsCalibrated = true;
        setCalibrationState(false);
        setManualMoovement(false);
        break;
    case Protocol::Replies::NEED_CALIBRATION: {
        // qDebug() << "Get: Replies:: NEED_CALIBRATION" << reply.val;
        labInfoY->setText("Calibration need started");
        IsCalibrated = false;
        setCalibrationState(true);
        // TODO: send settings from file
        Fileops::Settings settings = fileops.getSettings();
        currentCommandSet.currentCommand =
            Protocol::Commands::SEND_SET_SETTINGS;
        currentCommandSet.coefY = settings.coefY;
        currentCommandSet.coefX = settings.coefX;
        currentCommandSet.deviationY = settings.deviationY;
        currentCommandSet.deviationX = settings.deviationX;
        set->firstSendSettings(currentCommandSet);
    } break;
    default:
        break;
    }
}

void Screen::moveCycle() {}
