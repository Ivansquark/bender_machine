#ifndef CONTROL_H
#define CONTROL_H

#include "fileops.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>

class Control : public QWidget {
    Q_OBJECT
  public:
    explicit Control(QWidget* parent = nullptr);
    ~Control();
    void setStart(bool state);
    void setEnabledCalibration(bool state);
    void setManualMoovement(bool state);

  private:
    void init();

  public:
    void setPmode(Fileops::Pmode mode);
    void setPauto(Fileops::Pauto autoState);
  signals:
    void onButStartPressed();
    void onButStartReleased();
    void sendCurrentModePressed(Fileops::Pmode mode);
    void onButManual();
    void onButSemiAuto();
    void onButAuto();
    void onButPlusPressed();
    void onButPlusReleased();
    void onButMinusPressed();
    void onButMinusReleased();
    void onButDashPressed();
    void onButDashReleased();

  private:
    QVBoxLayout* layV = new QVBoxLayout;
    QHBoxLayout* layH1 = new QHBoxLayout;
    QHBoxLayout* layH2 = new QHBoxLayout;
    QHBoxLayout* layHstart = new QHBoxLayout;
    QHBoxLayout* layHplus = new QHBoxLayout;
    QHBoxLayout* layHminus = new QHBoxLayout;
    QHBoxLayout* layHdash = new QHBoxLayout;

    QRadioButton* radioP1 = new QRadioButton("P1", this);
    QRadioButton* radioP2 = new QRadioButton("P2", this);

    QPushButton* butManual = new QPushButton("", this);
    QIcon* iconManual = new QIcon(":/new/prefix1/hand.png");
    QIcon* iconSemiAuto = new QIcon(":/new/prefix1/semiauto.png");
    QIcon* iconAuto = new QIcon(":/new/prefix1/auto.png");
    QPushButton* butSemiAuto = new QPushButton("", this);
    QPushButton* butAuto = new QPushButton("", this);

    QPushButton* butStart = new QPushButton("Start", this);
    QPushButton* butPlus = new QPushButton("+", this);
    QPushButton* butMinus = new QPushButton("-", this);
    QPushButton* butDash = new QPushButton("__", this);
};

#endif // CONTROL_H
