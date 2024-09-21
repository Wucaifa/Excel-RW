QT       += core gui sql xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataBaseOperation.cpp \
    excelread.cpp \
    main.cpp \
    tinyxml2.cpp \
    widget.cpp \
    xmlFileOperation.cpp

HEADERS += \
    CommonStructDefine.h \
    DBTableClass.h \
    DataBaseOperation.h \
    DataBaseOperation_p.h \
    excelread.h \
    tinyxml2.h \
    widget.h \
    xmlFileOperation.h

FORMS += \
    widget.ui


include(src/xlsx/qtxlsx.pri)
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    path/1-C2.xml \
    path/10-C4.xml \
    path/11-C1.xml \
    path/11-C2.xml \
    path/12-C2.xml \
    path/12-C3.xml \
    path/13-C1.xml \
    path/13-C4.xml \
    path/14-29.xml \
    path/14-C1.xml \
    path/15-30.xml \
    path/15-C2.xml \
    path/16-31.xml \
    path/16-C1.xml \
    path/17-32.xml \
    path/17-C2.xml \
    path/18-33.xml \
    path/18-C3.xml \
    path/19-34.xml \
    path/19-C3.xml \
    path/2-C3.xml \
    path/20-C4.xml \
    path/23-C2.xml \
    path/23-C4.xml \
    path/24-C1.xml \
    path/24-C3.xml \
    path/25-C2.xml \
    path/25-C4.xml \
    path/26-C1.xml \
    path/26-C3.xml \
    path/27-C4.xml \
    path/28-C3.xml \
    path/29-C4.xml \
    path/3-C4.xml \
    path/30-C3(1).xml \
    path/31-C4.xml \
    path/32-C3.xml \
    path/33-C3.xml \
    path/34-C3.xml \
    path/35-C2(2).xml \
    path/36-C2.xml \
    path/37-C2.xml \
    path/38-C2.xml \
    path/4-C2.xml \
    path/40-C2.xml \
    path/41-C2.xml \
    path/42-C2.xml \
    path/43-11.xml \
    path/43-14(1).xml \
    path/43-6.xml \
    path/43-7.xml \
    path/44-12.xml \
    path/44-15(1).xml \
    path/44-4.xml \
    path/44-8.xml \
    path/45-13.xml \
    path/45-16(1).xml \
    path/45-35.xml \
    path/45-9.xml \
    path/46-10.xml \
    path/46-17(1).xml \
    path/46-32(1).xml \
    path/46-32.xml \
    path/46-36.xml \
    path/47-11.xml \
    path/47-18(1).xml \
    path/47-31.xml \
    path/47-33.xml \
    path/47-37.xml \
    path/48-12.xml \
    path/48-19(1).xml \
    path/48-3.xml \
    path/48-30.xml \
    path/48-34.xml \
    path/49-13.xml \
    path/49-2.xml \
    path/49-29.xml \
    path/5-C3.xml \
    path/50-14.xml \
    path/50-28.xml \
    path/50-38.xml \
    path/51-15.xml \
    path/51-27.xml \
    path/51-39.xml \
    path/52-16.xml \
    path/52-26.xml \
    path/52-40.xml \
    path/53-17.xml \
    path/53-25.xml \
    path/53-41.xml \
    path/54-18.xml \
    path/54-24(1).xml \
    path/54-42.xml \
    path/55-1.xml \
    path/56-23.xml \
    path/57-24.xml \
    path/58-25.xml \
    path/59-26.xml \
    path/6-C4.xml \
    path/60-27.xml \
    path/61-28.xml \
    path/7-C2.xml \
    path/8-C4.xml \
    path/9-C3.xml \
    path/L-19.xml \
    path/L-20.xml \
    path/L-23.xml \
    path/L-43(1).xml \
    path/L-44.xml \
    path/L-45(1).xml \
    path/L-46.xml \
    path/L-47.xml \
    path/L-48.xml \
    path/L-49.xml \
    path/L-5.xml \
    path/L-50.xml \
    path/L-51.xml \
    path/L-52.xml \
    path/L-53.xml \
    path/L-54.xml \
    path/L-55.xml \
    path/L-56.xml \
    path/L-57.xml \
    path/L-58.xml \
    path/L-59.xml \
    path/L-60.xml \
    path/L-61.xml
