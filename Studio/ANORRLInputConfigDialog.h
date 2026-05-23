/**
 * ANORRLInputConfigDialog.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

#include <QAction>
#include <QDialog>
#include <QString>
#include <QWidget>

#include "arl/BaldPtr.h"

class ANORRLMainWindow;
class QEvent;
class QTreeView;
class QDialogButtonBox;
class QAbstractButton;
class ANORRLKeyboardConfigWidget;
class ANORRLMouseConfigWidget;

//
// A dialog for configuring mouse and keyboard shortcuts for all actions
// under the ANORRLMainWindow.
//
class ANORRLInputConfigDialog : public QDialog
{
    Q_OBJECT
    
public:

    ANORRLInputConfigDialog(ANORRLMainWindow& MainWindow);
    virtual ~ANORRLInputConfigDialog();
    
private Q_SLOTS:
    void accept();
    void cancel();
    void clicked(QAbstractButton* button);
    
    void dataChanged();
    
private:
    virtual void closeEvent(QCloseEvent* event);
    void restoreAllDefaults();
    void initialize();

    QDialogButtonBox*           m_dialogButtonBox;
    ANORRLKeyboardConfigWidget* m_keyboardConfigWidget;
    ANORRLMouseConfigWidget*    m_mouseConfigWidget;
    ANORRLMainWindow&           m_mainWindow;
    bool                        m_dataChanged;
};

