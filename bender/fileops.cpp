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
        vectStr[1] = std::to_string(tempNum);
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
        vectStr[1] = std::to_string(index);
        vectStr[index + 1] = std::to_string(yx.Y) + ',' + std::to_string(yx.X);

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

        std::string str = vectStr[index + 1];
        size_t pos = str.find(',');
        tempYX.Y = std::stoi(str.substr(0, pos));
        tempYX.X = std::stoi(str.substr(pos + 1));
    }

    return tempYX;
}

Fileops::Pauto Fileops::getFilePauto() {
    std::fstream file("programs.csv", std::ios::in);
    if (file.is_open()) {
        std::string line;
        getline(file, line);
        qDebug() << line.data();
        currentPauto = (Pauto)std::stoi(line);
        file.close();
    }
    return currentPauto;
}

void Fileops::setFilePauto(Pauto val) {
    std::fstream file("programs.csv", std::ios::in);
    if (file.is_open()) {
        std::string line;
        std::vector<std::string> vectStr;
        while (std::getline(file, line)) {
            vectStr.emplace_back(line);
        }
        file.close();
        // change values
        currentPauto = val;
        vectStr[0] = std::to_string(currentPauto);
        // write
        file.open("programs.csv", std::ios_base::out);
        std::string fullStr;
        for (auto& i : vectStr) {
            fullStr += i + "\n";
        }
        // qDebug() << fullStr.data();
        file.write(fullStr.data(), fullStr.size());
        file.close();
    }
}

Fileops::Settings Fileops::getSettings() {
    std::fstream file("programs.csv", std::ios::in);
    if (file.is_open()) {
        std::string line;
        std::vector<std::string> vectStr;
        while (std::getline(file, line)) {
            vectStr.push_back(line);
        }
        file.close();
        // change values
        currentSettings.coefY = std::stoi(vectStr[34]);
        currentSettings.coefX = std::stoi(vectStr[35]);
        currentSettings.deviationY = std::stoi(vectStr[36]);
        currentSettings.deviationX = std::stoi(vectStr[37]);
    }
    return currentSettings;
}

void Fileops::setSettings(Settings set) {
    currentSettings = set;
    // write to file
    std::fstream file("programs.csv", std::ios::in);
    if (file.is_open()) {
        std::string line;
        std::vector<std::string> vectStr;
        while (std::getline(file, line)) {
            vectStr.push_back(line);
        }
        file.close();

        // change values
        vectStr[34] = std::to_string(currentSettings.coefY);
        vectStr[35] = std::to_string(currentSettings.coefX);
        vectStr[36] = std::to_string(currentSettings.deviationY);
        vectStr[37] = std::to_string(currentSettings.deviationX);

        file.open("programs.csv", std::ios_base::out);
        std::string fullStr;
        for (auto& i : vectStr) {
            fullStr += i + "\n";
        }
        // qDebug() << fullStr.data();
        file.write(fullStr.data(), fullStr.size());
        file.close();
    }
}

void Fileops::init() {
    //------------- filesystem ------------------------------------------------
    // TODO: open file csv (create if not opened)
    // 1 line - current AUTO MODE
    // 2 line - current program num
    // 3 - 34 lines - Yfloat, Xfloat
    // 35 - 38 - Settings
    std::fstream file("programs.csv", std::ios::in);
    if (file.is_open()) {
        std::string line;
        getline(file, line);
        qDebug() << line.data();
        currentPauto = (Pauto)std::stoi(line);
        line.clear();
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
        // read program values
        for (int i = 0; i < 32; ++i) {
            getline(file, line);
        }
        line.clear();
        getline(file, line);
        currentSettings.coefY = std::stoi(line);
        line.clear();
        getline(file, line);
        currentSettings.coefX = std::stoi(line);
        line.clear();
        getline(file, line);
        currentSettings.deviationY = std::stoi(line);
        line.clear();
        getline(file, line);
        currentSettings.deviationX = std::stoi(line);

    } else {
        qDebug() << "cant open file";
        file.open("programs.csv", std::ios_base::out);
        QString str;
        str += "0\n"; // MANUAL MODE
        str += "1\n"; // Pnum
        // program values
        for (int i = 0; i < 32; i++) {
            str += "0,0\n";
        }
        // settings
        for (int i = 0; i < 4; i++) {
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
