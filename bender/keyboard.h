#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class Keyboard : public QWidget {
    Q_OBJECT
  public:
    explicit Keyboard(QWidget* parent = nullptr);
    ~Keyboard();

    void setX_pressed(bool state);
    void setY_pressed(bool state);

  signals:
    void onButEnterClicked();
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

  private:
    void init();
    QVBoxLayout* layV = new QVBoxLayout;
    QHBoxLayout* layH1 = new QHBoxLayout;
    QVBoxLayout* layV1 = new QVBoxLayout;
    QHBoxLayout* layH1_1 = new QHBoxLayout;
    QHBoxLayout* layH1_2 = new QHBoxLayout;
    QHBoxLayout* layH2 = new QHBoxLayout;
    QVBoxLayout* layV2_1 = new QVBoxLayout;
    QVBoxLayout* layV2_2 = new QVBoxLayout;
    QHBoxLayout* layH2_1 = new QHBoxLayout;

    QPushButton* butEnter = new QPushButton("E", this);
    QPushButton* butX = new QPushButton("--", this);
    QPushButton* butY = new QPushButton("|", this);
    QPushButton* but0 = new QPushButton("0", this);
    QPushButton* but1 = new QPushButton("1", this);
    QPushButton* but2 = new QPushButton("2", this);
    QPushButton* but3 = new QPushButton("3", this);
    QPushButton* but4 = new QPushButton("4", this);
    QPushButton* but5 = new QPushButton("5", this);
    QPushButton* but6 = new QPushButton("6", this);
    QPushButton* but7 = new QPushButton("7", this);
    QPushButton* but8 = new QPushButton("8", this);
    QPushButton* but9 = new QPushButton("9", this);
};

#endif // KEYBOARD_H
