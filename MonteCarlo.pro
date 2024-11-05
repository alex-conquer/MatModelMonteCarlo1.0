QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    analytical.cpp \
    button.cpp \
    interface.cpp \
    main.cpp \
    test.cpp \
    wall.cpp

HEADERS += \
    analytical.h \
    button.h \
    calculate.h \
    exception.h \
    generator.h \
    interface.h \
    test.h \
    wall.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_CXXFLAGS_RELEASE += -O0
QMAKE_CXXFLAGS_RELEASE -= -O2 -O3 -O1
