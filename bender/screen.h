#ifndef SCREEN_H
#define SCREEN_H

#include "control.h"
// #include "fileops.h"
#include "interface.h"
#include "keyboard.h"
#include "programs.h"
#include "settings.h"
#include "touch.h"
#include "valtostr.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>
#include <QThread>
#include <QVBoxLayout>
#include <QWindow>
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
    void onButStartPressed();
    void onButStartReleased();

    void getCurrentReply(const Protocol::Reply& reply);

    void moveCycle();


  private:
    void addSymbol(char sym);
    QLabel* labSetPosY = new QLabel("0", this);
    QLabel* labInfoY = new QLabel("NEED CALIBRATION", this);
    QLabel* labPosY = new QLabel("|  Y", this);
    QLabel* labGetPosY = new QLabel("0", this);
    QLabel* labSetPosX = new QLabel("0", this);
    QLabel* labInfoX = new QLabel("___", this);
    QLabel* labPosX = new QLabel("-- X", this);
    QLabel* labGetPosX = new QLabel("0", this);

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
    // QString valXToString(uint32_t val);
    // QString valYToString(uint32_t val);
    std::unique_ptr<QString> strValX = std::make_unique<QString>("");
    std::unique_ptr<QString> strValY = std::make_unique<QString>("");
    std::unique_ptr<QString> strTempValX = std::make_unique<QString>("000.0");
    uint32_t x = 0;
    std::unique_ptr<QString> strTempValY = std::make_unique<QString>("000.00");
    uint32_t y = 0;
    uint8_t symCountX = 0;
    uint8_t symCountY = 0;
    enum class XorY : uint8_t {
        NONE,
        X,
        Y
    };
    XorY currentXorY = XorY::NONE;
    bool IsPlusOrMinusX = false;
    enum class MoveXorY : uint8_t {
        NONE,
        X,
        Y
    };
    MoveXorY currentMoveXorY = MoveXorY::NONE;
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
    Protocol::CommandSet currentCommandSet;
    Protocol::Reply currentReply;
    std::unique_ptr<Interface> interface = std::make_unique<Interface>();
    //---------------- timers -------------------------------------------------
    // QTimer moveTimer;
    // QTimer* timerWaitForPress;
    std::unique_ptr<QTimer> timerWaitForPress = std::make_unique<QTimer>();
    std::unique_ptr<QTimer> timerForAutoSend = std::make_unique<QTimer>();
    std::unique_ptr<QTimer> timerWaitForSaveInFile = std::make_unique<QTimer>();
    static constexpr uint32_t TIME_TO_PRESS = 200;
    static constexpr uint32_t TIME_TO_AUTO_SEND = 90;
    static constexpr uint32_t TIME_TO_SAVE_IN_FILE = 100;



    //---------------- Calibration --------------------------------------------
    bool IsCalibrated = false;
    void setCalibrationState(bool state);
    //---------------- Manual movement ----------------------------------------
    void setManualMoovement(bool state);
    bool IsPlusPressed = false;
    bool IsMinusPressed = false;
    bool WasXStopped = false;
    bool WasYStopped = false;
    //---------------- Settings -----------------------------------------------
    std::unique_ptr<Settings> set = std::make_unique<Settings>(this);


    //---------------- Mouse handling -----------------------------------------
  protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

  protected:
    // bool event(QEvent *ev) override;

    // void touchEvent(QTouchEvent *ev);
};
#endif // SCREEN_H
