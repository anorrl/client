/**
 * ANORRLDocManager.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QObject>
#include <QMap>
#include <QIcon>

// 3rd Party Headers
#include "boost/shared_ptr.hpp"

// ANORRL Headers
#include "arl/BaldPtr.h"
#include "V8DataModel/DataModel.h"

// ANORRL Studio Headers
#include "IANORRLDoc.h"
#include "LuaSourceBuffer.h"

class QPoint;

class ANORRLMainWindow;
class ANORRLObjectBrowserDoc;
class ANORRLWebDoc;
class ANORRLScriptDoc;
class DocTabManager;
class DocDockManager;
class ANORRLIDEDoc;

namespace ARL
{
	class Instance;
    class DataModel;
}

typedef QMap< QString,ARL::BaldPtr<IANORRLDoc> > tKeyDocMap;
typedef QList< ARL::BaldPtr<ANORRLScriptDoc> > tScriptDocs;
typedef QList<boost::shared_ptr<ARL::Instance> > tPlayModifiedScriptList;

class ANORRLDocManager : public QObject
{
    Q_OBJECT

public:
	
	static ANORRLDocManager& Instance();

	void initialize(ANORRLMainWindow& mainWindow);
	void shutDown();

    bool requestCloseAllDocs();
    bool requestCloseAndSaveAllDocs();
    void closeAllDocs();

    void registerOpenedScriptDoc(ANORRLScriptDoc& doc, LuaSourceBuffer script);
	void deregisterScriptDoc(ANORRLScriptDoc& doc);
    void closeDoc(const LuaSourceBuffer& script);
	ANORRLObjectBrowserDoc* getObjectBrowserDoc();

    const tScriptDocs& getOpenScriptDocs() { return m_OpenScriptDocs; }
	ANORRLScriptDoc* findOpenScriptDoc(LuaSourceBuffer script);
    
    tPlayModifiedScriptList& getPlayModifiedScriptList() { return m_PlayModifiedScriptList; }

	IANORRLDoc* getOrCreateDoc(IANORRLDoc::ARLDocType docType);
	ANORRLScriptDoc* openDoc(LuaSourceBuffer script);
	
    void removeDoc(IANORRLDoc& doc);

	void configureDocWidget(IANORRLDoc& doc);
	
	void setCurrentDoc(IANORRLDoc* doc);
    IANORRLDoc* getCurrentDoc() const       { return m_pCurrentDoc; }
    ANORRLIDEDoc* getPlayDoc() const        { return m_pPlayDoc; }
	ANORRLWebDoc* getStartPageDoc() const   { return m_pWebDoc; }

    int getDocCount() const                 { return m_KeyDocMap.size(); }

	void updateWindowMenu();
    void setDocTitle(IANORRLDoc& doc,const QString& displayName,const QString& fileName,const QIcon& icon = QIcon());

    void detachDoc(IANORRLDoc& doc,bool dragging);
    void attachDoc(IANORRLDoc& doc, int index = -1);

    void setDockHoverOverPos(const QPoint& globalPos);
    bool attemptAttach(IANORRLDoc& doc,const QPoint& globalPos);

    void setPlayDocCentralWidget();
    void restoreCentralWidget();

Q_SIGNALS:
	void currentDocChanged();

public Q_SLOTS:

	void openDoc(shared_ptr<ARL::Instance> script, int lineNumber);
	
    void onActivateWindow();

private:

	ANORRLDocManager();
	virtual ~ANORRLDocManager();

    void activateDoc(int index);
    ARL::BaldPtr<ANORRLMainWindow>          m_pMainWindow;
	tKeyDocMap			                    m_KeyDocMap;
	ARL::BaldPtr<IANORRLDoc>                m_pCurrentDoc;
	ARL::BaldPtr<ANORRLIDEDoc>              m_pPlayDoc;
	ARL::BaldPtr<ANORRLObjectBrowserDoc>    m_pObjectBrowser;
	ARL::BaldPtr<ANORRLWebDoc>              m_pWebDoc;
	tScriptDocs				                m_OpenScriptDocs;
    ARL::BaldPtr<DocTabManager>             m_DocTabManager;
    ARL::BaldPtr<DocDockManager>            m_DocDockManager;
    tPlayModifiedScriptList                 m_PlayModifiedScriptList;
};
