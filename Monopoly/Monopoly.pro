QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    data/auctiondialog.cpp \
    data/chancecard.cpp \
    data/dice.cpp \
    data/exchangedialog.cpp \
    data/field.cpp \
    data/offer.cpp \
    data/offersdialog.cpp \
    data/player.cpp \
    data/windialog.cpp \
    interface/game.cpp \
    interface/mainmenu.cpp \
    interface/mainwidget.cpp \
    main.cpp

HEADERS += \
    data/auctiondialog.h \
    data/chancecard.h \
    data/dice.h \
    data/exchangedialog.h \
    data/field.h \
    data/offer.h \
    data/offersdialog.h \
    data/player.h \
    data/windialog.h \
    interface/game.h \
    interface/mainmenu.h \
    interface/mainwidget.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
