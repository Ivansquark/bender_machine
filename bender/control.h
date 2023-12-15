#ifndef CONTROL_H
#define CONTROL_H

#include "fileops.h"
#include "touch.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>

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
    void setButDashColor(bool state);
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

  public slots:
    void getTouchCoord(bool WasPressed, uint16_t x, uint16_t y);
  private slots:
    void timeoutPressed();
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
    QPoint butStartParentPos;
    QPoint butPlusParentPos;
    QPoint butMinusParentPos;
    QPoint butDashParentPos;

    Fileops::Pauto currentAutoState = Fileops::Pauto::MANUAL;

    //---------------- Touch handling -----------------------------------------
    std::unique_ptr<Touch> touch = std::make_unique<Touch>(this);
    bool CheckPressed = false;
    bool IsPressed = false;
    bool IsButStartPressed = false;
    bool IsButPlusPressed = false;
    bool IsButMinusPressed = false;
    bool IsButDashPressed = false;
    uint16_t X = 0;
    uint16_t Y = 0;
    uint8_t counterAdc = 0;
    static constexpr uint8_t COUNTER_ADC_MAX = 5;
    //---------------- timer --------------------------------------------------
    std::unique_ptr<QTimer> timPressed = std::make_unique<QTimer>(this);
};

#endif // CONTROL_H
