QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    promptdb.cpp \
    GalleryView/promptgalleryview.cpp \
    ListView/promptlistview.cpp \
    promptdialog.cpp \
    promptmanager.cpp \
    promptmodel.cpp \
    prompttoolbar.cpp \
    backupmanager.cpp \
    tagfilterwidget.cpp

HEADERS += \
    promptdb.h \
    GalleryView/promptgalleryview.h \
    ListView/promptlistview.h \
    promptdialog.h \
    promptmanager.h \
    promptmodel.h \
    prompttoolbar.h \
    backupmanager.h \
    tagfilterwidget.h



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# SUBDIRS += \
#     config

# TEMPLATE = subdirs

RESOURCES += \
    qrc.qrc
