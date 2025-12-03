QT       += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = ATC_System
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    addtariffdialog.cpp \
    addclientdialog.cpp \
    addvipclientdialog.cpp \
    addcalldialog.cpp \
    Person.cpp \
    Client.cpp \
    LoyaltyProgram.cpp \
    VIPClient.cpp \
    Tariff.cpp \
    Call.cpp \
    DataManager.cpp

HEADERS += \
    mainwindow.h \
    addtariffdialog.h \
    addclientdialog.h \
    addvipclientdialog.h \
    addcalldialog.h \
    Person.h \
    Client.h \
    LoyaltyProgram.h \
    VIPClient.h \
    Tariff.h \
    Call.h \
    DataManager.h

# НЕ НУЖНЫ .ui файлы - UI создается в коде!

MOC_DIR = build/moc
OBJECTS_DIR = build/obj
RCC_DIR = build/rcc
UI_DIR = build/ui

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 13.0
}

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
