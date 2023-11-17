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

QString Screen::valXToString(uint32_t val) {
    QString str = "";
    int count = 0;
    uint8_t arr[4] = {0};
    while (val) {
        arr[count] = val % 10;
        val /= 10;
        count++;
    }
    if (count == 1) {
        str += "000.";
        str += QString::number(arr[0]);
    } else if (count == 2) {
        str += "00";
        str += QString::number(arr[1]);
        str += ".";
        str += QString::number(arr[0]);
    } else if (count == 3) {
        str += "0";
        str += QString::number(arr[2]);
        str += QString::number(arr[1]);
        str += ".";
        str += QString::number(arr[0]);
    } else if (count == 4) {
        str += QString::number(arr[3]);
        str += QString::number(arr[2]);
        str += QString::number(arr[1]);
        str += ".";
        str += QString::number(arr[0]);
    } else {
        str = "000.0";
    }
    return str;
}
QString Screen::valYToString(uint32_t val) {
    QString str = "";
    int count = 0;
    uint8_t arr[5] = {0};
    while (val) {
        arr[count] = val % 10;
        val /= 10;
        count++;
    }
    if (count == 1) {
        str += "000.0";
        str += QString::number(arr[0]);
    } else if (count == 2) {
        str += "000.";
        str += QString::number(arr[1]);
        str += QString::number(arr[0]);
    } else if (count == 3) {
        str += "00";
        str += QString::number(arr[2]);
        str += ".";
        str += QString::number(arr[1]);
        str += QString::number(arr[0]);
    } else if (count == 4) {
        str += "0";
        str += QString::number(arr[3]);
        str += QString::number(arr[2]);
        str += ".";
        str += QString::number(arr[1]);
        str += QString::number(arr[0]);
    } else if (count == 5) {
        str += QString::number(arr[4]);
        str += QString::number(arr[3]);
        str += QString::number(arr[2]);
        str += ".";
        str += QString::number(arr[1]);
        str += QString::number(arr[0]);
    } else {
        str = "000.00";
    }
    return str;
}

void Screen::addSymbol(char sym) {
    if (currentXorY == XorY::X) {
        if (!symCountX) {
            if (sym == '0') return;
            x = sym - 0x30;
            *strTempValX = valXToString(x);
            symCountX++;
        } else if (symCountX < 4) {
            x *= 10;
            x += (sym - 0x30);
            *strTempValX = valXToString(x);
            symCountX++;
        } else
            return;
        labSetPosX->setStyleSheet(Style::TextTemp);
        labSetPosX->setText(*strTempValX);
    } else if (currentXorY == XorY::Y) {
        if (!symCountY) {
            if (sym == '0') return;
            y = sym - 0x30;
            *strTempValY = valYToString(y);
            symCountY++;
        } else if (symCountY < 5) {
            y *= 10;
            y += (sym - 0x30);
            *strTempValY = valYToString(y);
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
    } else if (currentXorY == XorY::Y) {
        *strValY = *strTempValY;
        labSetPosY->setStyleSheet(Style::TextFinal);
        labSetPosY->setText(*strValY);
    }
    keyboard->setX_pressed(false);
    keyboard->setY_pressed(false);
    currentXorY = XorY::NONE;
}

void Screen::onButStart() {
    if(isStarted) {
        control->setStart(true);
        isStarted = false;
    } else {
        control->setStart(false);
        isStarted = true;
    }
}

void Screen::init() {
    setFixedSize(1024, 600);
    //connect(butExit, &QPushButton::clicked, [this] { close(); });
    layVleftY->addWidget(labSetPosY);
    layVleftY->addWidget(labPosY);
    layVleftY->addWidget(labGetPosY);
    layVleftX->addWidget(labSetPosX);
    layVleftX->addWidget(labPosX);
    layVleftX->addWidget(labGetPosX);
    layVright->addWidget(programs);
    layVright->addStretch(0);
    layVright->addWidget(keyboard);
    //layVright->addWidget(butExit);
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
    //
    //-------------- control --------------------------------------------------
    connect(control, &Control::onButStart, this, &Screen::onButStart);
}
