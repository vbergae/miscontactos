# -------------------------------------------------
# Project created by QtCreator 2010-01-27T12:59:22
# -------------------------------------------------
QT += sql # \

# testlib
TARGET = MisContactos
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp
HEADERS += mainwindow.h \
    dbConnection.h
FORMS += mainwindow.ui
RESOURCES += RecursosAgenda.qrc

# Icono para Mac
ICON = iconos/MacIcon.icns

# Icono en Windows
RC_FILE = agenda.rc
OTHER_FILES += agenda.rc \
    COPYING.txt
