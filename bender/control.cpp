#include "control.h"
#include "style.h"
#include <iostream>

Control::Control(QWidget* parent) : QWidget{parent} { init(); }

Control::~Control() {
    delete layH1;
    delete layH2;
    delete layV;
    delete iconManual;
    delete iconSemiAuto;
    delete iconAuto;
}

void Control::setStart(bool state) {
    if (!state) {
        butStart->setStyleSheet(Style::ButStartReleased);
    } else {
        butStart->setStyleSheet(Style::ButStartPressed);
    }
}

void Control::setEnabledCalibration(bool state) {
    radioP1->setEnabled(!state);
    radioP2->setEnabled(!state);

    butManual->setEnabled(!state);
    butAuto->setEnabled(!state);
    butSemiAuto->setEnabled(!state);
    if (!state) {
        switch (currentAutoState) {
        case Fileops::MANUAL:
            butManual->setStyleSheet(Style::ButPressed);
            butSemiAuto->setStyleSheet(Style::ButReleased);
            butAuto->setStyleSheet(Style::ButReleased);
            break;
        case Fileops::SEMI_AUTO:
            butSemiAuto->setStyleSheet(Style::ButPressed);
            butManual->setStyleSheet(Style::ButReleased);
            butAuto->setStyleSheet(Style::ButReleased);
            break;
        case Fileops::AUTO:
            butAuto->setStyleSheet(Style::ButPressed);
            butSemiAuto->setStyleSheet(Style::ButReleased);
            butManual->setStyleSheet(Style::ButReleased);
            break;
        default:
            break;
        }
    }

    butStart->setEnabled(!state);
    butPlus->setEnabled(!state);
    butMinus->setEnabled(!state);
    butDash->setEnabled(state);
}

void Control::setManualMoovement(bool state) {
    radioP1->setEnabled(!state);
    radioP2->setEnabled(!state);
    butManual->setEnabled(!state);
    butAuto->setEnabled(!state);
    butSemiAuto->setEnabled(!state);
    butStart->setEnabled(!state);
    butPlus->setEnabled(state);
    butMinus->setEnabled(state);
}

void Control::init() {
    layH1->addWidget(radioP1);
    layH1->addWidget(radioP2);
    layV->addLayout(layH1);
    layV->addStretch(0);
    layH2->addWidget(butManual);
    layH2->addWidget(butSemiAuto);
    layH2->addWidget(butAuto);
    layV->addLayout(layH2);
    layV->addStretch(0);
    //    layV->addWidget(butStart);
    //    layV->addWidget(butPlus);
    //    layV->addWidget(butMinus);
    //    layV->addWidget(butDash);
    layHstart->addWidget(butStart);
    layHplus->addWidget(butPlus);
    layHminus->addWidget(butMinus);
    layHdash->addWidget(butDash);

    layV->addLayout(layHstart);
    layV->addLayout(layHplus);
    layV->addLayout(layHminus);
    layV->addLayout(layHdash);
    setLayout(layV);

    // radioP1->setFixedSize(50,50);
    radioP1->setChecked(true);
    radioP1->setStyleSheet(Style::radioP);
    // radioP2->setFixedSize(50,50);
    radioP2->setStyleSheet(Style::radioP);

    butManual->setFixedSize(80, 80);
    butManual->setIcon(*iconManual);
    butManual->setIconSize(QSize(40, 40));
    butManual->setStyleSheet(Style::ButReleased);
    butSemiAuto->setFixedSize(80, 80);
    butSemiAuto->setIcon(*iconSemiAuto);
    butSemiAuto->setIconSize(QSize(40, 40));
    butSemiAuto->setStyleSheet(Style::ButReleased);
    butAuto->setFixedSize(80, 80);
    butAuto->setIcon(*iconAuto);
    butAuto->setIconSize(QSize(45, 45));
    butAuto->setStyleSheet(Style::ButReleased);

    butStart->setFixedSize(100, 100);
    butStart->setStyleSheet(Style::ButStartReleased);
    butPlus->setFixedSize(95, 95);
    butPlus->setStyleSheet(Style::ButReleased);
    butMinus->setFixedSize(95, 95);
    butMinus->setStyleSheet(Style::ButReleased);
    butDash->setFixedSize(100, 100);
    butDash->setStyleSheet(Style::ButReleased);

    //-------------- connects -------------------------------------------------
    // connect(butStart, &QPushButton::clicked, [this] { emit onButStart(); });

    connect(radioP1, &QRadioButton::clicked,
            [this] { emit sendCurrentModePressed(Fileops::Pmode1); });
    connect(radioP2, &QRadioButton::clicked,
            [this] { emit sendCurrentModePressed(Fileops::Pmode2); });
    connect(butManual, &QPushButton::clicked, [this] {
        emit onButManual();
        setPauto(Fileops::MANUAL);
    });
    connect(butSemiAuto, &QPushButton::clicked, [this] {
        emit onButSemiAuto();
        setPauto(Fileops::SEMI_AUTO);
    });
    connect(butAuto, &QPushButton::clicked, [this] {
        emit onButAuto();
        setPauto(Fileops::AUTO);
    });
    //    connect(butStart, &QPushButton::pressed, [this] { emit
    //    onButStartPressed(); }); connect(butStart, &QPushButton::released,
    //    [this] { emit onButStartReleased(); }); connect(butPlus,
    //    &QPushButton::pressed,
    //            [this] { emit onButPlusPressed(); });
    //    connect(butPlus, &QPushButton::released,
    //            [this] { emit onButPlusReleased(); });
    //    connect(butMinus, &QPushButton::pressed,
    //            [this] { emit onButMinusPressed(); });
    //    connect(butMinus, &QPushButton::released,
    //            [this] { emit onButMinusReleased(); });
    //    connect(butDash, &QPushButton::pressed,
    //            [this] { emit onButDashPressed(); });
    //    connect(butDash, &QPushButton::released,
    //            [this] { emit onButDashReleased(); });

    //---------------- Touch handling -----------------------------------------
    connect(touch.get(), &Touch::sendScreenCoord, this,
            &Control::getTouchCoord);
    connect(timPressed.get(), &QTimer::timeout, this, &Control::timeoutPressed);
    timPressed->start(50);
    // CheckPressed = false;
}

void Control::setPmode(Fileops::Pmode mode) {
    if (mode == Fileops::Pmode1) {
        radioP1->setChecked(true);
    } else {
        radioP2->setChecked(true);
    }
}

void Control::setPauto(Fileops::Pauto autoState) {
    switch (autoState) {
    case Fileops::Pauto::MANUAL:
        butManual->setStyleSheet(Style::ButPressed);
        butSemiAuto->setStyleSheet(Style::ButReleased);
        butAuto->setStyleSheet(Style::ButReleased);
        break;
    case Fileops::Pauto::SEMI_AUTO:
        butSemiAuto->setStyleSheet(Style::ButPressed);
        butManual->setStyleSheet(Style::ButReleased);
        butAuto->setStyleSheet(Style::ButReleased);
        break;
    case Fileops::Pauto::AUTO:
        butAuto->setStyleSheet(Style::ButPressed);
        butSemiAuto->setStyleSheet(Style::ButReleased);
        butManual->setStyleSheet(Style::ButReleased);
        break;
    default:
        break;
    }
}

void Control::setButDashColor(bool state)
{
    if(!state) {
        butDash->setStyleSheet(Style::ButPressed);
    } else {
        butDash->setStyleSheet(Style::ButReleased);
    }
}

void Control::getTouchCoord(bool WasPressed, uint16_t x, uint16_t y) {
    X = x;
    Y = y;
    if (!WasPressed) {
        IsPressed = false;
        counterAdc = 0;
        // qDebug() << "released by signal" << x << y;
    } else {
        // TODO: counter
        if (counterAdc >= COUNTER_ADC_MAX) {
            IsPressed = true;
            counterAdc = 0;
            // qDebug() << "Pressed" << x << y;
        } else {
            counterAdc++;
        }
    }
    CheckPressed = false;
    //------------------- buttons Start handler -------------------------------
    // if(this->isEnabled()) {
    if (IsPressed) {
        butStartParentPos = QWidget::mapToParent(butStart->pos());
        butPlusParentPos = QWidget::mapToParent(butPlus->pos());
        butMinusParentPos = QWidget::mapToParent(butMinus->pos());
        butDashParentPos = QWidget::mapToParent(butDash->pos());
        if ((x >= butStartParentPos.x()) &&
            (x <= butStartParentPos.x() + butStart->width()) &&
            (y >= butStartParentPos.y()) &&
            (y <= butStartParentPos.y() + butStart->height())) {
            if (!IsButStartPressed) {
                IsButStartPressed = true;
                emit onButStartPressed();
            }
        } else {
            if (IsButStartPressed) {
                IsButStartPressed = false;
                emit onButStartReleased();
            }
        }
        if ((x >= butPlusParentPos.x()) &&
            (x <= butPlusParentPos.x() + butPlus->width()) &&
            (y >= butPlusParentPos.y()) &&
            (y <= butPlusParentPos.y() + butPlus->height())) {
            if (!IsButPlusPressed) {
                // std::cout << (x >= butPlusParentPos.x());
                // qDebug() << x << "butPlusPressed" << butPlusParentPos.x() <<
                // butPlus->width();
                IsButPlusPressed = true;
                emit onButPlusPressed();
            }
        } else {
            if (IsButPlusPressed) {
                IsButPlusPressed = false;
                emit onButPlusReleased();
            }
        }
        if ((x >= butMinusParentPos.x()) &&
            (x <= butMinusParentPos.x() + butMinus->width()) &&
            (y >= butMinusParentPos.y()) &&
            (y <= butMinusParentPos.y() + butMinus->height())) {
            if (!IsButMinusPressed) {
                // qDebug() << "butMinusPressed";
                IsButMinusPressed = true;
                emit onButMinusPressed();
            }
        } else {
            if (IsButMinusPressed) {
                // qDebug() << "butMinusPressed";
                IsButMinusPressed = false;
                emit onButMinusReleased();
            }
        }
        if ((x >= butDashParentPos.x()) &&
            (x <= butDashParentPos.x() + butDash->width()) &&
            (y >= butDashParentPos.y()) &&
            (y <= butDashParentPos.y() + butDash->height())) {
            if (!IsButDashPressed) {
                IsButDashPressed = true;
                emit onButDashPressed();
            }
        } else {
            if (IsButDashPressed) {
                IsButDashPressed = false;
                emit onButDashReleased();
            }
        }
    } else {
        butStartParentPos = QWidget::mapToParent(butStart->pos());
        butPlusParentPos = QWidget::mapToParent(butPlus->pos());
        butMinusParentPos = QWidget::mapToParent(butMinus->pos());
        butDashParentPos = QWidget::mapToParent(butDash->pos());
        IsButStartPressed = false;
        IsButPlusPressed = false;
        IsButMinusPressed = false;
        IsButDashPressed = false;
        if (x >= butStartParentPos.x() &&
            x <= butStartParentPos.x() + butStart->width() &&
            y >= butStartParentPos.y() &&
            y <= butStartParentPos.y() + butStart->height()) {
            emit onButStartReleased();
        }
        if (x >= butPlusParentPos.x() &&
            x <= butPlusParentPos.x() + butPlus->width() &&
            y >= butPlusParentPos.y() &&
            y <= butPlusParentPos.y() + butPlus->height()) {
            // qDebug() << "butPlusReleased";
            emit onButPlusReleased();
        }
        if (x >= butMinusParentPos.x() &&
            x <= butMinusParentPos.x() + butMinus->width() &&
            y >= butMinusParentPos.y() &&
            y <= butMinusParentPos.y() + butMinus->height()) {
            // qDebug() << "butMinusReleased";
            emit onButMinusReleased();
        }
        if (x >= butDashParentPos.x() &&
            x <= butDashParentPos.x() + butDash->width() &&
            y >= butDashParentPos.y() &&
            y <= butDashParentPos.y() + butDash->height()) {
            emit onButDashReleased();
        }
    }
    //}
}

void Control::timeoutPressed() {
    if (IsPressed) {
        if (CheckPressed) {
            // qDebug() << "released by timeout";
            IsPressed = false;
            if (X >= butStartParentPos.x() &&
                X <= butStartParentPos.x() + butStart->width() &&
                Y >= butStartParentPos.y() &&
                Y <= butStartParentPos.y() + butStart->height()) {
                emit onButStartReleased();
            }
            if (X >= butPlusParentPos.x() &&
                X <= butPlusParentPos.x() + butPlus->width() &&
                Y >= butPlusParentPos.y() &&
                Y <= butPlusParentPos.y() + butPlus->height()) {
                emit onButPlusReleased();
            }
            if (X >= butMinusParentPos.x() &&
                X <= butMinusParentPos.x() + butMinus->width() &&
                Y >= butMinusParentPos.y() &&
                Y <= butMinusParentPos.y() + butMinus->height()) {
                emit onButMinusReleased();
            }
            if (X >= butDashParentPos.x() &&
                X <= butDashParentPos.x() + butDash->width() &&
                Y >= butDashParentPos.y() &&
                Y <= butDashParentPos.y() + butDash->height()) {
                emit onButDashReleased();
            }
            IsButStartPressed = false;
            IsButPlusPressed = false;
            IsButMinusPressed = false;
            IsButDashPressed = false;
        }
        CheckPressed = true;
    }
}
