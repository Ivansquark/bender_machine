#include "control.h"
#include "style.h"

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
    butDash->setFixedSize(90, 90);
    butDash->setStyleSheet(Style::ButReleased);

    //-------------- connects -------------------------------------------------
    connect(butStart, &QPushButton::clicked, [this] { emit onButStart(); });
    connect(radioP1, &QRadioButton::clicked,
            [this] { emit sendCurrentModePressed(Fileops::Pmode1); });
    connect(radioP2, &QRadioButton::clicked,
            [this] { emit sendCurrentModePressed(Fileops::Pmode2); });
}

void Control::setPmode(Fileops::Pmode mode) {
    if (mode == Fileops::Pmode1) {
        radioP1->setChecked(true);
    } else {
        radioP2->setChecked(true);
    }
}
