QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DateEditDelegate.cpp \
    SpinBoxDelegate.cpp \
    Student.cpp \
    StudentModel.cpp \
    main.cpp \
    QtFileCRUD.cpp

HEADERS += \
    DateEditDelegate.hpp \
    QtFileCRUD.hpp \
    SpinBoxDelegate.hpp \
    Student.hpp \
    StudentModel.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
