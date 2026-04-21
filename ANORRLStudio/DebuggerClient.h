/**
 * DebuggerClient.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

//this will make sure that we can use qt with boost (else moc compiler errors)
#ifndef QT_NO_KEYWORDS
#define QT_NO_KEYWORDS
#endif

// 3rd Party Headers
#include "boost/shared_ptr.hpp"

// Roblox Headers
#include "rbx/signal.h"
#include "script/DebuggerManager.h"

// Qt Headers
#include <QObject>
#include <QTreeWidget>

// Roblox Studio Headers
#include "RobloxScriptDoc.h"

namespace ARL {
	class Script;
	class DataModel;
	class Heartbeat;
	class RunTransition;
	namespace Scripting
	{
		class ScriptDebugger;
	}
}

class QTextBlock;
class QDockWidget;
class ScriptTextEditor;
class DebuggerToolTipWidget;
class DebuggerClient;
class IRobloxDoc;
class QDebuggerClient;
class QStringList;

class DebuggerClientManager : public QObject
{
	Q_OBJECT
public:

	enum eStepMode
	{
		STEP_INTO,
		STEP_OUT,
		STEP_OVER
	};

	static DebuggerClientManager& Instance();

	void setDataModel(shared_ptr<ARL::DataModel> spDataModel);
	boost::shared_ptr<ARL::DataModel> getDataModel();

	bool actionState(const QString &actionID, bool &enableState, bool &checkedState);
	bool handleAction(const QString &actionID, bool checkedState);

	void addDebuggerClient(DebuggerClient* pDebuggerClient);
	void removeDebuggerClient(DebuggerClient* pDebuggerClient);
	void setActiveDebuggerClient(DebuggerClient* pDebuggerClient);

	void syncBreakpointState(shared_ptr<ARL::Scripting::DebuggerBreakpoint> spBreakpoint);
	
	ARL::Reflection::Variant getWatchValue(boost::shared_ptr<ARL::Scripting::DebuggerWatch> spWatch);
	void addWatch(const QString& watchKey);

	DebuggerClient* getOrCreateDebuggerClient(shared_ptr<ARL::Instance> spScript);
	DebuggerClient* getDebuggerClient(shared_ptr<ARL::Instance> spScript);
	const std::vector<DebuggerClient *>& getDebuggerClients();

	DebuggerClient* getCurrentDebuggerClient() { return m_pCurrentDebuggerClient; }

	void breakpointEncounter(DebuggerClient *pDebuggerClient);
	void onRunTransition(ARL::RunTransition evt);

	//signals emitted 
	rbx::signal<void(int, ARL::Scripting::ScriptDebugger::Stack)>     clientActivated;
	rbx::signal<void()>                                               clientDeactivated;

	rbx::signal<void()>                                               clearAll;

	rbx::signal<void()>                                               debuggingStarted;
	rbx::signal<void()>                                               debuggingStopped;

	rbx::signal<void()>                                               executionDataCleared;
	rbx::signal<void(int, DebuggerClient*)>                           breakpointEncountered;

	rbx::signal<void(shared_ptr<ARL::Scripting::DebuggerBreakpoint>)> breakpointAdded;
	rbx::signal<void(shared_ptr<ARL::Scripting::DebuggerBreakpoint>)> breakpointRemoved;
	
	rbx::signal<void(shared_ptr<ARL::Scripting::DebuggerWatch>)>      watchAdded;
	rbx::signal<void(shared_ptr<ARL::Scripting::DebuggerWatch>)>      watchRemoved;

	rbx::signal<void(DebuggerClient*)>                                debuggersListUpdated;

private Q_SLOTS:	
	void onStepAction();
	void updateScriptDocument();
	void updateDebuggersList();

private:
	DebuggerClientManager();
	
	void displayBreakOnErrorDialog();
	void updateDocumentData(bool openDoc = true, bool updateMarkers = true);

	void onDebuggerAdded(boost::shared_ptr<ARL::Instance> spInstance);
	void onDebuggerRemoved(boost::shared_ptr<ARL::Instance> spInstance);

	void startDebugging(ARL::RunTransition evt);
	void pauseDebugging(ARL::RunTransition evt);
	void stopDebugging(ARL::RunTransition evt);

	void resumeAllClients(bool forceAll = false);
	void pauseAllClients(bool forceAll = false);
	void resetAllClients();

	Q_INVOKABLE void updateDebugActions(bool add);	

	std::vector<DebuggerClient *>                       debuggerClients;
	boost::shared_ptr<ARL::DataModel>					m_spDataModel;

	rbx::signals::connection                            m_cRunTransitionConnection;
	rbx::signals::connection                            m_cDebuggerAddedConnection;
	rbx::signals::connection                            m_cDebuggerRemovedConnection;

	DebuggerClient*                     				m_pCurrentDebuggerClient;
	DebuggerClient*                                     m_pActiveDebuggerClient;

	QAction*                                            m_pSeparatorAction;

	QMutex                                              m_debuggerClientMutex;

	bool                                                m_bIgnorePauseExecution;
	bool                                                m_bDebuggerListUpdateRequested;
};

class DebuggerClient
{
public:

	struct BreakpointDetail
	{
		BreakpointDetail(int iLine, bool iIsEnabled)
			: line(iLine), isEnabled(iIsEnabled)
		{}
		int  line;
		bool isEnabled;
		//std::string condition;
	};

	typedef std::vector<BreakpointDetail> BreakpointDetails;

	DebuggerClient(boost::shared_ptr<ARL::Instance> script);
	~DebuggerClient();

	void activate();
	void deActivate();

	void setDocument(IRobloxDoc *pDocument);
	boost::shared_ptr<ARL::Instance> getScript();
	boost::shared_ptr<ARL::Instance> getScript(int frameNo);

	bool updateDocument(bool openDoc = true, bool updateMarkers = true);
	void setExternalDebuggerClient(DebuggerClient* pExtDebuggerClient);

	void highlightLine(int textEditLine);
	int getCurrentLine();
	
	void setCurrentLine(int textEditLine);
	void setMarker(int textEditLine, const QString& marker, bool setBlockCurrent);

	void setCurrentFrame(int frame);
	int getCurrentFrame() { return m_CurrentFrame; }

	ARL::Scripting::ScriptDebugger::Stack getCurrentCallStack() { return m_CallStackAtPausedLine; }

	bool getValue(const QString& key, ARL::Reflection::Variant& value, int frame = -1);

	ARL::Reflection::Variant getWatchValue(boost::shared_ptr<ARL::Scripting::DebuggerWatch> spWatch);
	void addWatch(const QString& watchKey);

	QString getSourceCodeAtLine(int breakpointLine);
	void syncBreakpointState(int breakpointLine);

	void syncScriptWithTextEditor(const BreakpointDetails& textEditBreakpoints);
	void syncTextEdtiorWithScript();
	
	void toggleBreakpoint(int breakpointLine);
	void toggleBreakpointState(int breakpointLine);

	void pauseExecution();
	void resumeExecution();
	void resetExecution();

	bool isDebugging();
	bool isPaused();

	bool hasError();
	std::string getErrorMessage() { return m_ErrorMessage; }

	const ARL::Scripting::ScriptDebugger::PausedThreads& getPausedThreads();
	int getCurrentThread();
	void setCurrentThread(int threadID);

	bool isHighlightingRequired();
	bool isTopFrameCurrent();

	bool actionState(const QString &actionID, bool &enableState, bool &checkedState);
	bool handleAction(const QString &actionID, bool checkedState);

	//signals emitted 
	rbx::signal<void()> debuggerResuming;

private:
	bool isValid();
	
	void onDebuggerResume();
	void onEncounteredBreakpoint(int line);
	void onBreakpointAdded(shared_ptr<ARL::Instance> spInstance);
	void onBreakpointRemoved(shared_ptr<ARL::Instance> spInstance);
	void onWatchAdded(shared_ptr<ARL::Instance> spInstance);
	void onWatchRemoved(shared_ptr<ARL::Instance> spInstance);
	void onScriptErrorDetected(int errorLine, std::string errorMessage, ARL::Scripting::ScriptDebugger::Stack stack);

	void onScriptStopped();
	
	bool isKeyDefined(boost::shared_ptr<const ARL::Reflection::ValueMap> variables, const QString &wordUnderCursor, ARL::Reflection::Variant &value);
	
	boost::shared_ptr<ARL::Instance>					m_spScript;
	boost::shared_ptr<ARL::DataModel>					m_spDataModel;
	boost::shared_ptr<ARL::Scripting::ScriptDebugger>	m_spDebugger;

	std::list<rbx::signals::connection>	   		        m_cConnections;

	DebuggerClient*                                     m_pExtDebuggerClient;
	QDebuggerClient*                                    m_pQDebuggerClient;

	QStringList                                         m_ScriptLines;

	ARL::Scripting::ScriptDebugger::Stack               m_CallStackAtPausedLine;
	int                                                 m_CurrentFrame;

	std::string                                         m_ErrorMessage;

	int                                                 m_currentThreadID;

	bool                                                m_bIgnoreBreakpointAddRemove;
	bool                                                m_bIsActive;
};

class QDebuggerClient : public QObject
{
	Q_OBJECT
public:
	QDebuggerClient(DebuggerClient* pDebuggerClient, ScriptTextEditor* pTextEdit);
	~QDebuggerClient();

	void activate();
	void deActivate();

	QString getSourceCodeAtLine(int textEditLine);
	void updateBreakpointState(int textEditLine, ARLTextUserData::eBreakpointState state = ARLTextUserData::NO_BREAKPOINT);

	void highlightLine(int textEditLine);
	void clearLineHighlight();
	void setCurrentLine(int textEditLine);
	void setMarker(int textEditLine, const QString& marker, bool setBlockCurrent);

	void setExternalDebuggerClient(DebuggerClient* pExtDebuggerClient) { m_pExtDebuggerClient = pExtDebuggerClient; }

	void updateEditor();

private Q_SLOTS:
	void onInsertBreakpoint();
	void onDeleteBreakpoint();
	void onToggleBreakpoint(int textEditLine);
	void onToggleBreakpointState();
	void onAddWatch();

	//slot methods to ensure public functions are called from main thread
	void highlightLine_MT(int textEditLine);
	void clearLineHighlight_MT();
	void setCurrentLine_MT(int textEditLine);
	void setMarker_MT(int textEditLine, QString marker, bool setBlockCurrent);

	bool onShowToolTip(const QPoint& pos, const QString& key);
	void onContentsChanged();
	
	void onUpdateContextualMenu(QMenu *pMenu, QPoint pos);
private:
	void updateTextUserData(int textEditLineNumber, ARLTextUserData::eBreakpointState state);
	void modifyContextualMenu(QMenu* pContextualMenu);
	void cleanupContextMenu();

	void setCurrentBlock(const QTextBlock &blockToSet);
	void highlightBlock(const QTextBlock &blockToHighlight);

	DebuggerClient::BreakpointDetails getBreakpointDetails();
	void removeBreakpointState();

	bool eventFilter(QObject *obj, QEvent *evt)	;

	DebuggerClient*                                     m_pDebuggerClient;
	DebuggerClient*                                     m_pExtDebuggerClient;

	QAction*                                            m_pBreakpointMenuAction;
	QAction*                                            m_pSeparatorAction1;
	QAction*                                            m_pSeparatorAction2;
	
	ScriptTextEditor*		                            m_pTextEdit;
	DebuggerToolTipWidget*                              m_pToolTipWidget;

	QList<QTextEdit::ExtraSelection>                    m_extraSelections;

	int                                                 m_lastBlockCount;
	bool                                                m_isModifiedByKeyBoard;
	bool                                                m_ignoreAutoRepeatEvent;
};


