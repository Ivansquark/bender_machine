#include "screen.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    a.setOverrideCursor(Qt::BlankCursor);
    Screen w;
    //w.showFullScreen();
    w.show();
    return a.exec();
}
