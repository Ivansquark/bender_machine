#include "fileops.h"

Fileops::Fileops() { init(); }

Fileops::~Fileops() {}

int Fileops::getIndex(Pnum num, Pmode mode) { return mode ? num + 16 : num; }

void Fileops::setFilePnum(Pnum num, Pmode mode) {
    int tempNum = getIndex(num, mode);
    std::fstream file("programs.csv", std::ios::in);
    if (file.is_open()) {
        std::string line;
        std::vector<std::string> vectStr;
        while (std::getline(file, line)) {
            vectStr.emplace_back(line);
        }
        vectStr[0] = std::to_string(tempNum);
        file.close();

        file.open("programs.csv", std::ios_base::out);
        std::string fullStr;
        for (auto& i : vectStr) {
            fullStr += i + "\n";
        }
        // qDebug() << fullStr.data();
        file.write(fullStr.data(), fullStr.size());
        file.close();
        currentPnum = num;
        currentPmode = mode;
    }
}

void Fileops::setFileValues(Pnum num, Pmode mode, YX yx) {
    int index = getIndex(num, mode);
    std::fstream file("programs.csv", std::ios::in);
    if (file.is_open()) {
        std::string line;
        std::vector<std::string> vectStr;
        while (std::getline(file, line)) {
            vectStr.emplace_back(line);
        }
        file.close();

        // change values
        vectStr[0] = std::to_string(index);
        vectStr[index] = std::to_string(yx.Y) + ',' + std::to_string(yx.X);

        file.open("programs.csv", std::ios_base::out);
        std::string fullStr;
        for (auto& i : vectStr) {
            fullStr += i + "\n";
        }
        // qDebug() << fullStr.data();
        file.write(fullStr.data(), fullStr.size());
        file.close();
        currentPnum = num;
        currentPmode = mode;
    }
}

Fileops::YX Fileops::getFileValues(Pnum num, Pmode mode) {
    YX tempYX;
    int index = getIndex(num, mode);
    std::fstream file("programs.csv", std::ios::in);
    if (file.is_open()) {
        std::string line;
        std::vector<std::string> vectStr;
        while (std::getline(file, line)) {
            vectStr.emplace_back(line);
        }
        file.close();

        // get values

        std::string str = vectStr[index];
        size_t pos = str.find(',');
        tempYX.Y = std::stoi(str.substr(0, pos));
        tempYX.X = std::stoi(str.substr(pos + 1));
    }

    return tempYX;
}

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
            currentPnum = (Pnum)(num - 16);
        }
    } else {
        qDebug() << "cant open file";
        file.open("programs.csv", std::ios_base::out);
        QString str;
        str += "1\n";
        for (int i = 0; i < 32; i++) {
            str += "0,0\n";
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
