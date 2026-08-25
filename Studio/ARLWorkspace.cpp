/**
 * ARLWorkspace.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ARLWorkspace.h"

// Qt Headers
#include <QMessageBox>
#include <QDrag>
#include <QMimeData>
#include <QUrl>
#include <QDir>
#include <QDesktopServices>
#include <QStringList>
#include <QWebEnginePage>

// ANORRL Headers
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Selection.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/ContentProvider.h"
#include "v8datamodel/Tool.h"
#include "v8datamodel/Decal.h"
#include "v8datamodel/Visit.h"
#include "v8datamodel/GameBasicSettings.h"
#include "V8DataModel/PartOperationAsset.h"
#include "v8datamodel/PlayerGui.h"
#include "v8datamodel/VehicleSeat.h"
#include "v8datamodel/KeyframeSequence.h"
#include "v8datamodel/KeyframeSequenceProvider.h"
#include "V8DataModel/NonReplicatedCSGDictionaryService.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"
#include "v8xml/XmlSerializer.h"
#include "v8xml/Serializer.h"
#include "v8xml/SerializerBinary.h"
#include "v8xml/WebParser.h"
#include "Network/Players.h"
#include "Network/api.h"
#include "script/script.h"
#include "util/FileSystem.h"
#include "util/HttpAsync.h"

// ANORRL Studio Headers
#include "ARLContent.h"
#include "ANORRLMainWindow.h"
#include "ANORRLPluginHost.h"
#include "ANORRLToolBox.h"
#include "AuthenticationHelper.h"
#include "UpdateUIManager.h"
#include "ANORRLGameExplorer.h"
#include "ANORRLSettings.h"
#include "StudioUtilities.h"
#include "UpdateUIManager.h"
#include "ANORRLStudioVerbs.h"
#include "ANORRLIDEDoc.h"
#include "ANORRLDocManager.h"
#include "ANORRLWebDoc.h"
#include "WebDialog.h"
#include "CommonInsertWidget.h"
#include "ANORRLView.h"

#include "QtUtilities.h"

// Publish Selection to ANORRL
bool ARLWorkspace::isScriptAssetUploadEnabled = true;
bool ARLWorkspace::isAnimationAssetUploadEnabled = true;
bool ARLWorkspace::isImageModelAssetUploadEnabled = false;

FASTFLAG(StudioCSGAssets)

FASTFLAGVARIABLE(StudioEnableGameAnimationsTab, true)
FASTFLAGVARIABLE(StudioRecordToolboxInsert, true)
FASTFLAGVARIABLE(StudioRemoveInsertEvent, false)
FASTFLAGVARIABLE(StudioExposeSessionID, true)

FASTINTVARIABLE(StudioPartSizeCameraDistRatio, 30)

//enable this code for mac when we have the video recording capability available

ARLWorkspace::ARLWorkspace( QObject *parent, ARL::DataModel *dm ) 
: QObject(parent)
, m_pDataModel(dm)
, m_pParent(parent)
, m_screenShotInProgressMsgBox(NULL)
{
	m_pContent = new ARLContent(this);
}

ARLWorkspace::~ARLWorkspace()
{
	delete m_pContent;
}

// Publish To ANORRL
bool ARLWorkspace::SaveUrl(const QString &saveUrl)
{
	if (!AuthenticationHelper::Instance().verifyUserAndAuthenticate() || 
		!AuthenticationHelper::validateMachine())
		return false;
	
	try 
	{	
		std::string urlStr = saveUrl.toStdString();

		QString url(urlStr.c_str());
		url = url.mid(url.indexOf("assetid=") + 8);
		url = url.left(url.indexOf("&"));
		int placeId = url.toInt();

		ARL::HttpFuture universeFuture;
		if (placeId > 0)
		{
			universeFuture = ARL::HttpAsync::get(
				ARL::format("%s/universes/get-universe-containing-place?placeId=%d",
				ARL::ContentProvider::getUnsecureApiBaseUrl(ANORRLSettings::getBaseURL().toStdString()).c_str(), placeId));
		}

		{
			ARL::DataModel::LegacyLock legacylock(m_pDataModel, ARL::DataModelJob::Write);
			ARL::Visit* visit = ARL::ServiceProvider::create<ARL::Visit>(m_pDataModel);
			visit->setUploadUrl(urlStr);  // Set the upload URL so that future publishes go to here as well.

			if (FFlag::StudioCSGAssets)
				ARL::PartOperationAsset::publishAll(m_pDataModel);

			m_pDataModel->save(ARL::ContentId(urlStr));

			if (placeId > 0)
			{
				m_pDataModel->setPlaceID(placeId, false);
				ANORRLMainWindow::get(this)->getStudioAnalytics()->reportPublishStats(m_pDataModel);

				try
				{
					shared_ptr<const ARL::Reflection::ValueTable> v(new ARL::Reflection::ValueTable);
					ARL::WebParser::parseJSONTable(universeFuture.get(), v);
					int universeId = v->at("UniverseId").cast<int>();
					m_pDataModel->setUniverseId(universeId);
					if (ANORRLIDEDoc* ideDoc = ANORRLDocManager::Instance().getPlayDoc())
					{
						ideDoc->updateFromPlaceID(placeId);
					}
				}
				catch (...)
				{
				}

				QMetaObject::invokeMethod(&UpdateUIManager::Instance().getViewWidget<ANORRLGameExplorer>(eDW_GAME_EXPLORER),
					"openGameFromPlaceId", Qt::QueuedConnection, Q_ARG(int, placeId));

				if (ANORRLDocManager::Instance().getPlayDoc())
					QMetaObject::invokeMethod(ANORRLDocManager::Instance().getPlayDoc(),
					"updateFromPlaceID", Qt::QueuedConnection, Q_ARG(int, placeId));
			}
		}

		if (!ANORRLDocManager::Instance().getPlayDoc()->isLocalDoc())
			ANORRLDocManager::Instance().getPlayDoc()->resetDirty(m_pDataModel);

		return true;
	}
	catch (std::exception&)
	{
		ANORRLMainWindow::sendCounterEvent("QTStudioPublishSaveUrlFailure", false);
		return false;
	}
}

bool ARLWorkspace::Save()
{
	if (!AuthenticationHelper::Instance().verifyUserAndAuthenticate() || 
		!AuthenticationHelper::validateMachine())
		return false;

	try 
	{
		ANORRLIDEDoc* pIDEDoc = ANORRLDocManager::Instance().getPlayDoc();
		if (!pIDEDoc)
			return false;

		ARL::Verb* pVerb = pIDEDoc->getVerb("PublishToANORRLVerb");
		PublishToANORRLVerb* pPublishVerb = dynamic_cast<PublishToANORRLVerb*>(pVerb);

		if (!pPublishVerb)
			return false;

		pPublishVerb->doIt(pIDEDoc->getDataModel());

		return true;
	}
	catch (std::exception&)
	{
		ANORRLMainWindow::sendCounterEvent("QTStudioPublishSaveFailure", false);
		return false;
	}
}

#ifdef ENABLE_EDIT_PLACE_IN_STUDIO
// To start edit of a place
bool ARLWorkspace::StartGame(const QString &ticket, const QString &url, const QString &script)
{
	static int errorCount = 0;
	Hide();
	bool result = UpdateUIManager::Instance().getMainWindow().handleFileOpen("", IANORRLDoc::IDE, script);
	if (result)
	{
		errorCount = 0;
		Close();
	}
	else
	{
		Show();
		errorCount++;
		QMessageBox::critical(
			NULL, 
			tr("ANORRL Studio"), 
			errorCount <= 2 ? 
				tr("There was a problem opening your place.  Please try again.")
				: tr("Oh no!  It seems we're still unable to open this place.  Please restart your application and try again.  If you continue to encounter this error, please contact customer service.")
			);
	}
	return result;
}
#endif

void ARLWorkspace::InstallPlugin(int assetId, int assetVersion)
{
	QString url = QString("%1/asset/?id=%2").arg(ANORRLSettings::getBaseURL()).arg(assetId);
	ARL::Http http(qPrintable(url));
	boost::function<void(bool)> completionCallback = 
		boost::bind(&ARLWorkspace::pluginInstallCompleteHelper, 
			boost::weak_ptr<ARLWorkspace>(shared_from_this()), _1, assetId);
	http.get(boost::bind(ANORRLPluginHost::processInstallPluginFromWebsite,
		assetId, assetVersion, _1, _2, completionCallback));
}

QString ARLWorkspace::GetInstalledPlugins()
{
	return ANORRLPluginHost::getPluginMetadataJson();
}

void ARLWorkspace::pluginInstallCompleteHelper(weak_ptr<ARLWorkspace> weakWorkspace, bool succeeded, int assetId)
{
	if (shared_ptr<ARLWorkspace> workspace = weakWorkspace.lock())
	{
		Q_EMIT workspace->PluginInstallComplete(succeeded, assetId);
	}
}

void ARLWorkspace::SetPluginEnabled(int assetId, bool enabled)
{
	ANORRLPluginHost::setPluginEnabled(assetId, enabled);
}

void ARLWorkspace::UninstallPlugin(int assetId)
{
	ANORRLPluginHost::uninstallPlugin(assetId);
}

// Publish Selection To ANORRL
QObject* ARLWorkspace::WriteSelection()
{
	if (!AuthenticationHelper::Instance().verifyUserAndAuthenticate() || 
		!AuthenticationHelper::validateMachine())
	{
		return NULL;
	}

	delete m_pContent;
	m_pContent = new ARLContent(this);

	try 
	{
		shared_ptr<ARL::Script> script;
		if (ARLWorkspace::isScriptAssetUploadEnabled)
		{
			ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Read);
			ARL::Selection* sel = ARL::ServiceProvider::find<ARL::Selection>(m_pDataModel);
			if (sel && sel->size()==1)
				script = ARL::Instance::fastSharedDynamicCast<ARL::Script>(sel->front());
		}

		if (script)
		{
			ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Read);
			if (script->isCodeEmbedded())
				m_pContent->data << script->getEmbeddedCode().get().getSource();
			else
			{
				ARL::BaseScript::Code code = script->requestCode();
				if (code.loaded)
					m_pContent->data << code.script.get().getSource();
			}			
		}
		else
		{
			ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);

            if (FFlag::StudioCSGAssets)
                ARL::PartOperationAsset::publishSelection(m_pDataModel);

			ARL::Selection* sel = ARL::ServiceProvider::create<ARL::Selection>(getWorkspace());
			ARL::CSGDictionaryService* dictionaryService = ARL::ServiceProvider::create<ARL::CSGDictionaryService>(m_pDataModel);
			ARL::NonReplicatedCSGDictionaryService* nrDictionaryService = ARL::ServiceProvider::create<ARL::NonReplicatedCSGDictionaryService>(m_pDataModel);

			if (dictionaryService && nrDictionaryService)
				for (ARL::Instances::const_iterator iter = sel->begin(); iter != sel->end(); ++iter)
				{
					dictionaryService->retrieveAllDescendants(*iter);
					nrDictionaryService->retrieveAllDescendants(*iter);
				}

            ARL::Instances instances(sel->begin(), sel->end());
            ARL::SerializerBinary::serialize(m_pContent->data, instances);

			if (dictionaryService && nrDictionaryService)
				for (ARL::Instances::const_iterator iter = sel->begin(); iter != sel->end(); ++iter)
				{
					dictionaryService->storeAllDescendants(*iter);
					nrDictionaryService->storeAllDescendants(*iter);
				}
		}
	}
	catch (std::exception &e) 
	{
		ANORRLMainWindow::sendCounterEvent("QTStudioPublishSelectionFailure", false);
		ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR, e.what());
		return NULL;
	}	
	
	return m_pContent;
}

void ARLWorkspace::writeToStream(const XmlElement* root, std::ostream& stream)
{
	TextXmlWriter machine(stream);
	machine.serialize(root);
}

// Do not confuse the DataModel Workspace with the ARLWorkspace. We are using ARLWorkspace as a Javascript Bridge Object
ARL::Workspace* ARLWorkspace::getWorkspace() const
{
	return m_pDataModel ? m_pDataModel->getWorkspace() : NULL;
}

std::auto_ptr<XmlElement> ARLWorkspace::writeSelectionToXml()
{
	std::auto_ptr<XmlElement> root(Serializer::newRootElement());
	{
		ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);
		ARL::Selection* sel = ARL::ServiceProvider::create<ARL::Selection>(getWorkspace());
		ARL::AddSelectionToRoot(root.get(), sel, ARL::SerializationCreator);
		if (sel && sel->size()==1 && dynamic_cast<ARL::KeyframeSequence*>(sel->front().get()))
		{
			root->addAttribute(tag_assettype, "animation");
		}
	}
	return root;
}

bool ARLWorkspace::Insert(const QString& urlQStr)
{	
	try
	{
		if (!m_pDataModel)
			throw std::runtime_error("Can't insert at this time");	
				
		QByteArray ba = urlQStr.toLocal8Bit();
		const char *url = ba.data();
		
		bool success = ARL::Http::trustCheck(url);
		std::string urlString = urlQStr.toStdString();

		insert(ARL::ContentId(urlString), false);

		ANORRLMainWindow::sendCounterEvent("StudioWorkspaceInsertCounter", false);
	}
	catch (std::exception const& e)
	{
		ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR, e.what());
		return false;
	}
	return true;
}

// InsertInto:	TREE_VIEW, SUPPRESS_PROMPTS
// Insert:      3D_VIEW, ALLOW_PROMPTS

void ARLWorkspace::insert(std::istream& stream, bool insertInto)
{
	if (!m_pDataModel)
		throw std::runtime_error("Can't insert at this time");
	
	// The following code happens BEFORE we do a lock because we can't display a message box within a lock
	ARL::Instances instances;
	
    Serializer().loadInstances(stream, instances);

	insert(instances, insertInto);
}

void ARLWorkspace::insert(ARL::Instances& instances, bool insertInto)
{	
	{
		// Lock the data model temporarily to make surARL::e insertion is legal
		ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);

		if (ARL::ContentProvider* cp = m_pDataModel->create<ARL::ContentProvider>())
		{
			ARL::LuaSourceContainer::blockingLoadLinkedScriptsForInstances(cp, instances);
		}
	}

	ARL::PromptMode promptMode = ARL::SUPPRESS_PROMPTS;
	if (!insertInto)
	{
		if (instances.size() == 1)
		{
			ARL::Instance* single = instances[0].get();
			if (ARL::Instance::fastDynamicCast<ARL::Tool>(single)) 
			{				
				if ( QMessageBox::question(
                        &UpdateUIManager::Instance().getMainWindow(), 
                        "Insert Tool", 
                        "Put this tool into the starter pack (otherwise drop into the 3d view)?", 
                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
					promptMode = ARL::PUT_TOOL_IN_STARTERPACK;
			}
		}
	}
	
	// Now lock the datamodel (after MessageBox)
	ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);
	
	shared_ptr<ARL::Workspace> spWorkspace = shared_from(getWorkspace());
	if(!spWorkspace)
		throw std::runtime_error("Can't insert at this time");

	if (DFFlag::MaterialPropertiesEnabled)
	{
		StudioUtilities::convertPhysicalPropertiesIfNeeded(instances, spWorkspace.get());
	}
	
	ARL::Selection* sel = ARL::ServiceProvider::create<ARL::Selection>(spWorkspace.get());
	
	ARL::Instance* requestedParent = 0;

    requestedParent = spWorkspace.get();
	
	ARL::InsertMode insertMode =  ARL::INSERT_TO_3D_VIEW;
	
	// if we have a selection make sure we can invoke insert decal tool
	if ((instances.size() == 1) && ARL::Instance::fastDynamicCast<ARL::Decal>(instances[0].get()) && (sel->size() == 1))
	{
		requestedParent = sel->front().get();
		insertMode = ARL::Instance::fastDynamicCast<ARL::PartInstance>(requestedParent) ? ARL::INSERT_TO_TREE : ARL::INSERT_TO_3D_VIEW;
	}
	
	try
	{
		// get position to insert at
		bool isOnPart = false;
		ARL::Vector3 pos = InsertObjectWidget::getInsertLocation(shared_from(m_pDataModel), NULL, &isOnPart);
		spWorkspace->insertInstances(instances, requestedParent, insertMode, promptMode, isOnPart ? &pos : NULL);

		if (promptMode != ARL::PUT_TOOL_IN_STARTERPACK)
		{
			// zoom camera at the inserted instances
			ARL::PartArray parts;
			for (size_t i = 0; i < instances.size(); ++i) 
				ARL::PartInstance::findParts(instances[i].get(), parts);
			if (parts.size() > 0)
			{
				ARL::Extents ext = ARL::DragUtilities::computeExtents(parts);
				if (!ext.isNanInf() && !ext.isNull())
				{
					ARL::Camera* camera = spWorkspace->getCamera();
					float cameraDistance = fabsf((camera->getCameraCoordinateFrame().translation - ext.center()).magnitude());
					// if extents are out of camera frustum or size of the part is small, we need to zoom in/out
					if (!camera->frustum().containsAABB(ext) || 
						((cameraDistance > 0) && ext.longestSide()/cameraDistance < FInt::StudioPartSizeCameraDistRatio/100.0f))
					{
						ext.expand(ext.longestSide());
						camera->zoomExtents(ext, ARL::Camera::ZOOM_IN_OR_OUT);
					}
				}
			}
		}
		
		boost::shared_ptr<ARL::RunService> runService = shared_from(m_pDataModel->create<ARL::RunService>());
		// If we're not playing a game then select the content
		if (runService->getRunState() == ARL::RS_STOPPED)
		{
			sel->setSelection(instances.begin(), instances.end());
			ANORRLIDEDoc* playDoc = ANORRLDocManager::Instance().getPlayDoc();
			if (playDoc)
			{
				UpdateUIManager::Instance().updateToolBars();
			}
		}
	}
	catch (std::exception&)
	{
		std::for_each(instances.begin(), instances.end(), boost::bind(&ARL::Instance::setParent, _1, (ARL::Instance*) NULL));
		throw;
	}
}


void ARLWorkspace::insert(ARL::ContentId contentId, bool insertInto)
{
	if (!m_pDataModel)
		throw std::runtime_error("Can't insert at this time");

	// load instances in a different thread
	bool hasError = false;
	ARL::Instances instances;
	UpdateUIManager::Instance().waitForLongProcess("Inserting...", 
		boost::bind(&ARLWorkspace::loadContent, this, contentId, boost::ref(instances), boost::ref(hasError)));

	if (hasError || instances.size() < 1)
	{
		ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR, "Selected object cannot be inserted at this time.");
		return;
	}

	if (FFlag::StudioRecordToolboxInsert)
		UpdateUIManager::Instance().getMainWindow().trackToolboxInserts(contentId, instances);

	// now insert instances
	insert(instances, insertInto);
}

void ARLWorkspace::loadContent(ARL::ContentId contentId, ARL::Instances& instances, bool& hasError)
{
	try
	{
		std::auto_ptr<std::istream> stream(ARL::ServiceProvider::create<ARL::ContentProvider>(m_pDataModel)->getContent(contentId));
		Serializer().loadInstances(*stream, instances);
		hasError = false;
	}
	catch (...)
	{
		hasError = true;
	}
}

#ifdef ENABLE_DRAG_DROP_TOOLBOX
bool ARLWorkspace::StartDrag(const QString& urlQStr)
{
	try
	{
		if (!m_pDataModel)
			throw std::runtime_error("Can't insert at this time");	
				
		bool success = ARL::Http::trustCheck(urlQStr.toLocal8Bit().data());
		if(success)
		{
			QDrag *pDrag = new QDrag(qobject_cast<QWidget*>(m_pParent));
			QMimeData *pMimeData = new QMimeData;
			
			QList<QUrl> listUrl;
			listUrl << urlQStr;
			
			pMimeData->setUrls(listUrl);
			pDrag->setMimeData(pMimeData);
			
			QPixmap pixMap(1, 1);
			pDrag->setPixmap(pixMap);
			
#ifdef Q_OS_MAC
			pDrag->exec();	
#else
			pDrag->exec(Qt::CopyAction);
#endif
			pDrag->deleteLater();
		}	
		
	}
	catch (std::exception const& e)
	{
		ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR, e.what());
		return false;
	}
	
	return true;
	
}
#endif

bool ARLWorkspace::OpenRecentFile(const QString &recentFileName)
{
	return UpdateUIManager::Instance().getMainWindow().openRecentFile(recentFileName);
}



bool ARLWorkspace::OpenPicFolder()
{
	QString userPictureFolder = QtUtilities::qstringFromStdString(ARL::FileSystem::getUserDirectory(true, ARL::DirPicture).native());
	QDir pictureDir(userPictureFolder);

	if (!pictureDir.exists())
		pictureDir.mkpath(userPictureFolder);

	QDesktopServices::openUrl(QUrl::fromLocalFile(userPictureFolder));
	
	return true;
}

void ARLWorkspace::PostImage(bool doPost, int postSetting, const QString &fileName, const QString &seoStr)
{
	ARL::GameSettings::singleton().setPostImageSetting((ARL::GameSettings::UploadSetting)postSetting);
	if (doPost)
		ScreenshotVerb::DoPostImage(shared_from(m_pDataModel), fileName, seoStr);
}

bool ARLWorkspace::TakeThumbnail(QString selector)
{
	m_selectorForFileUpload = selector;
	if (m_screenShotInProgressMsgBox)
		return false;

	// Hide the current web dialog
	Hide();

	// Pop up a message box that they can click OK on to take a photo
	QMessageBox* msgBox = new QMessageBox();
	msgBox->setAttribute( Qt::WA_DeleteOnClose );
	msgBox->setStandardButtons( QMessageBox::Ok );
	msgBox->setWindowTitle( tr("Take Snapshot") );
	msgBox->setIconPixmap(QPixmap(":/16x16/images/Studio 2.0 icons/16x16/picture-icon.png"));
	msgBox->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	msgBox->setText("<p align='center'>Press OK when you've positioned the camera, and are ready to take a snapshot.</p>");
	msgBox->setModal( false );
	msgBox->show(); // Has to be before moving of msg box or we get wrong dimensions

	// Position the message box
	ANORRLMainWindow& mainWindow = UpdateUIManager::Instance().getMainWindow();
	float winHeight = mainWindow.height();
	winHeight /= 1.3f;
	float winWidth = mainWindow.width();
	winWidth = winWidth - msgBox->frameGeometry().width();
	winWidth /= 2.0f;
	msgBox->move(winWidth, winHeight);

	// On OK Click, handle the thumbnail
	connect(msgBox, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(handleTakeThumbnail(QAbstractButton*)));

	return true;
}
void ARLWorkspace::onScreenShotFinished_MT(QString fileName)
{
	WebDialog* pWebDialog = dynamic_cast<WebDialog*>(m_pParent);

	// Delete our processing dialog
	if (m_screenShotInProgressMsgBox)
	{
		m_screenShotInProgressMsgBox->hide();
		m_screenShotInProgressMsgBox->deleteLater();
		m_screenShotInProgressMsgBox = NULL;
	}

	// Re-show the web page
	Show();

	// Tell the page we've added the thumb
	QImage image(fileName);
	Q_EMIT thumbnailTaken(image.width(), image.height());
}
void ARLWorkspace::onScreenshotFinished(const std::string &fileName)
{	
	// Re-enable other handling of screenshots 
	ANORRLIDEDoc* pIDEDoc = ANORRLDocManager::Instance().getPlayDoc();
	ScreenshotVerb* pVerb = dynamic_cast<ScreenshotVerb*>(pIDEDoc->getVerb("Screenshot"));
	shared_ptr<ARL::DataModel> pDataModel = shared_from(pIDEDoc->getDataModel());
	pDataModel->screenshotReadySignal.disconnectAll();
	pVerb->reconnectScreenshotSignal();

	QString qFileName = QString::fromStdString(fileName);

	// Switch it to the main thread
	if (QThread::currentThread() != qApp->thread())
	{	
		QMetaObject::invokeMethod(
            this,
            "onScreenShotFinished_MT",
            Qt::QueuedConnection,
            Q_ARG(QString, qFileName));
		return;
	}
	else
	{
		ARLASSERT(0);// Should never be here, but if so, handle it
		onScreenShotFinished_MT(qFileName);	
	}
}
void ARLWorkspace::handleTakeThumbnail(QAbstractButton*)
{
	ANORRLIDEDoc* pIDEDoc = ANORRLDocManager::Instance().getPlayDoc();
	if (!pIDEDoc)
		return;

	// Disabling other handling of screenshots
	shared_ptr<ARL::DataModel> pDataModel = shared_from(pIDEDoc->getDataModel());
	pDataModel->screenshotReadySignal.disconnectAll();
	pDataModel->screenshotReadySignal.connect(boost::bind(&ARLWorkspace::onScreenshotFinished, this, _1));
	pDataModel->submitTask( boost::bind(&ARL::DataModel::TakeScreenshotTask, weak_ptr<ARL::DataModel>(pDataModel)), ARL::DataModelJob::Write );

	// Give them a processing dialog
	m_screenShotInProgressMsgBox = new QMessageBox(&UpdateUIManager::Instance().getMainWindow());
	m_screenShotInProgressMsgBox->setAttribute( Qt::WA_DeleteOnClose ); 
	m_screenShotInProgressMsgBox->setStandardButtons( QMessageBox::NoButton );
	m_screenShotInProgressMsgBox->setWindowTitle( tr("Processing...") );
	m_screenShotInProgressMsgBox->setText( tr("Snapshot in progress...") );
	m_screenShotInProgressMsgBox->setWindowModality(Qt::NonModal);
	m_screenShotInProgressMsgBox->show();
}

void ARLWorkspace::OpenUniverse(int universeId)
{
	QAction* action = UpdateUIManager::Instance().getDockAction(eDW_GAME_EXPLORER);
	if (!action->isChecked())
	{
		action->activate(QAction::Trigger);
	}
    QMetaObject::invokeMethod(&UpdateUIManager::Instance().getViewWidget<ANORRLGameExplorer>(eDW_GAME_EXPLORER),
        "openGameFromGameId", Qt::QueuedConnection, Q_ARG(int, universeId));
}

void ARLWorkspace::PublishAsNewUniverse()
{
	QMetaObject::invokeMethod(&UpdateUIManager::Instance().getViewWidget<ANORRLGameExplorer>(eDW_GAME_EXPLORER),
		"publishGameToNewSlot", Qt::QueuedConnection);
}

void ARLWorkspace::PublishAsNewGroupUniverse(int groupId)
{
	QMetaObject::invokeMethod(&UpdateUIManager::Instance().getViewWidget<ANORRLGameExplorer>(eDW_GAME_EXPLORER),
		"publishGameToNewGroupSlot", Qt::QueuedConnection, Q_ARG(int, groupId));
}

void ARLWorkspace::PublishToUniverse(int universeId)
{
	QMetaObject::invokeMethod(&UpdateUIManager::Instance().getViewWidget<ANORRLGameExplorer>(eDW_GAME_EXPLORER),
		"publishGameToExistingSlot", Qt::QueuedConnection, Q_ARG(int, universeId));
}

void ARLWorkspace::ImportAsset(int assetId)
{
	if (!FFlag::StudioEnableGameAnimationsTab)
		return;

	ImportAssetDialog* pImportDlg = dynamic_cast<ImportAssetDialog*>(m_pParent);
	if (pImportDlg)
	{
		pImportDlg->setAssetId(assetId);
		pImportDlg->close();
	}
}

QString ARLWorkspace::GetGameAnimations(int universeId)
{
	if (!FFlag::StudioEnableGameAnimationsTab)
		return QString();

	ANORRLGameExplorer& gameExplorer = UpdateUIManager::Instance().getViewWidget<ANORRLGameExplorer>(eDW_GAME_EXPLORER);
	return gameExplorer.getAnimationsDataJson();
}