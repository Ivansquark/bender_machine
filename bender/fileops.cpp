#include "fileops.h"

Fileops::Fileops() {}

void Fileops::init() {
    //------------- filesystem ------------------------------------------------
    // TODO: open file csv (create if not opened)
    // 0 line - current program num
    // 1 - 32 lines - Yfloat, Xfloat
    std::fstream file("programs.csv", std::ios::in);
    if (file.is_open()) {
        std::string line;
        getline(file, line);
        qDebug() << line.data();
        int num = std::stoi(line);
        qDebug() << num;
        if (num < 17) {
            currentPmode = Pmode1;
            currentPnum = (Pnum)num;
        } else {
            currentPmode = Pmode2;
            currentPnum = (Pnum)(num / 2);
        }
        // TODO: send signal
    } else {
        qDebug() << "cant open file";
        file.open("programs.csv", std::ios_base::out);
        QString str;
        str += "1\n";
        for (int i = 0; i < 32; i++) {
            str += "0.0,0.0\n";
        }
        str += '\0';
        QByteArray arr;
        for (int i = 0; i < str.size(); ++i) {
            arr.append((char*)(str.data() + i));
        }
        file.write((char*)arr.data(), arr.length());
        // wait for file writted
        file.flush();
        currentPmode = Pmode1;
        currentPnum = P1;
    }
    file.close();
}
