#include "programs.h"
#include "style.h"

Programs::Programs(QWidget* parent) : QWidget(parent) { init(); }

Programs::~Programs()
{
    delete layV1;
    delete layV2;
    delete layH;
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
}
