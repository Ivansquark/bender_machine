#include "keyboard.h"
#include "style.h"

Keyboard::Keyboard(QWidget* parent) : QWidget(parent) { init(); }
Keyboard::~Keyboard() {
    delete layH1_1;
    delete layH1_2;
    delete layV1;
    delete layV2_1;
    delete layH2_1;
    delete layV2_2;
    delete layH1;
    delete layH2;
    delete layV;
}

void Keyboard::setX_pressed(bool state) {
    if (state) {
        butX->setStyleSheet(Style::ButPressed);
    } else {
        butX->setStyleSheet(Style::ButReleased);
    }
}

void Keyboard::setY_pressed(bool state) {
    if (state) {
        butY->setStyleSheet(Style::ButPressed);
    } else {
        butY->setStyleSheet(Style::ButReleased);
    }
}

void Keyboard::init() {
    butEnter->setSizePolicy(QSizePolicy::Policy::Maximum,
                            QSizePolicy::Policy::Expanding);

    layH1->addWidget(butEnter);
    layH1->addLayout(layV1);
    butEnter->setFixedWidth(80);
    butX->setFixedWidth(80);
    butY->setFixedWidth(80);
    //butX->setFixedHeight(80);
    butY->setMinimumHeight(50);
    QList<QPushButton*> listButtons = this->findChildren<QPushButton*>();
    for (auto it = listButtons.begin(); it != listButtons.end(); ++it) {
        (*it)->setMinimumHeight(43);
    }

    layH1_1->addWidget(but7);
    layH1_1->addWidget(but8);
    layH1_1->addWidget(but9);
    layH1_2->addWidget(but4);
    layH1_2->addWidget(but5);
    layH1_2->addWidget(but6);
    layV1->addLayout(layH1_1);
    layV1->addLayout(layH1_2);
    layH2->addLayout(layV2_1);
    layV2_1->addWidget(butX);
    layV2_1->addWidget(butY);
    layH2->addLayout(layV2_2);
    layV2_2->addLayout(layH2_1);
    layH2_1->addWidget(but1);
    layH2_1->addWidget(but2);
    layH2_1->addWidget(but3);
    layV2_2->addWidget(but0);
    layV->addLayout(layH1);
    layV->addLayout(layH2);
    setLayout(layV);

    connect(butEnter, &QPushButton::clicked,
            [this] { emit onButEnterClicked(); });
    connect(but0, &QPushButton::clicked, [this] { emit onBut0Clicked(); });
    connect(but1, &QPushButton::clicked, [this] { emit onBut1Clicked(); });
    connect(but2, &QPushButton::clicked, [this] { emit onBut2Clicked(); });
    connect(but3, &QPushButton::clicked, [this] { emit onBut3Clicked(); });
    connect(but4, &QPushButton::clicked, [this] { emit onBut4Clicked(); });
    connect(but5, &QPushButton::clicked, [this] { emit onBut5Clicked(); });
    connect(but6, &QPushButton::clicked, [this] { emit onBut6Clicked(); });
    connect(but7, &QPushButton::clicked, [this] { emit onBut7Clicked(); });
    connect(but8, &QPushButton::clicked, [this] { emit onBut8Clicked(); });
    connect(but9, &QPushButton::clicked, [this] { emit onBut9Clicked(); });
    connect(butX, &QPushButton::clicked, [this] { emit onButXClicked(); });
    connect(butY, &QPushButton::clicked, [this] { emit onButYClicked(); });

    butX->setStyleSheet(Style::ButReleased);
    butY->setStyleSheet(Style::ButReleased);
    but0->setStyleSheet(Style::ButReleased);
    but1->setStyleSheet(Style::ButReleased);
    but2->setStyleSheet(Style::ButReleased);
    but3->setStyleSheet(Style::ButReleased);
    but4->setStyleSheet(Style::ButReleased);
    but5->setStyleSheet(Style::ButReleased);
    but6->setStyleSheet(Style::ButReleased);
    but7->setStyleSheet(Style::ButReleased);
    but8->setStyleSheet(Style::ButReleased);
    but9->setStyleSheet(Style::ButReleased);
    butEnter->setStyleSheet(Style::ButReleased);
}
