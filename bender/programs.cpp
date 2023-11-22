#include "programs.h"
#include "style.h"

Programs::Programs(QWidget* parent) : QWidget(parent) { init(); }

Programs::~Programs() {
    delete layV1;
    delete layV2;
    delete layH;
}

void Programs::setCurrentButton(Fileops::Pnum num) {
    switch (num) {
    case Fileops::P1:
        but1->setChecked(true);
        break;
    case Fileops::P2:
        but2->setChecked(true);
        break;
    case Fileops::P3:
        but3->setChecked(true);
        break;
    case Fileops::P4:
        but4->setChecked(true);
        break;
    case Fileops::P5:
        but5->setChecked(true);
        break;
    case Fileops::P6:
        but6->setChecked(true);
        break;
    case Fileops::P7:
        but7->setChecked(true);
        break;
    case Fileops::P8:
        but8->setChecked(true);
        break;
    case Fileops::P9:
        but9->setChecked(true);
        break;
    case Fileops::P10:
        but10->setChecked(true);
        break;
    case Fileops::P11:
        but11->setChecked(true);
        break;
    case Fileops::P12:
        but12->setChecked(true);
        break;
    case Fileops::P13:
        but13->setChecked(true);
        break;
    case Fileops::P14:
        but14->setChecked(true);
        break;
    case Fileops::P15:
        but15->setChecked(true);
        break;
    case Fileops::P16:
        but16->setChecked(true);
        break;
    default:
        break;
    }
}

void Programs::init() {
    layV1->addWidget(but8);
    layV1->addWidget(but7);
    layV1->addWidget(but6);
    layV1->addWidget(but5);
    layV1->addWidget(but4);
    layV1->addWidget(but3);
    layV1->addWidget(but2);
    layV1->addWidget(but1);

    layV2->addWidget(but9);
    layV2->addWidget(but10);
    layV2->addWidget(but11);
    layV2->addWidget(but12);
    layV2->addWidget(but13);
    layV2->addWidget(but14);
    layV2->addWidget(but15);
    layV2->addWidget(but16);
    layH->addLayout(layV1);
    layH->addLayout(layV2);
    setLayout(layH);
    QList<QRadioButton*> listButtons = this->findChildren<QRadioButton*>();
    for (auto it = listButtons.begin(); it != listButtons.end(); ++it) {
        (*it)->setFixedWidth(100);
        (*it)->setFixedHeight(40);
        (*it)->setStyleSheet(Style::radioP);
    }
    // ------- connects on pressed buttons ------------------------------------
    connect(but1, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P1); });
    connect(but2, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P2); });
    connect(but3, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P3); });
    connect(but4, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P4); });
    connect(but5, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P5); });
    connect(but6, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P6); });
    connect(but7, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P7); });
    connect(but8, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P8); });
    connect(but9, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P9); });
    connect(but10, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P10); });
    connect(but11, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P11); });
    connect(but12, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P12); });
    connect(but13, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P13); });
    connect(but14, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P14); });
    connect(but15, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P15); });
    connect(but16, &QRadioButton::clicked,
            [this] { emit sendCurrentButtonPressed(Fileops::P16); });
}
