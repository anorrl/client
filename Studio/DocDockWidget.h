/**
 * DocDockWidget.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QDockWidget>

// ANORRL Headers
#include "arl/BaldPtr.h"

// ANORRL Studio Headers
#include "IANORRLDoc.h"

class ANORRLMainWindow;
class DocDockManager;

class DocDockWidget : public QDockWidget
{
    Q_OBJECT

public:

    DocDockWidget(
        ANORRLMainWindow&   mainWindow,
        DocDockManager&     docManager,
        IANORRLDoc&         doc );

    virtual ~DocDockWidget();
    
    void setTitle(const QString& displayName,const QString& fileName);
    void cleanup();
    void startDragging();

public Q_SLOTS:

    void onAttachDoc();
    void onTopLevelChanged(bool TopLevel);
    void onDockLocationChanged(Qt::DockWidgetArea area);
    void onFocusChanged(QWidget* oldWidget,QWidget* newWidget);

protected:

    virtual void closeEvent(QCloseEvent* event);
    virtual void moveEvent(QMoveEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual bool event(QEvent* event);
    virtual QSize sizeHint() const { return maximumSize(); }

    virtual bool nativeEvent(const QByteArray &eventType, MSG* msg, long* result);

private:

    void simulateDrag();
    bool stopDragging();

    ANORRLMainWindow&       m_MainWindow;
    DocDockManager&         m_DocManager;
    IANORRLDoc&             m_Doc;
    IANORRLDoc::ARLDocType  m_DocType;
    ARL::BaldPtr<QWidget>   m_Viewer;
    bool                    m_IsMoving;
    bool                    m_IsSimulatingDrag;
    QPoint                  m_StartPos;
};



