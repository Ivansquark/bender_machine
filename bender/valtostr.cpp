#include "valtostr.h"

ValToStr::ValToStr()
{

}

QString ValToStr::valXToString(uint32_t val) {
    QString str = "";
    int count = 0;
    uint8_t arr[4] = {0};
    while (val) {
        arr[count] = val % 10;
        val /= 10;
        count++;
    }
    if (count == 1) {
        str += "000.";
        str += QString::number(arr[0]);
    } else if (count == 2) {
        str += "00";
        str += QString::number(arr[1]);
        str += ".";
        str += QString::number(arr[0]);
    } else if (count == 3) {
        str += "0";
        str += QString::number(arr[2]);
        str += QString::number(arr[1]);
        str += ".";
        str += QString::number(arr[0]);
    } else if (count == 4) {
        str += QString::number(arr[3]);
        str += QString::number(arr[2]);
        str += QString::number(arr[1]);
        str += ".";
        str += QString::number(arr[0]);
    } else {
        str = "000.0";
    }
    return str;
}
QString ValToStr::valYToString(uint32_t val) {
    QString str = "";
    int count = 0;
    uint8_t arr[5] = {0};
    while (val) {
        arr[count] = val % 10;
        val /= 10;
        count++;
    }
    if (count == 1) {
        str += "000.0";
        str += QString::number(arr[0]);
    } else if (count == 2) {
        str += "000.";
        str += QString::number(arr[1]);
        str += QString::number(arr[0]);
    } else if (count == 3) {
        str += "00";
        str += QString::number(arr[2]);
        str += ".";
        str += QString::number(arr[1]);
        str += QString::number(arr[0]);
    } else if (count == 4) {
        str += "0";
        str += QString::number(arr[3]);
        str += QString::number(arr[2]);
        str += ".";
        str += QString::number(arr[1]);
        str += QString::number(arr[0]);
    } else if (count == 5) {
        str += QString::number(arr[4]);
        str += QString::number(arr[3]);
        str += QString::number(arr[2]);
        str += ".";
        str += QString::number(arr[1]);
        str += QString::number(arr[0]);
    } else {
        str = "000.00";
    }
    return str;
}
