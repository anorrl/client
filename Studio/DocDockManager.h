/**
 * DocDockWidget.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QDockWidget>
#include <QMap>

// ANORRL Headers
#include "ARL/BaldPtr.h"

// ANORRL Studio Headers
#include "IANORRLDoc.h"

class ANORRLMainWindow;
class DocDockWidget;

class DocDockManager : public QObject
{
    Q_OBJECT

    public:

        DocDockManager(ANORRLMainWindow& mainWindow);
        virtual ~DocDockManager();

        void addDoc(IANORRLDoc& doc);
        bool removeDoc(IANORRLDoc& doc);
        bool renameDoc(IANORRLDoc& doc,const QString& text,const QString& tooltip);

        bool setCurrentDoc(IANORRLDoc& doc);
        IANORRLDoc* getCurrentDoc() const;

        void startDrag(IANORRLDoc& doc);

    Q_SIGNALS:

        void attachTab(IANORRLDoc& doc);

    private:

        typedef QMap< ARL::BaldPtr<IANORRLDoc>,ARL::BaldPtr<DocDockWidget> > tDocDockMap;

        ANORRLMainWindow&   m_MainWindow;
        tDocDockMap         m_Docs;
};

