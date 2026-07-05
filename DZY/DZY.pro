INCLUDEPATH += $$PWD/../bighw
QT += widgets multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../bighw/bomb.cpp \
    ../bighw/bullet.cpp \
    ../bighw/button.cpp \
    ../bighw/card.cpp \
    ../bighw/coin.cpp \
    ../bighw/coinmaker.cpp \
    ../bighw/enemy.cpp \
    ../bighw/map.cpp \
    ../bighw/normalenemy.cpp \
    ../bighw/other.cpp \
    ../bighw/paotai.cpp \
    ../bighw/plusenemy.cpp \
    ../bighw/protecter.cpp \
    ../bighw/shooter.cpp \
    ../bighw/shop.cpp \
    gamewindow.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../bighw/CoinMaker.h \
    ../bighw/bomb.h \
    ../bighw/bullet.h \
    ../bighw/button.h \
    ../bighw/card.h \
    ../bighw/coin.h \
    ../bighw/enemy.h \
    ../bighw/map.h \
    ../bighw/normalenemy.h \
    ../bighw/other.h \
    ../bighw/paotai.h \
    ../bighw/plusenemy.h \
    ../bighw/protecter.h \
    ../bighw/shooter.h \
    ../bighw/shop.h \
    gamewindow.h \
    mainwindow.h

FORMS += \
    gamewindow.ui \
    mainwindow.ui

TRANSLATIONS += \
    DZY_zh_CN.ts
CONFIG += lrelease

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc


