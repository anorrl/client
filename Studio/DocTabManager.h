/**
 * DocTabWidget.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QRubberBand>
#include <QIcon>

// ANORRL Headers
#include "ARL/BaldPtr.h"

// ANORRL Studio Headers
#include "IANORRLDoc.h"

class QRubberBand;

class ANORRLMainWindow;
class ANORRLTabWidget;

class DocTabManager : public QObject
{
    Q_OBJECT

    public:

        DocTabManager(ANORRLMainWindow& mainWindow);
        virtual ~DocTabManager();

        void addDoc(IANORRLDoc& doc, int index = -1);
        bool removeDoc(IANORRLDoc& doc);
        bool renameDoc(IANORRLDoc& doc,const QString& text,const QString& tooltip,const QIcon& icon = QIcon());

        bool setCurrentDoc(IANORRLDoc& doc);
        IANORRLDoc* getCurrentDoc() const;

        void setDockHoverOverPos(const QPoint& globalPos);
        bool attemptAttach(IANORRLDoc& doc,const QPoint& globalPos);

        void restoreAsCentralWidget();

    public Q_SLOTS:
        void showNextTabPage();
        void showPreviousTabPage();

    protected:

        virtual bool eventFilter(QObject* object,QEvent* event);

    private Q_SLOTS:

        void onCurrentChanged(int index);
        bool onTabCloseRequested(int index);
        void onCurrentTabCloseRequested();
        void onDetachDoc();
        void onFocusChanged(QWidget* oldWidget,QWidget* newWidget);
        
    private:

        void onMousePressEvent(QMouseEvent* event);
        void onMouseMoveEvent(QMouseEvent* event);
        void onMouseReleaseEvent(QMouseEvent* event);
        bool isValidHoverOver(const QPoint& globalPos);
    
        void setupTabPageSwitchingShortcuts();
        void setCurrentTabPage(int index);

        typedef QVector< ARL::BaldPtr<IANORRLDoc> > tDocList;

        tDocList                		m_Docs;
        ANORRLMainWindow&       		m_MainWindow;
        QPoint                  		m_ClickPos;
        bool                    		m_MouseDown;
        int                     		m_DragTabIndex;
        QRubberBand             		m_RubberBand;
        ARL::BaldPtr<QWidget>   		m_FakeTab;
		ARL::BaldPtr<ANORRLTabWidget>	m_TabWidget;
};

