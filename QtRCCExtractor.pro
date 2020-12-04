QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -Wno-deprecated-copy

DESTDIR = $${PWD}
MOC_DIR += $${DESTDIR}/.tmp/$${TARGET}
OBJECTS_DIR += $${DESTDIR}/.tmp/$${TARGET}/obj
UI_DIR += $${DESTDIR}/.tmp/$${TARGET}
RCC_DIR += $${DESTDIR}/.tmp/$${TARGET}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    Utils.h \
    MainWindow.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
