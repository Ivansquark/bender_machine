#ifndef SETTINGS_H
#define SETTINGS_H

#include "protocol.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class Settings : public QDialog {
    Q_OBJECT
  public:
    explicit Settings(QWidget* parent = nullptr);

    void setData(const Protocol::ReplySet&);
    void firstSendSettings(const Protocol::CommandSet& val);

  signals:
    void sendGetData();
    void sendSetData(const Protocol::CommandSet&);

  private slots:

  private:
    void init();
    QPushButton* butExit = new QPushButton("Exit", this);
    //--------------- Layouts -------------------------------------------------
    std::unique_ptr<QHBoxLayout> layMain = std::make_unique<QHBoxLayout>();
    std::unique_ptr<QVBoxLayout> layPass = std::make_unique<QVBoxLayout>();
    std::unique_ptr<QHBoxLayout> layPass13 = std::make_unique<QHBoxLayout>();
    std::unique_ptr<QHBoxLayout> layPass46 = std::make_unique<QHBoxLayout>();
    std::unique_ptr<QHBoxLayout> layPass79 = std::make_unique<QHBoxLayout>();
    std::unique_ptr<QVBoxLayout> laySet = std::make_unique<QVBoxLayout>();
    std::unique_ptr<QHBoxLayout> laySetLabDev = std::make_unique<QHBoxLayout>();
    std::unique_ptr<QHBoxLayout> laySetSpinDev = std::make_unique<QHBoxLayout>();
    std::unique_ptr<QHBoxLayout> laySetLabCoef = std::make_unique<QHBoxLayout>();
    std::unique_ptr<QHBoxLayout> laySetSpinCoef = std::make_unique<QHBoxLayout>();
    std::unique_ptr<QHBoxLayout> laySetGetSend = std::make_unique<QHBoxLayout>();
    //--------------- Buttons -------------------------------------------------
    QPushButton* but1 = new QPushButton("1", this);
    QPushButton* but2 = new QPushButton("2", this);
    QPushButton* but3 = new QPushButton("3", this);
    QPushButton* but4 = new QPushButton("4", this);
    QPushButton* but5 = new QPushButton("5", this);
    QPushButton* but6 = new QPushButton("6", this);
    QPushButton* but7 = new QPushButton("7", this);
    QPushButton* but8 = new QPushButton("8", this);
    QPushButton* but9 = new QPushButton("9", this);

    //--------------- Settings ------------------------------------------------
    QLabel* labDeviationX = new QLabel("Deviation X", this);
    QLabel* labDeviationY = new QLabel("Deviation Y", this);
    QLabel* labCoefX = new QLabel("Coefficient X", this);
    QLabel* labCoefY = new QLabel("Coefficient Y", this);
    QSpinBox* spinDeviationX = new QSpinBox(this);
    QSpinBox* spinDeviationY = new QSpinBox(this);
    QSpinBox* spinCoefX = new QSpinBox(this);
    QSpinBox* spinCoefY = new QSpinBox(this);
    QPushButton* butGet = new QPushButton("Get", this);
    QPushButton* butSend = new QPushButton("Send", this);

    int code = 0;
    int codeCounter = 0;
    static constexpr int CODE = 756;
    void codeCheck(int i);
    void enable(bool state);

    //-------------- SEND GET -------------------------------------------------
    Protocol::ReplySet currentReplyGet;
    Protocol::CommandSet currentCommandSet;
};

#endif // SETTINGS_H
