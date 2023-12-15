#ifndef STYLE_H
#define STYLE_H

namespace Style {
constexpr const char* BackgroundMain = "background-color:#5F4545;}";
constexpr const char* TextTemp =
    "QLabel{color:yellow; background-color:#403F3F; "
    "font-size:50px;font-weight: bold;border:1px;border:solid "
    "grey;border-radius:5px;}";
constexpr const char* TextFinal =
    "QLabel{color:red; background-color:#403F3F; "
    "font-size:50px;font-weight: bold;border:1px;border:solid "
    "grey;border-radius:5px;}";
constexpr const char* TextCurrent =
    "QLabel{color:red; background-color:#403F3F; "
    "font-size:50px;font-weight: bold;border:1px;border:solid "
    "grey;border-radius:5px;}";
constexpr const char* TextMove =
    "QLabel{color:red; background-color:#009999; "
    "font-size:50px;font-weight: bold;border:1px;border:solid "
    "grey;border-radius:5px;}";

constexpr const char* ButPressed =
    //"QPushButton{background-color:red; font-size:25px;}";
    //"QPushButton{background-color:#EB2727; font-size:25px;border:2px solid
    // #ff0000;;}";
    //    "QPushButton{font-size: 19px;"
    //    "background-color:qradialgradient(cx:0.5, cy:0.5, radius: 1,"
    //    "fx:0.5, fy:0.5, stop:0 rgba(188, 198, 204, 160), stop:1 rgba(240,
    //    240, 240, 200));border:1px;border-radius:10px;" "border:solid grey;"
    //    "border-style: outset;"
    //    "border-width: 3px;"
    //    "border-color: (188, 198, 204, 100);"
    //    "padding: 6px;}";
    "QPushButton{font-size: 25px; border-radius:10px; background: "
    "qradialgradient(cx:0.5, "
    "cy:0.5, radius: 1,"
    "fx:0.5, fy:0.5, stop:0 rgba(220, 220, 0, 150), stop:1 rgba(250, 250, 250, "
    "150));border:1px;"
    "border:solid grey;"
    "border-style: outset;"
    "border-width: 1px;"
    "border-color: (188, 198, 204, 200);"
    "padding: 6px;}";
constexpr const char* ButReleased =
    //"QPushButton{background-color:grey; font-size:25px;border:2px solid
    // #ff0000;}";
    "QPushButton{font-size: 25px;"
    "background-color:qradialgradient(cx:0.5, cy:0.5, radius: 1,"
    "fx:0.5, fy:0.5, stop:0 rgba(188, 198, 204, 160), stop:1 rgba(240, 240, "
    "240, 200));border:1px;border-radius:10px;"
    "border:solid grey;"
    "border-style: outset;"
    "border-width: 3px;"
    "border-color: (188, 198, 204, 100);"
    "padding: 6px;}";
//    "QPushButton:pressed{border-radius:10px; background:
//    qradialgradient(cx:0.5, cy:0.5, radius: 1," "fx:0.5, fy:0.5, stop:0
//    rgba(220, 220, 0, 150), stop:1 rgba(250, 250, 250, 150));border:1px;"
//    "border:solid grey;"
//    "border-style: outset;"
//    "border-width: 1px;"
//    "border-color: (188, 198, 204, 200);"
//    "padding: 6px;}";

constexpr const char* ButProgPressed =
    "QPushButton{background-color:red; border: 1 px; font-size:25px;}"
    "border-style: solid;"
    "border-color: black;"
    "border-width: 3px;"
    "border-radius: 20px;}";
constexpr const char* butProgReleased =
    "QPushButton{background-color:grey; border: 1 px; font-size:25px;"
    "border-style: solid;"
    "border-color: black;"
    "border-width: 3px;"
    "border-radius: 20px;border: none;}";

constexpr const char* radioP =
    "QRadioButton{text-align: left;background-color: None; font-size:20px;}"
    "QRadioButton::indicator::checked{"
    "width:30px; height:30px;border: 3px solid darkgray; border-radius: 18px; "
    "background-color: #AC4949}"
    "QRadioButton::indicator::unchecked{"
    "width:30px; height:30px; border: 3px solid darkgray; border-radius: 18px; "
    "background-color: black}";

constexpr const char* ButStartPressed =
    //"QPushButton{background-color:red; font-size:25px;}";
    //"QPushButton{background-color:#EB2727; font-size:25px;border: none;}";
    "QPushButton{border-radius:10px; background: qradialgradient(cx:0.5, "
    "cy:0.5, radius: 1,"
    "fx:0.5, fy:0.5, stop:0 rgba(220, 220, 0, 150), stop:1 rgba(250, 250, 250, "
    "150));border:1px;"
    "border:solid grey;"
    "border-style: outset;"
    "border-width: 1px;"
    "border-color: (188, 198, 204, 200);"
    "padding: 6px;}";

constexpr const char* ButStartReleased =
    //"QPushButton{background-color:green; font-size:25px;border: none;}";
    "QPushButton{font-size: 19px;"
    "background-color:qradialgradient(cx:0.5, cy:0.5, radius: 1,"
    "fx:0.5, fy:0.5, stop:0 rgba(188, 198, 204, 160), stop:1 rgba(240, 240, "
    "240, 200));border:1px;border-radius:10px;"
    "border:solid grey;"
    "border-style: outset;"
    "border-width: 3px;"
    "border-color: (188, 198, 204, 100);"
    "padding: 6px;}";

constexpr const char* SpinSettings =
    "QSpinBox::down-button{width: 40} QSpinBox::up-button{width: 40}";

} // namespace Style

#endif // STYLE_H
