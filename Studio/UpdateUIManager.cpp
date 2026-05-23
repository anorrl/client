/**
 * UpdateUIManager.cpp
 * Copyright (c) 2013 ROBLOX Corp. All rights reserved.
 */

#include "stdafx.h"
#include "UpdateUIManager.h"

// Qt Headers
#include <QAction>
#include <QBitArray>
#include <QDateTime>
#include <QDockWidget>
#include <QScrollBar>
#include <QStatusBar>
#include <QLabel>
#include <QProgressBar>
#include <QWidgetAction>
#include <QRadioButton>

// ANORRL Headers
#include "arl/Log.h"
#include "arl/TaskScheduler.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/DebugSettings.h"
#include "v8datamodel/MouseCommand.h"
#include "v8datamodel/GameBasicSettings.h"

// ANORRL Studio Headers
#include "AuthoringSettings.h"
#include "ANORRL.h"
#include "ANORRLContextualHelp.h"
#include "ANORRLCustomWidgets.h"
#include "ANORRLDiagnosticsView.h"
#include "ANORRLDocManager.h"
#include "ANORRLFindWidget.h"
#include "ANORRLGameExplorer.h"
#include "ANORRLMainWindow.h"
#include "ANORRLPropertyWidget.h"
#include "ANORRLScriptReview.h"
#include "ANORRLSettings.h"
#include "ANORRLTaskScheduler.h"
#include "ANORRLTextOutputWidget.h"
#include "ANORRLToolBox.h"
#include "ANORRLTutorials.h"
#include "ANORRLTreeWidget.h"
#include "CommonInsertWidget.h"
#include "ANORRLIDEDoc.h"
#include "QtUtilities.h"
#include "AuthoringSettings.h"
#include "ScriptAnalysisWidget.h"
#include "ANORRLTeamCreateWidget.h"

QString UpdateUIManager::defaultCommands = 
    "fileOpenAction fileNewAction fileExitAction aboutANORRLAction actionStartPage actionFullScreen "
    "onlineHelpAction objectBrowserAction instanceDumpAction settingsAction openPluginsFolderAction "
    "fastLogDumpAction filePublishedProjectsAction shortcutHelpAction managePluginsAction switchWindowsMenu resetViewAction __qtn_Quick_Access_Button "
	"customizeQuickAccessAction viewCommandBarAction fileOpenRecentSavesAction ";
static QString kActionsThatBypassOnMenuShowStatusUpdates = 
	"publishGameAction";
// Added following actions if we want to support theme modification at runtime
// Office2007Blue Office2007Black Office2007Silver Office2007Aqua Windows7Scenic Office2010Blue Office2010Silver Office2010Black Office2013White Office2013Gray
bool UpdateUIManager::m_longProcessInProgress = false;

FASTFLAG(StudioShowTutorialsByDefault)
FASTFLAG(StudioSeparateActionByActivationMethod)

UpdateUIManager& UpdateUIManager::Instance()
{
	static UpdateUIManager instance;
	return instance;
}

UpdateUIManager::UpdateUIManager()
: m_pANORRLMainWindow(NULL)
, m_PauseStatusBar(0)
, m_isRunning(false)
, m_dockRestoreStates(eDW_MAX)
, m_BusyState(0)
{
}

UpdateUIManager::~UpdateUIManager()
{
}

void UpdateUIManager::saveDocksGeometry()
{
	ANORRLSettings settings;
	for (int i = 0; i < eDW_MAX; i++)
	{
		EDockWindowID id = (EDockWindowID)i;
		QDockWidget* dock = m_DockWidgets[id];
		if (dock->widget())
            settings.setValue(dock->objectName() + "/Geometry", dock->widget()->saveGeometry());
	}
}

void UpdateUIManager::loadDocksGeometry()
{
	ANORRLSettings settings;
	for (int i = 0; i < eDW_MAX; i++)
	{
		EDockWindowID id = (EDockWindowID)i;
		QDockWidget* dock = m_DockWidgets[id];
		QByteArray state = settings.value(dock->objectName() + "/Geometry").toByteArray();
		if(!state.isEmpty() && dock->widget())
			dock->widget()->restoreGeometry(state);

        // force the layout to be updated to fix issues initializing detached on a second monitor
        dock->layout()->update();
	}
}

QWidget* UpdateUIManager::getExplorerWidget()
{
	return m_ViewWidgets[eDW_OBJECT_EXPLORER];
}

QStackedWidget* UpdateUIManager::getChatWidgetStack()
{
	if (m_ChatWidgetStack.isNull())
	{
		// we need to call this function in Main Thread only!
		ARLASSERT(QThread::currentThread() == qApp->thread());

		QDockWidget* dockWidget = new QDockWidget(tr("Chat"), m_pANORRLMainWindow);
		dockWidget->setObjectName("chatDockWidget");

		dockWidget->toggleViewAction()->setIcon(QIcon(":/RibbonBar/images/RibbonIcons/View/Chat.png"));
		dockWidget->toggleViewAction()->setText(tr("Chat"));
		dockWidget->toggleViewAction()->setStatusTip(tr("Chat"));
		dockWidget->toggleViewAction()->setToolTip(tr("View Chat Window"));

		// add a stack widget with an empty widget (to be shown show during play solo/non cloudedit mode)
		m_ChatWidgetStack = new QStackedWidget(dockWidget);
		m_ChatWidgetStack->addWidget(new QWidget);

		dockWidget->setWidget(m_ChatWidgetStack);
		configureDockWidget(dockWidget);
	}

	return m_ChatWidgetStack;
}

void UpdateUIManager::initDocks()
{
	// TODO : convert this initialization stuff to a factory

	m_DockData[eDW_DIAGNOSTICS] = SDockData(
		UpdateUIManager::getAction("viewDiagnosticsAction"),
		false );        

	m_DockData[eDW_TASK_SCHEDULER] = SDockData(
		UpdateUIManager::getAction("viewTaskSchedulerAction"),
		false );        

	m_DockData[eDW_TOOLBOX] = SDockData(
		UpdateUIManager::getAction("viewToolboxAction"),
		true  );

	m_DockData[eDW_TUTORIALS] = SDockData(
		UpdateUIManager::getAction("viewTutorialsAction"),
		false,
		QSize(350, 300));

	m_DockData[eDW_BASIC_OBJECTS] = SDockData(
		UpdateUIManager::getAction("viewBasicObjectsAction"),
		false );

	m_DockData[eDW_SCRIPT_REVIEW] = SDockData(
		UpdateUIManager::getAction("viewScriptPerformanceAction"),
		false );

	m_DockData[eDW_OBJECT_EXPLORER] = SDockData(
		UpdateUIManager::getAction("viewObjectExplorerAction"),
		true,
		QSize(200, 300));

	m_DockData[eDW_PROPERTIES] = SDockData(
		UpdateUIManager::getAction("viewPropertiesAction"),
		false,
		QSize(200, 300));

	m_DockData[eDW_OUTPUT] = SDockData(
		UpdateUIManager::getAction("viewOutputWindowAction"),
		false);

    m_DockData[eDW_CONTEXTUAL_HELP] = SDockData(
        UpdateUIManager::getAction("viewContextualHelpAction"),
        false);
	m_DockData[eDW_FIND] = SDockData(
		UpdateUIManager::getAction("viewFindResultsWindowAction"),
		false);
    
	m_DockData[eDW_GAME_EXPLORER] = SDockData(
		UpdateUIManager::getAction("gameExplorerAction"),
		false);
    
	m_DockData[eDW_SCRIPT_ANALYSIS] = SDockData(
		UpdateUIManager::getAction("viewScriptAnalysisAction"),
		false);

	m_DockData[eDW_TEAM_CREATE] = SDockData(
		UpdateUIManager::getAction("viewTeamCreateAction"),
		false);
    
	m_DockWidgets[eDW_DIAGNOSTICS]      = m_pANORRLMainWindow->diagnosticsDockWidget;
	m_DockWidgets[eDW_TASK_SCHEDULER]   = m_pANORRLMainWindow->taskSchedulerDockWidget;
	m_DockWidgets[eDW_TOOLBOX]          = m_pANORRLMainWindow->toolBoxDockWidget;
	m_DockWidgets[eDW_TUTORIALS]		= m_pANORRLMainWindow->tutorialsDockWidget;
	m_DockWidgets[eDW_BASIC_OBJECTS]    = m_pANORRLMainWindow->basicObjectsDockWidget;
	m_DockWidgets[eDW_SCRIPT_REVIEW]    = m_pANORRLMainWindow->scriptReviewDockWidget;
	m_DockWidgets[eDW_OBJECT_EXPLORER]  = m_pANORRLMainWindow->objectExplorer;
	m_DockWidgets[eDW_PROPERTIES]       = m_pANORRLMainWindow->propertyBrowser;
	m_DockWidgets[eDW_OUTPUT]			= m_pANORRLMainWindow->outputWindow;
    m_DockWidgets[eDW_CONTEXTUAL_HELP]	= m_pANORRLMainWindow->contextualHelp;
	m_DockWidgets[eDW_FIND]				= m_pANORRLMainWindow->findResultsWindow;
   	m_DockWidgets[eDW_GAME_EXPLORER]    = m_pANORRLMainWindow->gameExplorerDockWidget;
	m_DockWidgets[eDW_SCRIPT_ANALYSIS]  = m_pANORRLMainWindow->scriptAnalysisDockWidget;
	m_DockWidgets[eDW_TEAM_CREATE]      = m_pANORRLMainWindow->teamCreateDockWidget;
	
	m_ViewWidgets[eDW_DIAGNOSTICS]      = new ANORRLDiagnosticsView();
	m_ViewWidgets[eDW_TASK_SCHEDULER]   = new ANORRLTaskScheduler();
	m_ViewWidgets[eDW_TOOLBOX]          = new ANORRLToolBox();
	m_ViewWidgets[eDW_TUTORIALS]		= new ANORRLTutorials();
	m_ViewWidgets[eDW_BASIC_OBJECTS]    = new InsertObjectWidget(m_pANORRLMainWindow->basicObjectsDockWidget);
	m_ViewWidgets[eDW_SCRIPT_REVIEW]    = new ANORRLScriptReview();
	m_ViewWidgets[eDW_TEAM_CREATE]      = new ANORRLTeamCreateWidget(m_pANORRLMainWindow->teamCreateDockWidget);

	m_ViewWidgets[eDW_OBJECT_EXPLORER]  = new ANORRLExplorerWidget(m_pANORRLMainWindow->objectExplorer);

	m_ViewWidgets[eDW_PROPERTIES]       = new ANORRLPropertyWidget(m_pANORRLMainWindow->propertyBrowser);
	m_ViewWidgets[eDW_OUTPUT]			= m_pANORRLMainWindow->getOutputWidget();
    m_ViewWidgets[eDW_CONTEXTUAL_HELP]	= new ANORRLContextualHelp();
	m_ViewWidgets[eDW_FIND]				= &ANORRLFindWidget::singleton();
    m_ViewWidgets[eDW_GAME_EXPLORER]    = new ANORRLGameExplorer(m_pANORRLMainWindow->gameExplorerDockWidget);
	m_ViewWidgets[eDW_SCRIPT_ANALYSIS]  = new ScriptAnalysisWidget(m_pANORRLMainWindow->scriptAnalysisDockWidget);

	for ( int i = 0 ; i < eDW_MAX ; ++i )
	{
		EDockWindowID id = (EDockWindowID)i;

		// Unfortunately we need to remove all the dockwidgets and re-add.  If we don't we end up with a bug (DE5372)
		m_pANORRLMainWindow->removeDockWidget(m_DockWidgets[id]);
		if (id == eDW_BASIC_OBJECTS ||
			id == eDW_DIAGNOSTICS || 
			id == eDW_SCRIPT_REVIEW ||
			id == eDW_TASK_SCHEDULER ||
			id == eDW_TOOLBOX ||
            id == eDW_CONTEXTUAL_HELP ||
			id == eDW_GAME_EXPLORER ||
			id == eDW_TUTORIALS ||
			id == eDW_TEAM_CREATE)
		{
			m_pANORRLMainWindow->addDockWidget(Qt::LeftDockWidgetArea, m_DockWidgets[id]);
		}
		else if (id == eDW_PROPERTIES
			|| id == eDW_OBJECT_EXPLORER)
		{
			m_pANORRLMainWindow->addDockWidget(Qt::RightDockWidgetArea, m_DockWidgets[id]);
		}
		else if (id == eDW_OUTPUT ||
			id == eDW_FIND ||
			id == eDW_SCRIPT_ANALYSIS)
		{
			m_pANORRLMainWindow->addDockWidget(Qt::BottomDockWidgetArea, m_DockWidgets[id]);
		}
	
		// make sure all the proper data and widgets were created
		ARLASSERT(m_DockData.contains(id));
		ARLASSERT(m_DockWidgets.contains(id));
		ARLASSERT(!m_DockWidgets[id]->objectName().isEmpty());
		ARLASSERT(m_ViewWidgets.contains(id));
		ARLASSERT(!m_DockData[id].UpdateAction->objectName().isEmpty());

		// hook up views to docks
		if (m_ViewWidgets.contains(id))
			m_DockWidgets[id]->setWidget(m_ViewWidgets[id]);

		m_actionDockMap.insert(m_DockData[id].UpdateAction->objectName(), id); 

		// make sure we disconnect the original SLOT
		QObject::disconnect(m_DockWidgets[id]->toggleViewAction(), SIGNAL(triggered(bool)), m_DockWidgets[id], SLOT(_q_toggleView(bool)));
		// now connect toggleViewAction of dock widget with UpdateAction
		QObject::connect(m_DockWidgets[id]->toggleViewAction(), SIGNAL(triggered(bool)), m_DockData[id].UpdateAction, SLOT(trigger()));

		// Used to handle close event, etc.
		m_DockWidgets[id]->installEventFilter(this);
	}
}

void UpdateUIManager::init(ANORRLMainWindow *pMainWindow)
{
	if (!pMainWindow)
		return;

	m_pANORRLMainWindow = pMainWindow;

#ifdef _WIN32
	if (!pMainWindow->isRibbonStyle()) // This fails for ribbon for some reason
		qApp->setStyleSheet("QScrollBar { scrollbar-contextmenu: 0}");//remove scrollbar contextmenu from our application (this doesn't work for QWebView related scrollbars)
#endif
	
	initDocks();
	setupStatusBar();
	setupSlots();

	// Init our axisWidgets and 3d grid
	pMainWindow->toggleAxisWidgetAction->setChecked(get3DAxisEnabled());
	pMainWindow->toggle3DGridAction->setChecked(get3DGridEnabled());

    m_isRunning 		   = true;

#if WINVER >= 0x601
    // initialize Vista/Win7 task bar interface

    m_isTaskBarInitialized 		= false;
    m_isIndeterminateProgress 	= false;
    m_taskbar 					= NULL;

    HRESULT hr = ::CoCreateInstance(
        CLSID_TaskbarList,
        0,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&m_taskbar) );

    if ( hr == S_OK )
    {
        hr = m_taskbar->HrInit();

        if ( hr == S_OK )
        {
            m_isTaskBarInitialized = true;
        }
        else
        {
            m_taskbar->Release();
            m_taskbar = NULL;
        }
    }
#endif

	//update status bar
	QTimer::singleShot(0, this, SLOT(updateStatusBar()));
}

void UpdateUIManager::setupStatusBar()
{		
	//don't create border for the permanent widgets
	QStatusBar *pStatusBar = m_pANORRLMainWindow->statusBar();
	if (!pStatusBar)
		return;

	pStatusBar->setStyleSheet("QStatusBar::item { border: none; } ");

	//create labels to show different messages
	m_pTaskSchedulerSBLabel = new QLabel;
	m_pTimeSBLabel          = new QLabel;
	m_pCPUSBLabel           = new QLabel;
	m_pRAMSBLabel           = new QLabel;
	m_pFPSSBLabel           = new QLabel;

	m_pTaskSchedulerSBLabel->setMargin(5);
	m_pTimeSBLabel->setMargin(5);
	m_pCPUSBLabel->setMargin(5);
	m_pRAMSBLabel->setMargin(5);
	m_pFPSSBLabel->setMargin(5);

	//add labels to statusbar
	pStatusBar->addPermanentWidget(m_pTaskSchedulerSBLabel);
	pStatusBar->addPermanentWidget(m_pTimeSBLabel);
	pStatusBar->addPermanentWidget(m_pFPSSBLabel);
	pStatusBar->addPermanentWidget(m_pCPUSBLabel);
	pStatusBar->addPermanentWidget(m_pRAMSBLabel);
}

void UpdateUIManager::setMenubarEnabled(bool state)
{
	QList<QMenu *> childMenus = m_pANORRLMainWindow->findChildren<QMenu *>();
	for (int jj=0; jj < childMenus.size(); ++jj)
	{
		if ( state )
			connect(childMenus.at(jj), SIGNAL(aboutToShow()), this, SLOT(onMenuShow()));
		else
			disconnect(childMenus.at(jj), SIGNAL(aboutToShow()), this, SLOT(onMenuShow()));
	}
}

void UpdateUIManager::setupSlots()
{
	QList<QMenu *> childMenus = m_pANORRLMainWindow->findChildren<QMenu *>();
	for ( int j = 0; j < childMenus.size() ; ++j )
		connect(childMenus.at(j),SIGNAL(aboutToShow()),this,SLOT(onMenuShow()));        

	connect(&m_pANORRLMainWindow->fillColorToolButton(),SIGNAL(frameShown()), this,SLOT(onMenuShow()));
	connect(&m_pANORRLMainWindow->materialToolButton(),SIGNAL(frameShown()), this,SLOT(onMenuShow()));
}

QString UpdateUIManager::getDockGeometryKeyName(EDockWindowID dockId) const
{
	return m_DockWidgets[dockId]->objectName() + "_geometry";
}

void UpdateUIManager::setDockVisibility(EDockWindowID dockId, bool visible)
{
    if (m_ViewWidgets.contains(dockId))
		m_ViewWidgets[dockId]->setVisible(visible);

	m_DockWidgets[dockId]->setVisible(visible);
}

/**
 * Sets up the view menu with the dock widget actions.
 * 
 * @param insertBefore  location to add the actions in the menu
 */
void UpdateUIManager::setupDockWidgetsViewMenu(QAction& insertBefore)
{
	// Connect all dock widgets to their appropriate actions and add to the view menu
	for ( int i = 0 ; i < eDW_MAX ; ++i )
	{        
		EDockWindowID id = (EDockWindowID)i;
		QAction& action = *m_DockData[id].UpdateAction;

		// TODO : show view icons in menu
		action.setIconVisibleInMenu(false);   
		
		m_pANORRLMainWindow->viewToolsToolBar->addAction(&action);
		m_pANORRLMainWindow->menuView->insertAction(&insertBefore,&action);
	}
}

void UpdateUIManager::shutDown()
{
	m_isRunning = false;

#if WINVER >= 0x601
    // delete Vista/Win7 task bar interface
    if ( m_isTaskBarInitialized )
        m_taskbar->Release();
#endif
}

void UpdateUIManager::onMenuShow()
{
	IANORRLDoc *pDoc = ANORRLDocManager::Instance().getCurrentDoc();
	if (!pDoc)
		return;

	const QObject *pSender = sender();
	if (!pSender)
		return;

	bool isTreeWidgetConextMenu = (pSender->parent() == m_pANORRLMainWindow->treeWidgetStack().currentWidget());
	IANORRLDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc(); 

	if (pSender->inherits("QMenu"))
	{
		QList<QAction*> menuActions = ((const QMenu *)pSender)->actions();
		QAction *pAction = NULL;
		bool checkedState = false, enabledState = false;
		for (int i = 0; i < menuActions.size(); ++i) 
		{
			pAction = menuActions.at(i);

			if (!pAction)
				continue;

			QString objectName = pAction->objectName();
			if (objectName.isEmpty())
				continue;

			if (kActionsThatBypassOnMenuShowStatusUpdates.contains(objectName))
				continue;

			if (defaultCommands.contains(objectName))
			{
				pAction->setEnabled(true);
			}
			else if (isTreeWidgetConextMenu && playDoc && playDoc->actionState(objectName, enabledState, checkedState))
			{
				pAction->setEnabled(enabledState);
				pAction->setChecked(checkedState);
			}
			else if (pDoc->actionState(objectName, enabledState, checkedState))
			{
				pAction->setEnabled(enabledState);
				pAction->setChecked(checkedState);
			}
		}
	}

	if ( isTreeWidgetConextMenu )
		pDoc->handleAction("actionCustomContextMenuShow");
}

void UpdateUIManager::onMenuHide()
{
	IANORRLDoc *pDoc = ANORRLDocManager::Instance().getCurrentDoc();
	if (!pDoc)
		return;

	const QObject *pSender = sender();
	if(!pSender)
		return;

	if(!pSender->inherits("QMenu"))
		return;

	if (pSender->parent() == m_pANORRLMainWindow->treeWidgetStack().currentWidget())
		pDoc->handleAction("actionCustomContextMenuHide");
}

void UpdateUIManager::setDefaultApplicationState()
{
	QList<QAction*> allActions = m_pANORRLMainWindow->findChildren<QAction *>();
	QAction *pAction = NULL;

	for (int i = 0; i < allActions.size(); ++i) 
	{
		pAction = allActions.at(i);
		if (!pAction || pAction->objectName().isEmpty() 
			|| defaultCommands.contains(pAction->objectName())
			|| pAction->objectName() == "executeScriptAction"
			|| getDockActionNames().contains(pAction->objectName())) // docks are always enabled
			continue;

		pAction->setEnabled(false);
		pAction->setChecked(false);
	}
}

void UpdateUIManager::updateToolBars()
{
    onToolBarsUpdate();
}

void UpdateUIManager::onToolBarsUpdate()
{
    updateToolbarTask();
}

void UpdateUIManager::updateToolbarTask()
{
	// update actions with shortcut, these need to be updated in both ribbon and menu
	updateAction(*m_pANORRLMainWindow->deleteSelectedAction);
	updateAction(*m_pANORRLMainWindow->selectAllAction);
	updateAction(*m_pANORRLMainWindow->screenShotAction);
    updateAction(*m_pANORRLMainWindow->pasteAction);
    updateAction(*m_pANORRLMainWindow->pasteIntoAction);

	updateAction(*m_pANORRLMainWindow->playSoloAction);
	updateAction(*m_pANORRLMainWindow->startServerAction);
	updateAction(*m_pANORRLMainWindow->startPlayerAction);

    updateAction(*m_pANORRLMainWindow->findAction);
	updateAction(*m_pANORRLMainWindow->replaceAction);
	updateAction(*m_pANORRLMainWindow->findNextAction);
    updateAction(*m_pANORRLMainWindow->findPreviousAction);
	updateAction(*m_pANORRLMainWindow->goToLineAction);
    
    updateAction(*m_pANORRLMainWindow->simulationPlayAction);
    updateAction(*m_pANORRLMainWindow->simulationRunAction);
    updateAction(*m_pANORRLMainWindow->simulationResetAction);
    updateAction(*m_pANORRLMainWindow->simulationStopAction);
	
    updateAction(*m_pANORRLMainWindow->rotateSelectionAction);
	updateAction(*m_pANORRLMainWindow->zoomExtentsAction);
    
	if (!m_pANORRLMainWindow->isRibbonStyle())
	{
		updateActions(m_pANORRLMainWindow->editCameraToolBar->actions());
		updateActions(m_pANORRLMainWindow->runToolBar->actions());
		updateActions(m_pANORRLMainWindow->advToolsToolBar->actions());
		updateActions(m_pANORRLMainWindow->commandToolBar->actions());
		updateActions(m_pANORRLMainWindow->menuTools->actions());
		updateActions(m_pANORRLMainWindow->scriptMenu->actions());
		updateActions(m_pANORRLMainWindow->standardToolBar->actions());
	}
	else
	{
		// Loop through every ribbon page, and group, and update all actions
		const RibbonMainWindow* pMainWindow = static_cast<RibbonMainWindow*>(m_pANORRLMainWindow);
		const int pagesCount = pMainWindow->ribbonBar()->getPageCount();
		for (int i = 0; i < pagesCount; i++)
		{
			const QList<RibbonGroup*> ribbonGroups = pMainWindow->ribbonBar()->getPage(i)->findChildren<RibbonGroup*>();
		    for (int k = 0; k < ribbonGroups.count(); k++)
		    {
                RibbonGroup* group = ribbonGroups[k];

			    const QList<RibbonGallery*> galleries = group->findChildren<RibbonGallery*>();
			    for (int j = 0; j < galleries.count(); j++)
			    {
				    const bool galleryEnabled = ANORRLDocManager::Instance().getPlayDoc() != NULL;
				    galleries[j]->setEnabled(galleryEnabled);
			    }

			    updateActions(group->actions());
				if (group->getOptionButtonAction() && group->getOptionButtonAction()->isVisible())
					updateAction(*group->getOptionButtonAction());
		    }
		}

		// Update quickaccess bar's actions
		if (pMainWindow->ribbonBar()->getQuickAccessBar())
			updateActions(pMainWindow->ribbonBar()->getQuickAccessBar()->actions());

		// Update all button groups, since they are not associated with an action
		QList<QButtonGroup*> buttonGroups = pMainWindow->findChildren<QButtonGroup*>();
		for (int ii=0; ii < buttonGroups.count(); ++ii)
			modifyButtonGroupState(*buttonGroups.at(ii));

		// Update all menu actions under ToolButtonProxyMenu
		QList<ToolButtonProxyMenu*> toolButtonProxies = pMainWindow->findChildren<ToolButtonProxyMenu*>();
		for (int ii=0; ii < toolButtonProxies.count(); ++ii)
			updateActions(toolButtonProxies.at(ii)->actions());

		updateAction(*m_pANORRLMainWindow->anchorAction); // Updated based on selection
	}
}

void UpdateUIManager::updateActions(const QList<QAction*>& actions)
{
	for (int i = 0; i < actions.size(); ++i) 
	{
		QAction* action = actions.at(i);
		updateAction(*action);

		if ( action->inherits("QWidgetAction") )
		{
            QWidgetAction* widgetAction = static_cast<QWidgetAction*>(action);
			QWidget* pDefaultWidget = widgetAction->defaultWidget();
			if ( !pDefaultWidget )
                continue;

			if ( pDefaultWidget->inherits("QToolButton") )
            {
                QToolButton* button = static_cast<QToolButton*>(pDefaultWidget);
				updateAction(*button->defaultAction());
            }
            else if ( pDefaultWidget->inherits("QRadioButton") )
            {
                QRadioButton* button = static_cast<QRadioButton*>(pDefaultWidget);
				// Block all signals: Temporary Hack to fix Ribbon Bar slow down caused by the Radio buttons.
				bool oldState = pDefaultWidget->blockSignals(true);
                const QString buttonName = button->objectName();
                QAction* action = m_pANORRLMainWindow->findChild<QAction*>(buttonName);

				if (action)
				{
					updateAction(*action);
					if (FFlag::StudioSeparateActionByActivationMethod)
					{
						button->setChecked(action->isChecked());
						button->setEnabled(action->isEnabled());
					}
				}

				// Unblock all signals: Temporary hack to fix Ribbon Bar slow down caused by Radio buttons.
				pDefaultWidget->blockSignals(oldState);
            }
		}
	}
}

void UpdateUIManager::updateAction(QAction& action)
{
	if (action.objectName().isEmpty() )
		return;

	bool checkedState = action.isChecked();
    bool enabledState = action.isEnabled();

	// These commands should always be enabled
	if ( UpdateUIManager::defaultCommands.contains(action.objectName()) )
	{
		modifyActionState(action,true,checkedState);
	}
	else 
	{
		IANORRLDoc* pDoc = ANORRLDocManager::Instance().getCurrentDoc();
		if ( !pDoc )
        {
            // No current doc, disabled, but keep current checkedstate
			modifyActionState(action, false, checkedState);
        }
		else if (pDoc->actionState(action.objectName(), enabledState, checkedState)) 
        {
            // pass on to current doc
			modifyActionState(action, enabledState, checkedState);
        }
	}
}

void UpdateUIManager::modifyActionState(QAction& action,bool enabledState,bool checkedState)
{
	// force disabled if we're in a busy state
    if ( UpdateUIManager::Instance().isBusy() )
        enabledState = false;

	m_pANORRLMainWindow->updateInternalWidgetsState(&action, enabledState, checkedState);
	
    // don't spam if we're already configured properly
    if ( action.isEnabled() == enabledState && 
        (!action.isCheckable() || action.isChecked() == checkedState) )
    {
        return;
    }

    if  (QThread::currentThread() == thread())
    {
        action.setEnabled(enabledState);
        action.setChecked(checkedState);
    }
    else
    {
        QMetaObject::invokeMethod(&action,"setEnabled",Qt::QueuedConnection,Q_ARG(bool,enabledState));
        QMetaObject::invokeMethod(&action,"setChecked",Qt::QueuedConnection,Q_ARG(bool,checkedState));
    }
}

void UpdateUIManager::modifyButtonGroupState(QButtonGroup &pButtonGroup)
{
	bool checkedState = false;
    bool enabledState = false;

	IANORRLDoc* pDoc = ANORRLDocManager::Instance().getCurrentDoc();
	if (pDoc)
		pDoc->actionState(pButtonGroup.objectName(), enabledState, checkedState);
	
	QList<QAbstractButton *> buttons = pButtonGroup.buttons();

    for (int ii=0; ii < buttons.count(); ++ii)
    {
        buttons.at(ii)->setEnabled(enabledState);
        if (buttons.at(ii)->isCheckable())
        {
            if  (QThread::currentThread() == thread())
            {
                buttons.at(ii)->setChecked(checkedState);
            }
            else
            {
                QMetaObject::invokeMethod(buttons.at(ii),"setChecked",Qt::QueuedConnection,Q_ARG(bool,checkedState));
            }
        }
    }
}

bool UpdateUIManager::toggleDockView(const QString& actionName)
{
	QMap<QString, EDockWindowID>::iterator iter = m_actionDockMap.find(actionName);
	if (iter == m_actionDockMap.end())
		return false;

	EDockWindowID id = iter.value();

	if ( m_DockWidgets[id]->widget() != m_ViewWidgets[id] )
	{
		ARLASSERT(0);
		m_DockWidgets[id]->setWidget(m_ViewWidgets[id]);
	}
	bool visibility1 = m_DockWidgets[id]->isVisible();
	bool wasSetChecked = m_DockData[id].UpdateAction->isChecked(); 
	setDockVisibility(id, wasSetChecked);
	bool visibility2 = m_DockWidgets[id]->isVisible();


	if (wasSetChecked)
		m_DockWidgets[id]->raise();

	if (visibility1 == visibility2)
		return false;

	return true;
}

/**
 * Stops status bar updates.
 *  Increments pause state.  Paused if pause state > 0.
 */
void UpdateUIManager::pauseStatusBarUpdate()
{
	++m_PauseStatusBar;
}

#if WINVER >= 0x601
/**
 * Synchronizes the task bar progress with the progress bar progress.
 */
void UpdateUIManager::configureTaskBar(QProgressBar* progressBar)
{
    if ( !m_isTaskBarInitialized )
        return;

    m_taskProgressBar = progressBar;

    if ( !progressBar )
    {
        m_taskbar->SetProgressState(m_pANORRLMainWindow->winId(),TBPF_NOPROGRESS);
        return;
    }

    int min = progressBar->minimum();
    int max = progressBar->maximum();

    if ( min == max )   // indeterminate
    {
        m_isIndeterminateProgress = true;
        m_taskbar->SetProgressState(m_pANORRLMainWindow->winId(),TBPF_INDETERMINATE);
    }
    else
    {
        m_isIndeterminateProgress = false;
        m_taskbar->SetProgressState(m_pANORRLMainWindow->winId(),TBPF_NORMAL);

        m_totalProgress = max - min;    // convert to 0 to max
        m_taskbar->SetProgressValue(m_pANORRLMainWindow->winId(),0,m_totalProgress);
    }
}
#endif // WINVER

/**
 * Change the cursor to the wait cursor and shut off some operations while busy.
 *  Only marked unbusy if busy state is 0.  Each setBusy(true) must be followed by a
 *  setBusy(false) so the busy state will reach 0.
 */
void UpdateUIManager::setBusy(bool busy, bool useWaitCursor)
{
    m_BusyState += busy ? 1 : -1;

    m_pANORRLMainWindow->setUpdatesEnabled(false);

    if ( m_BusyState == 1 && busy )
    {
        // only go into busy state 1 for the first time.
        // if we go to 2+ then back to 1, we don't want to redo this code

        if (useWaitCursor)
            QApplication::setOverrideCursor(Qt::WaitCursor);

        pauseStatusBarUpdate();

        //save currently focused widget
        m_BusyFocusWidget = QApplication::focusWidget();

        // disable any menu updates
		UpdateUIManager::Instance().setMenubarEnabled(false);

        // disable widgets
        m_pANORRLMainWindow->enableScriptCommandInput(false);
        m_pANORRLMainWindow->centralWidget()->setEnabled(false);

        // disable all actions
        m_BusyEnabledActions.clear();
        QList<QAction*> actions = m_pANORRLMainWindow->findChildren<QAction*>();
        QList<QAction*>::iterator iter = actions.begin();
        while ( iter != actions.end() )
        {
            QAction* action = *iter;

			// only worry about actions with names
            if ( !action->objectName().isEmpty() )
            {
                m_BusyEnabledActions[action] = action->isEnabled();
                action->setEnabled(false);
            }

            ++iter;
        }

        // disable the dock widgets
        for ( int i = 0 ; i < eDW_MAX ; ++i )
	    {
		    const EDockWindowID id = (EDockWindowID)i;
            m_DockWidgets[id]->setEnabled(false);
        }

#if WINVER >= 0x601
        // change task bar overlay icon to a busy icon
        if ( m_isTaskBarInitialized )
        {
            QIcon icon = m_pANORRLMainWindow->style()->standardIcon(QStyle::SP_MessageBoxWarning);

            HICON overlay_icon = icon.isNull() ? NULL : icon.pixmap(48).toWinHICON();
            m_taskbar->SetOverlayIcon(m_pANORRLMainWindow->winId(),overlay_icon,L"Busy");

            if ( overlay_icon )
                ::DestroyIcon(overlay_icon);
        }
#endif
    }
    else if ( m_BusyState == 0 )
    {
        if (useWaitCursor)
            QApplication::restoreOverrideCursor();
        
        resumeStatusBarUpdate();

        // enable menu updates
		UpdateUIManager::Instance().setMenubarEnabled(true);

        // enable widgets
        m_pANORRLMainWindow->enableScriptCommandInput(true);
        m_pANORRLMainWindow->centralWidget()->setEnabled(true);
        
        // restore focus if we can, otherwise set to main window
        //  this fixes some focus issues on mac
        m_pANORRLMainWindow->centralWidget()->setFocus();
        if ( !m_BusyFocusWidget.isNull() )
            m_BusyFocusWidget->setFocus();
        else if ( QApplication::focusWidget() )
            QApplication::focusWidget()->setFocus();
        else
            m_pANORRLMainWindow->setFocus();
        m_BusyFocusWidget = NULL;
        
        // enable all main actions
        QList<QAction*> actions = m_BusyEnabledActions.keys();
        QList<QAction*>::iterator iter = actions.begin();
        while ( iter != actions.end() )
        {
            QAction* action = *iter;
            action->setEnabled(m_BusyEnabledActions[action]);
            ++iter;
        }
        m_BusyEnabledActions.clear();

        // enable the dock widgets
        for ( int i = 0 ; i < eDW_MAX ; ++i )
	    {
		    const EDockWindowID id = (EDockWindowID)i;
            m_DockWidgets[id]->setEnabled(true);
        }

#if WINVER >= 0x601
        // restore task bar overlay icon
        if ( m_isTaskBarInitialized )
            m_taskbar->SetOverlayIcon(m_pANORRLMainWindow->winId(),NULL,L"");
#endif
    }

    m_pANORRLMainWindow->setUpdatesEnabled(true);
}

/**
 * Resumes the status bar updates.
 *  The pause state is decremented.  Only at 0 will the status bar be updated.
 */
void UpdateUIManager::resumeStatusBarUpdate()
{
	--m_PauseStatusBar;
	if (m_isRunning && !m_PauseStatusBar)
		QTimer::singleShot(200, this, SLOT(updateStatusBar()));
}

/**
 * Creates a new progress bar and adds it to the status bar.
 *  If min == max then the progress bar is set to indeterminate.
 *  Synchronizes the task bar progress.
 *
 * @param   message     text message displayed in a label to the left
 * @param   min         minimum progress bar value (start)
 * @param   max         maximum progress bar value (end)
 */
QProgressBar* UpdateUIManager::showProgress(const QString& message,int min,int max)
{
    QStatusBar* statusBar = m_pANORRLMainWindow->statusBar();

    // configure progress bar widget
    QProgressBar* progressBar = new QProgressBar(statusBar);
	progressBar->setTextVisible(true);
    progressBar->setMinimum(min);
    progressBar->setMaximum(max);
    progressBar->setValue(min);

    // configure static label
    QLabel* label = new QLabel(message,progressBar); 

    statusBar->setUpdatesEnabled(false);
    statusBar->clearMessage();
    statusBar->addWidget(label,0);
    statusBar->addWidget(progressBar,1);
    statusBar->setUpdatesEnabled(true);

    // set up indeterminate if necessary
    if ( min == max )
        progressBar->setValue(-1);
    
    m_ProgressBars.append(progressBar);
    m_ProgressLabels.append(label);

    // make sure they get rendered at least once
    progressBar->show();
    label->show();

#if WINVER >= 0x601
    // sync task bar progress with progress bar
    configureTaskBar(progressBar);
#endif

    return progressBar;
}

/**
 * Deletes the progress bar.
 */
void UpdateUIManager::hideProgress(QProgressBar* progressBar)
{
    ARLASSERT(m_ProgressBars.contains(progressBar));
    int index = m_ProgressBars.indexOf(progressBar);
    if ( index != -1 )
    {
        delete m_ProgressBars[index];
        m_ProgressBars.remove(index);
        
        delete m_ProgressLabels[index];
        m_ProgressLabels.remove(index);        
    }

#if WINVER >= 0x601
    if ( m_isTaskBarInitialized )
    {
        // reset the "main" progress bar used to synchronize the task bar
        if ( progressBar == m_taskProgressBar )
        {
            configureTaskBar(NULL);
            if ( !m_ProgressBars.empty() )
                configureTaskBar(m_ProgressBars.back());
        }        
    }
#endif
}

/**
 * Updates the progress bar with a new value.
 *  If the value is -1 and the progress bar is indeterminate, update
 *  the indeterminate value.
 *  Synchronizes the task bar if this progress bar is the "main" one.
 */
void UpdateUIManager::updateProgress(QProgressBar* progressBar,int value)
{
	ARLASSERT(progressBar);

    // check for indeterminate and update
    if ( value == -1 )
        value = progressBar->value() + 1;

    progressBar->setValue(value);
    progressBar->update();
    QApplication::processEvents();

#if WINVER >= 0x601
    if ( progressBar == m_taskProgressBar )
    {
        // synchronize the task bar progress
        if ( m_isTaskBarInitialized && !m_isIndeterminateProgress )
            m_taskbar->SetProgressValue(m_pANORRLMainWindow->winId(),value,m_totalProgress);
    }
#endif
}

/**
 * "Blocks" the main thread while a child thread is processed.
 *  The main thread can still handles events and GUI updates.  This allows it to be
 *  responsive and update the progress bar.  The application can set the state
 *  as "busy" to prevent reentrancy.  Long running processes general lock resources and
 *  reentrancy will cause a deadlock.
 * 
 * @param   message         message to display for the progress bar
 * @param   userWorkFunc    long running function to run in a child thread
 */
void UpdateUIManager::waitForLongProcess(
    const QString&          message,
    boost::function<void()> userWorkFunc )
{
    QEventLoop eventLoop;
    QMutex eventLoopMutex;
    
    volatile bool done = false;

	m_longProcessInProgress = true;

    
        new boost::thread(
            boost::bind(
                &UpdateUIManager::exectueProgressFunction,
                this,
                userWorkFunc,
                &eventLoop,
                &eventLoopMutex,
                (bool*)&done ) );

    // wait for approx 300 ms, check if done and abort
    //  this avoids needing to show the progress and setting busy if the operation is fast
     for ( int i = 0 ; i < 30 && !done ; i++ )
		 QtUtilities::sleep(10);

    if ( !done )
    {
        setBusy(true);
        QProgressBar* progressBar = showProgress(message);

        // if still not done, then start the event loop
        if ( !done )
        {
            QMutexLocker locker(&eventLoopMutex);
            if ( !done )
                eventLoop.exec();
        }

        hideProgress(progressBar);
        setBusy(false);
    }
	m_longProcessInProgress = false;
}

/**
 * Thread main for a progress function.
 *  Unblocks the main thread when done.
 */
void UpdateUIManager::exectueProgressFunction(
    boost::function<void()> userWorkFunc,
    QEventLoop*             eventLoop,
    QMutex*                 eventLoopMutex,
    bool*                   done )
{
    ARLASSERT(eventLoop);
    ARLASSERT(eventLoopMutex);
    ARLASSERT(done);

    try 
	{
		userWorkFunc();
	}
	catch(...)
	{
        ARLASSERT(false);
	}

    // check to see if the event loop is running
    if ( eventLoopMutex->tryLock() )
    {
        // abort starting the event loop completely
        *done = true;
        eventLoopMutex->unlock();
    }
    else
    {
        // we're already executing or starting to execute, can't set done now

        // wait until we really are executing
        while ( !eventLoop->isRunning() )
            QtUtilities::sleep(10);
        
        // now shut it down since we know its running
        eventLoop->exit();
    }
}

void UpdateUIManager::commonContextMenuActions(QList<QAction *> &commonActions, bool insertIntoPasteMode)
{
	commonActions.append(m_pANORRLMainWindow->cutAction);
	commonActions.append(m_pANORRLMainWindow->copyAction);
	commonActions.append(insertIntoPasteMode ? m_pANORRLMainWindow->pasteIntoAction : m_pANORRLMainWindow->pasteAction);
	commonActions.append(m_pANORRLMainWindow->duplicateSelectionAction);
	commonActions.append(m_pANORRLMainWindow->deleteSelectedAction);
	commonActions.append(m_pANORRLMainWindow->renameObjectAction);
	commonActions.append(NULL);
	commonActions.append(m_pANORRLMainWindow->groupSelectionAction);
	commonActions.append(m_pANORRLMainWindow->ungroupSelectionAction);
	commonActions.append(m_pANORRLMainWindow->selectChildrenAction);
	commonActions.append(m_pANORRLMainWindow->zoomExtentsAction);

	commonActions.append(NULL);
	commonActions.append(m_pANORRLMainWindow->insertIntoFileAction);

	commonActions.append(NULL);
	commonActions.append(m_pANORRLMainWindow->selectionSaveToFileAction);
	commonActions.append(m_pANORRLMainWindow->saveToANORRLAction);
	commonActions.append(m_pANORRLMainWindow->createNewLinkedSourceAction);

	commonActions.append(m_pANORRLMainWindow->publishAsPluginAction);
	commonActions.append(m_pANORRLMainWindow->exportSelectionAction);
}

void UpdateUIManager::updateStatusBar()
{
    if (!AuthoringSettings::singleton().diagnosticsBarEnabled)
    {
        QString empty;
        m_pTaskSchedulerSBLabel->setText(empty);
        m_pCPUSBLabel->setText(empty);
        m_pRAMSBLabel->setText(empty);
        m_pTimeSBLabel->setText(empty);
        m_pFPSSBLabel->setText(empty);
        return;
    }

	const ARL::TaskScheduler& ts(ARL::TaskScheduler::singleton());
	QString str;
	str.sprintf(
		"Sleep: %.1f Wait: %.1f Run: %.2f Affinity: %d%% Scheduler: %.0f/s %d%% SortFreq: %.0f/s ErrorCalc: %.0f/s", 
		ts.numSleepingJobs(), 
		ts.numWaitingJobs(), 
		ts.numRunningJobs(), 
		(int)(100.0 * ts.threadAffinity()),
		ts.schedulerRate(),
		(int)(100.0 * ts.getSchedulerDutyCyclePerThread()),
        ts.getSortFrequency(),
        ts.getErrorCalculationRate());

	m_pTaskSchedulerSBLabel->setText(str);

	// CPU label
	double cores = ARL::DebugSettings::singleton().processCores();
	QString coresStr;
	coresStr.sprintf("Cores: %.1g", cores);
	m_pCPUSBLabel->setText(coresStr);

	// RAM Label
	int mem = ARL::DebugSettings::singleton().privateWorkingSetBytes();
	m_pRAMSBLabel->setText(ARL::Log::formatMem(mem).c_str());

	IANORRLDoc *pDoc = ANORRLDocManager::Instance().getCurrentDoc();
	if (pDoc)
	{
		pDoc->handleAction("actionUpdateStatusBar");
	}

	if (m_isRunning && !m_PauseStatusBar)
		QTimer::singleShot(200, this, SLOT(updateStatusBar()));
}

void UpdateUIManager::setStatusLabels(QString sTimeLabel, QString sFpsLabel)
{
	m_pTimeSBLabel->setText(sTimeLabel);
	m_pFPSSBLabel->setText(sFpsLabel);
}

void UpdateUIManager::onQuickInsertFocus()
{
    ANORRLIDEDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc();
    if (!playDoc)
        return;

	getViewWidget<InsertObjectWidget>(eDW_BASIC_OBJECTS).startingQuickInsert(QApplication::focusWidget(),
		!m_DockWidgets[eDW_BASIC_OBJECTS]->isVisible());

    setDockVisibility(eDW_BASIC_OBJECTS, true);

    m_DockWidgets[eDW_BASIC_OBJECTS]->raise();

    m_ViewWidgets[eDW_BASIC_OBJECTS]->activateWindow();
    m_ViewWidgets[eDW_BASIC_OBJECTS]->setFocus(Qt::ShortcutFocusReason);
}

void UpdateUIManager::filterExplorer()
{
	m_ViewWidgets[eDW_OBJECT_EXPLORER]->setFocus(Qt::ShortcutFocusReason);
}

bool UpdateUIManager::eventFilter(QObject* watched, QEvent* evt)
{
	// Capture dock widget closing and set checked to false
    if((evt->type() == QEvent::Show || evt->type() == QEvent::Hide) && watched->inherits("QDockWidget"))
	{
		QString dockName = watched->objectName();
		for (int i = 0; i < eDW_MAX; i++)
		{
			EDockWindowID id = (EDockWindowID)i;
			if (m_DockWidgets[id]->objectName() == dockName)
                m_DockData[id].UpdateAction->setChecked(m_DockWidgets[id]->isVisible());
		}
	}

	return false;
}

QAction* UpdateUIManager::getAction(const QString &actionName)
{
	QList<QAction*> allActions = m_pANORRLMainWindow->findChildren<QAction*>(actionName);
	if (allActions.size())
		return allActions.at(0);
	return NULL;
}

QList<QString> UpdateUIManager::getDockActionNames() const
{
	return m_actionDockMap.keys();
}

QAction* UpdateUIManager::getDockAction(const EDockWindowID id) const
{
	return m_DockData[id].UpdateAction;
}

void UpdateUIManager::updateBuildMode()
{
    eBuildMode buildMode = m_pANORRLMainWindow->getBuildMode();

    m_pANORRLMainWindow->setUpdatesEnabled(false);

    ANORRLIDEDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc();
    if ( playDoc )
    {
        playDoc->enableUndo(buildMode != BM_BASIC);

		if ( buildMode == BM_BASIC )
		    ANORRLDocManager::Instance().setPlayDocCentralWidget();
        else
            ANORRLDocManager::Instance().restoreCentralWidget();
    }

    if ( buildMode == BM_BASIC )
    {
        // hide the dock widgets
        for ( int i = 0 ; i < eDW_MAX ; ++i )
	    {
		    const EDockWindowID id = (EDockWindowID)i;
            setDockVisibility(id,false);
        }

        // hide the toolbars
        QObjectList objects = m_pANORRLMainWindow->children();
        QObjectList::iterator iter = objects.begin();
        for ( ; iter != objects.end() ; ++iter )
        {
            QToolBar* toolbar = dynamic_cast<QToolBar*>(*iter);
            if ( toolbar )
                toolbar->setVisible(false);
        }

		// hide menu and status
        m_pANORRLMainWindow->menuBar()->setVisible(false);
        m_pANORRLMainWindow->statusBar()->setVisible(false);
		m_pANORRLMainWindow->setWindowState(Qt::WindowFullScreen);
    }
    else if ( buildMode == BM_ADVANCED )
    {
		// show menu and status
        if (!m_pANORRLMainWindow->isRibbonStyle())
		{
			m_pANORRLMainWindow->menuBar()->setVisible(true);
			m_pANORRLMainWindow->statusBar()->setVisible(true);
		}

        // reload layout - this will restore all docks and toolbars to previous state
		//m_pANORRLMainWindow->loadApplicationStates();
		//m_pANORRLMainWindow->show();
		
        // show the dock view widgets
        for ( int i = 0 ; i < eDW_MAX ; ++i )
	    {
		    const EDockWindowID id = (EDockWindowID)i;
            if ( m_DockWidgets[id]->isVisible() && m_ViewWidgets[id])
                m_ViewWidgets[id]->setVisible(true);
        }
    }

	updateDockActionsCheckedStates();
    m_pANORRLMainWindow->updateWindowTitle();

	ARL::MouseCommand::enableAdvArrowTool(buildMode != BM_BASIC);

	ARL::GameBasicSettings::singleton().setStudioMode(buildMode != BM_BASIC);
	ARL::GameBasicSettings::singleton().setFullScreen(buildMode == BM_BASIC);

    m_pANORRLMainWindow->setUpdatesEnabled(true);
}

void UpdateUIManager::updateDockActionsCheckedStates()
{
	// Sets the actions to be checked if the dock widget is opened.
	for ( int i = 0 ; i < eDW_MAX ; ++i )
	{
		EDockWindowID id = (EDockWindowID)i;
		m_DockData[id].UpdateAction->setChecked(m_DockWidgets[id]->isVisible());
	}
}

bool UpdateUIManager::get3DAxisEnabled()
{
	ANORRLSettings settings; 
	return settings.value("View_AxisWidget", false).toBool();
}

void UpdateUIManager::set3DAxisEnabled( bool value )
{
	ANORRLSettings settings; 
	return settings.setValue("View_AxisWidget", value);
}

bool UpdateUIManager::get3DGridEnabled()
{
	ANORRLSettings settings; 
	return settings.value("View_ZeroPlaneGrid", false).toBool();
}

void UpdateUIManager::set3DGridEnabled( bool value )
{
	ANORRLSettings settings; 
	return settings.setValue("View_ZeroPlaneGrid", value);
}

QDockWidget* UpdateUIManager::getDockWidget( EDockWindowID dockId )
{
	return m_DockWidgets[dockId];
}

bool UpdateUIManager::isLongProcessInProgress()
{
    return m_longProcessInProgress;
}

void UpdateUIManager::showErrorMessage(QString title, QString message)
{
    QMessageBox::critical(&getMainWindow(), title, message);
}

void UpdateUIManager::configureDockWidget(QDockWidget* dockWidget)
{
	if(!m_pANORRLMainWindow->restoreDockWidget(dockWidget))
		m_pANORRLMainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

	if (!m_pANORRLMainWindow->isRibbonStyle())
	{
		m_pANORRLMainWindow->viewToolsToolBar->addAction(dockWidget->toggleViewAction());
	}
	else
	{
		if (RibbonPage* pPage = m_pANORRLMainWindow->ribbonBar()->findChild<RibbonPage*>("View"))
		{
			if (RibbonGroup* pGroup = pPage->findChild<RibbonGroup*>("Show"))
				pGroup->addAction(dockWidget->toggleViewAction(), Qt::ToolButtonTextBesideIcon);
		}
	}

	dockWidget->toggleViewAction()->setIconVisibleInMenu(false);
	QObject::disconnect(dockWidget->toggleViewAction(), SIGNAL(triggered(bool)), dockWidget, SLOT(_q_toggleView(bool)));
	QObject::connect(dockWidget->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(updateActionWidgetVisibility()));
}

void UpdateUIManager::updateActionWidgetVisibility()
{
	QAction* pAction = qobject_cast<QAction*>(sender());
	if (!pAction || !pAction->parentWidget())
		return;

	pAction->parentWidget()->setVisible(pAction->isChecked());
	if (pAction->isChecked())
		pAction->parentWidget()->raise();
}