/**
 * ANORRL.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRL.h"

// Qt Headers
#include <QDateTime>
#include <QTimer>
#include <QThread>

// 3rd Party Headers
#include "boost/function.hpp"

// ANORRL Headers
#include "v8datamodel/GameBasicSettings.h"
#include "v8datamodel/DebugSettings.h"
#include "v8datamodel/Game.h"
#include "v8datamodel/MouseCommand.h"
#include "v8datamodel/TeleportService.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Light.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/HttpService.h"
#include "arl/TaskScheduler.h"
#include "arl/ProcessPerfCounter.h"
#include "Util/standardout.h"
#include "Util/Statistics.h"
#include "NetworkSettings.h"
#include "RenderSettingsItem.h"
#include "V8DataModel/Workspace.h"
#include "v8datamodel/CustomParticleEmitter.h"
#include "v8datamodel/ContentProvider.h"

// ANORRL Studio Headers
#include "ANORRLIDEDoc.h"
#include "ANORRLMainWindow.h"
#include "UpdateUIManager.h"
#include "FunctionMarshaller.h"
#include "ANORRLSettings.h"
#include "AuthoringSettings.h"
#include "ExternalHandlers.h"
#include "ANORRLDocManager.h"
#include "StudioUtilities.h"
#include "MachineConfig.h"
#include "ANORRLUser.h"

#include "arl/Profiler.h"

FASTFLAG(SurfaceLightEnabled)
DYNAMIC_FASTFLAG(CustomEmitterInstanceEnabled)
FASTFLAG(UsePGSSolver)

FASTFLAGVARIABLE(StudioABTestEnabled, false)

#ifdef STUDIO_ADMIN_BUILD
    #include "util/MD5Hasher.h"
#endif

bool ANORRL::sInitialized = false;
arl::signals::scoped_connection messageOutConnection;

#ifdef Q_WS_WIN
static boost::shared_ptr<CProcessPerfCounter> s_perfCounter;
#endif

#ifdef Q_WS_MAC

extern "C" {
	void writeFastLogDumpHelper(const char* fileName, int numEntries)
	{
		FLog::WriteFastLogDump(fileName, numEntries);
	}
};
#endif

//this function is being used in ARLGraphics.cpp also!
std::string macBundlePath()
{
#ifdef Q_WS_MAC
	char path[1024];
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	assert(mainBundle);
	
	CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
	assert(mainBundleURL);
	
	CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
	assert(cfStringRef);
	
	CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);
	
	CFRelease(mainBundleURL);
	CFRelease(cfStringRef);
	
	return std::string(path);
#endif
	return "";
}

ANORRL& ANORRL::Instance()
{			
	static ANORRL singleton;
	return singleton;
}

ANORRL::ANORRL()
: m_pMarshaller(ARL::FunctionMarshaller::GetWindow())
, m_pTimer(new QTimer(this))
{
    qRegisterMetaType<ARL::MessageType>("ARL::MessageType");
	qRegisterMetaType<boost::shared_ptr<ARL::Instance> >("shared_ptr<ARL::Instance>");
	qRegisterMetaType<const ARL::Reflection::PropertyDescriptor*>("const ARL::Reflection::PropertyDescriptor*");

	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

ANORRL::~ANORRL()
{
	ARL::FunctionMarshaller::ReleaseWindow(m_pMarshaller);
}

void ANORRL::startTimer()
{
	m_pTimer->start(1000);
}

void ANORRL::stopTimer()
{
	m_pTimer->stop();
}

void ANORRL::onTimeOut()
{
#ifdef Q_WS_WIN
	s_perfCounter->CollectData();
#endif
}

void ANORRL::globalInit(const QString& urlArg, const QString& ticketArg, ARL::HttpFuture& settingsFuture)
{
	if (sInitialized)
		return;	

#ifdef STUDIO_ADMIN_BUILD
	ARL::DataModel::hash = ANORRLSettings::getAdminKey().toStdString();
#endif

	messageOutConnection = ARL::StandardOut::singleton()->messageOut.connect(&onMessageOut);	

	if (FFlag::StudioABTestEnabled)
	{
		AuthenticationHelper::Instance().waitForHttpAuthentication();

		QSettings retentionData("ANORRL", "Retention");
		static const char* const kBrowserTrackerId = "BrowserTrackerId";
		QString browserTrackerId = retentionData.value(kBrowserTrackerId, "").toString();

		const std::string& apiUrl = ARL::ContentProvider::getUnsecureApiBaseUrl(GetBaseURL());
		std::string abUrl = ARL::format("%s/users/get-studio-experiment-enrollments?firstStudioVisit=%s%s", apiUrl.c_str(), StudioUtilities::isFirstTimeOpeningStudio() ? "true" : "false",
			browserTrackerId.isEmpty() ? "" : ARL::format("&browserTrackerId=%s", browserTrackerId.toStdString().c_str()).c_str());
		ARL::HttpFuture abTestResult = FetchABTestDataAsync(abUrl);

		try 
		{
			std::string newBrowserTrackerId = LoadABTestFromString(abTestResult.get());
			if(browserTrackerId.isEmpty() && !newBrowserTrackerId.empty())
			{
				retentionData.setValue(kBrowserTrackerId, QString(newBrowserTrackerId.c_str()));
			}
		} 
		catch(std::exception&)
		{
			FASTLOG(FLog::Error, "Failed to load AB test data");
		}
	}

    ARL::Http::SetUseStatistics(true);

	ANORRLSettings::recoverAssets();
	ANORRLSettings::initWorkspaceSettings();

	ARL::MouseCommand::enableAdvArrowTool(true);

#ifdef Q_WS_WIN
	s_perfCounter = CProcessPerfCounter::getInstance();
#endif

	// Force loading of settings classes
	ARL::GameBasicSettings::singleton();
	AuthoringSettings::singleton();
	ARL::NetworkSettings::singleton();
	CRenderSettingsItem::singleton();
	ARL::DebugSettings::singleton();
			
	ARL::GlobalAdvancedSettings::singleton()->loadState(std::string());
	{
		ARL::Security::Impersonator impersonate(ARL::Security::ANORRLGameScript_);
		ARL::GlobalBasicSettings::singleton()->loadState(std::string());
	}

	ARL::GameBasicSettings::singleton().setStudioMode(true);

    // now that fast flags are loaded we can reconfigure our settings if necessary
    AuthoringSettings::singleton().configureBasedOnFastFlags();
	
	ARL::Profiler::onThreadCreate("Main");

	// Initialize the TaskScheduler (after loading configs)
	ARL::TaskScheduler::singleton().setThreadCount(ARL::TaskSchedulerSettings::singleton().getThreadPoolConfig());

	ANORRL::Instance().startTimer();
	
	MachineConfig::setupMachineConfig();

	ARL::TeleportService::SetCallback(&ANORRL::Instance());
	ARL::TeleportService::SetBaseUrl(ANORRLSettings::getBaseURL().toStdString().c_str());

	sInitialized = true;		

    if (!FFlag::SurfaceLightEnabled)
    {
        ARL::Reflection::ClassDescriptor* classDesc = &ARL::SurfaceLight::classDescriptor();
        ARL::Reflection::Metadata::Class* pMetadataDynamic = ARL::Reflection::Metadata::Reflection::singleton()->get(*classDesc, false);
        if (pMetadataDynamic)
        {
            ARL::Reflection::Metadata::Class::prop_deprecated.setValue(pMetadataDynamic, true);
        }
    }
    if (!DFFlag::CustomEmitterInstanceEnabled)
    {
        ARL::Reflection::ClassDescriptor* classDesc = &ARL::CustomParticleEmitter::classDescriptor();
        ARL::Reflection::Metadata::Class* pMetadataDynamic = ARL::Reflection::Metadata::Reflection::singleton()->get(*classDesc, false);
        if (pMetadataDynamic)
        {
            ARL::Reflection::Metadata::Class::prop_deprecated.setValue(pMetadataDynamic, true);
        }
    }

	if (!FFlag::UsePGSSolver)
	{
		ARL::Reflection::PropertyDescriptor* propDesc = ARL::Workspace::classDescriptor().findPropertyDescriptor("PGSPhysicsSolverEnabled");
		if(propDesc) {
			ARL::Reflection::Metadata::Member* pMetadataDynamic = ARL::Reflection::Metadata::Reflection::singleton()->get(*propDesc);
			if(pMetadataDynamic)
				ARL::Reflection::Metadata::Item::prop_deprecated.setValue(pMetadataDynamic, true);
		}
	}
}

void ANORRL::globalShutdown()
{
	ANORRLSettings::saveAssets();
#ifdef Q_WS_WIN
	s_perfCounter.reset();
#endif
	ANORRL::Instance().stopTimer();
	ARL::GlobalBasicSettings::singleton()->saveState();
	ARL::Game::globalExit();
    messageOutConnection.disconnect();
}

// FunctionMarshaller help - posts worker thread message to UI thread and deals with it next time through, wait till the event is processed
void ANORRL::sendAppEvent(void *pClosure)
{
	ARL::CEvent *waitEvent = ((ARL::FunctionMarshaller::Closure *) pClosure)->waitEvent;
	bool waitFlag = (waitEvent == NULL);
	
	Q_EMIT ANORRL::Instance().marshallAppEvent(pClosure, waitFlag);
	
	if (waitEvent)
		waitEvent->Wait();
}

// FunctionMarshaller help - posts worker thread message to UI thread and deals with it next time through, do not wait
void ANORRL::postAppEvent(void *pClosure)
{
	Q_EMIT ANORRL::Instance().marshallAppEvent(pClosure, false);	
}

bool ANORRL::isTeleportEnabled() const
{
	return true;
}

void ANORRL::onMessageOut(const ARL::StandardOutMessage& message)
{
	Q_EMIT ANORRL::Instance().newOutputMessage(message.message.c_str(), message.type);	
}

void ANORRL::Teleport(const std::string& url, const std::string& ticket, const std::string& script)
{
    // Teleporting in Studio is disabled.
	ANORRLIDEDoc* pPlayDoc = ANORRLDocManager::Instance().getPlayDoc();
	if(!pPlayDoc)
		return;

	pPlayDoc->teleportToURL(script.c_str(), true);
}

void ANORRL::doTeleport(const std::string& url, const std::string& ticket, const std::string& script)
{
    ARLASSERT(isTeleportEnabled());
	boost::function<void()> callback = boost::bind(&ANORRL::Teleport, this, url, ticket, script);
	m_pMarshaller->Submit(callback);
}