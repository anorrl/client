/**
 * RobloxTabWidget.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QTabWidget>

class ANORRLTabWidget : public QTabWidget
{
    public:

		ANORRLTabWidget(QWidget* parent)
            : QTabWidget(parent)
        {
        }

        QTabBar& getTabBar() { return *tabBar(); }
};


