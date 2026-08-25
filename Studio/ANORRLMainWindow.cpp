/**
 * ANORRLMainWindow.cpp
 * Copyright (c) 2013 ROBLOX Corp. All rights reserved.
 */

#include "stdafx.h"
#include "ANORRLMainWindow.h"

// Qt Headers
#include <QNetworkProxyFactory>
#include <QFileOpenEvent>
#include <QDateTime>
#include <QScrollBar>
#include <QFileInfo>
#include <QDesktopServices>
#include <QDir>
#include <QComboBox>
#include <QToolTip>
#include <QFileDialog>
#include <QLineEdit>
#include <QLabel>
#include <QToolBar>
#include <QToolButton>
#include <QCompleter>
#include <QWidgetAction>
#include <QProcess>
#include <QSignalMapper>
#include <QSplashScreen>
#include <QSharedMemory>
#include <QShortcut>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPageSetupDialog>
#include <QResource>
#include <QDesktopWidget>
#include <QNetworkProxy>
#include <QtConcurrentRun>
// 3rd Party Headers
#include "boost/filesystem/path.hpp"
#include "boost/iostreams/stream.hpp"
#include "boost/iostreams/device/array.hpp"

// ANORRL Headers
#include "util/standardout.h"
#include "util/ScopedAssign.h"
#include "util/RbxStringTable.h"
#include "util/SoundService.h"
#include "script/ScriptContext.h"
#include "v8datamodel/DebugSettings.h"
#include "v8datamodel/PartInstance.h"	
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/Explosion.h"
#include "v8datamodel/PluginManager.h"
#include "v8datamodel/GameBasicSettings.h"
#include "v8datamodel/FastLogSettings.h"
#include "v8datamodel/Stats.h"
#include "v8world/Contact.h"
#include "v8kernel/Body.h"
#include "v8kernel/ContactConnector.h"
#include "arl/BaldPtr.h"
#include "arl/CEvent.h"
#include "Network/Players.h"
#include "CountersClient.h"
#include "FastLog.h"
#include "SharedLauncher.h"
#include "ANORRLStudioVersion.h"
#include "ANORRLQuickAccessConfig.h"
#include "RenderSettingsItem.h"
#include "ANORRLServicesTools.h"

#include "v8datamodel/CSGMesh.h"
#include "../CSG/CSGKernel.h"

#ifdef _WIN32
	#include "VersionInfo.h"
	#include "WinInet.h"
    #include <atlsync.h>
#else
    #include "BreakpadCrashReporter.h"
    #include <semaphore.h>
#endif

// ANORRL Studio Headers
#include "FunctionMarshaller.h"
#include "ANORRLDocManager.h" 
#include "ANORRLCustomWidgets.h"
#include "ANORRL.h"
#include "ANORRLSettings.h"
#include "ANORRLSettingsDialog.h"
#include "ANORRLDiagnosticsView.h"
#include "ANORRLScriptReview.h"
#include "ANORRLTaskScheduler.h"
#include "CommonInsertWidget.h"
#include "ANORRLToolBox.h"
#include "StudioUtilities.h"
#include "ANORRLTextOutputWidget.h"
#include "QtUtilities.h"
#include "AuthoringSettings.h"
#include "UpdateUIManager.h"
#include "ANORRLPluginHost.h"
#include "WebDialog.h"
#include "ANORRLApplicationManager.h"
#include "AuthenticationHelper.h"
#include "ANORRLIDEDoc.h"
#include "ANORRLInputConfigDialog.h"
#include "ANORRLKeyboardConfig.h"
#include "ANORRLMouseConfig.h"
#include "ANORRLStudioVerbs.h"
#include "ANORRLScriptDoc.h"
#include "ShortcutHelpDialog.h"
#include "ANORRLUser.h"
#include "AutoSaveDialog.h"
#include "InsertServiceDialog.h"
#include "StudioIntellesense.h"
#include "ANORRLWebDoc.h"
#include "ARLWorkspace.h"
#include "FindDialog.h"
#include "ScriptTextEditor.h"

#include "StudioDeviceEmulator.h"

#include "DiscordHandler.h"

FASTFLAGVARIABLE(StudioCheckForUpgradeEnabled, false)
FASTFLAGVARIABLE(StudioSeparateActionByActivationMethod, false)
FASTFLAGVARIABLE(StudioFixMacStartPage, false)
FASTFLAGVARIABLE(StudioSplashScreenUntilOpen, true)
FASTFLAGVARIABLE(StudioEarlyCookieConstraintCheckGlobal, false)
FASTSTRINGVARIABLE(StudioCookieConstraintUrlFragment, "fulfillconstraint")

FASTINTVARIABLE(StudioANORRLAnalyticsLoad, 100);
FASTINTVARIABLE(StudioBootstrapperVersionNumber, 52886)
FASTINTVARIABLE(StudioInsertDeletionCheckTimeMS, 8000)

FASTFLAGVARIABLE(StudioCustomStatsEnabled, false)
FASTFLAGVARIABLE(TeamCreateEnableDownloadLocalCopy, true)
FASTFLAGVARIABLE(StudioDoublingOnUploadFixEnabled, true)

FASTFLAG(StudioInSyncWebKitAuthentication)
FASTFLAG(StudioDataModelIsStudioFix)
FASTFLAG(StudioShowTutorialsByDefault)
FASTFLAG(DontSwallowInputForStudioShortcuts)
FASTFLAG(UseBuildGenericGameUrl)

FASTINT(StudioWebDialogMinimumWidth)
FASTINT(StudioWebDialogMinimumHeight)

LOGGROUP(Network)

FASTFLAGVARIABLE(Dep, true)

static const int MaxLengthFilenameMRU   = 64;                    //!< maximum length of a filename in the MRU list
static const char* sWindow_Title        = "ANORRL Studio";  //!< default main window title when no place selected
static const char* sDialog_Title        = "ANORRL Studio";       //!< default child dialog title
static const int SplashTotalTime        = 3505;                  //!< minimum time to display splashscreen

bool ANORRLMainWindow::sIsAppRunning = false;
const QString ANORRLMainWindow::NEW_PLACE_FILENAME = "ANORRL_NEW_PLACE";

bool ANORRLMainWindow::preventActionCounterSending = false;

QString ANORRLMainWindow::sWindowStateKey = "window_state";
QString ANORRLMainWindow::sGeometryKey = "window_geometry";

#ifdef _WIN32
	#include "LogManager.h"
	#include "DumpErrorUploader.h"

	static MainLogManager mainLogManager("ANORRLStudio", ".ANORRLStudio.dmp", ".ANORRLStudio.crashevent");
	static boost::scoped_ptr<DumpErrorUploader> dumpErrorUploader;
#endif

#ifdef __APPLE__
	#include "LogProvider.h"
	static LogProvider logProvider;

    #include "StudioMacUtilities.h"
#endif

ANORRLMainWindow* ANORRLMainWindow::get(QObject* context)
{
	while(context != NULL && dynamic_cast<ANORRLMainWindow*>(context) == NULL)
	{
		context = context->parent();
	}

	return static_cast<ANORRLMainWindow*>(context);
}

ANORRLMainWindow::ANORRLMainWindow(const QMap<QString, QString> argMap)
: ANORRLRibbonMainWindow(this)
, m_pSettingsDialog(NULL)
, m_pMinutesPlayedTimer(NULL)
, m_pInsertServiceDlg(NULL)
, m_pShortcutHelpDialog(NULL)
, m_pInputConfigDialog(NULL)
, m_pPluginHost(new ANORRLPluginHost(this))
, m_splashScreen(NULL)
, m_publishGameDialog(NULL)
, m_pCounterSenderTimer(NULL)
, m_AutoSaveAccum(0)
, m_IsInitialized(false)
, m_pPublishedProjectsWebDialog(NULL)
, m_isRibbon(false)
, m_fileOpenHandled(false)
, m_BuildMode(BM_ADVANCED)
, m_IgnoreCloudEditDisconnect(false)
, m_editScriptActions("EditScript", &studioAnalytics)
, m_mouseActions("ManipulatePart", &studioAnalytics)
, m_inserts("Inserts", &studioAnalytics)
{
	try
	{
		QNetworkProxyFactory::setUseSystemConfiguration(true);

		// grab all of our known args from the map (to be used later)
		parseCommandLineOptions(argMap);

        QTime splashTime = QTime::currentTime();
        if ( showEventArg.isEmpty() )
        {
            // can't set window flags in constructor because other ones are appended that we don't want

			srand(time(NULL)); // call just once or explode or something

			int numberOfSplashes = 60;

			int randSplashNumber = rand() % numberOfSplashes + 1;
			if (randSplashNumber <= 0) {
				randSplashNumber = 1;
			}
			if (randSplashNumber > numberOfSplashes) {
				randSplashNumber = numberOfSplashes;
			}

			std::string path = ARL::format(":/images/ANORRLStudioSplash-%s.png", std::to_string(randSplashNumber).c_str());

            m_splashScreen = new QSplashScreen(this,QPixmap(path.c_str()));

#ifdef Q_OS_WIN32
            Qt::WindowFlags flags = Qt::SplashScreen | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint;
#else
            // Qt::Tool makes the window on top of the Z order on Mac
            Qt::WindowFlags flags = Qt::Tool | Qt::FramelessWindowHint;
#endif
            // now we can set window flags
            m_splashScreen->setWindowFlags(flags);  

            m_splashScreen->raise();
            m_splashScreen->show();
		}

		//start the bootstrapper on Mac, start the Event on Windows
		checkUpdater(true,startEventArg);
		        
		SetBaseURL(ANORRLSettings::getBaseURL().toStdString());
		
		// make sure we call this first, this will initialize boost related stuff also 
		// or else we can run into raise condition which doing HTTP init as being mentioned here
		// - https://svn.boost.org/trac/boost/ticket/6320
		boost::filesystem::path::codecvt();

		ARL::Game::globalInit(true);

		// get logging going quickly, so any output is recorded
		setupLogging();
        ARL::Log::current()->writeEntry(ARL::Log::Information,"ANORRLMainWindow::ANORRLMainWindow - start");

		ARL::HttpFuture settingsFuture = FetchClientSettingsDataAsync(CLIENT_APP_SETTINGS_STRING, CLIENT_SETTINGS_API_KEY);
		try 
		{
			LoadClientSettingsFromString(CLIENT_APP_SETTINGS_STRING, settingsFuture.get(), &ARL::ClientAppSettings::singleton());
		} 
		catch(std::exception&)
		{
			FASTLOG(FLog::Error, "Failed to load settings");
		}

		AuthenticationHelper::Instance().authenticateUserAsync(urlArg, ticketArg);

        //set up ui using the .ui file
        setupUi(this);

		// set up external analytics reporting variables
		QString country = QLocale::countryToString(QLocale::system().country());
#ifdef _WIN32
		ARL::Analytics::setReporter("PC Studio");
#else
		ARL::Analytics::setReporter("Mac Studio");
#endif
		ARL::Analytics::setLocation(country.toStdString());
		ARL::Analytics::setAppVersion(ANORRLSettings::getVersionString().toStdString());

		// set up influxdb reporting
		ARL::Analytics::InfluxDb::init();

        //initializing textOutputWidget early to catch any output in globalInit
        m_pTextOutput = new ANORRLTextOutputWidget(dockWidgetContents_2);
		
		ARL::Http anorrlRequest(AuthenticationHelper::getLoggedInUserUrl().toStdString());
		
		QSettings retentionData("ANORRL", "Retention");

		static const char* const kRetentionInstallDateKey = "InstallDate";
		static const char* const kRetentionLastRunDateKey = "LastRunDate";
		QString dateString = QDate::currentDate().toString("yyyyMMdd");
		QString installDate = retentionData.value(kRetentionInstallDateKey, "").toString();

		if (installDate.isEmpty())
		{
			StudioUtilities::setIsFirstTimeOpeningStudio(true);
		}

		// Init our engine - can only use ClientAppSettings AFTER this!!!
		ANORRL::globalInit(urlArg, ticketArg, settingsFuture);
		ARL::Stats::reportGameStatus("AppStarted");	// must be after gloablInit due to http initialization

        Qtitan::RibbonStyle::setStyleVersion(0);
		
		// Disable FRM in studio by default
		CRenderSettingsItem::singleton().setEnableFRM(false);
        
		// Following are the different modes launched from website 
		// (this is required since website conveys 'only' to be launched from Studio but doesn't specify the mode
		// 1) Build mode: BuildArgument == True (default mode set)
		// 2) Edit mode: BuildArgument == True && ScriptArgument has edit.slua
		// 3) Start Page mode: BuildArgument == True && ScriptArgument is empty
		if (StudioUtilities::containsEditScript(argMap[SharedLauncher::ScriptArgument])
			|| ((argMap[SharedLauncher::BuildArgument]=="TRUE") && argMap[SharedLauncher::ScriptArgument].isEmpty()))
		{
			m_BuildMode = BM_ADVANCED;
			StudioUtilities::setAvatarMode(false);
		}
        
		{
#ifdef __APPLE__
			int useCurlPercentage = ARL::ClientAppSettings::singleton().GetValueHttpUseCurlPercentageMacStudio();
#else
			int useCurlPercentage = ARL::ClientAppSettings::singleton().GetValueHttpUseCurlPercentageWinStudio();
#endif
			bool useCurl = rand() % 100 < useCurlPercentage;
#ifdef _WIN32
			{
				INTERNET_PER_CONN_OPTION_LIST    List;
				INTERNET_PER_CONN_OPTION         Option[1];
				unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

				Option[0].dwOption = INTERNET_PER_CONN_FLAGS;

				List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
				List.pszConnection = NULL;
				List.dwOptionCount = 1;
				List.dwOptionError = 0;
				List.pOptions = Option;

				if (InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize))
				{
					if (Option[0].Value.dwValue & PROXY_TYPE_PROXY)
					{
						// As of when this was added, curl could not handle ssl over proxies
						// so fall back to win inet when proxy is detected
						useCurl = false;
					}
				}
			}
#endif
			FASTLOG1(FLog::Network, "Use CURL = %d", useCurl);
			ARL::Http::SetUseCurl(useCurl);
		}

        ARL::Http::useDefaultTimeouts = false;
		
		if(StudioUtilities::isFirstTimeOpeningStudio())
		{
			installDate = dateString;
			retentionData.setValue(kRetentionInstallDateKey, dateString);
		}

		QString lastRun = retentionData.value(kRetentionLastRunDateKey, "").toString();
		if (lastRun != dateString)
		{
			retentionData.setValue(kRetentionLastRunDateKey, dateString);
		}

		// think smart
		QtConcurrent::run(this, &ANORRLMainWindow::checkInternetConnectionSendCounter, anorrlRequest);

		if (AuthoringSettings::singleton().getUIStyle() == AuthoringSettings::Ribbon)
        {
			m_isRibbon = true;
        }

		//check for crash
		ANORRLSettings settings;
		if (ANORRLApplicationManager::instance().getApplicationCount() == 1 && settings.contains("appClosed") && !settings.value("appClosed").toBool())
		{
			sendCounterEvent("QTStudioCrashRecorded");
		}
		settings.setValue("appClosed", false);

#ifdef _WIN32
        if (FFlag::Dep)
        {
            typedef BOOL (WINAPI *SetProcessDEPPolicyPfn)(DWORD);
            SetProcessDEPPolicyPfn pfn= reinterpret_cast<SetProcessDEPPolicyPfn>(GetProcAddress(GetModuleHandle("Kernel32"), "SetProcessDEPPolicy"));
            if (pfn)
            {
                static const DWORD kEnable = 1;
                pfn(kEnable);
            }
        }
#endif

		if (FFlag::StudioCheckForUpgradeEnabled)
		{
			ANORRLSettings settings;
			if(settings.value("studioVersion").toString() != ANORRLSettings::getVersionString())
			{
				settings.setValue("studioVersion", ANORRLSettings::getVersionString());
				sendCounterEvent("StudioUpgradedCounter", false);
			}
		}
		
        // Set the CSGMesh Factory to use sgCore.
        ARL::CSGMeshFactory::set(new ARL::CSGMeshFactorySgCore());

        initializeUI();

		//setup WikiSearch in scripts
		ScriptTextEditor::setupWikiLookup();

		// reset the position of the toolbars. so they are identical to the old studio.
		setToolbarPosition();

		//set up command toolbar
		setupCommandToolBar();

		//create material and color tool button
		setupCustomToolButton();

		//sends out slow counters every X seconds
		setupCounterSender();
		       
		//set default commands
		UpdateUIManager::Instance().setDefaultApplicationState();

		sIsAppRunning = true;

		// Set Security context Identity
		ARL::Security::Impersonator impersonate(ARL::Security::LocalGUI_);

		//setup statusbar and docks
		UpdateUIManager::Instance().init(this);
		
		ANORRLScriptDoc::init(*this);

		QPainter p(this);

		// Set the background to white
		p.fillRect(rect(), Qt::white);
		
		if (isRibbonStyle())
		{
			setupRibbonBar();
		}
		else
		{
			setupViewMenu();
		}

		//set up slots
		setupSlots();
		
		 // set up shortcut key sequences
		assignAccelerators();

		updateRecentFilesUI();

        // Idempotent function - and if there is no open tab, we need to update actions (disable ones that need to be)
		UpdateUIManager::Instance().updateToolBars();

        setBuildMode(m_BuildMode);

		// save current state as default state
		saveDefaultApplicationState();

		// load the application states from memory
		loadApplicationStates();
				
        m_PropertyChangedConnection = AuthoringSettings::singleton().propertyChangedSignal.connect(
            boost::bind(&ANORRLMainWindow::onPropertyChanged,this,_1));
        onPropertyChanged(NULL);
        
#ifdef _WIN32
        if ( showEventArg.isEmpty() ) 
            show();
#else
        show();
        
        // bootstrapper will wait for this event to exit
        sem_t *sem = sem_open("/anorrlStudioStartedEvent", 0);
        if (sem != SEM_FAILED)
        {
            sem_post(sem);
            sem_close(sem);
        }
#endif
		
        // take the args that were passed in and act upon them
		// has to be at the very end of the constructor, or else the menu bar will behave weird.

		handleCommandLineOptions();
    	m_fileOpenHandled = false;

        //Mac will open file through finder event; need to handle open start page this way
        IANORRLDoc* currentPlayDoc = ANORRLDocManager::Instance().getPlayDoc();
        if (!currentPlayDoc)
        {
            // if nothing else opened, open a start web page
            actionStartPage->setChecked(true);
        }

#ifdef _WIN32
        if ( !showEventArg.isEmpty() ) 
        {
            // Application waits for this event before showing to the user
	        ATL::CEvent showEvent;
	        if ( showEvent.Open(EVENT_MODIFY_STATE, FALSE, qPrintable(showEventArg)) )
                ::WaitForSingleObject(showEvent,30 * 1000);
        }

        show();

		if ( !readyEventArg.isEmpty() ) 
		{
			// Bootstrapper will wait for this event, to make sure that app is loaded and ready
			ATL::CEvent readyEvent;
			if ( readyEvent.Open(EVENT_MODIFY_STATE, FALSE, qPrintable(readyEventArg)) )
				::WaitForSingleObject(readyEvent,30 * 1000);
		}
#endif

		// Unfortunately has to be called after show due to windowState not loading until then.
		UpdateUIManager::Instance().updateDockActionsCheckedStates();

		viewCommandBarAction->setChecked(commandToolBar->isVisible());

		//set window layout setting according to application states.
		//it has to be after loadApplicationStates. or else full screen button will not be initialized correctly.
		setWindowLayout();

        if ( AutoSaveDialog::checkForAutoSaveFiles() )
        {
            AutoSaveDialog dialog(this);
            while ( dialog.exec() == QDialog::Rejected )
            {
                /* nothing */
            }
        }

		if ( m_splashScreen )
        {
            // show splash screen for a minimum amount of time
            int waitTime = 0;
            if ( ANORRLApplicationManager::instance().getApplicationCount() == 1 )
                waitTime = qMax(0,SplashTotalTime - splashTime.elapsed());
            QTimer::singleShot(waitTime,this,SLOT(onDeleteSplashScreen()));
		}

        ANORRLKeyboardConfig::singleton().storeDefaults(*this);
        ANORRLKeyboardConfig::singleton().loadKeyboardConfig(*this);
        ANORRLMouseConfig::singleton().loadMouseConfig();

		ARL::DiscordHandler::Initialise("1456345979635892400");

		m_IsInitialized = true;
	}
	catch (std::runtime_error const& exp)
	{
		// must make sure the splash screen is hidden before popping up any errors
        try
        {
            if ( m_splashScreen )
				onDeleteSplashScreen();
        }
        catch (...)
        {
			// ignore errors
        }

		QtUtilities::ARLMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setText(exp.what());
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok); 
		msgBox.exec();
		throw;
	}
    catch (...)
    {
        // must make sure the splash screen is hidden before popping up any errors
        try
        {
            if ( m_splashScreen )
                onDeleteSplashScreen();
        }
        catch (...)
        {
			// ignore errors
        } 
        throw;
    }

    if (FFlag::StudioSplashScreenUntilOpen && m_splashScreen)
    {
        m_splashScreen->raise();
        m_splashScreen->show();
    }

    ARL::Log::current()->writeEntry(ARL::Log::Information,"ANORRLMainWindow::ANORRLMainWindow - end");
}

ANORRLMainWindow::~ANORRLMainWindow()
{
    ARL::Log::current()->writeEntry(ARL::Log::Information,"ANORRLMainWindow::~ANORRLMainWindow");
    StudioUtilities::stopMobileDevelopmentDeployer();

    m_PropertyChangedConnection.disconnect();

    //remove all documents
	ANORRLDocManager::Instance().shutDown();

	//stop UI update
	UpdateUIManager::Instance().shutDown();

	//do a global shut down
	ANORRL::globalShutdown();

	//Set appClosed true before shutdown
	if (ANORRLApplicationManager::instance().getApplicationCount() == 1)
	{
		ANORRLSettings settings;
		settings.setValue("appClosed", true);
	}
}

void ANORRLMainWindow::setupLogging()
{
#ifdef _WIN32
	ARL::Log::setLogProvider(&mainLogManager);
	mainLogManager.WriteCrashDump();

	dumpErrorUploader.reset(new DumpErrorUploader(true, "Studio"));
	std::string dmpHandlerUrl =  GetDmpUrl(::GetBaseURL(), true);
	dumpErrorUploader->InitCrashEvent(dmpHandlerUrl, mainLogManager.getCrashEventName());
	dumpErrorUploader->Upload(dmpHandlerUrl);
#endif

#ifdef __APPLE__
	ARL::Log::setLogProvider(&logProvider);
#endif
}

void ANORRLMainWindow::causeCrash()
{
	ARLCRASH();
}

bool ANORRLMainWindow::checkUpdater(bool showUpdateOptionsDialog, const QString& initDoneEventName) const
{
#ifdef Q_OS_MAC
	QString bootstrapper = QCoreApplication::applicationFilePath(); 
	if(bootstrapper.lastIndexOf("ANORRLStudio") > -1)
		bootstrapper = bootstrapper.replace(bootstrapper.lastIndexOf("ANORRLStudio"), 12, "ANORRLStudio.app/Contents/MacOS/ANORRLStudio");
	else
		return false; //can't find proper path
	
	ARL::StandardOut::singleton()->printf(ARL::MESSAGE_INFO, "boostrapper path: %s", qPrintable(bootstrapper));
	
	QStringList args;
	pid_t processID = getpid();
	QString ppid;
	ppid.setNum(processID);
	QString showUpdate = QString(showUpdateOptionsDialog ? "true" : "false");
	args << "-check" << "true" << "-ppid" << ppid << "-updateUI" << showUpdate;
	QProcess::startDetached(bootstrapper, args);
#else
    if ( !initDoneEventName.isEmpty() )
    {
	    // Bootstrapper will wait for this event, to make sure that app was started
	    ATL::CEvent anorrlStudioStartedEvent;
	    if ( anorrlStudioStartedEvent.Open(EVENT_MODIFY_STATE, FALSE, qPrintable(initDoneEventName)) )
		    anorrlStudioStartedEvent.Set();
    }
#endif
	return true;
}

void ANORRLMainWindow::parseCommandLineOptions(const QMap<QString, QString> argMap)
{
	fileLocationArg	= argMap[SharedLauncher::FileLocationArgument];
	urlArg 			= argMap[SharedLauncher::AuthUrlArgument];
	ticketArg 		= argMap[SharedLauncher::AuthTicketArgument];
    startEventArg   = argMap[SharedLauncher::StartEventArgument];
    readyEventArg   = argMap[SharedLauncher::ReadyEventArgument];
	showEventArg    = argMap[SharedLauncher::ShowEventArgument];
	scriptArg 		= argMap[SharedLauncher::ScriptArgument];
	QString sBrowserTrackerId = argMap[SharedLauncher::BrowserTrackerId];

	QString sWidth	= argMap[StudioUtilities::StudioWidthArgument];
	QString sHeight	= argMap[StudioUtilities::StudioHeightArgument];

	// convert loadfile('http://www.anorrl.com/game/join.ashx')() to just the url
	if (StudioUtilities::containsJoinScript(scriptArg) && scriptArg.contains("loadfile("))
	{
		int urlBegin = scriptArg.indexOf("(")+2; // skip over the qoute
		int urlEnd = scriptArg.indexOf(")")-1;
		scriptArg = scriptArg.mid(urlBegin, urlEnd - urlBegin);
	}

    if ( argMap[SharedLauncher::BuildArgument] == "TRUE" )
        m_BuildMode = BM_BASIC;
    else if ( argMap[SharedLauncher::IDEArgument] == "TRUE" )
        m_BuildMode = BM_ADVANCED;

    if ( argMap[SharedLauncher::TestModeArgument] == "TRUE" )
        StudioUtilities::setTestMode(true);
    if ( argMap[SharedLauncher::AvatarModeArgument] == "TRUE" )
        StudioUtilities::setAvatarMode(true);

	if ( argMap[StudioUtilities::EmulateTouchArgument] == "TRUE" )
		StudioDeviceEmulator::Instance().setIsEmulatingTouch(true);

	if ( !sWidth.isEmpty())
		StudioDeviceEmulator::Instance().setCurrentDeviceWidth(sWidth.toInt());

	if ( !sHeight.isEmpty())
		StudioDeviceEmulator::Instance().setCurrentDeviceHeight(sHeight.toInt());

	if ( !sBrowserTrackerId.isEmpty())
		ARL::Stats::setBrowserTrackerId(sBrowserTrackerId.toStdString());
}

void ANORRLMainWindow::handleCommandLineOptions()
{    
	// make sure sound is disabled if it should be before we create datamodel 
	if (StudioUtilities::isTestMode() && !StudioUtilities::isAvatarMode() && StudioUtilities::containsGameServerScript(scriptArg))
	{
		if (AuthoringSettings::singleton().getTestServerAudioBehavior() == AuthoringSettings::OnlineGame)
		{
			ARL::Soundscape::SoundService::soundDisabled = true;
		}
	}

    // check if fileOpen has already been handled
    if (m_fileOpenHandled)
        return;

    bool openedPlace = handleFileOpen(fileLocationArg,IANORRLDoc::IDE,scriptArg);
    if(!openedPlace)
    {
        // if nothing else opened, open a start web page
        actionStartPage->setChecked(true);
    }
#ifdef __APPLE__
    // if Studio is opening in sever/client mode then diable App Nap on Mac (DE13936)
    else if (StudioUtilities::containsGameServerScript(scriptArg) || StudioUtilities::containsJoinScript(scriptArg))
    {
        StudioMacUtilities::disableAppNap("Server/Client Mode");
    }
#endif

	if (openedPlace && isRibbonStyle() && StudioUtilities::isTestMode() && !StudioUtilities::isAvatarMode() && StudioUtilities::containsGameServerScript(scriptArg))
	{
		// start local server, so we can cleanup processes started by 'server'
		ANORRLApplicationManager::instance().startLocalServer();
		// this is required to update client server related actions
		UpdateUIManager::Instance().updateToolBars();

		ANORRLIDEDoc* pIDEDoc = ANORRLDocManager::Instance().getPlayDoc();

		int numPlayers = ANORRLSettings().value("arlRibbonNumPlayer").toInt();
		if (numPlayers > 0)
		{
			if (pIDEDoc && pIDEDoc->getDataModel())
			{
				StartServerAndPlayerVerb* pVerb = dynamic_cast<StartServerAndPlayerVerb*>(pIDEDoc->getDataModel()->getVerb("StartServerAndPlayerVerb"));
				if (pVerb)
					pVerb->launchPlayers(numPlayers);
			}			
		}
		StudioUtilities::startMobileDevelopmentDeployer();

		if (pIDEDoc && pIDEDoc->getDataModel())
		{
			pIDEDoc->initServerAudioBehavior();
		}
	}

    if (!openedPlace)
    {
        if (m_splashScreen)
            onDeleteSplashScreen();
    }
}

void ANORRLMainWindow::moveEvent(QMoveEvent * event)
{
	ANORRLRibbonMainWindow::moveEvent(event);
	updateEmbeddedFindPosition();
}

bool ANORRLMainWindow::eventFilter(QObject* watched,QEvent* evt)
{
	if (FFlag::StudioSeparateActionByActivationMethod && evt->type() == QEvent::Shortcut)
	{
		if (QAction* action = dynamic_cast<QAction*>(watched))
		{
			if (commonSlotShortcut(action, !action->isChecked()))
			{
				evt->accept();
				return true;
			}
		}
	}

#ifdef Q_OS_MAC
	// handle special file open command on Mac coming from Finder
	if ( evt->type() == QEvent::FileOpen && watched == qApp )
	{
		if (FFlag::StudioFixMacStartPage)
    	{
        	// Make sure the start page is the first thing opened.
        	actionStartPage->setChecked(true);
		}

		m_fileOpenHandled = handleFileOpen(static_cast<QFileOpenEvent*>(evt)->file(), IANORRLDoc::IDE);
		return true;
	}
    else if (isRibbonStyle() && (evt->type() == QEvent::Polish) && watched->inherits("QMenu"))
    {
        static_cast<QMenu*>(watched)->setFont(QApplication::font());
    }
    else
#endif
    if (evt->type() == QEvent::KeyPress || evt->type() == QEvent::ShortcutOverride)
    {
        // ignore shortcut events during busy state
        if ( UpdateUIManager::Instance().isBusy() )
        {
            evt->accept();
            return true;
        }
    }

    if (evt->type() == QEvent::NonClientAreaMouseButtonPress)
        Studio::Intellesense::singleton().deactivate();

 	if (evt->type() == QEvent::MouseMove ||
 		evt->type() == QEvent::KeyPress)
 	{
 		keepAliveAnalyticsSession();
 	}

	if ( (evt->type() == QEvent::FocusIn || evt->type() == QEvent::FocusOut) &&
		 AuthoringSettings::singleton().onlyPlayFocusWindowAudio )
	{
		if (IANORRLDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc())
		{
			if (ANORRLIDEDoc* ide = dynamic_cast<ANORRLIDEDoc*>(playDoc))
			{
				ide->setMuteAllSounds(!QApplication::activeWindow());
				if (QAction* audioToggleWidget = findChild<QAction*>("audioToggleAction"))
				{
					audioToggleWidget->setChecked(!QApplication::activeWindow());
				}
			}
		}
	}
	
	if ( watched == qApp )
		return qApp->eventFilter(watched,evt);
	else
		return QMainWindow::eventFilter(watched,evt);
}

void ANORRLMainWindow::setWindowLayout()
{
	actionFullScreen->setChecked(isFullScreen());
}

void ANORRLMainWindow::setupViewMenu()
{
	menuView->insertAction(menuView->actions().value(1), resetViewAction);
	menuView->insertSeparator(resetViewAction);		

	// Add dock widgets to view menu
    //  Insert after Start Page.  Start Page is first (index 0).
	UpdateUIManager::Instance().setupDockWidgetsViewMenu(*menuView->actions().value(1));    

	// Set up the toolbar
	menuToolBars->addAction(standardToolBar->toggleViewAction());
	menuToolBars->addAction(advToolsToolBar->toggleViewAction());
	menuToolBars->addAction(editCameraToolBar->toggleViewAction());
	menuToolBars->addAction(commandToolBar->toggleViewAction());
	menuToolBars->addAction(runToolBar->toggleViewAction());
	menuToolBars->addAction(viewToolsToolBar->toggleViewAction());
	if (!ARL::MouseCommand::isAdvArrowToolEnabled()) // Remove this, this flag is true
		menuToolBars->addAction(oldToolsToolBar->toggleViewAction());
}

void ANORRLMainWindow::toggleFullScreen(bool state)
{
	if (isFullScreen() == state)
		return;
	
	// Toggle
	setWindowState(windowState() ^ Qt::WindowFullScreen);

    // Toggle fullscreen glitches with layout on Mac for Ribbon Bar (send event to update layout)
#ifdef Q_OS_MAC
	if (isRibbonStyle())
	{
        Qtitan::RibbonPage* pPage = ribbonBar()->getPage(ribbonBar()->currentIndexPage());
        if (pPage)
        {
            QEvent evt(QEvent::LayoutRequest);
            QApplication::sendEvent(pPage, &evt);
        }
    }
#endif

	// if we have an IDE, we need to lock the datamodel
    IANORRLDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc();
    if ( playDoc )
    {
        ANORRLIDEDoc* ide = dynamic_cast<ANORRLIDEDoc*>(playDoc);
        if ( ide )
        {
            ARL::DataModel::LegacyLock lock(ide->getDataModel(),ARL::DataModelJob::Write);
	        ARL::GameBasicSettings::singleton().setFullScreen(state);
            return;
        }
    }

    ARL::GameBasicSettings::singleton().setFullScreen(state);
}

bool ANORRLMainWindow::commonSlotHelper(const QString& objectName, bool isChecked)
{
	bool handled = false;

	//action to be handled by the relevant document
	if (ANORRLDocManager::Instance().getCurrentDoc())
		handled = ANORRLDocManager::Instance().getCurrentDoc()->handleAction(objectName, isChecked);

	// Otherwise handle it ourselves
	if (!handled)
	{

		// Handle global dock actions here
		if (UpdateUIManager::Instance().getDockActionNames().contains(objectName))
		{
			handled = UpdateUIManager::Instance().toggleDockView(objectName);
		}
		else
		{
			// Check if IDE Doc can handle the action
			IANORRLDoc* pPlayDoc = ANORRLDocManager::Instance().getPlayDoc(); 
			if (pPlayDoc)
				handled = pPlayDoc->handleAction(objectName, isChecked);
		}
	}

	//update toolbar status
	UpdateUIManager::Instance().updateToolBars();

	return handled;
}

void ANORRLMainWindow::commonSlot(bool isChecked)
{
	const QObject *pSender = sender();
	if (!pSender || dynamic_cast<QuickAccessBarProxyAction*>(sender()))
		return;

	commonSlotHelper(pSender->objectName(), isChecked);
}

bool ANORRLMainWindow::commonSlotShortcut(QAction* action, bool isChecked)
{
	return commonSlotHelper(action->objectName(), isChecked);
}

bool ANORRLMainWindow::commonSlotQuickAccess(QAction* action)
{
	return commonSlotHelper(action->objectName(), !action->isChecked());
}

void ANORRLMainWindow::processAppEvent(void* pClosure)
{
	// Handle the event
	if (sIsAppRunning)
		ARL::FunctionMarshaller::handleAppEvent(pClosure);
	else	
		ARL::FunctionMarshaller::freeAppEvent(pClosure); // control flow comes here means application is closed
}

bool ANORRLMainWindow::requestDocClose(IANORRLDoc& doc, bool closeIfLastDoc)
{
    if ( !doc.getViewer() )
    {
		ANORRLDocManager::Instance().removeDoc(doc);
        return true;
    }

	if (StudioUtilities::isScreenShotUploading())
	{
		QtUtilities::ARLMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Question);
		msgBox.setText(QString("ANORRL is uploading an image. Quit anyway?"));
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);

		//user has pressed NO or close button, do not close the document
		if ( msgBox.exec() != QMessageBox::Yes )
			return false;	

		// user has pressed YES, modifying the uploading status to make sure 
		//  we don't show the message multiple times
		StudioUtilities::setScreenShotUploading(false);
	}

	if ( doc.isModified() )
	{
		IANORRLDoc::ARLCloseRequest closeMode = doc.requestClose();

		if ( closeMode == IANORRLDoc::CLOSE_CANCELED )
		{
			if ( &doc != ANORRLDocManager::Instance().getCurrentDoc() )
                ANORRLDocManager::Instance().setCurrentDoc(&doc);
			return false;
		}
        else if ( closeMode == IANORRLDoc::REQUEST_HANDLED )
		{
            // LUA will handle it from here on out
			return false;
		}
		else if ( closeMode != IANORRLDoc::NO_SAVE_NEEDED )
		{
            // ask the user if they want to save before close
		
			QtUtilities::ARLMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Question);
			msgBox.setText("Save changes to " + doc.displayName() + " ?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Yes);
			int ret = msgBox.exec();

			//if the user wants to save
			if ( ret == QMessageBox::Yes && !fileSave(&doc) )
            {
				// User hit the Yes to Save in the Yes No Cancel MsgBox,
				// but then did a Cancel on Save Dialog, change the state to Cancel
				ret = QMessageBox::Cancel;
            }
			
			//if the user want to cancel close
			if ( ret == QMessageBox::Cancel )
			{
				if ( &doc != ANORRLDocManager::Instance().getCurrentDoc() )
                    ANORRLDocManager::Instance().setCurrentDoc(&doc);
				return false;
			}
		}
    }

    // special case Start Page since it can be toggled in the View menu
	if ( doc.keyName() == "StartPage" && doc.docType() == IANORRLDoc::BROWSER )
	    actionStartPage->setChecked(false);
	else if ( doc.docType() == IANORRLDoc::OBJECTBROWSER )
	    objectBrowserAction->setChecked(false);
#ifdef Q_OS_MAC
    // On Mac, if we do not make the IDE document current then it results in DE7073
    // Combination of QWidget and ViewBase creation coupled with deletion of IDE document
    // without making it current results in non responding keyboard (?)
    else if ((doc.docType() == IANORRLDoc::IDE) && (&doc != ANORRLDocManager::Instance().getCurrentDoc()))
        ANORRLDocManager::Instance().setCurrentDoc(&doc);
#endif

	// WARNING: Do not use doc to call any function after this (it will get deleted)
    ANORRLDocManager::Instance().removeDoc(doc);

    updateWindowTitle();

	if ( ANORRLDocManager::Instance().getDocCount() == 0 && closeIfLastDoc &&
        (ANORRLApplicationManager::instance().getApplicationCount() > 1 || getBuildMode() == BM_BASIC))
	{
		// If there are no tabs remaining and this is another instance of Studio open, close this one
		close();
	}

    return true;
}

void ANORRLMainWindow::fileNew()
{
	handleFileOpen(NEW_PLACE_FILENAME, IANORRLDoc::IDE);
}

void ANORRLMainWindow::openStartPage(bool checked, QString optionalQueryParams /* = "" */)
{
	if(checked)
	{
		QString fileToOpen;

		const char* startPageUrl = ARL::ClientAppSettings::singleton().GetValueStartPageUrl();
		if (startPageUrl && startPageUrl[0] != '\0')
		{
			// Generate start page url from our recent files -- these files are put into the query string
			fileToOpen = QString::fromStdString(GetBaseURL()).append(startPageUrl).append("?");
			ANORRLSettings settings;
			QStringList recentFiles = settings.value("arlRecentFiles").toStringList();
			for(int i = 0; i < qMin(recentFiles.length(), (int)MAX_RECENT_FILES); i++)
			{
				QFileInfo info(recentFiles.at(i));
				fileToOpen.append("&filepath=" + QUrl::toPercentEncoding(info.filePath()) + "&filename=" + QUrl::toPercentEncoding(info.fileName()));	
			}				
		}
		else
			fileToOpen = QString::fromStdString(GetBaseURL() + "/ide/projects");

		if (!optionalQueryParams.isEmpty())
			fileToOpen.append("&").append(optionalQueryParams);

		// make sure we are authenticated (if there's any authentication happening) before we open the browser
		AuthenticationHelper::Instance().waitForQtWebkitAuthentication();

		handleFileOpen(fileToOpen, IANORRLDoc::BROWSER);
	}
	else
	{
		IANORRLDoc* pDoc = ANORRLDocManager::Instance().getOrCreateDoc(IANORRLDoc::BROWSER);
        requestDocClose(*pDoc, false);
	}
}

void ANORRLMainWindow::fileOpen()
{
    QString fileName;
	
	fileName = QFileDialog::getOpenFileName(
		this, 
		tr("Open ANORRL File"), 
		m_LastDirectory, 
		tr(ANORRLIDEDoc::getOpenFileFilters()));

	if (fileName.isEmpty())
		return;

    setCurrentDirectory(fileName);
	handleFileOpen(fileName, IANORRLDoc::IDE);
}

void ANORRLMainWindow::fileOpenRecentSaves()
{
	QString fileName;
	
	fileName = QFileDialog::getOpenFileName(
		this, 
		tr("Open ANORRL File"), 
		AuthoringSettings::singleton().recentSavesDir.absolutePath(), 
		tr(ANORRLIDEDoc::getOpenFileFilters()));

	if (fileName.isEmpty())
		return;

    setCurrentDirectory(fileName);
	handleFileOpen(fileName, IANORRLDoc::IDE);
}

void ANORRLMainWindow::fileClose()
{
    IANORRLDoc* doc = ANORRLDocManager::Instance().getCurrentDoc();
	if ( doc )
        requestDocClose(*doc);
}

bool ANORRLMainWindow::verifyFilePermissions(const QString &fileName)
{
	bool retVal = false;

	if (!fileName.isEmpty())
	{
		QFileInfo fileInfo(fileName);
		if (fileInfo.exists() && !fileInfo.isWritable()) 
		{
			QtUtilities::ARLMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.setText(fileName + "\nThis file is set to read-only.\nTry again with a different name");
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setDefaultButton(QMessageBox::Ok);
			msgBox.exec();
		}
		else 
		{
			retVal = true;
		}
	}
	else /// In case of Empty File return it as True as the file is not present and new file should be created
	{
		retVal = true;
	}
	return retVal;
}

bool ANORRLMainWindow::fileSave(IANORRLDoc *pDoc)
{
	bool retVal = false;
	
	//if no document is provided then get the play document. 
	if (!pDoc)
		pDoc = ANORRLDocManager::Instance().getPlayDoc();
	
    if (pDoc && pDoc->isModified())
    {
        if (verifyFilePermissions(pDoc->fileName()))
        {
            if ( pDoc->fileName().isEmpty() || !pDoc->save() )
                retVal = fileSaveAs(pDoc);
            else
                retVal = true;
        }
    }
	return retVal;
}

bool ANORRLMainWindow::fileSaveAs(IANORRLDoc *pDoc)
{
	bool retVal = false;
	
	//if no document is provided then get the play document
	if (!pDoc)
		pDoc = ANORRLDocManager::Instance().getPlayDoc();
	
	if (pDoc)
	{
		QString fileName(pDoc->fileName());
		if ( fileName.isEmpty() &&
				boost::filesystem::portable_file_name(pDoc->displayName().toStdString()) )
			fileName = m_LastDirectory + "/" + pDoc->displayName();

		fileName = QFileDialog::getSaveFileName(this,tr("Save As"),fileName,pDoc->saveFileFilters());
			
		if( !fileName.isEmpty() && verifyFilePermissions(fileName) && pDoc->saveAs(fileName))
		{
            setCurrentDirectory(fileName);
            ANORRLDocManager::Instance().setDocTitle(*pDoc,pDoc->displayName(),pDoc->titleTooltip(),pDoc->titleIcon());
            updateRecentFile(fileName);

            m_AutoSaveAccum = 0;
			retVal = true;
		}
	}

	return retVal;
}

bool ANORRLMainWindow::filePublishedProjects()
{
	QString url = QString(ARL::ClientAppSettings::singleton().GetValuePublishedProjectsPageUrl());
	if (url.isEmpty())
		return false;

	if (!StudioUtilities::checkNetworkAndUserAuthentication())
		return false;
	
	WebDialog dlg(
        this, 
        url.prepend(ANORRLSettings::getBaseURL()),
        NULL, 
		ARL::ClientAppSettings::singleton().GetValuePublishedProjectsPageWidth(), 
		ARL::ClientAppSettings::singleton().GetValuePublishedProjectsPageHeight() );
    dlg.setMinimumSize( FInt::StudioWebDialogMinimumWidth, FInt::StudioWebDialogMinimumHeight);
	dlg.exec();
	
	return true;
}

bool ANORRLMainWindow::openRecentFile(const QString &fileName)
{
	if(!fileName.isEmpty())
    {
		QFileInfo fileInfo(fileName);
		if(!fileInfo.exists())
        {
            updateRecentFile(fileName);

            QtUtilities::ARLMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.setText(fileName + "\nThis file doesn't exist.");
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setDefaultButton(QMessageBox::Ok);
	        msgBox.exec();
            return false;
        }
    }

	return handleFileOpen(fileName, IANORRLDoc::IDE);
}

bool ANORRLMainWindow::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QString fileName = action->data().toString();
	if (action)
	{
		return openRecentFile(fileName);
	}
	else
		return false;
}

void ANORRLMainWindow::publishGame()
{
	if (!StudioUtilities::checkNetworkAndUserAuthentication())
		return;

	QString initialUrl = QString("%1/IDE/publishgameas").arg(ANORRLSettings::getBaseURL());

	if (!m_publishGameDialog) 
    {
		m_publishGameDialog = new WebDialog(this, initialUrl, NULL);
        m_publishGameDialog->setMinimumSize( FInt::StudioWebDialogMinimumWidth, FInt::StudioWebDialogMinimumHeight);
    }
	else
	{
		m_publishGameDialog->load(initialUrl);
	}

	m_publishGameDialog->show();
	m_publishGameDialog->raise();
	m_publishGameDialog->activateWindow();
}

void ANORRLMainWindow::closePublishGameWindow()
{
	ARLASSERT(m_publishGameDialog);
	if (m_publishGameDialog)
	{
		m_publishGameDialog->close();
	}
}

void ANORRLMainWindow::updateRecentFilesUI()
{
	if (isRibbonStyle())
	{
		RibbonPageSystemRecentFileList* pRecentFileList = findChild<RibbonPageSystemRecentFileList*>("recentFileList");
		if (pRecentFileList)
			pRecentFileList->updateRecentFileActions(getRecentFiles());
	}
	else
	{
		updateRecentFilesList(getRecentFiles());
	}
}

void ANORRLMainWindow::updateRecentFilesList(const QStringList& files)
{
	int numRecentFiles = qMin(files.size(), (int)MAX_RECENT_FILES_SHOWN);

	for (int i = 0; i < numRecentFiles; ++i) 
    {
        QFileInfo fileInfo(files[i]);
        QString fullFilename = fileInfo.absoluteFilePath();
		QString filename = fullFilename;

        if ( filename.startsWith(m_LastDirectory) )
            filename = filename.right(filename.length() - 1 - m_LastDirectory.length());

		bool isTextModified = false;
		if ( filename.length() > MaxLengthFilenameMRU )
		{
			QFontMetrics fm = fontMetrics();
			filename = fm.elidedText(filename,Qt::ElideMiddle,MaxLengthFilenameMRU * fm.width('0'));
		    isTextModified = true;
		}

		recentOpenedFiles[i]->setText(QString("&%1 %2").arg(i + 1).arg(filename));

		if(isTextModified)
			recentOpenedFiles[i]->setToolTip(fullFilename);

		recentOpenedFiles[i]->setData(fullFilename);
		recentOpenedFiles[i]->setStatusTip(fullFilename);
		recentOpenedFiles[i]->setVisible(true);
	}

	for (int j = numRecentFiles; j < (int)MAX_RECENT_FILES_SHOWN; ++j)
		recentOpenedFiles[j]->setVisible(false);

	separator->setVisible(numRecentFiles > 0);
}

void ANORRLMainWindow::updateInternalWidgetsState(QAction* pAction, bool enabledState,bool checkedState)
{
	if (isRibbonStyle())
		ANORRLRibbonMainWindow::updateInternalWidgetsState(pAction, enabledState, checkedState);
}

#define COPYRIGHT_PREFIX 0xA9

void ANORRLMainWindow::about()
{
	QString aboutMsg(tr("<div style=\"text-align: center\">"));
	aboutMsg.append(tr("<p style=\"font-weight: bold\">"));
    aboutMsg.append(tr("ANORRL Studio "));
	aboutMsg.append(tr("Version %1</p>"));

#if defined(_NOOPT) || defined(_DEBUG) 
    aboutMsg.append(tr("<br><hr><br><br>DEBUG ONLY<br><hr>"));

    QString build_type;

#ifdef _NOOPT
    build_type = "NoOpt";
#else
    build_type = "Debug";
#endif

    aboutMsg.append("Build Type: " + build_type);
    aboutMsg.append("<br>Path: ");

    const int threshold = 50;
    QString full_path = QCoreApplication::applicationFilePath();
    while ( !full_path.isEmpty() )
    {
        aboutMsg.append(full_path.left(threshold) + "<br>");
        if ( full_path.length() >= threshold )
        {
            full_path = full_path.right(full_path.length() - threshold);
            if ( !full_path.isEmpty() )
                aboutMsg.append("&nbsp;&nbsp;&nbsp;&nbsp;");
        }
        else
        {
            full_path = "";
        }
    }

    aboutMsg.append("Qt Version: ");
    aboutMsg.append(qVersion());

	// display command line argument
    aboutMsg.append("<br><br>");
    aboutMsg.append("Arguments:<br>");
    
    QStringList args = QApplication::arguments();
    for ( int i = 1 ; i < args.size() ; ++i )
        aboutMsg.append(QString::number(i) + ") '" + args[i] + "'<br>");

    aboutMsg.append("<hr><br>");
#endif

	aboutMsg = aboutMsg.arg(ANORRLSettings::getVersionString());
	aboutMsg.append(tr("<p>ANORRL and like everything here is made by the community and primarily kuro. "));
	
	QString copyrightString;
	copyrightString.sprintf("%c2026 kuro and others.</p> ", COPYRIGHT_PREFIX);
							
    aboutMsg.append(copyrightString);
					   
	aboutMsg.append(tr("<p>Contact us at info@anorrl.com.</p></div>"));
	
	QMessageBox::about(this, tr("Welcome to ANORRL!"), aboutMsg);
}

void ANORRLMainWindow::onlineHelp()
{
	QUrl helpUrl("http://wiki.anorrl.com/index.php/StudioJABONER", QUrl::TolerantMode);
	QDesktopServices::openUrl(helpUrl);
}

void ANORRLMainWindow::shortcutHelp()
{
    if ( !m_pInputConfigDialog )
        m_pInputConfigDialog = new ANORRLInputConfigDialog(*this);

    if ( m_pInputConfigDialog->isVisible() )
        m_pInputConfigDialog->hide();
    else
    {
        m_pInputConfigDialog->show();
        m_pInputConfigDialog->raise();
    }
}

void ANORRLMainWindow::fastLogDump()
{
	ARL::StandardOut::singleton()->printf(ARL::MESSAGE_INFO, "Log Dumps");
#ifdef _WIN32
	mainLogManager.CreateFakeCrashDump();
#else
	CrashReporter::CreateLogDump();
#endif
}

void ANORRLMainWindow::instanceDump()
{
}

void ANORRLMainWindow::openPluginsFolder()
{	
	QString userPluginPath = ANORRLPluginHost::userPluginPath();
	QDir pluginDir(userPluginPath);

	if (!pluginDir.exists())
		pluginDir.mkpath(userPluginPath);

	QDesktopServices::openUrl(QUrl::fromLocalFile(userPluginPath));
}

void ANORRLMainWindow::managePlugins()
{
	if (!StudioUtilities::checkNetworkAndUserAuthentication())
		return;
	
	if (!m_managePluginsDoc)
	{
		m_managePluginsDoc.reset(new ANORRLWebDoc(tr("Plugin Management"), "PluginManagement"));
		ANORRLWebDoc* webDoc = dynamic_cast<ANORRLWebDoc*>(
			ANORRLDocManager::Instance().getOrCreateDoc(IANORRLDoc::BROWSER));
		connect(webDoc->getWorkspace().get(), SIGNAL(PluginInstallComplete(bool, int)),
				m_managePluginsDoc.get(),       SLOT(          refreshPage()));
	}
	m_managePluginsDoc->open(this,
		QString("%1/studio/plugins/manage").arg(ANORRLSettings::getBaseURL()));
	QWidget *widgetToAddInTab = m_managePluginsDoc->getViewer();
	if (widgetToAddInTab)
	{
        ANORRLDocManager::Instance().configureDocWidget(*m_managePluginsDoc);
	}
}

ANORRLWebDoc* ANORRLMainWindow::getConfigureWebDoc()
{
	if (!m_configureGameDoc)
	{
		m_configureGameDoc.reset(new ANORRLWebDoc(tr("Configure"), "GameEntityConfigure"));
        ANORRLDocManager::Instance().getOrCreateDoc(IANORRLDoc::BROWSER);
	}
	// The widget doesn't show up in the tabs unless open() has been called at least once.
    m_configureGameDoc->open(this, "");
	QWidget *widgetToAddInTab = m_configureGameDoc->getViewer();
	if (widgetToAddInTab)
	{
        ANORRLDocManager::Instance().configureDocWidget(*m_configureGameDoc);
	}

	return m_configureGameDoc.get();
}

void ANORRLMainWindow::closeConfigureWebDoc()
{
	if (m_configureGameDoc)
	{
		requestDocClose(m_configureGameDoc.get());
	}
}

void ANORRLMainWindow::openObjectBrowser(bool checked)
{
	if ( checked )
    {
        handleFileOpen(QString(""), IANORRLDoc::OBJECTBROWSER);
    }
    else
    {
		IANORRLDoc* pDoc = ANORRLDocManager::Instance().getOrCreateDoc(IANORRLDoc::OBJECTBROWSER);
        requestDocClose(*pDoc);
	}
}

void ANORRLMainWindow::openSettingsDialog()
{
	if ( !m_pSettingsDialog )
	    m_pSettingsDialog = new ANORRLSettingsDialog(this);
	m_pSettingsDialog->exec();

    // perform any fixup if settings changed
    assignAccelerators();
}

void ANORRLMainWindow::executeScriptFile()
{
	ARLASSERT(ANORRLDocManager::Instance().getCurrentDoc());
	ARLASSERT(ANORRLDocManager::Instance().getCurrentDoc() == ANORRLDocManager::Instance().getPlayDoc());

	//check is there's no current document then create one
	if (!ANORRLDocManager::Instance().getCurrentDoc())
		fileNew();

	//still no document? can't do anything!
	if (!ANORRLDocManager::Instance().getCurrentDoc())
		return;

	//open the script file
    QString fileName;
	
	fileName = QFileDialog::getOpenFileName(
		this,
		tr("Open Script File"),
		AuthoringSettings::singleton().defaultScriptFileDir.absolutePath(),
		tr("Scripts (*.rbxs *.lua *.txt);;All Files (*.*)"));

	if (fileName.isEmpty())
		return;

    QFileInfo info(fileName);
	AuthoringSettings::singleton().defaultScriptFileDir = info.dir();

	QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

	//read file
	QTextStream inStream(&file);
	QString toExecute = inStream.readAll();
	
	if (!toExecute.isEmpty())
    {
		if (ANORRLIDEDoc* ideDoc = ANORRLDocManager::Instance().getPlayDoc())
			ideDoc->handleScriptCommand(toExecute);
    }
}

void ANORRLMainWindow::onCustomToolButton(const QString& selectedItem)
{
	const QObject *pSender = sender();
	if (!pSender)
		return;

	//action to be handled by the relevant document
	if (!ANORRLDocManager::Instance().getCurrentDoc())
		return;
	
	QString modifiedActionName(pSender->objectName());
	modifiedActionName.append("_"); 
    modifiedActionName.append(selectedItem);

	ANORRLDocManager::Instance().getCurrentDoc()->handleAction(modifiedActionName);

	//update toolbar status
	UpdateUIManager::Instance().updateToolBars();
}

void ANORRLMainWindow::onMenuActionHovered(QAction* action)
{
	if(!action)
		return;

    for (int i = 0; i < MAX_RECENT_FILES_SHOWN; ++i) 
    {
		if(action == recentOpenedFiles[i])
		{
			QString tip = action->toolTip();
			if(tip != action->text().remove(0,1))
				QToolTip::showText(QCursor::pos(), tip);
			else
				QToolTip::hideText();
			break;
		}		
	}	
}

void ANORRLMainWindow::closeEvent(QCloseEvent* evt)
{
    ARL::Log::current()->writeEntry(ARL::Log::Information,"ANORRLMainWindow::closeEvent");

    if ( !m_IsInitialized || UpdateUIManager::Instance().isBusy() )
    {
        evt->ignore();
        return;
    }

    Studio::Intellesense::singleton().deactivate();

	// cleanup Wiki search lookup table
	ScriptTextEditor::cleanupWikiLookup();


	// first save and close out the play doc if we're in build mode
    if ( m_BuildMode == BM_BASIC )
    {
        IANORRLDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc();       
        if ( playDoc && !requestDocClose(*playDoc) )
        {
            evt->ignore();
			return;
        }
    }

    // save and close the rest of the docs
    if ( !ANORRLDocManager::Instance().requestCloseAllDocs() )
    {
        evt->ignore();
        return;
    }
	
	//before close the application, save its states.
	saveApplicationStates();
	// remove event filter
	if (FFlag::StudioInSyncWebKitAuthentication)
		qApp->removeEventFilter(&AuthenticationHelper::Instance());

    hide();

	studioAnalytics.savePersistentVariables();
	studioAnalytics.reportPersistentVariables();

    QMainWindow::closeEvent(evt);
}

/**
 * Close all documents and save locally.
 */
void ANORRLMainWindow::saveAndClose()
{
    if ( ANORRLDocManager::Instance().requestCloseAndSaveAllDocs() )
        close();
}

/**
 * Closes all documents but doesn't save.
 */
void ANORRLMainWindow::forceClose()
{
    ANORRLDocManager::Instance().closeAllDocs();
    close();
}

void ANORRLMainWindow::contextMenuEvent(QContextMenuEvent* evt)
{
	// don't show the main window context menu if in basic mode
    if ( m_BuildMode != BM_BASIC )
        QMainWindow::contextMenuEvent(evt);
}

void ANORRLMainWindow::dragEnterEvent(QDragEnterEvent *evt)
{
	const QMimeData *pMimeData = evt->mimeData();
	if (!pMimeData || !pMimeData->hasUrls()) 
		return QMainWindow::dragEnterEvent(evt);

	bool isValidFileList = false;

	QList<QUrl> urlList = pMimeData->urls();
	for (int i = 0; i < urlList.size() && i < 6; ++i) 
	{
		QString filePath = urlList.at(i).toLocalFile();
		isValidFileList =
            !filePath.isEmpty() &&
                (filePath.endsWith(".arl", Qt::CaseInsensitive) ||
                 filePath.endsWith(".arlx", Qt::CaseInsensitive) ||
                 filePath.endsWith(".arlm", Qt::CaseInsensitive) ||
                 filePath.endsWith(".arlmx", Qt::CaseInsensitive));

		if (!isValidFileList)
			break;
	}

	if (!isValidFileList)
		return QMainWindow::dragEnterEvent(evt);
	
	evt->acceptProposedAction();
}

void ANORRLMainWindow::dragMoveEvent(QDragMoveEvent *evt)
{
	evt->acceptProposedAction();
}

void ANORRLMainWindow::dropEvent(QDropEvent *evt)
{
	const QMimeData *pMimeData = evt->mimeData();

	if (!pMimeData || !pMimeData->hasUrls())
		return QMainWindow::dropEvent(evt);
	
	QList<QUrl> urlList = pMimeData->urls();
	for (int i = 0; i < urlList.size() && i < 6; ++i) 
	{
		QString fileName = urlList.at(i).toLocalFile();
		if (!fileName.isEmpty())
		{
			if(fileName.endsWith(".arl", Qt::CaseInsensitive) || fileName.endsWith(".arlx", Qt::CaseInsensitive))
				handleFileOpen(fileName, IANORRLDoc::IDE);
			else 
			{
				if(ANORRLDocManager::Instance().getCurrentDoc())
					ANORRLDocManager::Instance().getCurrentDoc()->handleDrop(fileName);
			}
		}
	}

	evt->acceptProposedAction();
}

void ANORRLMainWindow::dragLeaveEvent(QDragLeaveEvent *evt)
{
	evt->accept();
}

void ANORRLMainWindow::initializeUI()
{

    // Apply the global ANORRL Studio style sheet to the entire application.
    // ANORRLStudio.css is where we can tweak and tune the look-and-feel
    // of the Studio UI.  All style overrides that are application-wide
    // should be put there.

	setStyleSheet(QtUtilities::getResourceFileText(":/resources/css/ANORRLStudio.css"));
    
    // force height so we don't get any resizing when adding and removing controls to the status bar
    statusBar()->setFixedHeight(32);
	
	//TODO: Move all these customizations to .ui file
	menuTools->insertAction(openPluginsFolderAction, screenShotAction);

	menuTools->insertSeparator(openPluginsFolderAction);

	advToolsToolBar->insertAction(advanceJointCreationManualAction, toggleCollisionCheckAction);
	advToolsToolBar->insertSeparator(advanceJointCreationManualAction);

	if (ARL::MouseCommand::isAdvArrowToolEnabled())
		removeToolBar(oldToolsToolBar);

	// Added so that we can dump Crash for Breakpad as needed
	// To enable crashing add following to AppSettings.xml
	//   <CrashMenu>1</CrashMenu>
	if(ANORRLSettings::showCrashMenu())
	{
		QAction *crashAction = new QAction("&Crash", this);
		connect(crashAction, SIGNAL(triggered()), this, SLOT(causeCrash()));
		QMenu* debugMenu = menuBar()->addMenu("&Debug");
		debugMenu->addAction(crashAction);
	}

	advToolsToolBar->insertAction(glueSurfaceAction, smoothNoOutlinesAction);

	Studio::Intellesense::singleton().setStyleSheet(QtUtilities::getResourceFileText(":/resources/css/ANORRLStudioRibbon.css"));

    //support drag drop
    setAcceptDrops(true);

	// fix focus issues in Mac
	setFocusPolicy(Qt::StrongFocus);
    
    //default is max screen
    setWindowState(Qt::WindowMaximized);
    
    QTimer* minute_timer = new QTimer(this);
    connect(minute_timer,SIGNAL(timeout()),this,SLOT(onMinuteTimer()));
    minute_timer->start(60 * 1000);
    
    qApp->installEventFilter(this);

	// Initialize the doc manager
	ANORRLDocManager::Instance().initialize(*this);
}

void ANORRLMainWindow::setupSlots()
{
	//file menu
	connect(menuFile, SIGNAL(hovered(QAction*)), this, SLOT(onMenuActionHovered(QAction*)));
	connect(fileNewAction,    SIGNAL(triggered()), this, SLOT(fileNew()));
	connect(fileOpenAction,   SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(fileCloseAction,  SIGNAL(triggered()), this, SLOT(fileClose()));
	connect(fileSaveAction,   SIGNAL(triggered()), this, SLOT(fileSave()));
	connect(fileSaveAsAction, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
	connect(fileExitAction,   SIGNAL(triggered()), this, SLOT(close()));
	connect(publishGameAction, SIGNAL(triggered()), this, SLOT(publishGame()));
	publishGameAction->setEnabled(false);

	connect(fileOpenRecentSavesAction, SIGNAL(triggered()), this, SLOT(fileOpenRecentSaves()));
    
    // create global shortcuts that should always work even if actions are disabled

    QShortcut* shortcut = new QShortcut(fileExitAction->shortcut(),this);
    connect(shortcut,SIGNAL(activated()),this,SLOT(close()));

    shortcut = new QShortcut(toggleBuildModeAction->shortcut(),this);
    connect(shortcut,SIGNAL(activated()),this,SLOT(toggleBuildMode()));
	
    addAction(zoomExtentsAction);
    addAction(toggleLocalSpaceAction);
    addAction(quickInsertAction);
	addAction(explorerFilterAction);

	pairRbxDeviceAction->setEnabled(true);
	playRbxDeviceAction->setEnabled(true);
	
	if (ARL::ClientAppSettings::singleton().GetValuePublishedProjectsPageUrl() && ARL::ClientAppSettings::singleton().GetValuePublishedProjectsPageUrl()[0] != '\0')
	{			
		connect(filePublishedProjectsAction, SIGNAL(triggered()), this, SLOT(filePublishedProjects()));
	}
	else
	{
		filePublishedProjectsAction->setEnabled(false);
		filePublishedProjectsAction->setVisible(false);
	}

	//set up recent open files slots
    for (int i = 0; i < MAX_RECENT_FILES_SHOWN; ++i) 
    {
		if (!isRibbonStyle())
		{
			recentOpenedFiles[i] = new QAction(this);
			menuFile->insertAction(fileExitAction, recentOpenedFiles[i]);
			recentOpenedFiles[i]->setVisible(false);
			connect(recentOpenedFiles[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
		}
		else
		{
			recentOpenedFiles[i] = NULL;
		}
	}

	QMenu* pMenu = isRibbonStyle() ? findChild<QMenu*>("switchWindowsMenu") : menuWindow;

	//for window menu open tabs slots
    for ( int i = 0 ; i < MAX_DOC_WINDOWS ; ++i ) 
    {
		currentOpenedfiles[i] = new QAction(this);

		if (pMenu)
			pMenu->addAction(currentOpenedfiles[i]);

		currentOpenedfiles[i]->setVisible(false);
        connect(
            currentOpenedfiles[i],SIGNAL(triggered()), 
            &ANORRLDocManager::Instance(),SLOT(onActivateWindow()) );
		currentOpenedfiles[i]->setCheckable(true);
	}
		
	separator = menuFile->insertSeparator(fileExitAction);
	
	//reset view
	connect(resetViewAction, SIGNAL(triggered()), this, SLOT(loadDefaultApplicationState()));
	
	//execute script
	connect(executeScriptAction, SIGNAL(triggered()), this, SLOT(executeScriptFile()));

	// Tools->Settings....
	connect(settingsAction, SIGNAL(triggered()), this, SLOT(openSettingsDialog()));

	// Tools Menu
	instanceDumpAction->setVisible(false);

	connect(openPluginsFolderAction, SIGNAL(triggered()), this, SLOT(openPluginsFolder()));
	connect(managePluginsAction, SIGNAL(triggered()), this, SLOT(managePlugins()));	
	
	// Help Menu
	connect(aboutANORRLAction, SIGNAL(triggered()), this, SLOT(about()));
	connect(onlineHelpAction, SIGNAL(triggered()), this, SLOT(onlineHelp()));
    connect(shortcutHelpAction, SIGNAL(triggered()), this, SLOT(shortcutHelp()));
    connect(objectBrowserAction, SIGNAL(toggled(bool)), this, SLOT(openObjectBrowser(bool)));
	connect(fastLogDumpAction, SIGNAL(triggered()), this, SLOT(fastLogDump()));

	connect(viewCommandBarAction, SIGNAL(toggled(bool)), commandToolBar, SLOT(setVisible(bool)));

    //Insert Object
	if (FFlag::StudioSeparateActionByActivationMethod)
    	connect(quickInsertAction, SIGNAL(triggered()), &UpdateUIManager::Instance(), SLOT(commonSlot()));
	else
		connect(quickInsertAction, SIGNAL(triggered()), &UpdateUIManager::Instance(), SLOT(onQuickInsertFocus()));
    
	connect(explorerFilterAction, SIGNAL(triggered()), &UpdateUIManager::Instance(), SLOT(filterExplorer()));

	//Cleaup players and servers
	connect(cleanupServersAndPlayersAction, SIGNAL(triggered()), this, SLOT(cleanupPlayersAndServers()));
    
	// Insert Service
	if (!m_pInsertServiceDlg)
		m_pInsertServiceDlg = new InsertServiceDialog(this);
	connect(insertServiceAction, SIGNAL(triggered()), m_pInsertServiceDlg, SLOT(show()));

	connect(findInScriptsAction, SIGNAL(triggered()), this, SLOT(showFindAllDialog()));

	emulateDeviceAction->setEnabled(true);
	manageEmulationDeviceAction->setEnabled(true);
	
	viewCommandBarAction->setEnabled(true);
	testCustomStatsAction->setEnabled(FFlag::StudioCustomStatsEnabled);

    QAction* commonActions[] =
    {
        rotateSelectionAction, tiltSelectionAction, groupSelectionAction, ungroupSelectionAction,
		selectChildrenAction, moveUpBrickAction, moveDownBrickAction, deleteSelectedAction, 
		selectAllAction, lockAction, glueSurfaceAction,

        // stats menu
        testStatsAction, testRenderStatsAction, testPhysicsStatsAction, testNetworkStatsAction, 
        testSummaryStatsAction, testCustomStatsAction, testClearStatsAction,

		smoothSurfaceAction, weldSurfaceAction, studsAction, inletAction, universalsAction, 
		hingeAction, anchorAction, motorRightAction, smoothNoOutlinesAction,
		dropperAction, simulationRunAction, simulationPlayAction, simulationStopAction, simulationResetAction, 
        zoomInAction, zoomOutAction, tiltUpAction, tiltDownAction,
		zoomExtentsAction, panRightAction, panLeftAction, advanceJointCreationManualAction, 
		gridToOneAction,gridToOneFifthAction, gridToOffAction, 
        actionFillColor, actionMaterial, advTranslateAction, advRotateAction, resizeAction,
		cutAction, copyAction, pasteAction, duplicateSelectionAction, 
		pasteIntoAction, undoAction, redoAction, playSoloAction, startServerAction, 
		startPlayerAction, pairRbxDeviceAction, insertModelAction, insertIntoFileAction, 
		selectionSaveToFileAction, publishToANORRLAction, publishToANORRLAsAction, publishGameAction, publishSelectionToANORRLAction, 
		saveToANORRLAction, publishAsPluginAction, createNewLinkedSourceAction, advArrowToolAction, toggleAxisWidgetAction, toggle3DGridAction,
		viewDiagnosticsAction, viewTaskSchedulerAction, viewToolboxAction,
		viewBasicObjectsAction, viewScriptPerformanceAction, viewObjectExplorerAction,
		viewPropertiesAction, viewOutputWindowAction, viewContextualHelpAction, viewFindResultsWindowAction, viewScriptAnalysisAction,
        gameExplorerAction, toggleCollisionCheckAction,
		renameObjectAction, unlockAllAction, openPluginsFolderAction, screenShotAction, toggleLocalSpaceAction,
        quickInsertAction, explorerFilterAction, exportSelectionAction, exportPlaceAction,
        unionSelectionAction, negateSelectionAction, separateSelectionAction, viewTutorialsAction, viewTeamCreateAction,

        // script actions
        goToScriptErrorAction, commentSelectionAction, uncommentSelectionAction, toggleCommentAction,
        expandAllFoldsAction, collapseAllFoldsAction, findAction, replaceAction, findNextAction, goToLineAction, 
        findPreviousAction, resetScriptZoomAction, reloadScriptAction, neverBreakOnScriptErrorsAction, breakOnAllScriptErrorsAction,
		breakOnUnhandledScriptErrorsAction, manageEmulationDeviceAction, 

		gridSizeToTwoAction, gridSizeToFourAction, gridSizeToSixteenAction, launchHelpForSelectionAction, downloadPlaceCopyAction
	};
	
	viewTutorialsAction->setVisible(FFlag::StudioShowTutorialsByDefault);
	downloadPlaceCopyAction->setVisible(false);

	int numActions = sizeof(commonActions) / sizeof(QAction*);
    for ( int i = 0; i < numActions; ++i )
    {
        ARL::BaldPtr<QAction> action = commonActions[i];
		if (!FFlag::StudioSeparateActionByActivationMethod)
       		connect(action,SIGNAL(triggered(bool)),this,SLOT(commonSlot(bool)));

        QList<QKeySequence> shortcuts = action->shortcuts();
        if ( !shortcuts.isEmpty() )
        {
			// add the shortcuts to the tooltip
            QtUtilities::setActionShortcuts(*action,shortcuts);
        }
        else
        {
            // set status tip
            if ( action->statusTip().isEmpty() )
                action->setStatusTip(action->toolTip());
        }
    }
	
	connect(&ANORRL::Instance(),SIGNAL(marshallAppEvent(void*,bool)), this,SLOT(processAppEvent(void*)));
	connect(actionStartPage, SIGNAL(toggled(bool)), this, SLOT(openStartPage(bool)));
	connect(actionFullScreen, SIGNAL(toggled(bool)), this, SLOT(toggleFullScreen(bool)));

	updateShortcutSet();

	QPainter p(this);

	// Set the background to white
	p.fillRect(rect(), Qt::white);
}

void ANORRLMainWindow::updateShortcutSet()
{
	if (FFlag::DontSwallowInputForStudioShortcuts)
	{
		shortcutSet.clear();
		QObjectList objects = UpdateUIManager::Instance().getMainWindow().children();
		for (QObjectList::iterator iter = objects.begin(); iter != objects.end() ; ++iter )
		{
			ARL::BaldPtr<QAction> action = dynamic_cast<QAction*>(*iter);
			if (action && !action->text().isEmpty() &&!action->text().startsWith("&"))
			{
				shortcutSet.insert(action->shortcut().toString());
			}
		}
	}
}

bool ANORRLMainWindow::isShortcut(const QKeySequence& keySequence)
{
	return (shortcutSet.find(keySequence.toString()) != shortcutSet.end());
}

/**
 * Configure the shortcuts for the actions that are platform specific.
 *  Common actions have different shortcuts on different platforms.  This
 *  function will reconfigure the common action shortcuts for the platform.
 */
void ANORRLMainWindow::assignAccelerators()
{
	// file menu
    QtUtilities::setActionShortcuts(*fileNewAction,QKeySequence::keyBindings(QKeySequence::New));
	QtUtilities::setActionShortcuts(*fileOpenAction,QKeySequence::keyBindings(QKeySequence::Open));
	QtUtilities::setActionShortcuts(*fileSaveAction,QKeySequence::keyBindings(QKeySequence::Save));
	QtUtilities::setActionShortcuts(*fileSaveAsAction,QKeySequence::keyBindings(QKeySequence::SaveAs));
	QtUtilities::setActionShortcuts(*fileCloseAction,QKeySequence::keyBindings(QKeySequence::Close));
	QtUtilities::setActionShortcuts(*fileExitAction,QKeySequence::keyBindings(QKeySequence::Quit));

	// edit menu
	QtUtilities::setActionShortcuts(*copyAction,QKeySequence::keyBindings(QKeySequence::Copy));
	QtUtilities::setActionShortcuts(*cutAction,QKeySequence::keyBindings(QKeySequence::Cut));
	QtUtilities::setActionShortcuts(*pasteAction,QKeySequence::keyBindings(QKeySequence::Paste));
	QtUtilities::setActionShortcuts(*redoAction,QKeySequence::keyBindings(QKeySequence::Redo));
	QtUtilities::setActionShortcuts(*undoAction,QKeySequence::keyBindings(QKeySequence::Undo));
	QtUtilities::setActionShortcuts(*selectAllAction,QKeySequence::keyBindings(QKeySequence::SelectAll));

    // set up delete action
	QList<QKeySequence> shortcuts = QKeySequence::keyBindings(QKeySequence::Delete);
    shortcuts.append(QKeySequence(Qt::Key_Backspace));
    QtUtilities::setActionShortcuts(*deleteSelectedAction,shortcuts);
    
	// script menu
	QtUtilities::setActionShortcuts(*findAction,QKeySequence::keyBindings(QKeySequence::Find));
    QtUtilities::setActionShortcuts(*replaceAction,QKeySequence::keyBindings(QKeySequence::Replace));

    shortcuts.clear();
    shortcuts.append(QKeySequence("F3"));
	QtUtilities::setActionShortcuts(*findNextAction,shortcuts);
    
    shortcuts.clear();
    shortcuts.append(QKeySequence("Shift+F3"));
    QtUtilities::setActionShortcuts(*findPreviousAction,shortcuts);
	
    // tools menu
    QtUtilities::setActionShortcuts(*settingsAction,QKeySequence::keyBindings(QKeySequence::Preferences));

	// help menu
    shortcuts.clear();
    shortcuts.append(QKeySequence("F1"));
	QtUtilities::setActionShortcuts(*onlineHelpAction, shortcuts);

    // camera toolbar
    QtUtilities::setActionShortcuts(*zoomInAction, QList<QKeySequence>() << QKeySequence(QKeySequence::ZoomIn) << QKeySequence("Ctrl+="));
    QtUtilities::setActionShortcuts(*zoomOutAction, QList<QKeySequence>() << QKeySequence(QKeySequence::ZoomOut) << QKeySequence("Ctrl+_"));

    shortcuts.clear();
#ifndef Q_OS_MAC
	// TODO: remove shortcut collision for debugger action - stepinto
	QtUtilities::setActionShortcuts(*stepIntoAction,shortcuts);
    // F11 works differently on Mac
    shortcuts.append(QKeySequence("F11"));
#endif
    QtUtilities::setActionShortcuts(*actionFullScreen,shortcuts);

	if (isRibbonStyle())
	{
		// remove shortcut set for startServerAction
		shortcuts.clear();
		QtUtilities::setActionShortcuts(*startServerAction,shortcuts);

		// add shortcut F7 for startServerAndPlayersAction
		QAction* pAction = findChild<QAction*>("startServerAndPlayersAction");
		if (pAction)
		{
			shortcuts.append(QKeySequence("F7"));
			QtUtilities::setActionShortcuts(*pAction,shortcuts);
		}
	}
}

void ANORRLMainWindow::showInsertServiceDialog()
{
	if (!m_pInsertServiceDlg)
		m_pInsertServiceDlg = new InsertServiceDialog(this);
	
	if(ANORRLDocManager::Instance().getCurrentDoc())
		ANORRLDocManager::Instance().getCurrentDoc()->handleAction("actionUpdateInsertServiceDialog");
	
	m_pInsertServiceDlg->setVisible(true);
}

void ANORRLMainWindow::showFindAllDialog()
{
	FindReplaceProvider::instance().getFindAllDialog()->show();
	FindReplaceProvider::instance().getFindAllDialog()->activateWindow();
	FindReplaceProvider::instance().getFindAllDialog()->setFocus();
}

void ANORRLMainWindow::updateEmbeddedFindPosition()
{
	if (IANORRLDoc* doc = ANORRLDocManager::Instance().getCurrentDoc())
	{
		if (ANORRLScriptDoc* scriptDoc = dynamic_cast<ANORRLScriptDoc*>(doc))
		{
			if (ScriptTextEditor* textEditor = scriptDoc->getTextEditor())
			{
				textEditor->updateEmbeddedFindPosition();
			}
		}
	}
}

void ANORRLMainWindow::setupCommandToolBar()
{
    m_pScriptComboBox = new ScriptComboBox(commandToolBar);
    commandToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    commandToolBar->addWidget(m_pScriptComboBox);

    connect(
        commandToolBar,SIGNAL(topLevelChanged(bool)),
        this,SLOT(onCommandToolBarTopLevelChanged(bool)) );
}

void ANORRLMainWindow::sendCounterEvent(std::string counterName, bool fireImmediately)
{
	if (!ARL::ClientAppSettings::singleton().GetValueCaptureQTStudioCountersEnabled())
		return;

#ifdef _WIN32
	// Can't initialize in global init so must do lazy init
	static boost::scoped_ptr<CountersClient> m_pCountersClient;

	if (!m_pCountersClient)
		m_pCountersClient.reset(new CountersClient(GetBaseURL().c_str(), "76E5A40C-3AE1-4028-9F10-7C62520BD94F", NULL));

	// TODO - change project settings to not have wchar as a native type.  Qt can't do wide string operations.
	std::wstring wstringCounterName = convert_s2w(counterName);
	m_pCountersClient->registerEvent(wstringCounterName, fireImmediately);
#endif
}

void ANORRLMainWindow::setupCounterSender()
{
#ifdef _WIN32
	int interval = ARL::ClientAppSettings::singleton().GetValueCaptureSlowCountersIntervalInSeconds();
	m_pCounterSenderTimer = new QTimer(this);
	connect(m_pCounterSenderTimer, SIGNAL(timeout()), this, SLOT(sendOffCounters()));
	m_pCounterSenderTimer->start(interval * 1000);
#endif
}

void ANORRLMainWindow::sendOffCounters()
{
#ifdef _WIN32
	boost::scoped_ptr<CountersClient> countersClient;

	if (!countersClient)
		countersClient.reset(new CountersClient(GetBaseURL().c_str(), "76E5A40C-3AE1-4028-9F10-7C62520BD94F", NULL));

	countersClient->reportEvents();
#endif
}

void ANORRLMainWindow::logIntervalCounter()
{
	int interval = ARL::ClientAppSettings::singleton().GetValueCaptureCountersIntervalInMinutes();

	sendCounterEvent(QString("QTStudio_%1MinCounter").arg(interval).toStdString(), false);
}

void ANORRLMainWindow::handleFileOpenSendCounter(const QString& fileName)
{
	// This person is a builder, set up a timer that starts recording every minute they're using our app
	if (!m_pMinutesPlayedTimer && ARL::ClientAppSettings::singleton().GetValueCaptureQTStudioCountersEnabled())
	{
		// Record counters every XX minutes (based on config)
		int interval = ARL::ClientAppSettings::singleton().GetValueCaptureCountersIntervalInMinutes();
		m_pMinutesPlayedTimer = new QTimer(this);
		connect(m_pMinutesPlayedTimer, SIGNAL(timeout()), this, SLOT(logIntervalCounter()));
		m_pMinutesPlayedTimer->start(interval * 60 * 1000);
	} 
	sendCounterEvent(fileName.isEmpty() ? "QTStudio_FileNew" : "QTStudio_FileOpen", false);
}
	
void ANORRLMainWindow::checkInternetConnectionSendCounter(ARL::Http& anorrlRequest)
{
	bool anorrlAccessible;
	std::string result;
	ANORRLSettings settings;

	int unconnectedRuns = settings.value("appNoInternet", 0).toInt();

	try
	{
		anorrlRequest.get(result);
		anorrlAccessible = true;
	}
	catch(...)
	{
		anorrlAccessible = false;
	}

	if (anorrlAccessible)
	{
		settings.setValue("appNoInternet", 0);
	}
	else
	{
		settings.setValue("appNoInternet", unconnectedRuns + 1);
	}
	
}

bool ANORRLMainWindow::handleFileOpen(const QString &fileName, IANORRLDoc::ARLDocType type, const QString &script)
{
    ARL::Log::current()->writeEntry(ARL::Log::Information, ARL::format("ANORRLMainWindow::handleFileOpen %s", script.toStdString().c_str()).c_str());

	QString fileToOpen = fileName;

	if (type == IANORRLDoc::IDE)
	{
		// we aren't specifying a file to open or a script to open, we aren't opening anything
		if(fileName.isEmpty() && script.isEmpty())
			return false;

        if (fileName == NEW_PLACE_FILENAME)
        {
            fileToOpen = "";
        }

		// Gross, if it's a build button click, add the avatar (and guis)
		StudioUtilities::setAvatarMode(false);
       
		if (StudioUtilities::containsVisitScript(script) ||
			StudioUtilities::containsJoinScript(script))
			StudioUtilities::setAvatarMode(true);

		handleFileOpenSendCounter(fileToOpen);
	
		// We already have an IDE doc open, launch a new version of Studio (SDI)
		IANORRLDoc* currentPlayDoc = ANORRLDocManager::Instance().getPlayDoc();
		if (currentPlayDoc)
		{
			// If we're opening a branch new place in our new process, send over the new place file name so it can differentiate
			QString fileNameToOpen = fileName.isEmpty() ? NEW_PLACE_FILENAME : fileName;
			ANORRLApplicationManager::instance().createNewStudioInstance(script, fileNameToOpen);
			return true;
		}
	}

	IANORRLDoc* newDoc = ANORRLDocManager::Instance().getOrCreateDoc(type);

    if ( type == IANORRLDoc::IDE )
    {
        // static cast ok because we know we made a IDEDoc due to type
        // only ideDoc needs a script, set it here before we open file
        ANORRLIDEDoc* ideDoc = static_cast<ANORRLIDEDoc*>(newDoc);
        ideDoc->setInitializationScript(script);

        m_AutoSaveAccum = 0;
    }
        
    if (!newDoc->open(this, fileToOpen))
    {
        QString text = tr("Error in opening file - %1").arg(fileToOpen);
        if (ARL::StandardOut::singleton() && (!fileToOpen.isEmpty()) && type == IANORRLDoc::IDE)
            ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR,qPrintable(text));

        requestDocClose(*newDoc);
        
        if ( type == IANORRLDoc::IDE )
            QMessageBox::critical(this,"Open File Failure",text);
        return false;
    }
    else
    {
        if (ARL::StandardOut::singleton() && (!fileToOpen.isEmpty()) && type == IANORRLDoc::IDE)
            ARL::StandardOut::singleton()->printf(ARL::MESSAGE_INFO, "Successfully opened file - %s", qPrintable(fileToOpen));
    }

    if ( fileToOpen.isEmpty() && type == IANORRLDoc::IDE && script.isEmpty())
    {
		ANORRLIDEDoc* idedoc = static_cast<ANORRLIDEDoc*>(newDoc);
		idedoc->initializeNewPlace();
    }


	QWidget *widgetToAddInTab = newDoc->getViewer();
	if (widgetToAddInTab)
	{
        ANORRLDocManager::Instance().configureDocWidget(*newDoc);

        // update build mode with new doc information
        //saveApplicationStates(); // save state in case update reloads it
        UpdateUIManager::Instance().updateBuildMode();
	}

    if ( type == IANORRLDoc::IDE && !fileToOpen.isEmpty() )
        updateRecentFile(fileToOpen);

	return true;
}

//for making the position of toolbar identical to old studio.
void ANORRLMainWindow::setToolbarPosition()
{
	insertToolBar(runToolBar, advToolsToolBar);
	
	if (ARL::MouseCommand::isAdvArrowToolEnabled())
	{
		insertToolBar(advToolsToolBar, runToolBar);
	}
    else
	{
		insertToolBar(advToolsToolBar, oldToolsToolBar);
		insertToolBar(oldToolsToolBar, runToolBar);
	}

	insertToolBar(runToolBar, editCameraToolBar);
	insertToolBar(editCameraToolBar, viewToolsToolBar);
}

QAction* ANORRLMainWindow::getActionByName(const QString& actionName)
{
	QList<QAction*> actionList = actions();
	for (QList<QAction*>::const_iterator iter = actionList.begin(); iter != actionList.end(); ++iter)
		if ((*iter)->objectName() == actionName)
			return *iter;

	return NULL;
}

void ANORRLMainWindow::saveDefaultApplicationState()
{
	ANORRLSettings settings;

	//save all the positions and sizes for all dockWidgets and toolbars.
	settings.setValue("default_window_state", saveState());
	settings.setValue("default_geometry_state", saveGeometry());
}

void ANORRLMainWindow::saveApplicationStates()
{
    // don't save anything in basic mode
    if ( m_BuildMode == BM_BASIC )
        return;

    if ( !isVisible() )
        return;

	UpdateUIManager::Instance().saveDocksGeometry();

	ANORRLSettings settings;
	// Clear legacy values
	settings.remove("geometry");
	settings.remove("windowState");
	
	//save all the positions and sizes for all dockWidgets and toolbars.	
	settings.setValue(sGeometryKey, saveGeometry());
	settings.setValue(sWindowStateKey, saveState());
	
	// Save command history from output window
	settings.setValue("arlCommandHistory", m_pScriptComboBox->commandHistory());

	//TODO: Move this to ANORRLRibbonMainWindow.cpp
	if (isRibbonStyle())
		ANORRLQuickAccessConfig::singleton().saveQuickAccessConfig();
}

void ANORRLMainWindow::loadDefaultApplicationState()
{
	ANORRLSettings settings;
	restoreState(settings.value("default_window_state").toByteArray());
	restoreGeometry(settings.value("default_geometry_state").toByteArray());

	// since players dockwidget and chat dockwidget gets created only after saving application default state so it doesn't get restored
	QDockWidget* playersDockWidget = findChild<QDockWidget*>("playersDockWidget");
	if (playersDockWidget)
		playersDockWidget->setVisible(false);
	QDockWidget* chatDockWidget = findChild<QDockWidget*>("chatDockWidget");
	if (chatDockWidget)
		chatDockWidget->setVisible(false);

	UpdateUIManager::Instance().setDockVisibility(eDW_OBJECT_EXPLORER, true);

	saveApplicationStates();
}

void ANORRLMainWindow::loadApplicationStates()
{
	if (isRibbonStyle()) // Ribbon mode saves to its own settings
	{
		sGeometryKey = sGeometryKey.append("_ribbon");
		sWindowStateKey = sWindowStateKey.append("_ribbon");
	}

	//load all the positions and sizes for all dockWidgets and toolbars from memory.
	ANORRLSettings settings;
	restoreState(settings.value(sWindowStateKey).toByteArray());
	restoreGeometry(settings.value(sGeometryKey).toByteArray());

    m_pScriptComboBox->setCommandHistory(settings.value("arlCommandHistory").toStringList());
	
	UpdateUIManager::Instance().loadDocksGeometry();

    setCurrentDirectory(settings.value("arl_last_directory").toString());
}

/**
 * Update the window title to show the current doc name.
 *  If a doc is not selected, show the default.
 *  
 * @see getDialogTitle
 */
void ANORRLMainWindow::updateWindowTitle()
{
    IANORRLDoc* pDoc = ANORRLDocManager::Instance().getCurrentDoc();

	// if no document or we're in basic build mode just use the simple title
    if ( !pDoc || m_BuildMode == BM_BASIC )
        setWindowTitle(sWindow_Title);
	else
	{
		updateDiscordRPC();

		setWindowTitle(pDoc->windowTitle() + " - " + QString(sWindow_Title));
    }
}

void ANORRLMainWindow::updateDiscordRPC()
{
	IANORRLDoc* pDoc = ANORRLDocManager::Instance().getCurrentDoc();

	bool discord = true;
	if (IANORRLDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc())
	{
		if (ANORRLIDEDoc* ide = dynamic_cast<ANORRLIDEDoc*>(playDoc)) {
			discord = !ide->isPlaySolo() && !ide->isSimulating();
		}
	}

	if (discord)
	{
		std::string docTypeString = "";

		switch (pDoc->docType()) {
		case IANORRLDoc::ARLDocType::BROWSER:
			docTypeString = "Browsing";
			break;
		case IANORRLDoc::ARLDocType::IDE:
			docTypeString = "Developing";
			break;
		case IANORRLDoc::ARLDocType::SCRIPT:
			docTypeString = "Scripting";
			break;
		case IANORRLDoc::ARLDocType::OBJECTBROWSER:
			docTypeString = "Browsing";
			break;
		}

		if (IANORRLDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc())
		{
			if (ANORRLIDEDoc* ide = dynamic_cast<ANORRLIDEDoc*>(playDoc)) {
				if (ide->isCloudEditSession())
					docTypeString = "Cloud Editing!";
			}
		}

		//ARL::StandardOut::singleton()->printf(ARL::MESSAGE_INFO, docTypeString.c_str());

		ARL::DiscordHandler::SetDetails(pDoc->displayName().toStdString());
		ARL::DiscordHandler::SetState(docTypeString);
	}
}

/**
 * Get the title string to be used for child dialogs.
 *  This is different than the main window's title because the main one can
 *  have the current file opened and [*] in the name.
 * 
 * @see updateWindowTitle()
 * @return default child dialog title
 */
QString ANORRLMainWindow::getDialogTitle() const
{
    return sDialog_Title;
}

/**
 * Callback every minute.
 */
void ANORRLMainWindow::onMinuteTimer()
{
	// prevent re-entrancy
    static bool processing = false;
    if ( processing )
        return;
    processing = true;

    // periodically save state in case we crash and lose all the layout changes
    saveApplicationStates();

	// auto-save
    if ( AuthoringSettings::singleton().autoSaveEnabled )
    {
        ++m_AutoSaveAccum;

        // correct for bad values
        const int autoSaveMinutes = qMax(AuthoringSettings::singleton().autoSaveMinutesInterval,1);
        AuthoringSettings::singleton().autoSaveMinutesInterval = autoSaveMinutes;

        if ( m_AutoSaveAccum >= autoSaveMinutes )
        {
            m_AutoSaveAccum = 0;
            ANORRLIDEDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc();
            if ( playDoc && !playDoc->autoSave(false) )
            {
                const int result = QMessageBox::question(
                    this,
                    tr("Failed to AutoSave"),
                    tr("Failed to AutoSave.  Do you want to temporarily disable AutoSave?"),
                    QMessageBox::Yes | QMessageBox::No );
                if ( result == QMessageBox::Yes )
                    AuthoringSettings::singleton().autoSaveEnabled = false;
            }
        }
    }

    processing = false;
}

/**
 * Deletes the splash screen.
 *  After deleting, shows the tip of the day dialog.
 */
void ANORRLMainWindow::onDeleteSplashScreen()
{
    if ( m_splashScreen )
    {
        delete m_splashScreen;
        m_splashScreen = NULL;
    }
}

void ANORRLMainWindow::toggleBuildMode()
{
    if ( m_BuildMode == BM_ADVANCED )
        setBuildMode(BM_BASIC);
    else if ( m_BuildMode == BM_BASIC )
        setBuildMode(BM_ADVANCED);

	UpdateUIManager::Instance().updateBuildMode();

    // TODO - handle other build modes
}

/**
 * Sets the current mode for building.
 *  Basic mode disables all dock widgets and toolbars.
 *  Advanced mode shows everything. 
 */
void ANORRLMainWindow::setBuildMode(eBuildMode buildMode)
{
    //saveApplicationStates();

    QSettings settings;

    // if using the default, set it from the settings
    if ( buildMode == BM_LASTMODE )
        buildMode = (eBuildMode)settings.value("BuildMode",BM_ADVANCED).toInt();
    
    m_BuildMode = buildMode;
    settings.setValue("BuildMode",m_BuildMode);
}

void ANORRLMainWindow::closePlayDoc()
{
	IANORRLDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc();       
	if (playDoc)
		requestDocClose(playDoc);
}

/**
 * Callback for the command toolbar changing floating/docking state.
 *  Handles setting the maximum size of the script input combobox so it works as expected.
 */
void ANORRLMainWindow::onCommandToolBarTopLevelChanged(bool topLevel)
{
    if ( topLevel )
        m_pScriptComboBox->setFixedWidth(800);
    else
    {
        m_pScriptComboBox->setMinimumWidth(300);
        m_pScriptComboBox->setMaximumWidth(QWIDGETSIZE_MAX);
    }

    commandToolBar->layout()->invalidate();
}

void ANORRLMainWindow::onPropertyChanged(const ARL::Reflection::PropertyDescriptor* pDescriptor)
{
    if (AuthoringSettings::singleton().getOutputLayoutMode() == AuthoringSettings::OutputLayoutHorizontal)
    {
        setCorner(Qt::TopLeftCorner, Qt::TopDockWidgetArea);
        setCorner(Qt::TopRightCorner, Qt::TopDockWidgetArea);
        setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
        setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
    }
    else
    {
        setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
        setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
        setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
        setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    }

    if (AuthoringSettings::singleton().diagnosticsBarEnabled) 
    {
        UpdateUIManager::Instance().updateStatusBar();
    }
}

void ANORRLMainWindow::trackToolboxInserts(ARL::ContentId contentId, const ARL::Instances& instances)
{
	std::vector<weak_ptr<ARL::Instance> > weakInstances;

	for (ARL::Instances::const_iterator iter = instances.begin(); iter != instances.end(); ++iter)
		weakInstances.push_back(weak_from(iter->get()));

	m_insertObjectItems.push(InsertObjectItem(contentId.getAssetId(), weakInstances));

	QTimer::singleShot(FInt::StudioInsertDeletionCheckTimeMS, this, SLOT(checkInsertedObjects()));
}

void ANORRLMainWindow::checkInsertedObjects()
{
	if (m_insertObjectItems.size() == 0)
		return;

	InsertObjectItem insertedInstance = m_insertObjectItems.front();
	m_insertObjectItems.pop();

	bool objectsStillExist = true;

	for (std::vector<weak_ptr<ARL::Instance> >::const_iterator iter = insertedInstance.weakInstances.begin(); iter != insertedInstance.weakInstances.end(); ++iter)
	{
		if (shared_ptr<ARL::Instance> sharedInstance = iter->lock())
		{
			if (!ARL::DataModel::get(sharedInstance.get()))
			{
				objectsStillExist = false;
				break;
			}
		}
		else
		{
			objectsStillExist = false;
			break;
		}
	}
}

void ANORRLMainWindow::notifyCloudEditConnectionClosed()
{
	if (m_cloudEditAwaitingShutdown)
	{
		m_cloudEditAwaitingShutdown->Set();
		return;
	}

	ANORRLIDEDoc* ideDoc = ANORRLDocManager::Instance().getPlayDoc();
	if (ideDoc && !m_IgnoreCloudEditDisconnect)
	{
		QString script = ideDoc->initializationScript();
		QtUtilities::ARLMessageBox notificationDialog;
		notificationDialog.setIcon(QMessageBox::Warning);
		if (FFlag::TeamCreateEnableDownloadLocalCopy)
		{
			notificationDialog.setText("You have lost connection to the server. "
				"Do you want to reconnect or save a copy of the place to a new file?");
		}
		else
		{
			notificationDialog.setText("You have lost connection to the server. "
				"Do you want to reconnect or work locally (you have to save local work to a new file)?");
		}
		notificationDialog.addButton("Reconnect", QMessageBox::ResetRole);
		if (FFlag::TeamCreateEnableDownloadLocalCopy)
			notificationDialog.addButton("Save Local Copy", QMessageBox::ActionRole);
		else
			notificationDialog.addButton("Work Offline", QMessageBox::DestructiveRole);
		notificationDialog.exec();
		QMessageBox::ButtonRole role = notificationDialog.buttonRole(notificationDialog.clickedButton());
		if (role == QMessageBox::ResetRole)
		{
			// close the play doc if it is still open
			if (IANORRLDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc())
			{
				requestDocClose(*playDoc, false /*closeiflastdoc*/);
			}
			handleFileOpen(fileLocationArg, IANORRLDoc::IDE, script);
		}
		else if (FFlag::TeamCreateEnableDownloadLocalCopy && (role == QMessageBox::ActionRole))
		{
			if (ANORRLIDEDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc())
			{
				// let users save a local copy
				QString fileName = playDoc->downloadPlaceCopy();
				// close the online document
				requestDocClose(*playDoc, false /*closeiflastdoc*/);
				if (!fileName.isEmpty())
					handleFileOpen(fileName, IANORRLDoc::IDE);
			}
		}
	}
}

void ANORRLMainWindow::cloudEditStatusChanged(int placeId, int universeId)
{
	ARL::ScopedAssign<bool> ignoreDocActivate(m_IgnoreCloudEditDisconnect, true);

	if (IANORRLDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc())
	{
		requestDocClose(*playDoc, false /*closeiflastdoc*/);
	}

	QString initScript = QString("%1/game/edit.slua?PlaceID=%2&upload=%2&testmode=false&universeId=%3").arg(ANORRLSettings::getBaseURL()).arg(placeId).arg(universeId);

	handleFileOpen(FFlag::StudioDoublingOnUploadFixEnabled ? "" : fileLocationArg,
		IANORRLDoc::IDE, initScript);
}

void ANORRLMainWindow::shutDownCloudEditServer()
{
	using namespace ARL;
	using namespace Network;

	ARLASSERT(QThread::currentThread() == qApp->thread());

	ANORRLIDEDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc();
	ARLASSERT(playDoc);
	if (!playDoc) return;

	ARLASSERT(playDoc->isCloudEditSession());
	if (!playDoc->isCloudEditSession()) return;

	shared_ptr<DataModel> dm = playDoc->getEditDataModel();
	ARLASSERT(dm);
	if (!dm) return;

	long placeId, universeId;
	{
		{
			DataModel::LegacyLock lock(dm.get(), DataModelJob::Write);

			placeId = dm->getPlaceID();
			universeId = dm->getUniverseId();

			Players* players = dm->find<Players>();
			ARLASSERT(players);
			if (players)
			{
				ARLASSERT(!m_cloudEditAwaitingShutdown);
				m_cloudEditAwaitingShutdown.reset(new ARL::CEvent(true /* manual reset */));
				Players::event_requestCloudEditShutdown.replicateEvent(players);
			}
		}
		
		if (m_cloudEditAwaitingShutdown)
		{
			UpdateUIManager::Instance().waitForLongProcess("Waiting for Shutdown", boost::bind(&ARL::CEvent::Wait, m_cloudEditAwaitingShutdown.get()));
			m_cloudEditAwaitingShutdown.reset();
		}
	}

	QMetaObject::invokeMethod(this, "cloudEditStatusChanged", Qt::QueuedConnection, Q_ARG(int, placeId), Q_ARG(int, universeId));
}

void ANORRLMainWindow::setupCustomToolButton()
{	
	m_pFillColorToolButton = new FillColorPickerToolButton(this);
	m_pFillColorToolButton->setObjectName("actionFillColor");
	m_pFillColorToolButton->setDefaultAction(actionFillColor);
	connect(m_pFillColorToolButton, SIGNAL(changed(const QString &)), this, SLOT(onCustomToolButton(const QString &)));

	m_pMaterialToolButton = new MaterialPickerToolButton(this);
	m_pMaterialToolButton->setObjectName("actionMaterial");
	m_pMaterialToolButton->setDefaultAction(actionMaterial);
	connect(m_pMaterialToolButton, SIGNAL(changed(const QString &)), this, SLOT(onCustomToolButton(const QString &)));

	//add tool buttons into toolbar
	QAction *fillAction = advToolsToolBar->insertWidget(dropperAction, m_pFillColorToolButton);
	fillAction->setObjectName("actionFillColor_WidgetAction");

	QAction *materialAction = advToolsToolBar->insertWidget(smoothSurfaceAction, m_pMaterialToolButton);
	materialAction->setObjectName("actionMaterial_WidgetAction");

	advToolsToolBar->insertSeparator(materialAction);
	advToolsToolBar->insertSeparator(smoothSurfaceAction);
}

void ANORRLMainWindow::onAnalyticTracked()
{
	m_lastAnalyticTrackedTime = QTime::currentTime();
}


void ANORRLMainWindow::keepAliveAnalyticsSession()
{
	m_lastAnalyticKeepAliveTime = QTime::currentTime();
}

void ANORRLMainWindow::cleanupPlayersAndServers()
{
	ANORRLApplicationManager::instance().cleanupChildProcesses();
	cleanupServersAndPlayersAction->setEnabled(false);
}

void ANORRLMainWindow::onIDEDocViewInitialized()
{
    if ( m_splashScreen )
        onDeleteSplashScreen();
}


