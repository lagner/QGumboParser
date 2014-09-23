#-------------------------------------------------
#
# Project created by QtCreator 2014-09-23T16:10:11
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET = tst_qgumboparsertest
CONFIG += console c++11 testcase testlib
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += tst_qgumboparsertest.cpp
HEADERS += tst_qgumboparsertest.h
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QGumboParser/release/ -lQGumboParser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QGumboParser/debug/ -lQGumboParser
else:unix: LIBS += -L$$OUT_PWD/../QGumboParser/ -lQGumboParser

INCLUDEPATH += $$PWD/../QGumboParser
DEPENDPATH += $$PWD/../QGumboParser

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QGumboParser/release/libQGumboParser.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QGumboParser/debug/libQGumboParser.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QGumboParser/release/QGumboParser.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QGumboParser/debug/QGumboParser.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../QGumboParser/libQGumboParser.a

