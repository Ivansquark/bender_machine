#include "settings.h"
#include "style.h"

Settings::Settings(QWidget* parent) : QDialog{parent} { init(); }

void Settings::setData(const Protocol::ReplySet& val) {
    currentReplyGet = val;
    spinCoefY->setValue(val.coefY);
    spinCoefX->setValue(val.coefX);
    spinDeviationY->setValue(val.deviationY);
    spinDeviationX->setValue(val.deviationX);
}

void Settings::firstSendSettings(const Protocol::CommandSet& val) {
    currentCommandSet.currentCommand = Protocol::Commands::SEND_SET_SETTINGS;
    currentCommandSet.coefY = val.coefY;
    currentCommandSet.coefX = val.coefX;
    currentCommandSet.deviationY = val.deviationY;
    currentCommandSet.deviationX = val.deviationX;
    emit sendSetData(currentCommandSet);
}

void Settings::init() {
    setFixedSize(1024, 600);

    //--------------- Layouts -------------------------------------------------
    layPass13->addWidget(but1);
    layPass13->addWidget(but2);
    layPass13->addWidget(but3);
    layPass->addLayout(layPass13.get());
    layPass46->addWidget(but4);
    layPass46->addWidget(but5);
    layPass46->addWidget(but6);
    layPass->addLayout(layPass46.get());
    layPass79->addWidget(but7);
    layPass79->addWidget(but8);
    layPass79->addWidget(but9);
    layPass->addLayout(layPass79.get());
    layMain->addLayout(layPass.get());

    laySetLabCoef->addWidget(labCoefY);
    laySetLabCoef->addWidget(labCoefX);
    laySet->addLayout(laySetLabCoef.get());
    laySetSpinCoef->addWidget(spinCoefY);
    laySetSpinCoef->addWidget(spinCoefX);
    laySet->addLayout(laySetSpinCoef.get());
    laySetLabDev->addWidget(labDeviationY);
    laySetLabDev->addWidget(labDeviationX);
    laySet->addLayout(laySetLabDev.get());
    laySetSpinDev->addWidget(spinDeviationY);
    laySetSpinDev->addWidget(spinDeviationX);
    laySet->addLayout(laySetSpinDev.get());
    laySetGetSend->addWidget(butGet);
    laySetGetSend->addWidget(butSend);
    laySet->addLayout(laySetGetSend.get());
    layMain->addLayout(laySet.get());
    layMain->addWidget(butExit);
    setLayout(layMain.get());
    //-------------------- buttons --------------------------------------------

    QList<QPushButton*> listButs = this->findChildren<QPushButton*>();
    //    for (auto it = listButs.begin(); it != listButs.end(); ++it) {
    for (auto&& buts : listButs) {
        buts->setFixedSize(100, 100);
        buts->setStyleSheet(Style::ButReleased);
    }
    connect(but1, &QPushButton::clicked, [this] { codeCheck(1); });
    connect(but2, &QPushButton::clicked, [this] { codeCheck(2); });
    connect(but3, &QPushButton::clicked, [this] { codeCheck(3); });
    connect(but4, &QPushButton::clicked, [this] { codeCheck(4); });
    connect(but5, &QPushButton::clicked, [this] { codeCheck(5); });
    connect(but6, &QPushButton::clicked, [this] { codeCheck(6); });
    connect(but7, &QPushButton::clicked, [this] { codeCheck(7); });
    connect(but8, &QPushButton::clicked, [this] { codeCheck(8); });
    connect(but9, &QPushButton::clicked, [this] { codeCheck(9); });

    connect(butSend, &QPushButton::clicked, [this] {
        currentCommandSet.currentCommand =
            Protocol::Commands::SEND_SET_SETTINGS;
        currentCommandSet.coefY = spinCoefY->value();
        currentCommandSet.coefX = spinCoefX->value();
        currentCommandSet.deviationY = spinDeviationY->value();
        currentCommandSet.deviationX = spinDeviationX->value();
        emit sendSetData(currentCommandSet);
    });
    connect(butGet, &QPushButton::clicked, [this] { emit sendGetData(); });
    butExit->setFixedSize(100, 100);
    connect(butExit, &QPushButton::clicked, [this] { close(); });
    //-------------------- Spinboxes ------------------------------------------
    QList<QSpinBox*> listSpins = this->findChildren<QSpinBox*>();
    for (auto&& s : listSpins) {
        s->setFixedSize(200, 100);
        s->setStyleSheet(Style::SpinSettings);
        s->setMaximum(65535);
    }
    enable(false);
}

void Settings::codeCheck(int i) {
    code *= 10;
    code += i;
    if (code == CODE) {
        enable(true);
    }
}

void Settings::enable(bool state) {
    spinCoefX->setEnabled(state);
    spinCoefY->setEnabled(state);
    spinDeviationX->setEnabled(state);
    spinDeviationY->setEnabled(state);
}
