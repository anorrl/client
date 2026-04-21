/**
 *  RobloxStudioVerbs.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "RobloxStudioVerbs.h"

// Qt Headers
#include <QByteArray>
#include <QDataStream>
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QProcess>
#include <QTextStream>
#include <QImage>
#include <QFile>
#include <QUrl>
#include <QDialogButtonBox>
#include <QTime>
#include <QDockWidget>
#include <QDialog>

// 3rd Party Headers
#include "G3D/System.h"
#include <boost/unordered_map.hpp>

// Roblox Headers
#include "V8Datamodel/Animation.h"
#include "V8DataModel/BasicPartInstance.h"
#include "V8Datamodel/ChangeHistory.h"
#include "V8Datamodel/ContentProvider.h"
#include "V8DataModel/CSGDictionaryService.h"
#include "V8Datamodel/DataModel.h"
#include "V8Datamodel/Decal.h"
#include "V8Datamodel/GameBasicSettings.h"
#include "V8Datamodel/KeyframeSequence.h"
#include "V8Datamodel/KeyframeSequenceProvider.h"
#include "V8DataModel/NonReplicatedCSGDictionaryService.h"
#include "V8DataModel/PartOperation.h"
#include "V8DataModel/PartOperationAsset.h"
#include "V8Datamodel/PhysicsSettings.h"
#include "V8Datamodel/PlayerGui.h"
#include "V8Datamodel/SpecialMesh.h"
#include "V8Datamodel/Tool.h"
#include "V8Datamodel/Visit.h"
#include "v8DataModel/Workspace.h"
#include "v8datamodel/ToolsPart.h"
#include "V8Tree/Instance.h"
#include "V8Xml/SerializerBinary.h"
#include "V8Xml/WebParser.h"
#include "V8Xml/XmlSerializer.h"
#include "network/api.h"
#include "network/Players.h"
#include "tool/AdvMoveTool.h"
#include "tool/AdvRotateTool.h"
#include "util/Statistics.h"
#include "script/LuaSourceContainer.h"
#include "script/ScriptContext.h"
#include "script/script.h"
#include "rbx/Log.h"
#include "tool/ToolsArrow.h"
#include "Util/RobloxGoogleAnalytics.h"
#include "GeometryGenerator.h"
#include "../CSG/CSGKernel.h"
#include "RobloxServicesTools.h"
#include "tool/MoveResizeJoinTool.h"

// Roblox Studio Headers
#include "StudioUtilities.h"
#include "WebDialog.h"
#include "RbxWorkspace.h"
#include "RobloxSettings.h"
#include "RobloxDocManager.h"
#include "CommonInsertWidget.h"
#include "RenderSettingsItem.h"
#include "QtUtilities.h"
#include "RobloxMainWindow.h"
#include "UpdateUIManager.h"
#include "RobloxApplicationManager.h"
#include "RobloxIDEDoc.h"
#include "StudioSerializerHelper.h"
#include "AuthenticationHelper.h"
#include "Roblox.h"
#include "NameValueStoreManager.h"
#include "RobloxUser.h"
#include "RobloxToolBox.h"
#include "RobloxContextualHelp.h"
#include "GfxBase/ViewBase.h"
#include "CSGOperations.h"
#include "RobloxGameExplorer.h"
#include "ScriptPickerDialog.h"

#include "v8xml/SerializerBinary.h"

//Video record related includes
#ifdef _WIN32
    #include "VideoControl.h"
    #include "DSVideoCaptureEngine.h"
#endif
#include "ManageEmulationDeviceDialog.h"

FASTFLAG(PrefetchResourcesEnabled)
FASTFLAG(LuaDebugger)
FASTFLAG(GoogleAnalyticsTrackingEnabled)

FASTFLAG(StudioCSGAssets)
FASTFLAG(StudioNewWiki)
FASTFLAG(GameExplorerUseV2AliasEndpoint)
FASTFLAG(StudioEnableGameAnimationsTab)
FASTFLAG(PhysicsAnalyzerEnabled)

DYNAMIC_FASTFLAG(DraggerUsesNewPartOnDuplicate)
DYNAMIC_FASTFLAG(RestoreTransparencyOnToolChange)

FASTFLAGVARIABLE(StudioMimeDataContainsInstancePath, false)
FASTFLAGVARIABLE(StudioOperationFailureHangFix, true)

FASTINTVARIABLE(StudioWebDialogMinimumWidth, 970)
FASTINTVARIABLE(StudioWebDialogMinimumHeight, 480)

DYNAMIC_FASTFLAG(UseRemoveTypeIDTricks)

static const char* sCollisionToggleModeSetting = "CollisionToggleMode";
static const char* sLocalTranslationModeSetting = "LocalTranslationMode";
static const char* sLocalRotationModeSetting = "LocalRotationMode";
static const char* sRibbonJointCreationMode = "rbxRibbonJointCreationMode";
static const char* sRibbonStartServerSetting = "rbxRibbonStartServer";
static const char* sRibbonNumPlayerSetting = "rbxRibbonNumPlayer";

static const char* sRobloxMimeType = "application/x-roblox-studio";

bool StudioMaterialVerb::sMaterialActionActAsTool = false;
bool StudioColorVerb::sColorActionActAsTool = false;

using namespace ARL;

GroupSelectionVerb::GroupSelectionVerb(DataModel* dataModel) :
	EditSelectionVerb("Group", dataModel)
{;}

bool GroupSelectionVerb::isEnabled() const
{
	if (!Super::isEnabled())
		return false;
	else {
		Selection* sel = ServiceProvider::find< Selection >(dataModel);
		return sel && sel->size() > 0;
	}
}

void GroupSelectionVerb::doIt(IDataState* dataState)
{
	FASTLOG(FLog::Verbs, "Gui:GroupSelection");

	Selection* sel = ServiceProvider::create< Selection >(dataModel);

	ModelInstance* groupInstance = workspace->group(sel->begin(), sel->end()).get();

	sel->setSelection(groupInstance);

	// TODO: Undo/Redo...
	{
		DataModel::LegacyLock lock(dataModel, DataModelJob::Write);
		ChangeHistoryService::requestWaypoint(getName().c_str(), workspace.get());
	}

	dataState->setDirty(true);
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

namespace
{
static void operationFailed(const char* title, const char* msg)
{
    QMetaObject::invokeMethod(&UpdateUIManager::Instance(),
                              "showErrorMessage",
                              FFlag::StudioOperationFailureHangFix ? Qt::QueuedConnection : Qt::BlockingQueuedConnection,
                              Q_ARG(QString, title),
                              Q_ARG(QString, msg));
}
} // anonymous namespace
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

UngroupSelectionVerb::UngroupSelectionVerb(DataModel* dataModel) :
	EditSelectionVerb("UnGroup", dataModel)
{
	// Force creation here, rather than in isEnabled
	ServiceProvider::create< FilteredSelection<ModelInstance> >(dataModel);
}

bool canUngroup(ModelInstance* wInstance)
{
	return (wInstance->numChildren() > 0);
}

bool UngroupSelectionVerb::isEnabled() const
{
	if (!Super::isEnabled())
		return false;

	// enabled if one ore more selected items has children and can be ungrouped
	Selection* selection = ServiceProvider::create< Selection >(dataModel);

	for (std::vector<boost::shared_ptr<ARL::Instance> >::const_iterator iter = selection->begin(); iter != selection->end(); ++iter)
	{
		if (shared_ptr<ModelInstance> m = ARL::Instance::fastSharedDynamicCast<ARL::ModelInstance>(*iter))
			return true;
	}
	return false;
}

class Ungroup
{
public:
	std::vector<Instance*>& ungroupedItems;
	bool& didSomething;
	Ungroup(std::vector<Instance*>& ungroupedItems, bool& didSomething)
		:ungroupedItems(ungroupedItems)
		,didSomething(didSomething)
	{
	}

	void operator()(const shared_ptr<Instance>& wInstance)
	{
		if (wInstance->numChildren() > 0) {
			for (size_t j = 0; j < wInstance->numChildren(); ++j) {
				ungroupedItems.push_back(wInstance->getChild(j));
			}
			wInstance->promoteChildren();
			wInstance->setParent(NULL);
			didSomething = true;
		}
	}
};

void UngroupSelectionVerb::doIt(IDataState* dataState)
{
	FASTLOG(FLog::Verbs, "Gui:UngroupSelection");

	Selection* selection = ServiceProvider::create< Selection >(dataModel);

	// First make a copy of the selection list
	std::vector<shared_ptr<Instance> > itemsToUngroup;
	for (std::vector<boost::shared_ptr<ARL::Instance> >::const_iterator iter = selection->begin(); iter != selection->end(); ++iter)
	{
		if (shared_ptr<ModelInstance> m = ARL::Instance::fastSharedDynamicCast<ARL::ModelInstance>(*iter))
			itemsToUngroup.push_back(m);
	}

	// Now iterate over the objects to be ungrouped and ungroup them
	std::vector<Instance*> ungroupedItems;
	bool didSomething = false;
	std::for_each(itemsToUngroup.begin(), itemsToUngroup.end(), Ungroup(ungroupedItems, didSomething));

	if (didSomething) {
		selection->setSelection(ungroupedItems.begin(), ungroupedItems.end());
	}
	else {
		debugAssertM(0, "Calling ungroup command without checking is-enabled");
	}

	// TODO: Undo/Redo...
	{
		DataModel::LegacyLock lock(dataModel, DataModelJob::Write);
		ChangeHistoryService::requestWaypoint(getName().c_str(), workspace.get());
	}

	dataState->setDirty(true);
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

UnionSelectionVerb::UnionSelectionVerb(DataModel* dataModel)
	: EditSelectionVerb("UnionSelection", dataModel)
{}

bool UnionSelectionVerb::isEnabled() const
{
    if (!Super::isEnabled())
		return false;
	else
	{
		Selection* sel = ServiceProvider::find<Selection>(dataModel);
		return sel && ((sel->size() > 0 && !(sel->size() == 1 && ARL::Instance::fastSharedDynamicCast<ARL::PartOperation>(*sel->begin()))) ||
            (sel->size() == 1 && ARL::Instance::fastSharedDynamicCast<ARL::BasicPartInstance>(*sel->begin())));
	}
}

void UnionSelectionVerb::performUnion(IDataState* dataState)
{
	DataModel::LegacyLock lock(dataModel, ARL::DataModelJob::Write);
	FASTLOG(FLog::Verbs, "Gui:UnionSelection");

    CSGOperations csgOps(dataModel, operationFailed);
    Selection* selection = ServiceProvider::create< Selection >(dataModel);
    shared_ptr<ARL::PartOperation> partOperation;
    if ( csgOps.doUnion( selection->begin(), selection->end(), partOperation ) )
        ChangeHistoryService::requestWaypoint(getName().c_str(), workspace.get());
    if ( partOperation )
        selection->setSelection(partOperation.get());
	dataState->setDirty(true);
}

void UnionSelectionVerb::doIt(IDataState* dataState)
{
    UpdateUIManager::Instance().waitForLongProcess("Union", boost::bind(&UnionSelectionVerb::performUnion, this, dataState) );
}

NegateSelectionVerb::NegateSelectionVerb(DataModel* dataModel)
	: EditSelectionVerb("NegateSelection", dataModel)
{}

bool NegateSelectionVerb::isEnabled() const
{
	if (!Super::isEnabled())
		return false;
	else
	{
		Selection* sel = ServiceProvider::find<Selection>(dataModel);
		return sel && sel->size() > 0;
	}
}

void NegateSelectionVerb::doIt(IDataState* dataState)
{
    FASTLOG(FLog::Verbs, "Gui:NegateSelection");

    CSGOperations csgOps(dataModel, operationFailed);
    Selection* selection = ServiceProvider::create< Selection >(dataModel);
    std::vector<shared_ptr<ARL::Instance> > toSelect;
    if ( csgOps.doNegate(selection->begin(), selection->end(), toSelect) )
    {
		DataModel::LegacyLock lock(dataModel, DataModelJob::Write);
		ChangeHistoryService::requestWaypoint(getName().c_str(), workspace.get());
	}
    selection->setSelection(toSelect.begin(), toSelect.end());
	dataState->setDirty(true);
}

SeparateSelectionVerb::SeparateSelectionVerb(DataModel* dataModel)
	: EditSelectionVerb("SeparateSelection", dataModel)
{}

bool SeparateSelectionVerb::isEnabled() const
{
	if (!Super::isEnabled())
		return false;

	// enabled if one ore more selected items has children and can be ungrouped
	Selection* selection = ServiceProvider::create< Selection >(dataModel);

	for (std::vector<boost::shared_ptr<ARL::Instance> >::const_iterator iter = selection->begin(); iter != selection->end(); ++iter)
		if (shared_ptr<PartOperation> o = ARL::Instance::fastSharedDynamicCast<PartOperation>(*iter))
			return true;

	return false;
}

void SeparateSelectionVerb::doIt(IDataState* dataState)
{
    UpdateUIManager::Instance().waitForLongProcess("Separate", boost::bind(&SeparateSelectionVerb::performSeparate, this, dataState));
}

void SeparateSelectionVerb::performSeparate(IDataState* dataState)
{
	DataModel::LegacyLock lock(dataModel, ARL::DataModelJob::Write);
	FASTLOG(FLog::Verbs, "Gui:SeparateSelection");

    CSGOperations csgOps( dataModel, operationFailed );
    Selection* selection = ServiceProvider::create< Selection >(dataModel);
    std::vector<shared_ptr<Instance> > ungroupedItems;

    if (csgOps.doSeparate(selection->begin(), selection->end(), ungroupedItems))
        ARL::ChangeHistoryService::requestWaypoint(getName().c_str(), workspace.get());

    selection->setSelection(ungroupedItems.begin(), ungroupedItems.end());
    dataState->setDirty(true);
}

CutVerb::CutVerb(ARL::DataModel* dataModel)
: DeleteSelectionVerb(dataModel, dataModel, "Cut")
{}

void CutVerb::doIt(ARL::IDataState* dataState)
{
	// First copy selection to clipboard
	ARL::Verb *pCopyVerb = getContainer()->getVerb("Copy");
	if (!pCopyVerb)
		return;

	pCopyVerb->doIt(dataState);

	//now call delete
	DeleteSelectionVerb::doIt(dataState);
}

CopyVerb::CopyVerb(ARL::DataModel* dataModel)
: EditSelectionVerb("Copy", dataModel)
{}

void CopyVerb::doIt(ARL::IDataState*)
{
	QClipboard *pClipboard = QApplication::clipboard();
	pClipboard->clear();

	ARL::Selection  *pSelection = ARL::ServiceProvider::create<ARL::Selection>(dataModel);

	ARL::CSGDictionaryService* dictionaryService = ARL::ServiceProvider::create< ARL::CSGDictionaryService >(dataModel);
	ARL::NonReplicatedCSGDictionaryService* nrDictionaryService = ARL::ServiceProvider::create<ARL::NonReplicatedCSGDictionaryService>(dataModel);
	for (std::vector<shared_ptr<ARL::Instance> >::const_iterator iter = pSelection->begin(); iter != pSelection->end(); ++iter)
	{
		dictionaryService->retrieveAllDescendants(*iter);
		nrDictionaryService->retrieveAllDescendants(*iter);
	}

    std::ostringstream selectionStream;

    ARL::Instances instances(pSelection->begin(), pSelection->end());
    ARL::SerializerBinary::serialize(selectionStream, instances);

    std::string contents = selectionStream.str();

    QMimeData* pMimeDataForClipboard = new QMimeData;
	pMimeDataForClipboard->setData(sRobloxMimeType,QByteArray(contents.data(), contents.length()));


	for (std::vector<shared_ptr<ARL::Instance> >::const_iterator iter = pSelection->begin(); iter != pSelection->end(); ++iter)
	{
		dictionaryService->storeAllDescendants(*iter);
		nrDictionaryService->storeAllDescendants(*iter);
	}

    // enable following code for debugging
	if (FFlag::StudioMimeDataContainsInstancePath && pSelection->size() == 1)
		pMimeDataForClipboard->setText(("Game." + (*pSelection->begin())->getFullName()).c_str());

	//set mime data in clipboard
	pClipboard->setMimeData(pMimeDataForClipboard);
}

PasteVerb::PasteVerb(ARL::DataModel* dataModel, bool pasteInto)
: ARL::Verb(dataModel, pasteInto ? "PasteInto" : "Paste")
, m_pDataModel(dataModel)
, m_bPasteInto(pasteInto)
{}

bool PasteVerb::isEnabled() const
{
    if (!isPasteInfoAvailable())
		return false;

	if (!m_bPasteInto)
		return true;

	//for paste into (used in tree view contextual menu)
	ARL::Selection* pSelection = ARL::ServiceProvider::create<ARL::Selection>(m_pDataModel);
	return (pSelection && pSelection->size()==1);
}

void PasteVerb::onClipboardModified()
{	m_bIsPasteInfoAvailable = isPasteInfoAvailable(); }

bool PasteVerb::isPasteInfoAvailable() const
{
	QClipboard      *pClipboard = QApplication::clipboard();
	if (pClipboard)
	{
		const QMimeData *pMimeData  = pClipboard->mimeData();
		if (pMimeData && pMimeData->hasFormat(sRobloxMimeType))
			return true;
	}
	return false;
}

void PasteVerb::doIt(ARL::IDataState* dataState)
{
	shared_ptr<ARL::Instances> itemsToPaste(new Instances);
	createInstancesFromClipboard(itemsToPaste);

	if (itemsToPaste->empty())
		return;

	insertInstancesIntoParent(itemsToPaste);

	// Save Undo/Redo state
	{
		DataModel::LegacyLock lock(m_pDataModel, DataModelJob::Write);
		ARL::ChangeHistoryService::requestWaypoint(getName().c_str(), m_pDataModel);
	}

	dataState->setDirty(true);
}

void PasteVerb::createInstancesFromClipboard(shared_ptr<ARL::Instances> itemsToPaste)
{
	QClipboard      *pClipboard = QApplication::clipboard();
	const QMimeData *pMimeData  = pClipboard->mimeData();
	if (!pMimeData || !pMimeData->hasFormat(sRobloxMimeType))
		return;

	QByteArray robloxData = pMimeData->data(sRobloxMimeType);
	std::istringstream stream(std::string(robloxData.data(), robloxData.data() + robloxData.size()));

	try
	{
		Serializer().loadInstances(stream, *itemsToPaste);
	}
	catch (...)
	{
		//clear clipboard
		pClipboard->clear();
	}
}

void PasteVerb::insertInstancesIntoParent(shared_ptr<ARL::Instances> itemsToPaste)
{
	ARL::Selection* pSelection = ARL::ServiceProvider::create<ARL::Selection>(m_pDataModel);
	if (!pSelection)
		return;

	shared_ptr<ARL::Instance> sel = pSelection->front();
	ARL::Instance* pParentInstance;

	bool isDecal = false;

	isDecal = itemsToPaste->size() == 1 && ARL::Instance::fastSharedDynamicCast<ARL::Decal>(*itemsToPaste->begin());

	if (pSelection->size()!=1 || (isDecal && !m_bPasteInto))
		pParentInstance = m_pDataModel->getWorkspace();
	else
		pParentInstance = m_bPasteInto || !sel->getParent() || !sel->getParent()->getParent() ? sel.get() : sel->getParent();

	if (!pParentInstance)
		return;

	//insert items into appropriate parent
	ARL::InsertMode insertMode = m_bPasteInto ? ARL::INSERT_TO_TREE : ARL::INSERT_TO_3D_VIEW;
	if (isDecal)
		insertMode = ARL::INSERT_TO_TREE;

	m_pDataModel->getWorkspace()->insertPasteInstances(*itemsToPaste, pParentInstance, insertMode, ARL::SUPPRESS_PROMPTS);
	pSelection->setSelection(itemsToPaste);
}

void PasteVerb::createInstancesFromClipboardDep(ARL::Instances& itemsToPaste)
{
	QClipboard      *pClipboard = QApplication::clipboard();
	const QMimeData *pMimeData  = pClipboard->mimeData();
	if (!pMimeData || !pMimeData->hasFormat(sRobloxMimeType))
		return;

	QByteArray robloxData = pMimeData->data(sRobloxMimeType);
	std::istringstream stream(std::string(robloxData.data(), robloxData.data() + robloxData.size()));

	try
	{
		Serializer().loadInstances(stream, itemsToPaste);
	}
	catch (...)
	{
		//clear clipboard
		pClipboard->clear();
	}
}

void PasteVerb::insertInstancesIntoParentDep(ARL::Instances& itemsToPaste)
{
	ARL::Selection* pSelection = ARL::ServiceProvider::create<ARL::Selection>(m_pDataModel);
	if (!pSelection)
		return;

	shared_ptr<ARL::Instance> sel = pSelection->front();
	ARL::Instance* pParentInstance;

	bool isDecal = false;

	isDecal = itemsToPaste.size() == 1 && ARL::Instance::fastSharedDynamicCast<ARL::Decal>(*itemsToPaste.begin());

	if (pSelection->size()!=1 || (isDecal && !m_bPasteInto))
		pParentInstance = m_pDataModel->getWorkspace();
	else
		pParentInstance = m_bPasteInto || !sel->getParent() || !sel->getParent()->getParent() ? sel.get() : sel->getParent();

	if (!pParentInstance)
		return;

	//insert items into appropriate parent
	ARL::InsertMode insertMode = m_bPasteInto ? ARL::INSERT_TO_TREE : ARL::INSERT_TO_3D_VIEW;
	if (isDecal)
		insertMode = ARL::INSERT_TO_TREE;

	m_pDataModel->getWorkspace()->insertPasteInstances(itemsToPaste, pParentInstance, insertMode, ARL::SUPPRESS_PROMPTS);
	pSelection->setSelection(itemsToPaste.begin(), itemsToPaste.end());
}

DuplicateSelectionVerb::DuplicateSelectionVerb(ARL::DataModel* dataModel)
	: EditSelectionVerb("Duplicate", dataModel)
{}

void DuplicateSelectionVerb::doIt(ARL::IDataState*)
{
	ARL::Selection* selection = ARL::ServiceProvider::create<ARL::Selection>(dataModel);
	if (!selection)
		return;

	if (DFFlag::RestoreTransparencyOnToolChange)
		ARL::AdvArrowToolBase::restoreSavedPartsTransparency();

	shared_ptr<Instances> instances(new Instances);
	std::vector<ARL::PVInstance*> pvInstances;

	CSGDictionaryService* dictionaryService = ARL::ServiceProvider::create<CSGDictionaryService>(dataModel);
	NonReplicatedCSGDictionaryService* nrDictionaryService = ARL::ServiceProvider::create<NonReplicatedCSGDictionaryService>(dataModel);

	for (std::vector< shared_ptr<ARL::Instance> >::const_iterator iter = selection->begin(); iter != selection->end(); ++iter)
	{
		if (shared_ptr<PartOperation> partOperation = ARL::Instance::fastSharedDynamicCast<PartOperation>(*iter))
		{
			dictionaryService->retrieveData(*partOperation);
			nrDictionaryService->retrieveData(*partOperation);
		}

		if ((*iter)->getParent() && (*iter)->getParent()->getParent() && !(*iter)->getIsParentLocked())
		{
			if (shared_ptr<ARL::Instance> clonedInstance = (*iter)->clone(ARL::SerializationCreator))
			{
				clonedInstance->setParent((*iter)->getParent());

				instances->push_back(clonedInstance);

				if (DFFlag::DraggerUsesNewPartOnDuplicate)
					MoveResizeJoinTool::setSelection(*iter, clonedInstance);

				if (ARL::AdvArrowTool::advCollisionCheckMode && clonedInstance->isDescendantOf(dataModel->getWorkspace()))
					if (shared_ptr<ARL::PVInstance> pvInstance = ARL::Instance::fastSharedDynamicCast<ARL::PVInstance>(clonedInstance))
						pvInstances.push_back(pvInstance.get());
			}
		}

		if (shared_ptr<PartOperation> partOperation = ARL::Instance::fastSharedDynamicCast<PartOperation>(*iter))
		{
			dictionaryService->storeData(*partOperation);
			nrDictionaryService->storeData(*partOperation);
		}
	}

	if (ARL::AdvArrowTool::advCollisionCheckMode && pvInstances.size() > 0)
	{
		ARL::MegaDragger megaDragger(NULL, pvInstances, dataModel->getWorkspace());
		megaDragger.startDragging();
		megaDragger.safeMoveNoDrop(ARL::Vector3(0.0f, 0.0f, 0.0f));
		megaDragger.finishDragging();
	}

	if (instances->size() == 0)
		return;

	selection->setSelection(instances);

	{
		DataModel::LegacyLock lock(dataModel, DataModelJob::Write);
		ARL::ChangeHistoryService::requestWaypoint(getName().c_str(), dataModel);
	}

	dataModel->setDirty(true);
}

UndoVerb::UndoVerb(ARL::VerbContainer* pVerbContainer)
: Verb(pVerbContainer, "UndoVerb")
, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
{
	m_pChangeHistory = ARL::shared_from(m_pDataModel->create<ARL::ChangeHistoryService>());
	ARLASSERT(m_pChangeHistory);
}

void UndoVerb::doIt(ARL::IDataState*)
{
    UpdateUIManager::Instance().waitForLongProcess(
        "Undo",
		boost::bind(&ARL::ChangeHistoryService::unplay,m_pChangeHistory.get()) );
}

bool UndoVerb::isEnabled() const
{	return m_pChangeHistory->canUnplay(); }

std::string UndoVerb::getText() const
{
	std::string name;
	m_pChangeHistory->getUnplayWaypoint(name);

	return std::string("Undo " + name);
}

RedoVerb::RedoVerb(ARL::VerbContainer* pVerbContainer)
: Verb(pVerbContainer, "RedoVerb")
, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
{
	m_pChangeHistory = ARL::shared_from(m_pDataModel->create<ARL::ChangeHistoryService>());
	ARLASSERT(m_pChangeHistory);
}

void RedoVerb::doIt(ARL::IDataState*)
{
    UpdateUIManager::Instance().waitForLongProcess(
        "Redo",
		boost::bind(&ARL::ChangeHistoryService::play,m_pChangeHistory.get()) );
}

bool RedoVerb::isEnabled() const
{	return m_pChangeHistory->canPlay(); }

std::string RedoVerb::getText() const
{
	std::string name;
	m_pChangeHistory->getPlayWaypoint(name);

	return std::string("Redo " + name);
}

InsertModelVerb::InsertModelVerb(ARL::VerbContainer* pVerbContainer, bool insertInto)
: Verb(pVerbContainer, insertInto ? "InsertIntoFromFileVerb" :"InsertModelVerb")
, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
, m_bInsertInto(insertInto)
{}

void InsertModelVerb::doIt(ARL::IDataState*)
{
	insertModel();
}

void InsertModelVerb::insertModel()
{
	RobloxSettings settings;

	QString rbxmLastDir = settings.value("rbxm_last_directory").toString();
	if ( rbxmLastDir.isEmpty() )
        rbxmLastDir = RobloxMainWindow::getDefaultSavePath();

	QString dlgTitle = m_bInsertInto ? QObject::tr("Open file to Insert") : QObject::tr("Open Roblox Model");
	QString fileExtn = m_bInsertInto ? QObject::tr("Roblox Model Files (*.rbxm *.rbxmx);;Scripts (*.rbxs *.lua *.txt)") : QObject::tr("Roblox Model Files (*.rbxm *.rbxmx)");

	QString fileName;

	fileName = QFileDialog::getOpenFileName(
		&UpdateUIManager::Instance().getMainWindow(),
		dlgTitle,
		rbxmLastDir,
		fileExtn);

	if (fileName.isEmpty())
		return;

	if (fileName.endsWith(".rbxm", Qt::CaseInsensitive) || fileName.endsWith(".rbxmx", Qt::CaseInsensitive))
	    StudioUtilities::insertModel(ARL::shared_from(m_pDataModel), fileName, m_bInsertInto);
	else
		StudioUtilities::insertScript(ARL::shared_from(m_pDataModel), fileName);

	settings.setValue("rbxm_last_directory", QFileInfo(fileName).absolutePath());
}

SelectionSaveToFileVerb::SelectionSaveToFileVerb(ARL::VerbContainer* pVerbContainer)
: Verb(pVerbContainer, "SelectionSaveToFile")
, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
{}

void SelectionSaveToFileVerb::doIt(ARL::IDataState*)
{
	saveToFile();
}

void SelectionSaveToFileVerb::saveToFile()
{
	RobloxSettings settings;

	QString rbxmLastDir = settings.value("rbxm_last_directory").toString();
	if ( rbxmLastDir.isEmpty() )
        rbxmLastDir = RobloxMainWindow::getDefaultSavePath();

    QString fileExtnModels("Roblox XML Model Files (*.rbxmx);;Roblox Model Files (*.rbxm)");
    QString fileExtnScripts("Roblox Lua Scripts (*.lua)");

	QString fileExtn = fileExtnModels;

	ARL::Selection* pSelection = ARL::ServiceProvider::create<ARL::Selection>(m_pDataModel);

	if (pSelection && (pSelection->size() == 1) && ARL::Instance::fastDynamicCast<ARL::Script>(pSelection->front().get()))
	{
		//update default file name for save
		rbxmLastDir.append("/");
		rbxmLastDir.append(pSelection->front()->getName().c_str());

		//if script doesn't have any children then default to lua
		if (!pSelection->front()->numChildren())
		{
            fileExtn = fileExtnScripts + ";;" + fileExtnModels;
            rbxmLastDir.append(".lua");
        }
		else
		{
            fileExtn = fileExtnModels + ";;" + fileExtnScripts;
			rbxmLastDir.append(".rbxm");
		}
	}

	QString fileName = QFileDialog::getSaveFileName(
		&UpdateUIManager::Instance().getMainWindow(),
		"Save As",
		rbxmLastDir,
		fileExtn);

	if (fileName.isEmpty())
		return;

	DataModel::LegacyLock lock(m_pDataModel, DataModelJob::Write);

	ARL::CSGDictionaryService* dictionaryService = ARL::ServiceProvider::create<ARL::CSGDictionaryService>(m_pDataModel);
	ARL::NonReplicatedCSGDictionaryService* nrDictionaryService = ARL::ServiceProvider::create<ARL::NonReplicatedCSGDictionaryService>(m_pDataModel);

	for (std::vector<shared_ptr<ARL::Instance> >::const_iterator iter = pSelection->begin(); iter != pSelection->end(); ++iter)
	{
		dictionaryService->retrieveAllDescendants(*iter);
		nrDictionaryService->retrieveAllDescendants(*iter);
	}

	if (fileName.endsWith(".rbxm", Qt::CaseInsensitive) || fileName.endsWith(".rbxmx", Qt::CaseInsensitive))
	{
	    QByteArray ba = fileName.toAscii();
	    const char *c_str = ba.constData();

	    // Stream the XML data
	    std::ofstream stream;
	    stream.open(c_str, std::ios_base::out | std::ios::binary);

        const bool useBinaryFormat = !(fileName.endsWith(".rbxmx", Qt::CaseInsensitive));

        if (useBinaryFormat)
        {
            ARL::Selection* pSelection = ARL::ServiceProvider::create< ARL::Selection >(m_pDataModel->getWorkspace());

            ARL::Instances instances(pSelection->begin(), pSelection->end());
            ARL::SerializerBinary::serialize(stream, instances);
        }
        else
        {
            TextXmlWriter machine(stream);

            UpdateUIManager::Instance().waitForLongProcess(
                "Saving",
                boost::bind(&TextXmlWriter::serialize,&machine,writeSelection().get()) );
        }
	}
	else
	{
		QString scriptText;
		{
			shared_ptr<ARL::Script> spScriptInstance = ARL::Instance::fastSharedDynamicCast<ARL::Script>(pSelection->front());
			if(spScriptInstance)
			{
				if(spScriptInstance->isCodeEmbedded())
				{
					scriptText = spScriptInstance->getEmbeddedCodeSafe().getSource().c_str();
				}
				else
				{
					ARL::ContentProvider* pContentProvider = ARL::ServiceProvider::create<ARL::ContentProvider>(spScriptInstance.get());
					if (pContentProvider)
					{
						std::auto_ptr<std::istream> stream = pContentProvider->getContent(spScriptInstance->getScriptId());
						std::string data = std::string(static_cast<std::stringstream const&>(std::stringstream() << stream->rdbuf()).str());
						scriptText = data.c_str();
					}
				}
			}
		}

		QFile file(fileName);
		if (!scriptText.isEmpty() && file.open(QFile::WriteOnly | QFile::Text))
		{
			QTextStream out(&file);
			out << scriptText;
		}
	}

	settings.setValue("rbxm_last_directory", QFileInfo(fileName).absolutePath());

	for (std::vector<shared_ptr<ARL::Instance> >::const_iterator iter = pSelection->begin(); iter != pSelection->end(); ++iter)
	{
		dictionaryService->storeAllDescendants(*iter);
		nrDictionaryService->storeAllDescendants(*iter);
	}
}

bool SelectionSaveToFileVerb::isEnabled() const
{
	ARL::Selection* pSelection = ARL::ServiceProvider::create<ARL::Selection>(m_pDataModel);
	return (pSelection && (pSelection->size() > 0));
}

std::auto_ptr<XmlElement> SelectionSaveToFileVerb::writeSelection()
{
	std::auto_ptr<XmlElement> root(Serializer::newRootElement());
	{
		ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);
		ARL::Selection* pSelection = ARL::ServiceProvider::create< ARL::Selection >(m_pDataModel->getWorkspace());
		ARL::AddSelectionToRoot(root.get(), pSelection, ARL::SerializationCreator);
	}

	return root;
}

PublishToRobloxAsVerb::PublishToRobloxAsVerb(ARL::VerbContainer* pVerbContainer, RobloxMainWindow* mainWnd)
: Verb(pVerbContainer, "PublishToRobloxAsVerb")
, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
, m_pMainWindow(mainWnd)
, m_dlg(NULL)
{
}

PublishToRobloxAsVerb::~PublishToRobloxAsVerb()
{
	if (m_dlg)
        delete m_dlg;
}

bool PublishToRobloxAsVerb::isEnabled() const
{
	return true;
}

void PublishToRobloxAsVerb::initDialog()
{
	static QMutex mutex;

	mutex.lock();
	try
	{
		QString initialUrl = QString("%1/IDE/publishas").arg(RobloxSettings::getBaseURL());

		if (!m_dlg)
        {
			m_dlg = new WebDialog(m_pMainWindow, initialUrl, m_pDataModel);
            m_dlg->setMinimumSize(FInt::StudioWebDialogMinimumWidth, FInt::StudioWebDialogMinimumHeight);
        }
		else
			m_dlg->load(initialUrl);
	}
	catch(std::exception& e)
	{
		ARL::Log::current()->writeEntry(ARL::Log::Error, e.what());
	}
	mutex.unlock();
}

void PublishToRobloxAsVerb::doIt(ARL::IDataState*)
{
	// autosave before we publish just in case
    RobloxDocManager::Instance().getPlayDoc()->autoSave(true);

	RobloxMainWindow::sendCounterEvent("QTStudio_IntendPublish");

	if (!StudioUtilities::checkNetworkAndUserAuthentication())
		return;

	ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);

	if (FFlag::PrefetchResourcesEnabled)
		initDialog();
	else
	{
		// "http://www.roblox.com/IDE/Upload.aspx"
		QString initialUrl = QString("%1/IDE/Upload.aspx").arg(RobloxSettings::getBaseURL());

		if (!m_dlg)
        {
			m_dlg = new WebDialog(m_pMainWindow, initialUrl, m_pDataModel);
            m_dlg->setMinimumSize( FInt::StudioWebDialogMinimumWidth, FInt::StudioWebDialogMinimumHeight);
        }
		else
			m_dlg->load(initialUrl);
	}

	m_dlg->show();
	m_dlg->raise();
	m_dlg->activateWindow();
}

QDialog* PublishToRobloxAsVerb::getPublishDialog()
{ 
	return qobject_cast<QDialog*>(m_dlg); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Bind fn for Publish Selection to Roblox
static void AnimationResponse(std::string* response, std::exception*, shared_ptr<ARL::Animation> animation)
{
	if(response){
		ARL::DataModel::LegacyLock lock(ARL::DataModel::get(animation.get()), ARL::DataModelJob::Write);
		int newAssetId;
		std::stringstream istream(*response);
		istream >> newAssetId;

		QString baseUrl = RobloxSettings::getBaseURL();
		animation->setAssetId(ARL::format("%s/Asset?ID=%d", qPrintable(baseUrl), newAssetId));
	}
}

static void SaveDecalAssetId(shared_ptr<ARL::Decal> decal, std::string assetId)
{
	decal->setTexture(assetId);
}

static void SaveMeshMeshId(shared_ptr<ARL::SpecialShape> mesh, std::string assetId)
{
	mesh->setMeshId(assetId);
}

static void SaveMeshTextureId(shared_ptr<ARL::SpecialShape> mesh, std::string assetId)
{
	mesh->setTextureId(assetId);
}

static void PostSaveHelper(boost::function<void(std::string)> saveFunction, std::string assetId)
{
	saveFunction(assetId);
}

static void PostFromContentProvider(ARL::AsyncHttpQueue::RequestResult result, std::istream* stream,
									int type, std::string name, weak_ptr<ARL::DataModel> weakDataModel, boost::function<void(std::string)> saveFunction)
{
	QString baseUrl = RobloxSettings::getBaseURL();
	if(result == ARL::AsyncHttpQueue::Succeeded){
		ARL::Http http(ARL::format("%s/Data/NewAsset.ashx?type=%d&Name=%s&Description=%s", qPrintable(baseUrl), type, name.c_str(), name.c_str()));
		try{
			std::string response;
			http.post(*stream, ARL::Http::kContentTypeDefaultUnspecified, true, response);

			int newAssetId;
			std::stringstream istream(response);
			istream >> newAssetId;

			if(shared_ptr<ARL::DataModel> dataModel = weakDataModel.lock()){
				dataModel->submitTask(boost::bind(&PostSaveHelper, saveFunction, ARL::format("%s/Asset?ID=%d", qPrintable(baseUrl), newAssetId)), ARL::DataModelJob::Write);
			}
		}
		catch(std::exception&){

		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

PublishSelectionToRobloxVerb::PublishSelectionToRobloxVerb(ARL::VerbContainer* pVerbContainer, RobloxMainWindow* mainWnd)
: Verb(pVerbContainer, "PublishSelectionToRobloxVerb")
, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
, m_pMainWindow(mainWnd)
, m_dlg(NULL)
{
}

PublishSelectionToRobloxVerb::~PublishSelectionToRobloxVerb()
{
    if (m_dlg)
		delete m_dlg;
}

bool PublishSelectionToRobloxVerb::isEnabled() const
{
	ARL::Selection* pSelection = ARL::ServiceProvider::create<ARL::Selection>(m_pDataModel);
	return (pSelection && (pSelection->size() > 0));
}

void PublishSelectionToRobloxVerb::doIt(ARL::IDataState*)
{
	// autosave before we publish just in case
    RobloxDocManager::Instance().getPlayDoc()->autoSave(true);

	RobloxMainWindow::sendCounterEvent("QTStudio_IntendPublishSelection");

	if (!StudioUtilities::checkNetworkAndUserAuthentication())
		return;

	ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);

	bool isScript = false;
	if (RbxWorkspace::isScriptAssetUploadEnabled)
	{

		ARL::Selection* sel = ARL::ServiceProvider::find<ARL::Selection>(m_pDataModel);
		if (sel && sel->size()==1)
			if (dynamic_cast<ARL::BaseScript*>(sel->front().get()))
				isScript = true;
	}
	if (RbxWorkspace::isImageModelAssetUploadEnabled)
	{
		ARL::Selection* sel = ARL::ServiceProvider::find<ARL::Selection>(m_pDataModel);
		if (sel && sel->size()==1){
			if (ARL::Decal* decal = dynamic_cast<ARL::Decal*>(sel->front().get())){
				if(!decal->getTexture().isHttp()){
					boost::function<void(std::string)> foo = boost::bind(&SaveDecalAssetId, shared_from(decal), _1);
					ARL::ServiceProvider::create<ARL::ContentProvider>(m_pDataModel)->getContent(decal->getTexture(), ARL::ContentProvider::PRIORITY_MFC,
																													boost::bind(&PostFromContentProvider, _1, _2, 1, decal->getName(), weak_from(ARL::DataModel::get(decal)),foo));

					return;
				}
			}
			if (ARL::SpecialShape* mesh = dynamic_cast<ARL::SpecialShape*>(sel->front().get())){
				bool uploadedSomething = false;
				if(!mesh->getMeshId().isHttp()){
					boost::function<void(std::string)> foo = boost::bind(&SaveMeshMeshId, shared_from(mesh), _1);
					ARL::ServiceProvider::create<ARL::ContentProvider>(m_pDataModel)->getContent(mesh->getMeshId(), ARL::ContentProvider::PRIORITY_MFC,
																													boost::bind(&PostFromContentProvider, _1, _2, 4, mesh->getName(), weak_from(ARL::DataModel::get(mesh)),foo));
					uploadedSomething = true;
				}
				if(!mesh->getTextureId().isHttp()){
					boost::function<void(std::string)> foo = boost::bind(&SaveMeshTextureId, shared_from(mesh), _1);
					ARL::ServiceProvider::create<ARL::ContentProvider>(m_pDataModel)->getContent(mesh->getTextureId(), ARL::ContentProvider::PRIORITY_MFC,
																													boost::bind(&PostFromContentProvider, _1, _2, 1, mesh->getName(), weak_from(ARL::DataModel::get(mesh)),foo));
					uploadedSomething = true;
				}
				if(uploadedSomething){
					return;
				}
			}
		}
	}

	bool isAnimation = false;
	ARL::Selection* sel = ARL::ServiceProvider::find<ARL::Selection>(m_pDataModel);
	if (sel && sel->size()==1) {
		if (dynamic_cast<ARL::KeyframeSequence*>(sel->front().get())) {
			isAnimation = true;
		}
	}

	// "http://www.roblox.com/UI/Save.aspx"
	QString initialUrl;
	if(isScript)
		initialUrl = QString("%1/UI/Save.aspx?type=Lua").arg(RobloxSettings::getBaseURL());
	else if (isAnimation)
	{
		initialUrl = QString("%1/studio/animations/publish").arg(RobloxSettings::getBaseURL());
		if (FFlag::StudioEnableGameAnimationsTab)
		{
			RobloxGameExplorer& gameExplorer = UpdateUIManager::Instance().getViewWidget<RobloxGameExplorer>(eDW_GAME_EXPLORER);
			if (gameExplorer.getCurrentGameId() > 0)
				initialUrl.append(QString("?universeId=%1").arg(gameExplorer.getCurrentGameId()));
		}
	}
	else
		initialUrl = QString("%1/UI/Save.aspx?type=Model").arg(RobloxSettings::getBaseURL());

	if (!m_dlg)
    {
		m_dlg = new WebDialog(m_pMainWindow, initialUrl, m_pDataModel);
        m_dlg->setMinimumSize( FInt::StudioWebDialogMinimumWidth, FInt::StudioWebDialogMinimumHeight);
    }
	else
		m_dlg->load(initialUrl);

	m_dlg->show();
	m_dlg->raise();
	m_dlg->activateWindow();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

CreateNewLinkedSourceVerb::CreateNewLinkedSourceVerb(DataModel* pVerbContainer)
	: Verb(pVerbContainer, "CreateNewLinkedSourceVerb")
	, m_pDataModel(pVerbContainer)
{
}

bool CreateNewLinkedSourceVerb::isEnabled() const
{
	DataModel::LegacyLock lock(m_pDataModel, DataModelJob::Read);
	shared_ptr<LuaSourceContainer> lsc = getLuaSourceContainer();
	
	return (UpdateUIManager::Instance().getViewWidget<RobloxGameExplorer>(eDW_GAME_EXPLORER)
		.getCurrentGameId() > 0) && lsc && lsc->getScriptId().isNull() &&
		(RobloxUser::singleton().getUserId() > 0);
}

void CreateNewLinkedSourceVerb::doIt(IDataState*)
{
	std::string instanceName;
	std::string source;
	{
		DataModel::LegacyLock lock(m_pDataModel, DataModelJob::Read);
		shared_ptr<Instance> instance = getLuaSourceContainer();
		instanceName = instance->getName();
		source = LuaSourceBuffer::fromInstance(instance).getScriptText();
	}

	ScriptPickerDialog::CompletedState state;
	QString newName;
	ScriptPickerDialog dialog;
	dialog.runModal(NULL, QString::fromStdString(instanceName), &state, &newName);

	if (state != ScriptPickerDialog::Completed)
	{
		ARLASSERT(state == ScriptPickerDialog::Abandoned);
		return;
	}

	bool success;
	RobloxGameExplorer& rge = UpdateUIManager::Instance().getViewWidget<RobloxGameExplorer>(eDW_GAME_EXPLORER);
	UpdateUIManager::Instance().waitForLongProcess("Creating LinkedSource",
		boost::bind(&CreateNewLinkedSourceVerb::doItThread, this, source, rge.getCurrentGameId(),
			rge.getCurrentGameGroupId(), newName, &success));
	rge.reloadDataFromWeb();

	if (success)
	{
		try
		{
			DataModel::LegacyLock lock(m_pDataModel, DataModelJob::Write);
	
			shared_ptr<LuaSourceContainer> lsc = getLuaSourceContainer();
			LuaSourceBuffer lsb = LuaSourceBuffer::fromInstance(lsc);
			ContentId scriptName = ContentId::fromGameAssetName(newName.toStdString());
			lsc->setScriptId(scriptName);

			ContentProvider* cp = m_pDataModel->create<ContentProvider>();
			// clear content provider cache, in case the script has been fetched previously
			cp->invalidateCache(scriptName);

			m_pDataModel->create<ChangeHistoryService>()->requestWaypoint("Store LinkedSource in cloud");
		}
		catch (const ARL::base_exception&)
		{
			// catch failure to grab legacy lock, do nothing
		}
	}
	else
	{
		QMessageBox mb;
		mb.setText("Unable to create LinkedSource, see output for details.");
		mb.setStandardButtons(QMessageBox::Ok);
		mb.exec();
	}
}

void CreateNewLinkedSourceVerb::doItThread(std::string source, int currentGameId, boost::optional<int> groupId,
	QString newName, bool* success)
{
	*success = false;
	try
	{
		EntityProperties createScriptAssetResponse;
		createScriptAssetResponse.setFromJsonFuture(RobloxGameExplorer::publishScriptAsset(source,
			boost::optional<int>(), groupId));

		int assetId = createScriptAssetResponse.get<int>("AssetId").get();
	
		EntityProperties createNameRequest;
		createNameRequest.set("Name", newName.toStdString());
		if (FFlag::GameExplorerUseV2AliasEndpoint)
		{
			createNameRequest.set("Type", (int)ALIAS_TYPE_Asset);
			createNameRequest.set("TargetId", assetId);
		}
		else
		{
			createNameRequest.set("AssetId", assetId);
		}

		QString postUrl;
		postUrl = QString(FFlag::GameExplorerUseV2AliasEndpoint ?
				"%1/universes/create-alias-v2?universeId=%2" : "%1/universes/create-alias?universeId=%2")
			.arg(QString::fromStdString(ContentProvider::getApiBaseUrl(RobloxSettings::getBaseURL().toStdString())))
			.arg(currentGameId);

		Http http(postUrl.toStdString());
		std::string propertiesJson = createNameRequest.asJson();
		std::istringstream propertiesStream(propertiesJson);
		std::string postResponse;
		// perform synchronous post
		http.post(propertiesStream, Http::kContentTypeApplicationJson, false, postResponse);
	
		*success = true;
		ARL::StandardOut::singleton()->print(MESSAGE_INFO, "Successfully created new LinkedSource");

	}
	catch (const ARL::base_exception&)
	{
		*success = false;
	}
}

shared_ptr<LuaSourceContainer> CreateNewLinkedSourceVerb::getLuaSourceContainer() const
{
	if (Selection* selection = m_pDataModel->find<Selection>())
	{
		if (selection->size() == 1)
		{
			return Instance::fastSharedDynamicCast<LuaSourceContainer>(selection->front());
		}
	}
	return shared_ptr<LuaSourceContainer>();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

PublishAsPluginVerb::PublishAsPluginVerb(ARL::VerbContainer* pVerbContainer, RobloxMainWindow* mainWnd)
	: Verb(pVerbContainer, "PublishAsPluginVerb")
	, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
	, m_pMainWindow(mainWnd)
	, m_dlg(NULL)
{
}

bool PublishAsPluginVerb::isEnabled() const
{
	return true;
}

void PublishAsPluginVerb::doIt(ARL::IDataState*)
{
	// autosave before we publish just in case
    RobloxDocManager::Instance().getPlayDoc()->autoSave(false /*do not force autosave*/);

	if (!StudioUtilities::checkNetworkAndUserAuthentication())
			return;

	ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);

	QString initialUrl = QString("%1/studio/plugins/publish").arg(RobloxSettings::getBaseURL());

	if (!m_dlg)
		m_dlg.reset(new WebDialog(m_pMainWindow, initialUrl, m_pDataModel));
	else
		m_dlg->load(initialUrl);

	m_dlg->show();
	m_dlg->raise();
	m_dlg->activateWindow();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

LaunchInstancesVerb::LaunchInstancesVerb( ARL::VerbContainer* pVerbContainer )
: Verb(pVerbContainer, "LaunchInstancesVerb")
, m_pVerbContainer(pVerbContainer)
{
}

void LaunchInstancesVerb::doIt( ARL::IDataState* dataState )
{
	ARL::Verb* pVerb;
	switch (NameValueStoreManager::singleton().getValue("clientsAndServersOptions", "user_value").toInt())
	{
	case PLAYSOLO:
		pVerb = m_pVerbContainer->getVerb("PlaySoloVerb");
		pVerb->doIt(dataState);
		break;
	case SERVERONEPLAYER:
		pVerb = m_pVerbContainer->getVerb("StartServerVerb");
		pVerb->doIt(dataState);
		pVerb = m_pVerbContainer->getVerb("StartPlayerVerb");
		pVerb->doIt(dataState);
		break;
	case SERVERFOURPLAYERS:
		pVerb = m_pVerbContainer->getVerb("StartServerVerb");
		pVerb->doIt(dataState);
		pVerb = m_pVerbContainer->getVerb("StartPlayerVerb");
		pVerb->doIt(dataState);
		pVerb->doIt(dataState);
		pVerb->doIt(dataState);
		pVerb->doIt(dataState);
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

StartServerAndPlayerVerb::StartServerAndPlayerVerb(ARL::VerbContainer* pVerbContainer)
: Verb(pVerbContainer, "StartServerAndPlayerVerb")
, m_pVerbContainer(pVerbContainer)
{
}

void StartServerAndPlayerVerb::doIt(ARL::IDataState* dataState)
{
	bool startServer = NameValueStoreManager::singleton().getValue("startServerCB", "checked").toBool();
	int  numPlayers  = NameValueStoreManager::singleton().getValue("playersMode", "user_value").toInt();

	// save values in settings (so it can be used for launching players from launched server)
	RobloxSettings settings;
	settings.setValue(sRibbonStartServerSetting, startServer);
	settings.setValue(sRibbonNumPlayerSetting, numPlayers);

	if (startServer)
	{
		// cleanup existing servers and players
		QAction* cleanupAction = UpdateUIManager::Instance().getMainWindow().cleanupServersAndPlayersAction;
		if (cleanupAction && cleanupAction->isEnabled())
			cleanupAction->trigger();

		// server will launch the players (this will make sure players are launched once the server is loaded)
		ARL::Verb* pVerb = m_pVerbContainer->getVerb("StartServerVerb");
		if (pVerb)
			pVerb->doIt(dataState);
	}
	else if (numPlayers > 0)
	{
		// if there's no server to be launched then directly launch the number of players
		launchPlayers(numPlayers);
	}
}

void StartServerAndPlayerVerb::launchPlayers(int numPlayers)
{
	UpdateUIManager::Instance().waitForLongProcess(
        "Starting players",
		boost::bind(&StartServerAndPlayerVerb::launchStudioInstances, m_pVerbContainer, numPlayers) );
}

void StartServerAndPlayerVerb::launchStudioInstances(ARL::VerbContainer *pVerbContainer, int numPlayers)
{
	// this function will be called from a new thread
	if (numPlayers > 0)
	{
		ARL::Verb* pVerb = pVerbContainer->getVerb("StartPlayerVerb");
		if (pVerb)
		{
			for (int ii = 1; ii <= numPlayers; ++ii)
			{
				pVerb->doIt(NULL);
				if (ii < numPlayers)
					QtUtilities::sleep(1000);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

ServerPlayersStateInitVerb::ServerPlayersStateInitVerb(ARL::VerbContainer* pVerbContainer)
: Verb(pVerbContainer, "ServerPlayersStateInitVerb")
{
	RobloxSettings settings;
	//initialize start server mode
	NameValueStoreManager::singleton().setValue("startServerCB", "checked", settings.value(sRibbonStartServerSetting, true).toBool());
	//initialize default number of players
	int index = settings.value(sRibbonNumPlayerSetting, 1).toInt();
	NameValueStoreManager::singleton().setValue("playersMode", "user_value", index);
	QComboBox* pComboBox = UpdateUIManager::Instance().getMainWindow().findChild<QComboBox*>("playersMode");
	if (pComboBox)
		pComboBox->setCurrentIndex(index);
}

void ServerPlayersStateInitVerb::doIt(ARL::IDataState* )
{	RobloxSettings().setValue(sRibbonStartServerSetting, NameValueStoreManager::singleton().getValue("startServerCB", "checked").toBool()); }

bool ServerPlayersStateInitVerb::isChecked() const
{  return RobloxSettings().value(sRibbonStartServerSetting, true).toBool(); }

CreatePluginVerb::CreatePluginVerb( ARL::VerbContainer* pVerbContainer )
: Verb(pVerbContainer, "CreatePluginVerb")
, m_pVerbContainer(pVerbContainer)
{
}

void CreatePluginVerb::doIt( ARL::IDataState* dataState )
{
	QDesktopServices::openUrl(QUrl("http://wiki.lambda.cam/index.php/How_To_Make_Plugins"));
}

PlaySoloVerb::PlaySoloVerb(ARL::VerbContainer* pVerbContainer)
: Verb(pVerbContainer, "PlaySoloVerb")
, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
{
}

void PlaySoloVerb::doIt(ARL::IDataState*)
{
	// These should instead come from the Settings infra, once it is ready to go to the temp location
	QString fileSaveLocation = QString("%1/visit.rbxl").arg(RobloxSettings::getTempLocation());
	QString errorMessage;

	// Remove debug data file, before starting new process
	if (FFlag::LuaDebugger)
		QFile::remove(StudioUtilities::getDebugInfoFile(fileSaveLocation));

    if ( !StudioSerializerHelper::saveAs(fileSaveLocation,"Play Solo",false,true,m_pDataModel,errorMessage,true) )
    {
        QMessageBox::critical(
            &UpdateUIManager::Instance().getMainWindow(),
            "Play Solo - Save Failure",
            errorMessage );
        return;
    }

	// loadfile('http://www.roblox.com/game/visit.ashx')()
	QString script;
	script = QString("loadfile(\"%1/game/visit.ashx?IsPlaySolo=1&placeId=%2&universeId=%3\")()\n")
		.arg(RobloxSettings::getBaseURL())
		.arg(m_pDataModel->getPlaceID())
		.arg(m_pDataModel->getUniverseId());
	RobloxApplicationManager::instance().createNewStudioInstance(script, fileSaveLocation, true, true);
}

PairRbxDevVerb::PairRbxDevVerb(ARL::VerbContainer* pVerbContainer, QWidget* newParent)
	: Verb(pVerbContainer, "PairRbxDevVerb")
	, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
{
}

void PairRbxDevVerb::doIt(ARL::IDataState* dataState)
{
    if (!dialog)
    {
        shared_ptr<PairRbxDeviceDialog> newDialog( new PairRbxDeviceDialog(NULL) );
        dialog = newDialog;
    }

    dialog->updatePairCode();
	dialog->exec();

    dialog.reset();
}

ManageEmulationDevVerb::ManageEmulationDevVerb(ARL::VerbContainer* pVerbContainer, QWidget* newParent)
	: Verb(pVerbContainer, "ManageEmulationDevVerb")
	, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
{}

void ManageEmulationDevVerb::doIt(ARL::IDataState* dataState)
{
	ManageEmulationDeviceDialog dialog(NULL);
	dialog.exec();
}

AudioToggleVerb::AudioToggleVerb(ARL::VerbContainer* pVerbContainer)
	: Verb(pVerbContainer, "AudioEnableVerb")
	, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
{}

void AudioToggleVerb::doIt(ARL::IDataState* dataState)
{
	if (m_pDataModel)
	{
		if (ARL::Soundscape::SoundService* soundService = ARL::ServiceProvider::find<ARL::Soundscape::SoundService>(m_pDataModel))
		{
			soundService->muteAllChannels(!soundService->isMuted());
		}
	}
}

bool AudioToggleVerb::isChecked() const
{
	if (m_pDataModel)
	{
		if (ARL::Soundscape::SoundService* soundService = ARL::ServiceProvider::find<ARL::Soundscape::SoundService>(m_pDataModel))
		{
			return soundService->isMuted();
		}
	}

	return false;
}

AnalyzePhysicsToggleVerb::AnalyzePhysicsToggleVerb(ARL::DataModel* pDataModel)
    : ARL::Verb(pDataModel, "AnalyzeEnableVerb")
    , m_pDataModel(pDataModel)
{
}

bool AnalyzePhysicsToggleVerb::isEnabled() const
{
    return FFlag::PhysicsAnalyzerEnabled;
}

bool AnalyzePhysicsToggleVerb::isChecked() const
{
    return PhysicsSettings::singleton().getPhysicsAnalyzerState();
}

void AnalyzePhysicsToggleVerb::startAnalyze()
{
    PhysicsSettings::singleton().setPhysicsAnalyzerState(true);
}

void AnalyzePhysicsToggleVerb::stopAnalyze()
{
    PhysicsSettings::singleton().setPhysicsAnalyzerState(false);
}

void AnalyzePhysicsToggleVerb::doIt(ARL::IDataState*)
{
    PhysicsSettings::singleton().getPhysicsAnalyzerState() ? stopAnalyze() : startAnalyze();
}

StartServerVerb::StartServerVerb(ARL::VerbContainer* pVerbContainer)
: Verb(pVerbContainer, "StartServerVerb")
, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
{
}

void StartServerVerb::doIt(ARL::IDataState*)
{
	// These should instead come from the Settings infra, once it is ready to go to the temp location
	QString fileSaveLocation = QString("%1/server.rbxl").arg(RobloxSettings::getTempLocation());
	QString errorMessage;

    if ( !StudioSerializerHelper::saveAs(fileSaveLocation,"Start Server",false,true,m_pDataModel,errorMessage,true) )
    {
        QMessageBox::critical(
            &UpdateUIManager::Instance().getMainWindow(),
            "Start Server Failure",
            errorMessage );
        return;
    }

	// loadfile('http://www.roblox.com/game/gameserver.ashx')(<placeid>, 53640)
	QString script;
	script = QString(
		"loadfile(\"%1/game/gameserver.ashx\")("
		"%2, 53640, nil, nil, nil, "
		"\"%1\", nil, nil, nil, nil, "
		"nil, nil, nil, nil, nil, "
		"nil, nil, %3)\n")
		.arg(::trim_trailing_slashes(RobloxSettings::getBaseURL().toStdString()).c_str())
		.arg(m_pDataModel->getPlaceID())
		.arg(m_pDataModel->getUniverseId());

	if (RobloxIDEDoc::isEditMode(m_pDataModel) || RobloxIDEDoc::getIsCloudEditSession())
		RobloxApplicationManager::instance().createNewStudioInstance(script, fileSaveLocation, true, false, NewInstanceMode_Server);
}


StartPlayerVerb::StartPlayerVerb(ARL::VerbContainer* pVerbContainer)
: Verb(pVerbContainer, "StartPlayerVerb")
, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
{
}

void StartPlayerVerb::doIt(ARL::IDataState*)
{
    // loadfile('http://www.roblox.com//game/join.ashx?UserID=0&serverPort=53640')()
	QString script;
	script = QString("loadfile(\"%1/game/join.ashx?UserID=0&serverPort=53640&universeId=%2\")()\n")
		.arg(RobloxSettings::getBaseURL())
		.arg(m_pDataModel->getUniverseId());
	
	if (RobloxIDEDoc::getIsCloudEditSession() || (!ARL::Network::Players::clientIsPresent(m_pDataModel) && !ARL::Network::Players::findConstLocalPlayer(m_pDataModel)))
		RobloxApplicationManager::instance().createNewStudioInstance(script,QString::null,true,true,NewInstanceMode_Player);
}


ToggleFullscreenVerb::ToggleFullscreenVerb(ARL::VerbContainer* container) :
Verb(container, "ToggleFullScreen")
{
}

void ToggleFullscreenVerb::doIt(ARL::IDataState*)
{
	QAction& action = UpdateUIManager::Instance().getMainWindow().fullScreenAction();
	bool checkedState = action.isChecked();

	QMetaObject::invokeMethod(
        &action,
        "setChecked",
        Qt::QueuedConnection,
        Q_ARG(bool,!checkedState) );
}

bool ToggleFullscreenVerb::isEnabled() const
{
	return true;
}

ShutdownClientVerb::ShutdownClientVerb(ARL::VerbContainer* container, IRobloxDoc *pDoc)
: Verb(container, "ShutdownClient")
, m_pIDEDoc(pDoc)
{
}

void ShutdownClientVerb::doIt(ARL::IDataState*)
{
	RobloxMainWindow *pMainWindow = &UpdateUIManager::Instance().getMainWindow();
	if ( pMainWindow )
	{
		QMetaObject::invokeMethod(
            pMainWindow,
            "forceClose",
            Qt::QueuedConnection );
	}
}

ShutdownClientAndSaveVerb::ShutdownClientAndSaveVerb(ARL::VerbContainer* container, IRobloxDoc *pDoc)
    : Verb(container,"ShutdownClientAndSave"),
      m_pIDEDoc(pDoc)
{
}

void ShutdownClientAndSaveVerb::doIt(ARL::IDataState*)
{
	RobloxMainWindow* pMainWindow = &UpdateUIManager::Instance().getMainWindow();
	if ( pMainWindow )
	{
		QMetaObject::invokeMethod(
            pMainWindow,
            "saveAndClose",
            Qt::QueuedConnection );
	}
}

LeaveGameVerb::LeaveGameVerb(ARL::VerbContainer* container, IRobloxDoc *pDoc)
: Verb(container, "Exit")
, m_pIDEDoc(pDoc)
{
}

void LeaveGameVerb::doIt(ARL::IDataState*)
{
	if ( RobloxDocManager::Instance().getPlayDoc() )
	{
		RobloxMainWindow& mainWindow = UpdateUIManager::Instance().getMainWindow();
		QMetaObject::invokeMethod(&mainWindow,"closePlayDoc",Qt::QueuedConnection);
	}
}


ToggleAxisWidgetVerb::ToggleAxisWidgetVerb(ARL::DataModel* dataModel)
: ARL::Verb(dataModel,"ToggleAxisWidget")
, m_pDataModel(dataModel)
{
	ARL::Workspace* pWorkspace = m_pDataModel->getWorkspace();
	if (pWorkspace)
		pWorkspace->setShowAxisWidget(UpdateUIManager::Instance().get3DAxisEnabled());
}

void ToggleAxisWidgetVerb::doIt(ARL::IDataState*)
{
	ARL::Workspace* pWorkspace = m_pDataModel->getWorkspace();
	if (pWorkspace)
	{
		bool newValue = !pWorkspace->getShowAxisWidget();
		pWorkspace->setShowAxisWidget(newValue);
		UpdateUIManager::Instance().set3DAxisEnabled(newValue);
	}
}

bool ToggleAxisWidgetVerb::isChecked() const
{
	ARL::Workspace* pWorkspace = m_pDataModel->getWorkspace();
	if (pWorkspace)
		return pWorkspace->getShowAxisWidget();
	return false;
}

Toggle3DGridVerb::Toggle3DGridVerb(ARL::DataModel* dataModel)
: ARL::Verb(dataModel,"Toggle3DGrid")
, m_pDataModel(dataModel)
{
	ARL::Workspace* workspace = m_pDataModel->getWorkspace();
	if(workspace)
		workspace->setShow3DGrid(UpdateUIManager::Instance().get3DGridEnabled());
}

void Toggle3DGridVerb::doIt(ARL::IDataState*)
{
	ARL::Workspace* pWorkspace = m_pDataModel->getWorkspace();
	if (pWorkspace)
	{
		bool newValue = !pWorkspace->getShow3DGrid();
		pWorkspace->setShow3DGrid(newValue);
		UpdateUIManager::Instance().set3DGridEnabled(newValue);
	}
}

bool Toggle3DGridVerb::isEnabled() const
{
	ARL::Network::Player* pLocalPlayer = ARL::Network::Players::findLocalPlayer(m_pDataModel);
	return RobloxIDEDoc::getIsCloudEditSession() || (pLocalPlayer == NULL);
}

bool Toggle3DGridVerb::isChecked() const
{
	if (isEnabled())
	{
		ARL::Workspace* pWorkspace = m_pDataModel->getWorkspace();
		if (pWorkspace)
			return pWorkspace->getShow3DGrid();
	}
	return false;
}

ToggleCollisionCheckVerb::ToggleCollisionCheckVerb(ARL::DataModel* dataModel)
: ARL::Verb(dataModel,"ToggleCollisionCheckVerb")
{
	RobloxSettings settings;
	ARL::AdvArrowTool::advCollisionCheckMode = settings.value(sCollisionToggleModeSetting, true).toBool();
}

void ToggleCollisionCheckVerb::doIt(ARL::IDataState* dataState)
{
	ARL::AdvArrowTool::advCollisionCheckMode = !ARL::AdvArrowTool::advCollisionCheckMode;

	RobloxSettings settings;
	settings.setValue(sCollisionToggleModeSetting,ARL::AdvArrowTool::advCollisionCheckMode);
}

bool ToggleCollisionCheckVerb::isChecked() const
{ return ARL::AdvArrowTool::advCollisionCheckMode; }


ToggleLocalSpaceVerb::ToggleLocalSpaceVerb(ARL::DataModel* dataModel)
: ARL::Verb(dataModel,"ToggleLocalSpaceVerb")
, m_pDataModel(dataModel)
{
	RobloxSettings settings;
	ARL::AdvArrowTool::advLocalTranslationMode = settings.value(sLocalTranslationModeSetting, false).toBool();
    ARL::AdvArrowTool::advLocalRotationMode = settings.value(sLocalRotationModeSetting, true).toBool();
}

void ToggleLocalSpaceVerb::doIt(ARL::IDataState* dataState)
{
    ARL::MouseCommand* mouseCommand = m_pDataModel->getWorkspace()->getCurrentMouseCommand();

    if (!mouseCommand)
        return;

	if (DFFlag::UseRemoveTypeIDTricks)
	{
		if (ARL::AdvMoveTool::name() == mouseCommand->getName())
		{
			ARL::AdvArrowTool::advLocalTranslationMode = !ARL::AdvArrowTool::advLocalTranslationMode;

			RobloxSettings settings;
			settings.setValue(sLocalTranslationModeSetting, ARL::AdvArrowTool::advLocalTranslationMode);

		}
		else if (ARL::AdvRotateTool::name() == mouseCommand->getName())
		{
			ARL::AdvArrowTool::advLocalRotationMode = !ARL::AdvArrowTool::advLocalRotationMode;

			RobloxSettings settings;
			settings.setValue(sLocalRotationModeSetting, ARL::AdvArrowTool::advLocalRotationMode);
		}
	}
	else
	{
		if (typeid(ARL::AdvMoveTool) == typeid(*mouseCommand))
		{
			ARL::AdvArrowTool::advLocalTranslationMode = !ARL::AdvArrowTool::advLocalTranslationMode;

			RobloxSettings settings;
			settings.setValue(sLocalTranslationModeSetting, ARL::AdvArrowTool::advLocalTranslationMode);

		}
		else if (typeid(ARL::AdvRotateTool) == typeid(*mouseCommand))
		{
			ARL::AdvArrowTool::advLocalRotationMode = !ARL::AdvArrowTool::advLocalRotationMode;

			RobloxSettings settings;
			settings.setValue(sLocalRotationModeSetting, ARL::AdvArrowTool::advLocalRotationMode);
		}
	}
}

bool ToggleLocalSpaceVerb::isChecked() const
{
    ARL::MouseCommand* mouseCommand = m_pDataModel->getWorkspace()->getCurrentMouseCommand();

    if (!mouseCommand)
        return false;

    if (typeid(ARL::AdvMoveTool) == typeid(*mouseCommand))
    {
        return ARL::AdvArrowTool::advLocalRotationMode;
    }
    else if (typeid(ARL::AdvRotateTool) == typeid(*mouseCommand))
    {
        return ARL::AdvArrowTool::advLocalTranslationMode;
    }

    return false;
}


ScreenshotVerb::ScreenshotVerb(ARL::DataModel* dataModel)
: ARL::Verb(dataModel,"Screenshot")
, m_spDataModel(shared_from(dataModel))
{
	reconnectScreenshotSignal();
	m_spDataModel->screenshotUploadSignal.connect(boost::bind(&ScreenshotVerb::onUploadSignal, this, _1));
}

void ScreenshotVerb::doIt(ARL::IDataState* dataState)
{
	if (m_spDataModel)
	{
		m_spDataModel->submitTask( boost::bind(&ARL::DataModel::TakeScreenshotTask, weak_ptr<ARL::DataModel>(m_spDataModel)),
								  ARL::DataModelJob::Write );
	}
}

bool ScreenshotVerb::isEnabled() const
{	return !StudioUtilities::isScreenShotUploading(); }

void ScreenshotVerb::onScreenshotFinished(const std::string &fileName)
{
    if (!m_spDataModel)
        return;

	ARLASSERT(!fileName.empty());

	showMessage(m_spDataModel, "Screenshot saved");
	m_fileToUpload = QString::fromStdString(fileName);

	// copy saved image to clipboard (so it can be used as Ctrl+V in Paint)
	QTimer::singleShot(0, this, SLOT(copyImageToClipboard()));

	switch (ARL::GameSettings::singleton().getPostImageSetting())
	{
		case ARL::GameSettings::ASK:
			QTimer::singleShot(0, this, SLOT(showPostImageWebDialog()));
			break;
		case ARL::GameSettings::ALWAYS:
			ScreenshotVerb::DoPostImage(m_spDataModel, m_fileToUpload, getSEOStr());
			break;
		case ARL::GameSettings::NEVER:
			break;
	}
}

void ScreenshotVerb::copyImageToClipboard()
{
	if (m_fileToUpload.isEmpty())
		return;

	QImage imageForClipboard(m_fileToUpload);
	if (!imageForClipboard.isNull() && QApplication::clipboard())
	{
		QMimeData *pMimeData = new QMimeData;
		pMimeData->setImageData(imageForClipboard);
		QApplication::clipboard()->setMimeData(pMimeData);
	}
}

void ScreenshotVerb::showPostImageWebDialog()
{
    if (!m_spDataModel)
        return;

	/*QString url = QString("%1/UploadMedia/PostImage.aspx?from=client&rand=%2&seostr=%3&filename=%4").arg(RobloxSettings::getBaseURL())
																									.arg(rand())
																									.arg(getSEOStr())
																									.arg(m_fileToUpload);

	WebDialog *pWebDialog = new WebDialog(&UpdateUIManager::Instance().getMainWindow(), url, m_spDataModel.get());
	pWebDialog->exec();
	pWebDialog->deleteLater();*/

	UpdateUIManager::Instance().updateToolBars();
}

void ScreenshotVerb::DoPostImage(shared_ptr<ARL::DataModel> spDataModel, const QString &fileName, const QString &seoStr)
{
    ARLASSERT(spDataModel);

	if (!spDataModel)
        return;

	ARLASSERT(!fileName.isEmpty());

	bool isUploadStarted = false;

	try
	{
		QString url = QString("%1/UploadMedia/DoPostImage.ashx?from=client").arg(RobloxSettings::getBaseURL());
		std::string seoStrLocal(seoStr.toStdString());

		ARL::Http http(qPrintable(url));
		// in case the seo info contains nothing but whitespaces, add a line break to prevent facebook from returning errors
		http.additionalHeaders[seoStrLocal] = seoStrLocal + "%0D%0A";

		shared_ptr<std::ifstream> fileStream(new std::ifstream);
		fileStream->open(qPrintable(fileName), std::ios::binary);

		if (!fileStream->fail())
		{
			showMessage(spDataModel, "Uploading image ...");
			spDataModel->submitTask( boost::bind(&ARL::DataModel::ScreenshotUploadTask, weak_ptr<ARL::DataModel>(spDataModel), false),
				                     ARL::DataModelJob::Write);

			http.post(fileStream, ARL::Http::kContentTypeDefaultUnspecified, false,
				boost::bind(&ScreenshotVerb::PostImageFinished, _1, _2, weak_ptr<ARL::DataModel>(spDataModel)));
			isUploadStarted = true;
		}
	}
	catch (...)
	{
		// convey users uploading is finished (required even if there's a failure)
		spDataModel->submitTask( boost::bind(&ARL::DataModel::ScreenshotUploadTask, weak_ptr<ARL::DataModel>(spDataModel), true),
			                     ARL::DataModelJob::Write );
	}

	if (!isUploadStarted)
		showMessage(spDataModel, "Failed to upload image.");
}

void ScreenshotVerb::PostImageFinished(std::string *pResponse, std::exception *pException, weak_ptr<ARL::DataModel> pWeakDataModel)
{
	if(shared_ptr<ARL::DataModel> spDataModel = pWeakDataModel.lock())
	{
		if ((pException == NULL) && (pResponse->compare("ok") == 0))
		{
			showMessage(spDataModel, "Image uploaded to Facebook");
		}
		else
		{
			showMessage(spDataModel, "Failed to upload image.");
			ARL::GameSettings::singleton().setPostImageSetting(ARL::GameSettings::ASK);
		}

		//uploading finished
		spDataModel->submitTask( boost::bind(&ARL::DataModel::ScreenshotUploadTask, weak_ptr<ARL::DataModel>(spDataModel), true),
			ARL::DataModelJob::Write );
	}
}

void ScreenshotVerb::onUploadSignal(bool finished)
{
	StudioUtilities::setScreenShotUploading(!finished);
	if (finished)
		UpdateUIManager::Instance().updateToolBars();
}

QString ScreenshotVerb::getSEOStr()
{
	QString seo = QString::fromStdString(m_spDataModel->getScreenshotSEOInfo());
	if (seo.isEmpty())
		seo = tr("A screenshot from ANORRL.  Learn more at http://arl.lambda.cam");
	return seo;
}

void ScreenshotVerb::showMessage(shared_ptr<ARL::DataModel> spDataModel, const char* message)
{
	if (spDataModel)
		spDataModel->submitTask( boost::bind(&ARL::DataModel::ShowMessage, weak_ptr<ARL::DataModel>(spDataModel), 1, message, 6), ARL::DataModelJob::Write );
	ARL::StandardOut::singleton()->print(ARL::MESSAGE_INFO, message);
}

void ScreenshotVerb::reconnectScreenshotSignal()
{
    if (!m_spDataModel)
        return;

	m_spDataModel->screenshotReadySignal.connect(boost::bind(&ScreenshotVerb::onScreenshotFinished, this, _1));
}

//Video recording is currently supported on windows only
#ifdef _WIN32

RecordToggleVerb::RecordToggleVerb(ARL::DataModel* pDataModel, ARL::ViewBase* pViewGfx)
: ARL::Verb(pDataModel, "RecordToggle")
, m_pDataModel(pDataModel)
, m_jobWait(false)
, m_jobDone(false)
, m_threadDone(false)
, m_bStop(false)
, m_bIsBusy(false)
{
    m_pVideoControl.reset(new ARL::VideoControl(new ARL::DSVideoCaptureEngine(), pViewGfx,
                                                pViewGfx->getFrameRateManager(),
                                                this));

	m_helperThread.reset(new boost::thread(boost::bind(&RecordToggleVerb::action, this)));
}

RecordToggleVerb::~RecordToggleVerb()
{
	m_bStop = true;
	m_jobWait.Set();
	m_threadDone.Wait();
}

bool RecordToggleVerb::isEnabled() const
{	return (ARL::GameSettings::singleton().videoCaptureEnabled && !StudioUtilities::isVideoUploading() && !m_bIsBusy); }

bool RecordToggleVerb::isChecked() const
{	return isRecording(); }

bool RecordToggleVerb::isSelected() const
{	return isRecording(); }

void RecordToggleVerb::startRecording()
{
	ARL::Soundscape::SoundService* pSoundService = ARL::ServiceProvider::create<ARL::Soundscape::SoundService>(m_pDataModel);
	ARLASSERT(pSoundService);

	m_pVideoControl->startRecording(pSoundService);
	StudioUtilities::setVideoFileName(m_pVideoControl->getFileName());

	ARL::GameSettings::singleton().videoRecordingSignal(true);
	m_bIsBusy = false;
}

void RecordToggleVerb::stopRecording(bool showUploadDialog)
{
	m_pVideoControl->stopRecording();

	if (showUploadDialog && (ARL::GameBasicSettings::singleton().getUploadVideoSetting() != ARL::GameSettings::NEVER))
	{
		QTimer::singleShot(0, this, SLOT(uploadVideo()));
	}
	else
	{
		m_bIsBusy = false;
		UpdateUIManager::Instance().updateToolBars();
	}

	ARL::GameSettings::singleton().videoRecordingSignal(false);
}

bool RecordToggleVerb::isRecording() const
{ return m_pVideoControl->isVideoRecording(); }

void RecordToggleVerb::doIt(ARL::IDataState*)
{
	if (m_bIsBusy)
		return;
	m_bIsBusy = true;

	if (m_pVideoControl->isVideoRecording())
	{
		m_job = boost::bind(&RecordToggleVerb::stopRecording, this, true);
		m_jobWait.Set();
		m_jobDone.Wait();
	}
	else
	{
        m_job = boost::bind(&RecordToggleVerb::startRecording, this);
        m_jobWait.Set();
        m_jobDone.Wait();
    }
}

void RecordToggleVerb::uploadVideo()
{
	/*char n[16];
	itoa(rand(), n, 10);
	QString url = QString("%1/UploadMedia/UploadVideo.aspx?from=client&rand=").arg(RobloxSettings::getBaseURL());
	url.append(n);

	WebDialog *pWebDialog = new WebDialog(&UpdateUIManager::Instance().getMainWindow(), url, m_pDataModel);
	pWebDialog->exec();
	pWebDialog->deleteLater();*/

	m_bIsBusy = false;
	UpdateUIManager::Instance().updateToolBars();
}

void RecordToggleVerb::action()
{
	//ARL::StandardOut::singleton()->printf(ARL::MESSAGE_INFO,  "Starting vid. rec. helper thread");
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	while(!m_bStop)
	{
		m_jobWait.Wait();
		if (!m_bStop)
		{
			m_job();
			m_jobDone.Set();
		}
	}
	CoUninitialize();
	//ARL::StandardOut::singleton()->printf(ARL::MESSAGE_INFO,  "Exiting vid. rec. helper thread");
	m_threadDone.Set();
}

#endif

ExportSelectionVerb::ExportSelectionVerb(ARL::DataModel* pDataModel)
: ARL::Verb(pDataModel, "ExportSelectionVerb")
, m_pDataModel(pDataModel)
{}

void ExportSelectionVerb::doIt(ARL::IDataState*)
{
	if (FFlag::GoogleAnalyticsTrackingEnabled)
		ARL::RobloxGoogleAnalytics::trackEvent(GA_CATEGORY_STUDIO, "ExportSelection");

	QTimer::singleShot(0, RobloxDocManager::Instance().getPlayDoc(), SLOT(exportSelection()));
}

ExportPlaceVerb::ExportPlaceVerb(ARL::DataModel* pDataModel)
: ARL::Verb(pDataModel, "ExportPlaceVerb")
, m_pDataModel(pDataModel)
{}

void ExportPlaceVerb::doIt(ARL::IDataState*)
{
	if (FFlag::GoogleAnalyticsTrackingEnabled)
		ARL::RobloxGoogleAnalytics::trackEvent(GA_CATEGORY_STUDIO, "ExportPlace");

	QTimer::singleShot(0, RobloxDocManager::Instance().getPlayDoc(), SLOT(exportPlace()));
}


PublishToRobloxVerb::PublishToRobloxVerb(ARL::VerbContainer* pVerbContainer, RobloxMainWindow* pMainWindow)
: Verb(pVerbContainer, "PublishToRobloxVerb")
, m_pDataModel(dynamic_cast<ARL::DataModel*>(pVerbContainer))
, m_pMainWindow(pMainWindow)
, m_bIsPublishInProcess(false)
{
	// Hook up our signal from the datamodel
	m_pDataModel->saveFinishedSignal.connect(boost::bind(&PublishToRobloxVerb::onEventPublishingFinished, this));
}

void PublishToRobloxVerb::onEventPublishingFinished()
{
	m_bIsPublishInProcess = false;
}

void PublishToRobloxVerb::doIt(ARL::IDataState* dataState )
{
	// autosave before publish just in case
    RobloxDocManager::Instance().getPlayDoc()->autoSave(true);
    const char *failedToPublishErrorMsg = "Failed to Publish";

	if (!StudioUtilities::checkNetworkAndUserAuthentication())
		return;

    //check if mac banned
    if (!AuthenticationHelper::validateMachine())
        throw std::runtime_error(qPrintable(failedToPublishErrorMsg));

	std::string uploadUrl;
	if (RobloxIDEDoc::getIsCloudEditSession())
	{
		QString baseUrl = RobloxSettings::getBaseURL();
		baseUrl = QString::fromStdString(ReplaceTopSubdomain(baseUrl.toStdString(), "data"));
		uploadUrl = QString("%1/Data/Upload.ashx?assetid=%2").arg(baseUrl).arg(m_pDataModel->getPlaceID()).toStdString();
	}
	else
	{
		if (ARL::Visit* visit = ARL::ServiceProvider::find<ARL::Visit>(m_pDataModel))
			uploadUrl = visit->getUploadUrl();
	}
	
	if(!uploadUrl.empty())
	{
		{ // Concurrency guard
			publishingMutex.lock();
			if (m_bIsPublishInProcess)
			{
				publishingMutex.unlock();
                throw std::runtime_error(qPrintable(failedToPublishErrorMsg));
			}

			m_bIsPublishInProcess = true;
			publishingMutex.unlock();
		}


        bool error;
        QString errorTitle;
        QString errorText;

		UpdateUIManager::Instance().waitForLongProcess(
			"Publishing",
            boost::bind(
                &PublishToRobloxVerb::save,
                this,
                ARL::ContentId(uploadUrl),
                &error,
                &errorTitle,
                &errorText ) );

        if ( error )
        {
            QMessageBox::critical(m_pMainWindow, errorTitle, errorText);
        }
		else 
		{
			if (!RobloxDocManager::Instance().getPlayDoc()->isLocalDoc())
			{
				RobloxDocManager::Instance().getPlayDoc()->resetDirty(m_pDataModel);
			}
			ARL::StandardOut::singleton()->printf(ARL::MESSAGE_INFO, "Successfully published - %s", qPrintable(RobloxDocManager::Instance().getPlayDoc()->displayName()));
		}
	}
	else
		throw ARL::runtime_error("Cannot publish without a visit script!");
}

bool PublishToRobloxVerb::isEnabled() const
{
	return RobloxIDEDoc::getIsCloudEditSession() ||
		(!m_bIsPublishInProcess && m_pDataModel && ARL::ServiceProvider::find<ARL::Visit>(m_pDataModel) && ARL::DataModel::canSave(m_pDataModel));
}

void PublishToRobloxVerb::save(ARL::ContentId contentID,bool* outError,QString* outErrorTitle,QString* outErrorText)
{
    ARLASSERT(outError);
    ARLASSERT(outErrorTitle);
    ARLASSERT(outErrorText);

    *outError = true;
    *outErrorTitle = "Failed to Publish";
    *outErrorText = "Failed to publish place!";

    try
    {
        if (FFlag::StudioCSGAssets)
            PartOperationAsset::publishAll(m_pDataModel);

		// it is possible to publish from build mode, where game explorer isn't initialized
		RobloxGameExplorer& rge = UpdateUIManager::Instance().getViewWidget<RobloxGameExplorer>(eDW_GAME_EXPLORER);
		if (rge.getCurrentGameId() > 0)
			rge.publishNamedAssetsToCurrentSlot();

		if (!RobloxIDEDoc::getIsCloudEditSession())
		{
			m_pDataModel->save(contentID);
		}
		else
		{
			// not using datamodel save here, as it checks for DataModel::canSave which returns false in CloudEdit mode as visit->uploadUrl is empty
			shared_ptr<std::stringstream> stream = m_pDataModel->serializeDataModel();
			std::string response;
			ARL::Http saveRequest(contentID.toString());
			saveRequest.post(*stream, Http::kContentTypeDefaultUnspecified, true, response);
		}

		RobloxMainWindow::get(RobloxDocManager::Instance().getPlayDoc())->getStudioAnalytics()->reportPublishStats(m_pDataModel);

        *outError = false;
    }
    catch (ARL::DataModel::SerializationException e)
    {
        m_bIsPublishInProcess = false;

        shared_ptr<const ARL::Reflection::ValueTable> values(new ARL::Reflection::ValueTable);

        std::stringstream jsonStream(e.what());
        bool parsed = ARL::WebParser::parseJSONTable(e.what(), values);

		if (parsed)
		{
			ARL::Reflection::ValueTable::const_iterator itTitle = values->find("title");
			ARL::Reflection::ValueTable::const_iterator itError = values->find("error");

			if (itTitle != values->end())
				*outErrorTitle = itTitle->second.get<std::string>().c_str();

			if (itError != values->end())
				*outErrorText = itError->second.get<std::string>().c_str();

			if (itError == values->end())
			{
				ARL::StandardOut::singleton()->printf(ARL::MESSAGE_ERROR,
					"Error while publishing: %s", e.what());
			}
		}
		else
		{
			*outErrorText = e.what();
		}
    }
	catch (const ARL::base_exception& e)
	{
		m_bIsPublishInProcess = false;
		*outErrorTitle = "Error while publishing";
		*outErrorText = e.what();
		ARL::StandardOut::singleton()->printf(ARL::MESSAGE_ERROR,
			"Error while publishing: %s", e.what());
	}
    catch(...)
    {
        m_bIsPublishInProcess = false;
		// NOTE: because this function is called in waitForLong process, all exceptions are swallowed
		// without notifying user.
        throw;
    }
}
InsertAdvancedObjectViewVerb::InsertAdvancedObjectViewVerb(ARL::VerbContainer* pVerbContainer)
: Verb(pVerbContainer, "InsertAdvancedObjectDialogVerb")
{
	// make sure we remain in sync with dockwidget's toggled state
	UpdateUIManager& uiManager = UpdateUIManager::Instance();
	QObject::connect(uiManager.getDockAction(eDW_BASIC_OBJECTS), SIGNAL(toggled(bool)), uiManager.getAction("actionInsertAdvancedObject"), SLOT(toggle()));
}

InsertAdvancedObjectViewVerb::~InsertAdvancedObjectViewVerb()
{
	UpdateUIManager& uiManager = UpdateUIManager::Instance();
	QObject::disconnect(uiManager.getDockAction(eDW_BASIC_OBJECTS), SIGNAL(toggled(bool)), uiManager.getAction("actionInsertAdvancedObject"), SLOT(toggle()));
}

void InsertAdvancedObjectViewVerb::doIt( ARL::IDataState* dataState )
{
	if (UpdateUIManager::Instance().getDockAction(eDW_BASIC_OBJECTS))
		UpdateUIManager::Instance().getDockAction(eDW_BASIC_OBJECTS)->trigger();
}

bool InsertAdvancedObjectViewVerb::isEnabled() const
{
	return RobloxDocManager::Instance().getPlayDoc() != NULL;
}

bool InsertAdvancedObjectViewVerb::isChecked() const
{
	return UpdateUIManager::Instance().getDockWidget(eDW_BASIC_OBJECTS)->isVisible();
}

JointToolHelpDialogVerb::JointToolHelpDialogVerb( ARL::VerbContainer* pVerbContainer )
: Verb(pVerbContainer, "JointToolHelpDialogVerb")
{
}

void JointToolHelpDialogVerb::doIt( ARL::IDataState* dataState )
{
	QDesktopServices::openUrl(QUrl("http://wiki.lambda.cam/index.php/Joint#Automatic_creation"));
}

StudioMaterialVerb::StudioMaterialVerb(ARL::DataModel* dataModel)
: MaterialVerb(dataModel, "StudioMaterialVerb")
{
	// initialize default value
	StudioMaterialVerb::sMaterialActionActAsTool = RobloxSettings().value("rbxMaterialActionActAsTool", false).toBool();;
}

void StudioMaterialVerb::doIt( ARL::IDataState* dataState )
{
	//set material
	QString currentMaterial = NameValueStoreManager::singleton().getValue("actionMaterialSelector", "user_value").toString();
	if (currentMaterial.isEmpty())
		return;

	// check if we need to execute action as tool
	if (StudioMaterialVerb::sMaterialActionActAsTool)
	{
		// set material
		ARL::MaterialTool::material = ARL::MaterialVerb::parseMaterial(currentMaterial.toStdString());
		// get verb associated with tool and execute
		ARL::Verb* materialVerb = dataModel->getVerb("MaterialTool");
		if (materialVerb)
			materialVerb->doIt(dataState);
	}
	else
	{
		ARL::MaterialVerb::setCurrentMaterial(ARL::MaterialVerb::parseMaterial(currentMaterial.toStdString()));
		//execute verb
		ARL::MaterialVerb::doIt(dataState);
	}
}

bool StudioMaterialVerb::isChecked() const
{
	if (StudioMaterialVerb::sMaterialActionActAsTool)
	{
		ARL::Verb* materialVerb = dataModel->getVerb("MaterialTool");
		if (materialVerb)
			return materialVerb->isChecked();
	}

	return false;
}

StudioColorVerb::StudioColorVerb(ARL::DataModel* dataModel)
: ColorVerb(dataModel, "StudioColorVerb")
{
	addColorToIcon();
	// initialize default value
	StudioColorVerb::sColorActionActAsTool = RobloxSettings().value("rbxColorActionActAsTool", false).toBool();
}

void StudioColorVerb::doIt( ARL::IDataState* dataState )
{
	//set color
    ARL::BrickColor selectedBrickColor(NameValueStoreManager::singleton().getValue("actionColorSelector", "user_value").toInt());
	ARL::ColorVerb::setCurrentColor(selectedBrickColor);

	if (StudioColorVerb::sColorActionActAsTool)
	{
		// color for fill tool
		ARL::FillTool::color.set(selectedBrickColor);
		// execute fill tool
		ARL::Verb* fillToolVerb = dataModel->getVerb("FillTool");
		if (fillToolVerb)
			fillToolVerb->doIt(dataState);
	}
	else
	{
		//execute verb
		ARL::ColorVerb::doIt(dataState);
	}

	//updata action icon
	addColorToIcon();
}

bool StudioColorVerb::isChecked() const
{
	if (StudioColorVerb::sColorActionActAsTool)
	{
		ARL::Verb* fillToolVerb = dataModel->getVerb("FillTool");
		if (fillToolVerb)
			return fillToolVerb->isChecked();
	}

	return false;
}

void StudioColorVerb::addColorToIcon()
{
	//update icon
    QList<QAction*> colorActions = UpdateUIManager::Instance().getMainWindow().findChildren<QAction*>("actionColorSelector");
    for (int i = 0; i < colorActions.count(); i++)
    {
        QAction* pColorAction = colorActions[i];

        QColor selectedQColor = QtUtilities::toQColor(ARL::ColorVerb::getCurrentColor().color3());

        // Draw the color inside of a filled rectangle on the bottom of the icon
        QPixmap pix = pColorAction->icon().pixmap(pColorAction->icon().availableSizes()[0]);
        QPainter p;
        p.begin(&pix);
        QRect rect = pix.rect();
        p.fillRect(rect, selectedQColor);
        p.end();

        pColorAction->setIcon(QIcon(pix));
    }
}

OpenToolBoxWithOptionsVerb::OpenToolBoxWithOptionsVerb( ARL::VerbContainer* pVerbContainer )
: Verb(pVerbContainer, "OpenToolBoxWithOptionsVerb")
{
	connect(UpdateUIManager::Instance().getDockWidget(eDW_TOOLBOX), SIGNAL(visibilityChanged(bool)), this, SLOT(handleDockVisibilityChanged(bool)));
}

OpenToolBoxWithOptionsVerb::~OpenToolBoxWithOptionsVerb()
{
	disconnect(UpdateUIManager::Instance().getDockWidget(eDW_TOOLBOX), SIGNAL(visibilityChanged(bool)), this, SLOT(handleDockVisibilityChanged(bool)));
}

void OpenToolBoxWithOptionsVerb::doIt(ARL::IDataState* dataState)
{
	QString setUrl = NameValueStoreManager::singleton().getValue("openToolBoxWithOptions", "user_value").toString();
	if (!setUrl.isEmpty())
	{
		RobloxMainWindow& rbxMainWindow = UpdateUIManager::Instance().getMainWindow();
		if (NameValueStoreManager::singleton().getValue("openToolBoxWithOptions", "requiresauthentication").toBool() && !RobloxUser::singleton().getUserId())
		{
			QMessageBox::information(&rbxMainWindow, "Log in required", "You must log in to perform this action!", QDialogButtonBox::Ok);
			if (!rbxMainWindow.actionStartPage->isChecked())
				rbxMainWindow.actionStartPage->setChecked(true);

			rbxMainWindow.openStartPage(true, "showlogin=True");
			return;
		}

		RobloxToolBox& robloxToolBox = UpdateUIManager::Instance().getViewWidget<RobloxToolBox>(eDW_TOOLBOX);
		robloxToolBox.loadUrl(QString("%1/%2").arg(RobloxSettings::getBaseURL()).arg(setUrl));

		UpdateUIManager::Instance().setDockVisibility(eDW_TOOLBOX, true);
	}
	else
	{
		UpdateUIManager::Instance().setDockVisibility(eDW_TOOLBOX, false);
	}
}

bool OpenToolBoxWithOptionsVerb::isEnabled()
{
	return RobloxDocManager::Instance().getPlayDoc() != NULL;
}

void OpenToolBoxWithOptionsVerb::handleDockVisibilityChanged(bool isVisible)
{
	if (!isVisible)
	{
		QActionGroup* pActionGroup = UpdateUIManager::Instance().getMainWindow().findChild<QActionGroup*>("openToolBoxWithOptions");
		if (pActionGroup)
		{
			QList<QAction*> actions = pActionGroup->actions();
			for (int i = 0; i < actions.count(); i++)
				actions.at(i)->setChecked(false);
		}
	}
}

InsertBasicObjectVerb::InsertBasicObjectVerb(ARL::DataModel* dataModel)
: Verb(dataModel, "InsertBasicObjectVerb")
, m_pDataModel(dataModel)
{
}

void InsertBasicObjectVerb::doIt(ARL::IDataState* dataState)
{
	QString instanceName = NameValueStoreManager::singleton().getValue("insertBasicObject", "user_value").toString();
	if (instanceName.isEmpty())
		return;

	boost::shared_ptr<ARL::Instance> pInstance = ARL::Creatable<ARL::Instance>::createByName(ARL::Name::lookup(instanceName.toStdString()), ARL::EngineCreator);
	if (!pInstance)
	{
		// Custom logic here for data types that aren't in our instances
		if (instanceName == "Cylinder")
		{
			shared_ptr<ARL::BasicPartInstance> part = ARL::Creatable<ARL::Instance>::create<ARL::BasicPartInstance>();
			part->setLegacyPartTypeUi(ARL::BasicPartInstance::CYLINDER_LEGACY_PART);
			part->getPartPrimitive()->setSurfaceType(ARL::NORM_Y, ARL::NO_SURFACE);
			part->getPartPrimitive()->setSurfaceType(ARL::NORM_Y_NEG, ARL::NO_SURFACE);
			pInstance = part;
		}
		else if (instanceName == "Sphere")
		{
			shared_ptr<ARL::BasicPartInstance> part = ARL::Creatable<ARL::Instance>::create<ARL::BasicPartInstance>();
			part->setLegacyPartTypeUi(ARL::BasicPartInstance::BALL_LEGACY_PART);
			part->getPartPrimitive()->setSurfaceType(ARL::NORM_Y, ARL::NO_SURFACE);
			part->getPartPrimitive()->setSurfaceType(ARL::NORM_Y_NEG, ARL::NO_SURFACE);
			part->getPartPrimitive()->setSize(G3D::Vector3(4.0f, 4.0f, 4.0f));
			pInstance = part;
		}
		else
			return;
	}

	// Insert it
	InsertObjectWidget::InsertObject(pInstance, shared_from(m_pDataModel), InsertObjectWidget::InsertMode_RibbonAction);
}

bool InsertBasicObjectVerb::isEnabled()
{
	return RobloxDocManager::Instance().getPlayDoc() != NULL;
}

JointCreationModeVerb::JointCreationModeVerb(ARL::DataModel* dataModel)
: Verb(dataModel, "JointCreationModeVerb")
{
	RobloxSettings settings;
	int jointCreationMode  = settings.value(sRibbonJointCreationMode, 0).toInt();

	if (jointCreationMode == 2)
	{
		ARL::AdvArrowTool::advManualJointMode = false;
		ARL::AdvArrowTool::advCreateJointsMode = false;
	}
	else
	{
		ARL::AdvArrowTool::advManualJointMode = (bool)jointCreationMode;
		ARL::AdvArrowTool::advCreateJointsMode = true;
	}

	updateMenuActions();
	updateMenuIcon();
}

void JointCreationModeVerb::doIt(ARL::IDataState* dataState)
{
	QString jointCreationMode = NameValueStoreManager::singleton().getValue("jointCreationMode", "user_value").toString();
	if (jointCreationMode.isEmpty())
		return;

	RobloxSettings settings;

	if (jointCreationMode == "Never")
	{
		ARL::AdvArrowTool::advManualJointMode = false;
		ARL::AdvArrowTool::advCreateJointsMode = false;

		settings.setValue(sRibbonJointCreationMode, 2);
	}
	else if (jointCreationMode == "Always")
	{
		ARL::AdvArrowTool::advManualJointMode = true;
		ARL::AdvArrowTool::advCreateJointsMode = true;

		settings.setValue(sRibbonJointCreationMode, 1);
	}
	else
	{
		ARL::AdvArrowTool::advManualJointMode = false;
		ARL::AdvArrowTool::advCreateJointsMode = true;

		settings.setValue(sRibbonJointCreationMode, 0);
	}

	updateMenuIcon();
}

void JointCreationModeVerb::updateMenuIcon()
{
	QList<QMenu*>     menuList = UpdateUIManager::Instance().getMainWindow().findChildren<QMenu*>("jointCreationMode");
	QActionGroup* pActionGroup = UpdateUIManager::Instance().getMainWindow().findChild<QActionGroup*>("jointCreationMode");

	if (!menuList.size() || !pActionGroup || !pActionGroup->checkedAction())
		return;

	for (int ii = 0; ii < menuList.size(); ++ii)
		menuList[ii]->setIcon(pActionGroup->checkedAction()->icon());
}

void JointCreationModeVerb::updateMenuActions()
{
	QActionGroup* pActionGroup = UpdateUIManager::Instance().getMainWindow().findChild<QActionGroup*>("jointCreationMode");
	if (pActionGroup)
	{
		QList<QAction*> actions = pActionGroup->actions();
		if (actions.count() == 3)
		{
			actions.at(0)->setChecked(ARL::AdvArrowTool::advManualJointMode && ARL::AdvArrowTool::advCreateJointsMode);
			actions.at(1)->setChecked(!ARL::AdvArrowTool::advManualJointMode && ARL::AdvArrowTool::advCreateJointsMode);
			actions.at(2)->setChecked(!ARL::AdvArrowTool::advManualJointMode && !ARL::AdvArrowTool::advCreateJointsMode);
		}
	}
}

LaunchHelpForSelectionVerb::LaunchHelpForSelectionVerb(ARL::DataModel* pDataModel)
: ARL::Verb(pDataModel, "LaunchHelpForSelectionVerb")
, m_pDataModel(pDataModel)
{
}

bool LaunchHelpForSelectionVerb::isEnabled() const
{
	// do not enable help for multiple selection or no selection
	ARL::Selection* selection = ARL::ServiceProvider::find<ARL::Selection>(m_pDataModel);
	return (selection && selection->size() == 1);
}

void LaunchHelpForSelectionVerb::doIt(ARL::IDataState* dataState)
{
	ARL::Selection* selection = ARL::ServiceProvider::find<ARL::Selection>(m_pDataModel);
	if (selection && selection->size() == 1)
	{
		boost::shared_ptr<ARL::Instance> instance = *(selection->begin());
		QString className = instance->getClassName().c_str();
		if(FFlag::StudioNewWiki)
			className.prepend("API:Class/");

		if (!className.isEmpty())
		{
			// if Tutorial widget is visible and Context Help widget is not visible
			//    then add Context Help widget in a tab group with Tutorial widget
			UpdateUIManager& updateManager = UpdateUIManager::Instance();
			QDockWidget* tutorialWidget = updateManager.getDockWidget(eDW_TUTORIALS);
			if (tutorialWidget && tutorialWidget->isVisible())
			{
				QDockWidget* contextHelpWidget = updateManager.getDockWidget(eDW_CONTEXTUAL_HELP);
				if (contextHelpWidget && !contextHelpWidget->isVisible())
				{
					updateManager.getMainWindow().tabifyDockWidget(tutorialWidget, contextHelpWidget);
					updateManager.setDockVisibility(eDW_CONTEXTUAL_HELP, true);
					// bring help widget to front
					contextHelpWidget->raise();
				}
			}
			else
			{
				updateManager.setDockVisibility(eDW_CONTEXTUAL_HELP, true);
			}

			// update context help
			QMetaObject::invokeMethod(&RobloxContextualHelpService::singleton(), "onHelpTopicChanged", Q_ARG(QString, className));
		}
	}
}
