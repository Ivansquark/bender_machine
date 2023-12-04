QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    control.cpp \
    fileops.cpp \
    interface.cpp \
    keyboard.cpp \
    main.cpp \
    programs.cpp \
    protocol.cpp \
    screen.cpp \
    uart.cpp \
    udp.cpp \
    valtostr.cpp

HEADERS += \
    control.h \
    fileops.h \
    interface.h \
    keyboard.h \
    programs.h \
    protocol.h \
    screen.h \
    style.h \
    uart.h \
    udp.h \
    valtostr.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    img.qrc
