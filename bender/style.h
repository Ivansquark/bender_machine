#ifndef STYLE_H
#define STYLE_H

namespace Style {
constexpr const char* TextTemp =
    "QLabel{color:yellow; background-color:grey; font-size:40px;}";
constexpr const char* TextFinal =
    "QLabel{color:red; background-color:grey; font-size:40px;}";

constexpr const char* ButPressed =
    "QPushButton{background-color:red; font-size:25px;}";
constexpr const char* ButReleased =
    "QPushButton{background-color:grey; font-size:25px;}";

constexpr const char* ButProgPressed =
    "QPushButton{background-color:red; font-size:25px;}"
    "border-style: solid;"
    "border-color: black;"
    "border-width: 3px;"
    "border-radius: 20px;}";
constexpr const char* butProgReleased =
    "QPushButton{background-color:grey; font-size:25px;"
    "border-style: solid;"
    "border-color: black;"
    "border-width: 3px;"
    "border-radius: 20px;}";

constexpr const char* radioP =
    "QRadioButton{text-align: left;background-color: None; font-size:20px;}"
    "QRadioButton::indicator::checked{"
    "width:30px; height:30px;border: 3px solid darkgray; border-radius: 18px; "
    "background-color: #AC4949}"
    "QRadioButton::indicator::unchecked{"
    "width:30px; height:30px; border: 3px solid darkgray; border-radius: 18px; "
    "background-color: black}";

constexpr const char* ButStartPressed =
    "QPushButton{background-color:red; font-size:25px;}";
constexpr const char* ButStartReleased =
    "QPushButton{background-color:green; font-size:25px;}";

} // namespace Style

#endif // STYLE_H
