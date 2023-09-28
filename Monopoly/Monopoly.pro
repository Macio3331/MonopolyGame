QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    data/dice.cpp \
    data/player.cpp \
    interface/game.cpp \
    data/field.cpp \
    main.cpp \
    interface/mainmenu.cpp \
    interface/mainwidget.cpp

HEADERS += \
    data/dice.h \
    data/player.h \
    interface/game.h \
    interface/mainmenu.h \
    interface/mainwidget.h \
    data/field.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
