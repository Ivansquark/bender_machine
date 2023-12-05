#ifndef SCREEN_H
#define SCREEN_H

#include "control.h"
// #include "fileops.h"
#include "keyboard.h"
#include "programs.h"
#include "interface.h"
#include "valtostr.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <memory.h>

class Screen : public QDialog {
    Q_OBJECT

  public:
    Screen(QWidget* parent = nullptr);
    ~Screen();

    enum AutoState : uint8_t {
        MANUAL,
        SEMI_AUTO,
        AUTO
    };

  private:
    void init();

  public slots:
    void onBut0Clicked();
    void onBut1Clicked();
    void onBut2Clicked();
    void onBut3Clicked();
    void onBut4Clicked();
    void onBut5Clicked();
    void onBut6Clicked();
    void onBut7Clicked();
    void onBut8Clicked();
    void onBut9Clicked();
    void onButXClicked();
    void onButYClicked();
    void onButEnterClicked();
    void onButStart();

    void getCurrentReply(const Protocol::Reply& reply);

    void moveCycle();

  private:
    void addSymbol(char sym);
    QLabel* labSetPosY = new QLabel("SetY", this);
    QLabel* labPosY = new QLabel("NEED CALIBRATION, \nY", this);
    QLabel* labGetPosY = new QLabel("GetY", this);
    QLabel* labSetPosX = new QLabel("SetX", this);
    QLabel* labPosX = new QLabel("\nX", this);
    QLabel* labGetPosX = new QLabel("GetX", this);

    QVBoxLayout* layVleftY = new QVBoxLayout();
    QVBoxLayout* layVleftX = new QVBoxLayout();
    QVBoxLayout* layVleftBut = new QVBoxLayout();
    QVBoxLayout* layVleft = new QVBoxLayout();
    QVBoxLayout* layVright = new QVBoxLayout();
    QHBoxLayout* layH = new QHBoxLayout();
    Keyboard* keyboard = new Keyboard(this);

    // QPushButton* butExit = new QPushButton("Exit", this);

    //--------------- keyboard variables --------------------------------------
    ValToStr valToStr;
    //QString valXToString(uint32_t val);
    //QString valYToString(uint32_t val);
    std::unique_ptr<QString> strValX = std::make_unique<QString>("");
    std::unique_ptr<QString> strValY = std::make_unique<QString>("");
    std::unique_ptr<QString> strTempValX = std::make_unique<QString>("000.0");
    uint32_t x = 0;
    std::unique_ptr<QString> strTempValY = std::make_unique<QString>("000.00");
    uint32_t y = 0;
    uint8_t symCountX = 0;
    uint8_t symCountY = 0;
    enum XorY : uint8_t {
        NONE,
        X,
        Y
    };
    XorY currentXorY = XorY::NONE;
    bool IsPlusOrMinusX = false;
    //---------------- Programs -----------------------------------------------
    Programs* programs = new Programs(this);
    Fileops::Pmode currentPmode = Fileops::Pmode1;
    Fileops::Pnum currentPnum = Fileops::P1;
    Fileops::Pauto currentPauto = Fileops::MANUAL;
    uint32_t currentY = 0;
    uint32_t currentX = 0;
    uint32_t currentGetY = 0;
    uint32_t currentGetX = 0;
    void getYX();
    //---------------- Control ------------------------------------------------
    Control* control = new Control(this);
    bool isStarted = false;
    //---------------- File ops -----------------------------------------------
    Fileops fileops;
    //---------------- Interface ----------------------------------------------
    Protocol::Command currentCommand;
    Protocol::Reply currentReply;
    std::unique_ptr<Interface> interface = std::make_unique<Interface>();
    //---------------- timers -------------------------------------------------
    QTimer moveTimer;
    //---------------- Calibration --------------------------------------------
    bool IsCalibrated = false;
};
#endif // SCREEN_H
