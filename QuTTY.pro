#--------------------------------------------------------
# Copyright (C) 2012 Rajendran Thirupugalsamy
# See LICENSE for full copyright and license information.
# See COPYING for distribution information.
#--------------------------------------------------------
 
#-------------------------------------------------
#
# Project created by QtCreator 2012-01-02T17:45:25
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = ../QuTTY
TEMPLATE = lib


SOURCES +=  \
    GuiMain.cpp \
    GuiMainWindow.cpp \
    GuiTerminalWindow.cpp \
    GuiSettingsWindow.cpp \
    terminalmisc.cpp \
    QtCommon.cpp \
    QtNet.cpp \
    QtUnicode.cpp \
    QtPageant.cpp \
    QtDlg.cpp \
    QtGss.cpp \
    QtX11.cpp \
    QtConfig.cpp \
    GuiTabWidget.cpp \
    GuiMenu.cpp \
    GuiBase.cpp \
    GuiSplitter.cpp \
    GuiDrag.cpp \
    puttysrc/pgssapi.c \
    puttysrc/int64.c \
    puttysrc/ssh.c \
    puttysrc/backend.c \
    puttysrc/proxy.c \
    puttysrc/portfwd.c \
    puttysrc/misc.c \
    puttysrc/terminal.c \
    puttysrc/ldisc.c \
    puttysrc/ldiscucs.c \
    puttysrc/timing.c \
    puttysrc/sshbn.c \
    puttysrc/sshpubk.c \
    puttysrc/sshrand.c \
    puttysrc/sshrsa.c \
    puttysrc/sshsh256.c \
    puttysrc/sshsh512.c \
    puttysrc/sshsha.c \
    puttysrc/x11fwd.c \
    puttysrc/tree234.c \
    puttysrc/telnet.c \
    puttysrc/pinger.c \
    puttysrc/sshmd5.c \
    puttysrc/sshaes.c \
    puttysrc/sshdes.c \
    puttysrc/sshzlib.c \
    puttysrc/sshdh.c \
    puttysrc/sshblowf.c \
    puttysrc/ssharcf.c \
    puttysrc/sshcrcda.c \
    puttysrc/sshdss.c \
    puttysrc/cproxy.c \
    puttysrc/logging.c \
    puttysrc/minibidi.c \
    puttysrc/sshgssc.c \
    puttysrc/wcwidth.c \
    puttysrc/sshcrc.c \
    puttysrc/wildcard.c \
    puttysrc/BE_ALL_S.C \
    tmux/TmuxBackend.cpp \
    tmux/TmuxGateway.cpp \
    tmux/TmuxWindowPane.cpp \
    tmux/TmuxLayout.cpp

HEADERS +=  \
    GuiMainWindow.h \
    GuiTerminalWindow.h \
    GuiSettingsWindow.h \
    QtCommon.h \
    QtLogDbg.h \
    QtConfig.h \
    QtConfigTag.h \ QtTimer.h \ GuiTabWidget.h \
    GuiMenu.h \
    GuiBase.h \
    GuiSplitter.h \
    GuiDrag.h \
    puttysrc/tree234.h \
    puttysrc/terminal.h \
    puttysrc/sshgssc.h \
    puttysrc/sshgss.h \
    puttysrc/ssh.h \
    puttysrc/puttyps.h \
    puttysrc/puttymem.h \
    puttysrc/putty.h \
    puttysrc/proxy.h \
    puttysrc/pgssapi.h \
    puttysrc/network.h \
    puttysrc/missing.h \
    puttysrc/ldisc.h \
    puttysrc/int64.h \
    puttysrc/QtStuff.h \
    puttysrc/misc.h \
    tmux/tmux.h \
    tmux/TmuxGateway.h \
    tmux/TmuxWindowPane.h \
    tmux/TmuxLayout.h

win32 {
    DEFINES += "_WINDOWS"
    SOURCES += puttysrc/WINDOWS/winnoise.c \
      puttysrc/WINDOWS/winstore.c \
      puttysrc/WINDOWS/windefs.c \
      puttysrc/WINDOWS/settings.c 

    HEADERS += puttysrc/WINDOWS/STORAGE.H  
}

unix {
    DEFINES += "_LINUX"
}

INCLUDEPATH += ./ puttysrc/

FORMS += \
    GuiSettingsWindow.ui

OTHER_FILES += \
    qutty.rc \
    qutty.ico

RC_FILE = qutty.rc
