#include "screen.h"
#include "style.h"

Screen::Screen(QWidget* parent) : QDialog(parent) { init(); }

Screen::~Screen() {
    delete layVleftY;
    delete layVleftX;
    delete layVleftBut;
    delete layVright;
    delete layH;
}

void Screen::init() {
    setFixedSize(1024, 600);
    // connect(butExit, &QPushButton::clicked, [this] { close(); });
    layVleftY->addWidget(labSetPosY);
    layVleftY->addWidget(labPosY);
    layVleftY->addWidget(labGetPosY);
    layVleftX->addWidget(labSetPosX);
    layVleftX->addWidget(labPosX);
    layVleftX->addWidget(labGetPosX);
    layVright->addWidget(programs);
    layVright->addStretch(0);
    layVright->addWidget(keyboard);
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
    connect(control, &Control::onButStart, this, &Screen::onButStart);
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
    connect(control, &Control::onButPlus, [this] {
        if (currentXorY == XorY::Y) {
            currentCommand.currentCommand = Protocol::Commands::SEND_Y_PLUS;
            interface->sendData(currentCommand);
        } else if (currentXorY == XorY::X) {
            currentCommand.currentCommand = Protocol::Commands::SEND_X_PLUS;
            interface->sendData(currentCommand);
        }
    });
    connect(control, &Control::onButMinus, [this] {
        if (currentXorY == XorY::Y) {
            currentCommand.currentCommand = Protocol::Commands::SEND_Y_MINUS;
            interface->sendData(currentCommand);
        } else if (currentXorY == XorY::X) {
            currentCommand.currentCommand = Protocol::Commands::SEND_X_MINUS;
            interface->sendData(currentCommand);
        }
    });
    connect(control, &Control::onButDash, [this] {
        if(!IsCalibrated) {
            currentCommand.currentCommand = Protocol::Commands::SEND_CALIBRATION;
            interface->sendData(currentCommand);
        }
    });
    //-------------- interface ------------------------------------------------
    connect(interface.get(), &Interface::sendCurrentReply, this,
            &Screen::getCurrentReply);
    //-------------- timers ---------------------------------------------------
    connect(&moveTimer, &QTimer::timeout, this, &Screen::moveCycle);
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

void Screen::onButStart() {
    if (isStarted) {
        control->setStart(true);
        isStarted = false;
        moveTimer.stop();

    } else {
        control->setStart(false);
        isStarted = true;
        moveTimer.start(1);
        currentCommand.currentCommand = Protocol::SEND_START_X;
        currentCommand.val = currentX;
        interface->sendData(currentCommand);
    }
}

void Screen::getCurrentReply(const Protocol::Reply& reply) {
    switch (reply.currentReply) {
    case Protocol::Replies::START_X:
        qDebug() << "Get: Replies::START_X" << reply.val;
        break;
    case Protocol::Replies::START_Y:
        qDebug() << "Get: Replies::START_Y" << reply.val;
        break;
    case Protocol::Replies::VAL_X:
        qDebug() << "Get: Replies::VAL_X" << reply.val;
        break;
    case Protocol::Replies::VAL_Y:
        qDebug() << "Get: Replies::VAL_Y" << reply.val;
        break;
    case Protocol::Replies::CURRENT_Y:
        qDebug() << "Get: Replies::CURRENT_Y" << reply.val;
        labGetPosY->setText(valToStr.valYToString(reply.val));
        if (currentXorY == XorY::Y) {
            currentY = reply.val;
            labSetPosY->setStyleSheet(Style::TextFinal);
            labSetPosY->setText(valToStr.valYToString(currentY));
        }
        break;
    case Protocol::Replies::CURRENT_X:
        qDebug() << "Get: Replies::CURRENT_X" << reply.val;
        labGetPosX->setText(valToStr.valXToString(reply.val));
        if (currentXorY == XorY::X) {
            currentX = reply.val;
            labSetPosX->setStyleSheet(Style::TextFinal);
            labSetPosX->setText(valToStr.valXToString(currentX));
        }
        break;
    case Protocol::Replies::LIMIT_Y_PLUS:
        qDebug() << "Get: Replies::LIMIT_Y_PLUS" << reply.val;
        labGetPosY->setText(valToStr.valYToString(reply.val));
        //
        break;
    case Protocol::Replies::LIMIT_Y_MINUS:
        qDebug() << "Get: Replies::LIMIT_Y_MINUS" << reply.val;
        labGetPosY->setText(valToStr.valYToString(reply.val));
        break;
    case Protocol::Replies::LIMIT_X_PLUS:
        qDebug() << "Get: Replies::LIMIT_X_PLUS" << reply.val;
        labGetPosX->setText(valToStr.valXToString(reply.val));
        break;
    case Protocol::Replies::LIMIT_X_MINUS:
        qDebug() << "Get: Replies::LIMIT_X_MINUS" << reply.val;
        labGetPosX->setText(valToStr.valXToString(reply.val));
        break;
    case Protocol::Replies::STOP_X:
        qDebug() << "Get: Replies::STOP_X" << reply.val;
        labGetPosX->setText(valToStr.valXToString(reply.val));
        currentCommand.currentCommand = Protocol::SEND_START_Y;
        currentCommand.val = currentY;
        interface->sendData(currentCommand);
        break;
    case Protocol::Replies::STOP_Y:
        qDebug() << "Get: Replies::STOP_Y" << reply.val;
        labGetPosY->setText(valToStr.valYToString(reply.val));
        moveTimer.stop();
        break;
    default:
        break;
    }
}

void Screen::moveCycle() {}
