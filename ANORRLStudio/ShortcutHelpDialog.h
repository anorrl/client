/**
 * ShortcutHelpDialog.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QDialog>

// Roblox Headers
#include "arl/BaldPtr.h"

class QTreeWidget;
class ANORRLMainWindow;

/**
 * Dialog that displays all the shortcut key sequences for actions in the main window.
 *  Also displays the actions' icon and statustip text.
 *  The dialog does not delete on close.
 */
class ShortcutHelpDialog : public QDialog
{
    public:

        ShortcutHelpDialog(ANORRLMainWindow& MainWindow);
        virtual ~ShortcutHelpDialog();

    private:

        void initialize();
        void populate();

        ARL::BaldPtr<QTreeWidget>   mTree;
		ANORRLMainWindow&           mMainWindow;
};