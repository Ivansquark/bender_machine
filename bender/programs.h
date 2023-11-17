#ifndef PROGRAMS_H
#define PROGRAMS_H

#include <QHBoxLayout>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>

class Programs : public QWidget {
    Q_OBJECT
  public:
    explicit Programs(QWidget* parent = nullptr);
    ~Programs();

  private:
    void init();

    QHBoxLayout* layH = new QHBoxLayout;
    QVBoxLayout* layV1 = new QVBoxLayout;
    QVBoxLayout* layV2 = new QVBoxLayout;

    QRadioButton* but1 = new QRadioButton("1", this);
    QRadioButton* but2 = new QRadioButton("2", this);
    QRadioButton* but3 = new QRadioButton("3", this);
    QRadioButton* but4 = new QRadioButton("4", this);
    QRadioButton* but5 = new QRadioButton("5", this);
    QRadioButton* but6 = new QRadioButton("6", this);
    QRadioButton* but7 = new QRadioButton("7", this);
    QRadioButton* but8 = new QRadioButton("8", this);
    QRadioButton* but9 = new QRadioButton("9", this);
    QRadioButton* but10 = new QRadioButton("10", this);
    QRadioButton* but11 = new QRadioButton("11", this);
    QRadioButton* but12 = new QRadioButton("12", this);
    QRadioButton* but13 = new QRadioButton("13", this);
    QRadioButton* but14 = new QRadioButton("14", this);
    QRadioButton* but15 = new QRadioButton("15", this);
    QRadioButton* but16 = new QRadioButton("16", this);
};

#endif // PROGRAMS_H
