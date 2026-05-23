/**
 * ANORRLScriptDoc.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

//this will make sure that we can use qt with boost (else moc compiler errors)
#ifndef QT_NO_KEYWORDS
    #define QT_NO_KEYWORDS
#endif

#include <list>
#include <map>

// Qt Headers
#include <QIcon>
#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QStyle>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

// ANORRL Headers
#include "arl/signal.h"
#include "arl/BaldPtr.h"

// ANORRL Studio Headers
#include "LuaSourceBuffer.h"
#include "ANORRLBasicDoc.h"

namespace ARL 
{
	class Instance;
	class RunService;
	class DataModel;
	namespace Reflection
	{
		class PropertyDescriptor;
	}
}

class QTextBlock;
class ANORRLScriptDoc;
class ANORRLIDEDoc;
class ScriptTextEditor;
class CheckSyntaxThread;

class ARLTextUserData : public QTextBlockUserData
{
public:

	enum eBreakpointState
	{
		NO_BREAKPOINT,
		ENABLED,
		DISABLED
	};

	ARLTextUserData():m_foldState(0), m_lineState(0), m_BreakpointState(NO_BREAKPOINT) {}

	void setFoldState(int state) { m_foldState = state; }
	int getFoldState() const { return m_foldState;}

	void setLineState(int state) { m_lineState = state; }
	int getLineState() const { return m_lineState; }

	void setBreakpointState(eBreakpointState state) { m_BreakpointState = state; }
	eBreakpointState getBreakpointState() { return m_BreakpointState; }

	void setMarker(const QString& iconName) { m_MarkerIcon = iconName; }
	QString getMarker() { return m_MarkerIcon; }

private:

	QString             m_MarkerIcon;
	int					m_foldState;
	int					m_lineState;
	eBreakpointState  	m_BreakpointState;
};

class ANORRLScriptDoc: public QObject, public ANORRLBasicDoc
{
	Q_OBJECT

public:

	ANORRLScriptDoc();
	virtual ~ANORRLScriptDoc();
   
 	bool open(ANORRLMainWindow *pMainWindow, const QString &fileName);
	
	IANORRLDoc::ARLCloseRequest requestClose();

	IANORRLDoc::ARLDocType docType() { return IANORRLDoc::SCRIPT; }

    QString fileName() const;
    QString displayName() const { return m_displayName; }
    QString keyName() const     { return m_keyName; }
	virtual const QIcon& titleIcon() const;
			
	bool save();
	bool saveAs(const QString &fileName);

	void setScript(shared_ptr<ARL::DataModel> dm, LuaSourceBuffer script);

	ScriptTextEditor* getTextEditor() { return m_pTextEdit; }

	QString saveFileFilters();
	
	QWidget* getViewer();
	
	bool isModified();

	void activate();

	virtual void updateUI(bool state);
		
	virtual bool doHandleAction(const QString& actionID, bool isChecked);
	bool actionState(const QString &actionID, bool &enableState, bool &checkedState);

	bool handleDrop(const QString &fileName);
	bool handlePluginAction(void *, void *) { return false; }
	void handleScriptCommand(const QString &execCommand);

	void applyEditChanges();

    static void init(ANORRLMainWindow& mainWindow);

	LuaSourceBuffer getCurrentScript();
	shared_ptr<ARL::DataModel>	getDataModel() { return m_dataModel; }

	void refreshEditorFromSourceBuffer();
    
private Q_SLOTS:

	void requestScriptDeletion();
	void onSelectionChanged();
    void deActivate();
    void reloadLiveScript();
	void explainBreakpointsDisabled();
	void editingContents();
	void onScriptNamePropertyChanged(const QString& newName);
	void onScriptSourcePropertyChanged(const QString& newSource);
	
private:
	bool doClose();
	void setupScriptConnection();
	void deactivateConnection();
	void activateConnection();
	void disconnectScriptConnection();
	QString buildTabTitle();

	void onAncestryChanged(boost::shared_ptr<ARL::Instance> newParent);
	void onPropertyChanged(const ARL::Reflection::PropertyDescriptor* desc);

	void maybeUpdateText(const QString& code);
	void removeScriptLock();

	static void setMenuVisibility(bool visible);
	
	arl::signals::scoped_connection		m_ancestryChangedConnection;
	arl::signals::scoped_connection		m_propertyChangedConnection;
	LuaSourceBuffer                     m_script;
	boost::shared_ptr<ARL::DataModel>	m_dataModel;

	ARL::BaldPtr<ANORRLMainWindow>   	m_pMainWindow;
    ARL::BaldPtr<ANORRLIDEDoc> 			m_pParentDoc;
	ARL::BaldPtr<ScriptTextEditor>  	m_pTextEdit;
	
	QString             m_fileName;
	QString             m_displayName; //to be used if file name is empty
    QString             m_keyName;

	static int          sScriptCount;
	static int          slastActivatedRibbonPage;

	bool				m_undoAvailable;
	bool				m_redoAvailable;
	bool				m_copyAvailable;
	bool				m_updatingText;
	bool                m_currentlyAttemptingToGetScriptLock;
	bool                m_haveCloudEditScriptLock;
    
	friend class ScriptTextEditor;
};

