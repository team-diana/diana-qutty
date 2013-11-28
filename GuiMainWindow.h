/*
 * Copyright (C) 2012 Rajendran Thirupugalsamy
 * See LICENSE for full copyright and license information.
 * See COPYING for distribution information.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMenu>
#include <QToolButton>
#include <QShortcut>
#include "GuiMainWindow.h"
#include "QtCommon.h"
#include "GuiSettingsWindow.h"
#include "GuiMenu.h"
#include "GuiDrag.h"

class GuiSettingsWindow;

class GuiMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QAction *menuCommonActions[MENU_MAX_ACTION];
    QMenu menuCommonMenus[MENU_MAX_MENU];
    GuiTerminalWindow *menuCookieTermWnd;
    QMenu menuSavedSessions;

    // members for drag-drop support
    GuiDragDropSite dragDropSite;

    GuiMainWindow(QWidget *parent = 0);
    ~GuiMainWindow();
    void createNewTab(Config *cfg, GuiBase::SplitType splittype=GuiBase::TYPE_LEAF);
#ifdef _WINDOWS
    bool winEvent ( MSG * msg, long * result );
#endif
    void closeEvent ( QCloseEvent * event );
    GuiTerminalWindow *getCurrentTerminal();
    GuiTerminalWindow *getCurrentTerminalInTab(int tabIndex);

    QMenu *getMenuById(qutty_menu_id_t id) {
        qutty_assert(id > MENU_SEPARATOR && id <= MENU_SEPARATOR + MENU_MAX_MENU);
        return &menuCommonMenus[id - MENU_SEPARATOR - 1];
    }
    int setupLayout(GuiTerminalWindow *newTerm, GuiBase::SplitType split);

    QTabWidget *tabArea;
private:
    GuiSettingsWindow *settingsWindow;
    QList<GuiTerminalWindow *> terminalList;
    QToolButton newTabToolButton;   // shown in top right corner of tabbar

    void initializeMenuSystem();
    void inittializeDragDropWidget();
    void populateMenu(QMenu &menu, qutty_menu_id_t menu_list[], int len);

    void readSettings();
    void writeSettings();

public slots:
    void on_openNewWindow();
    void on_openNewSession(GuiBase::SplitType splittype);
    void on_openNewTab() { on_openNewSession(GuiBase::TYPE_LEAF); }
    void on_openNewSplitHorizontal() { on_openNewSession(GuiBase::TYPE_HORIZONTAL); }
    void on_openNewSplitVertical() { on_openNewSession(GuiBase::TYPE_VERTICAL); }
    void on_createNewSession(Config cfg, GuiBase::SplitType splittype);
    void on_settingsWindowClose();
    void on_changeSettingsTab(GuiTerminalWindow *termWnd);
    void on_changeSettingsTabComplete(Config cfg, GuiTerminalWindow *termWnd);
    void closeTab(GuiTerminalWindow *termWnd);
    void tabCloseRequested (int index);
    void currentChanged(int index);
    void focusChanged ( QWidget * old, QWidget * now );

    void tabNext();
    void tabPrev();

    void contextMenuPaste();
    void contextMenuSavedSessionsChanged();
    void contextMenuSavedSessionTriggered();
    void contextMenuDuplicateSessionTriggered();
    void contextMenuRestartSessionTriggered();
    void contextMenuChangeSettingsTriggered();
    void contextMenuCloseSessionTriggered();
    void contextMenuCloseWindowTriggered();
    void contextMenuMenuBar();
    void contextMenuFullScreen();
    void contextMenuAlwaysOnTop();
};

#endif // MAINWINDOW_H
