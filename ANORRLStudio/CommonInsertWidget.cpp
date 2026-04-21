/**
 * CommonInsertWidget.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "CommonInsertWidget.h"

// Qt Headers
#include <QTabWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QTimer>
#include <QDockWidget>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMouseEvent>
#include <QDrag>
#include <QKeyEvent>

// Roblox Headers
#include "RobloxSettings.h"
#include "script/script.h"
#include "Util/RobloxGoogleAnalytics.h"
#include "util/Math.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/Selection.h"
#include "v8datamodel/MegaCluster.h"
#include "v8datamodel/Explosion.h"
#include "v8datamodel/SpecialMesh.h"
#include "V8DataModel/GuiObject.h"
#include "V8DataModel/BillboardGui.h"
#include "V8DataModel/TextBox.h"
#include "V8DataModel/TextLabel.h"
#include "V8DataModel/TextButton.h"
#include "v8datamodel/ImageButton.h"
#include "v8datamodel/ImageLabel.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/Decal.h"
#include "v8datamodel/SpawnLocation.h"
#include "v8datamodel/Filters.h"
#include "V8World/World.h"
#include "v8tree/Service.h"

// Roblox Studio Headers
#include "AuthoringSettings.h"
#include "InsertObjectListWidget.h"
#include "InsertObjectListWidgetItem.h"
#include "LuaSourceBuffer.h"
#include "QtUtilities.h"
#include "ReflectionMetadata.h"
#include "RobloxMainWindow.h"
#include "RobloxToolBox.h"
#include "RobloxTreeWidget.h"
#include "RobloxCustomWidgets.h"
#include "RobloxDocManager.h"
#include "RobloxIDEDoc.h"
#include "UpdateUIManager.h"
#include "v8datamodel/BasicPartInstance.h"
#include "v8datamodel/VehicleSeat.h"


FASTFLAG(GoogleAnalyticsTrackingEnabled)

static const char* kInsertedItemSelectedSetting = "insertedItemSelectedSetting";

QSet<QString> InsertObjectWidget::m_sObjectExceptions;
QHash<QString, QVariant> InsertObjectWidget::m_objectWeights;

ARL::Vector3  InsertObjectWidget::m_sCachedInsertLocation;
ARL::CoordinateFrame InsertObjectWidget::m_sCameraCFrame;
bool InsertObjectWidget::m_sIsInsertLocationValid = false;;

QSize InsertObjectWidget::sizeHint() const 
{
	return QSize(width(), 700);
}

/*
	Creates an item to populate the Insert -> Basic Objects and Insert -> Service panels.

	Rules:
		Don't do Terrain
		Don't do DataModel
		AuthoringSettings::isShown() must return true
		The object must be creatable by Scripts
		The object must be visible in the tree view
		Don't show Services in the Basic Objects panel
*/
void InsertObjectWidget::populateListHelper(bool services, QList<InsertObjectListWidgetItem*> *itemList, const ARL::Reflection::ClassDescriptor& pDescriptor, shared_ptr<ARL::DataModel> pDataModel)
{
	if (pDescriptor.name == ARL::DataModel::className())
		return;

	// TODO: A better generic solution if we encounter more such cases
	// Currently hardcoding the removal of Terrain from the Insert Objects' list
	if(pDescriptor.isA(ARL::MegaClusterInstance::classDescriptor()) && (pDescriptor.name == "Terrain"))
		return;
	
	ARL::Reflection::Metadata::Class* pMetadata = ARL::Reflection::Metadata::Reflection::singleton()->get(pDescriptor, false);
	if (AuthoringSettings::singleton().isShown(pMetadata, pDescriptor))
	{
		shared_ptr<ARL::Instance> instance;
		
		try
		{
			instance = ARL::Creatable<ARL::Instance>::createByName(pDescriptor.name, ARL::EngineCreator);
			bool isService = dynamic_cast<ARL::Service*>(instance.get()) != NULL;
			if (isService != services)
				return;
		}
		catch (std::exception &)
		{
			// We couldn't create it, so we're done
			return;
		}
		
		if (instance)
		{
            if (services && pDataModel && instanceExistsInDataModel(instance, pDataModel))
                return;

			std::string preferredParentStr = pMetadata->getPreferredParent();
            itemList->append(new InsertObjectListWidgetItem(
                pDescriptor.name.c_str(),
                pMetadata->description.c_str(),
                instance, 
                preferredParentStr ));
		}
	}
}

/*
	Recursively creates items to populate the Insert -> Basic Object and Insert -> Service panels.
*/
void InsertObjectWidget::populateList(bool services, QList<InsertObjectListWidgetItem*> *itemList, const ARL::Reflection::ClassDescriptor* pDescriptor, shared_ptr<ARL::DataModel> pDataModel)
{
    ARLASSERT(pDescriptor);

    // recursive
    std::for_each(pDescriptor->derivedClasses_begin(), pDescriptor->derivedClasses_end(), boost::bind(&populateList, services, itemList, _1, pDataModel));

    populateListHelper(services, itemList, *pDescriptor, pDataModel);
}


void InsertObjectWidget::createWidgets(bool services, InsertObjectListWidget* listWidget, const ARL::Reflection::ClassDescriptor* pDescriptor, shared_ptr<ARL::DataModel> pDataModel)
{
    QList<InsertObjectListWidgetItem*>* itemList = new QList<InsertObjectListWidgetItem*>();
    populateList(services, itemList, pDescriptor, pDataModel);

    for (QList<InsertObjectListWidgetItem*>::iterator i = itemList->begin(); i != itemList->end(); i++)
        listWidget->addItem(*i);
}

bool InsertObjectWidget::instanceExistsInDataModel(shared_ptr<ARL::Instance> instance, shared_ptr<ARL::DataModel> pDataModel)
{
    if (!pDataModel)
        return false;

    ARL::Instance* pInstance = pDataModel.get();

    if (pInstance->getChildren())
        for (ARL::Instances::const_iterator iter = pInstance->getChildren()->begin(); iter != pInstance->getChildren()->end(); ++iter)
            if (instance->getClassNameStr() == iter->get()->getClassNameStr())
                return true;
    
    return false;
}

/**
 * Generates a list of instances that can be used to populate basic objects or service insert dialogs/widgets.
 *  Populates the map recusively.  If a parent is set, the objects must be able to be children
 *  of the parent.
 *
 * @param descriptor	parent descriptor to start getting child descriptors
 * @param instanceMap	output mapping of class names to instances
 * @param parent		optional parent to check if the instances can be a child of
 */
void InsertObjectWidget::generateInstanceMap(
    const ARL::Reflection::ClassDescriptor* descriptor,
    tInstanceMap*                           instanceMap,
    ARL::Instance*                          parent )
{
    std::for_each(
        descriptor->derivedClasses_begin(),
        descriptor->derivedClasses_end(),
        boost::bind(&generateInstanceMap,_1,instanceMap,parent) );

	if ( descriptor->name == ARL::DataModel::className() )
		return;

	if ( descriptor->isA(ARL::MegaClusterInstance::classDescriptor()) && (descriptor->name == "Terrain") )
		return;
	
	ARL::Reflection::Metadata::Class* pMetadata = ARL::Reflection::Metadata::Reflection::singleton()->get(*descriptor,false);
	if ( AuthoringSettings::singleton().isShown(pMetadata,*descriptor) )
	{
		shared_ptr<ARL::Instance> instance;
		
		try
		{
			instance = ARL::Creatable<ARL::Instance>::createByName(descriptor->name, ARL::EngineCreator);
			bool isService = dynamic_cast<ARL::Service*>(instance.get()) != NULL;
			if ( isService )
				return;
		}
		catch (std::exception &)
		{
			// We couldn't create it, so we're done
			return;
		}
		
		if ( instance )
		{
			if ( !descriptor->isScriptCreatable() )
				return;

			if ( !pMetadata || !pMetadata->isExplorerItem() )
				return;

            if ( parent && !instance->canSetParent(parent) )
                return;

            if(m_sObjectExceptions.contains(QString(instance->getClassName().c_str())))
                return;

            (*instanceMap)[descriptor->name.c_str()] = instance;
		}
	}
}

/**
 * Creates a menu for inserting basic objects.
 *
 * @param	parent		optional parent that the instances can be children of
 * @param	receiver	SLOT receiver object when clicking on the menu actions
 * @param	member		SLOT when clicking on the menu actions
 */
QMenu* InsertObjectWidget::createMenu(
    ARL::Instance*  parent,
    const QObject*  receiver, 
    const char*     member )
{
    tInstanceMap instanceMap;
    generateInstanceMap(
        &ARL::Reflection::ClassDescriptor::rootDescriptor(),
        &instanceMap,
        parent );

    QMenu* insertObjectMenu = new QMenu("Insert Object");
    
    QList<QString> keys = instanceMap.keys();
    qSort(keys);
    
    QList<QString>::const_iterator iter = keys.begin();
    for ( ; iter != keys.end() ; ++iter )
    {
        QAction* action = insertObjectMenu->addAction(*iter);
        shared_ptr<ARL::Instance> instance = instanceMap[*iter];
        action->setIcon(QIcon(QtUtilities::getPixmap(":/images/ClassImages.PNG",RobloxTreeWidgetItem::getImageIndex(instance))));        

        connect(action,SIGNAL(triggered(bool)),receiver,member);
    }

    return insertObjectMenu;
}

InsertObjectWidget::InsertObjectWidget(QWidget *pParentWidget)
: m_bRedrawRequested(false)
, m_bInitializationRequired(true)
, m_ItemList(new QList<InsertObjectListWidgetItem*>())
, m_quickInsertPreviousWidget(NULL)
, m_closeDockWhenDoneWithQuickInsert(false)
{
	m_pInsertObjectListWidget = new InsertObjectListWidget(this);
	m_pInsertObjectListWidget->setAutoScroll(true);
	m_pInsertObjectListWidget->setDragEnabled(true);
	m_pInsertObjectListWidget->setDragDropMode(QAbstractItemView::DragOnly);
	m_pInsertObjectListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pInsertObjectListWidget->setDropIndicatorShown(true);
	m_pInsertObjectListWidget->setObjectName("InsertObjectListWidget");
	m_pInsertObjectListWidget->setResizeMode(QListView::Adjust);
	m_pInsertObjectListWidget->setAlternatingRowColors(true);

	m_sObjectExceptions.insert("Accoutrement");
	m_sObjectExceptions.insert("AnimationController");
	m_sObjectExceptions.insert("ArcHandles");
	m_sObjectExceptions.insert("Backpack");
	m_sObjectExceptions.insert("BodyColors");
	m_sObjectExceptions.insert("Camera");
	m_sObjectExceptions.insert("CharacterMesh");
	m_sObjectExceptions.insert("FileMesh");
	m_sObjectExceptions.insert("Flag");
	m_sObjectExceptions.insert("FlagStand");
	m_sObjectExceptions.insert("FloorWire");
	m_sObjectExceptions.insert("Glue");
	m_sObjectExceptions.insert("Handles");
	m_sObjectExceptions.insert("Hole");
	m_sObjectExceptions.insert("KeyFrame");
	m_sObjectExceptions.insert("Motor");
	m_sObjectExceptions.insert("MotorFeature");
	m_sObjectExceptions.insert("Pants");
	m_sObjectExceptions.insert("PlayerGui");
	m_sObjectExceptions.insert("Plugin");
	m_sObjectExceptions.insert("Pose");
	m_sObjectExceptions.insert("RotateP");
	m_sObjectExceptions.insert("RotateV");
	m_sObjectExceptions.insert("SelectionBox");
	m_sObjectExceptions.insert("SelectionSphere");
	m_sObjectExceptions.insert("Shirt");
	m_sObjectExceptions.insert("ShirtGraphic");
	m_sObjectExceptions.insert("SkateboardPlatform");
	m_sObjectExceptions.insert("Skin");
	m_sObjectExceptions.insert("Snap");
	m_sObjectExceptions.insert("StarterGear");
	m_sObjectExceptions.insert("Status");
	m_sObjectExceptions.insert("SurfaceSelection");
	m_sObjectExceptions.insert("TerrainRegion");
	m_sObjectExceptions.insert("TextureTrail");
	m_sObjectExceptions.insert("VelocityMotor");
	m_sObjectExceptions.insert("Weld");

	m_pFilterEdit = new QLineEdit(this);

	m_pFilterEdit->setPlaceholderText(QString("Search object (%1)").arg(UpdateUIManager::Instance().getMainWindow().quickInsertAction->shortcut().toString()));

	setFocusProxy(m_pFilterEdit);
	m_pFilterEdit->installEventFilter(this);

	m_pCheckBox = new QCheckBox(this);
	m_pCheckBox->setText("Select inserted object");	

	QGridLayout *pGridLayout = new QGridLayout();
	pGridLayout->setContentsMargins(0, 0, 0, 0);
	pGridLayout->setHorizontalSpacing(4);

	QGridLayout *checkBoxLayout = new QGridLayout();
	checkBoxLayout->setContentsMargins(5, 0, 0, 0);
	checkBoxLayout->setHorizontalSpacing(4);
	checkBoxLayout->addWidget(m_pCheckBox, 0, 0);

	pGridLayout->addWidget(m_pFilterEdit, 0, 0);
	pGridLayout->addWidget(m_pInsertObjectListWidget, 1, 0);
	pGridLayout->addLayout(checkBoxLayout, 2, 0);

	m_pCheckBox->setChecked(RobloxSettings().value(kInsertedItemSelectedSetting, false).toBool());
	connect(m_pCheckBox, SIGNAL(toggled(bool)), this, SLOT(onSelectOnInsertClicked(bool)));
	connect(m_pFilterEdit, SIGNAL(textChanged(QString)), this, SLOT(onFilter(QString)));
    connect(m_pFilterEdit, SIGNAL(returnPressed()), this, SLOT(onItemInsertRequested()));

	setLayout(pGridLayout);

	//update dockwidget and action name
	if (UpdateUIManager::Instance().getMainWindow().isRibbonStyle())
	{
		QDockWidget* pDockWidget = qobject_cast<QDockWidget*>(pParentWidget);
		if (pDockWidget)	
			pDockWidget->setWindowTitle(QObject::tr("Advanced Objects"));

		QAction* pDockAction = UpdateUIManager::Instance().getDockAction(eDW_BASIC_OBJECTS);
		if (pDockAction)
		{
			pDockAction->setText(QObject::tr("Advanced Objects"));
			pDockAction->setToolTip(QObject::tr("Advanced Objects"));
		}		
	}

    // Get Object Weights
	RobloxSettings settings;
	m_objectWeights = settings.value("rbxObjectWeights").toHash();
}

void InsertObjectWidget::onSelectOnInsertClicked(bool checked)
{
	RobloxSettings().setValue(kInsertedItemSelectedSetting, checked);
}

InsertObjectWidget::~InsertObjectWidget()
{
    // Set Object Weights
	RobloxSettings settings;
	settings.setValue("rbxObjectWeights", m_objectWeights);

    m_ItemList->clear();
	m_cSelectionChanged.disconnect();
}

void InsertObjectWidget::setDataModel(boost::shared_ptr<ARL::DataModel> pDataModel)
{
	if(m_pDataModel == pDataModel)
		return;
	
	//ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR, "InsertObjectWidget::setDataModel");
	m_pFilterEdit->clear();
	m_pInsertObjectListWidget->clear();	
	m_cSelectionChanged.disconnect();
	m_bInitializationRequired = true;
	
	m_pDataModel = pDataModel;

	if (m_pDataModel)
	{
		requestDialogRedraw();
		connectSelectionChangeEvent(true);
	}
}

void InsertObjectWidget::updateWidget(bool state)
{
	//ARL::StandardOut::singleton()->printf(ARL::MESSAGE_ERROR, "InsertObjectWidget::updateWidget %d", state);
	if (state)
		requestDialogRedraw();
	connectSelectionChangeEvent(state);
}

void InsertObjectWidget::setVisible(bool visible)
{
	//ARL::StandardOut::singleton()->printf(ARL::MESSAGE_ERROR, "InsertObjectWidget::setVisible %d", visible);
	updateWidget(visible);
	QWidget::setVisible(visible);
}

bool InsertObjectWidget::event(QEvent* e)
{    
    bool isHandled = QWidget::event(e);
    
    if ( e->type() == QEvent::ShortcutOverride )
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
        QChar key = keyEvent->key();

        // capture single key alpha numerical presses - we want these to be considered
        //  as shortcuts in the tree to jumped to items starting with that key, not global shortcuts
        if ( !(key >= Qt::Key_F1 && key <= Qt::Key_F35) && 
             key.isLetterOrNumber() && 
             keyEvent->modifiers() == Qt::NoModifier )
        {
            keyEvent->accept();
            isHandled = true;
        }
    }

    return isHandled;
}

bool InsertObjectWidget::eventFilter(QObject* watched, QEvent* evt)
{
    if (evt->type() == QEvent::FocusIn)
    {
        QFocusEvent* focusEvent = static_cast<QFocusEvent*>(evt);
        
        if (focusEvent && focusEvent->reason() == Qt::ShortcutFocusReason)
            m_pFilterEdit->selectAll();
    }
    else if (evt->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(evt);
        QChar key = keyEvent->key();
        if (key == Qt::Key_Up)
            if (m_pInsertObjectListWidget->currentRow() > 0)
                m_pInsertObjectListWidget->setCurrentRow(m_pInsertObjectListWidget->currentRow() - 1);

        if (key == Qt::Key_Down)
            if (m_pInsertObjectListWidget->currentRow() < m_pInsertObjectListWidget->count() - 1)
                m_pInsertObjectListWidget->setCurrentRow(m_pInsertObjectListWidget->currentRow() + 1);
    }
    return false;
}

void InsertObjectWidget::SetObjectDefaultValues(boost::shared_ptr<ARL::Instance> pObjectToInsert)
{
	if(ARL::GuiObject* guiObject = ARL::Instance::fastDynamicCast<ARL::GuiObject>(pObjectToInsert.get()))
	{
		guiObject->setSize(ARL::UDim2(0,100,0,100));
		guiObject->setBackgroundColor3(ARL::Color3(1.0f,1.0f,1.0f));

		ARL::TextService::FontSize defaultFontSize = ARL::TextService::SIZE_14;
		ARL::TextService::Font defaultFont = ARL::TextService::FONT_SOURCESANS;

		std::string defaultImage = "rbxassetid://133293265";

		// have to check for text/image elements separately (no inheritance is being used, only macros....)
		// trying to get rid of macros leads to diamond inheritance, needs to spend some time on that
		if(ARL::TextLabel* textLabel = ARL::Instance::fastDynamicCast<ARL::TextLabel>(pObjectToInsert.get()))
		{
			textLabel->setFont(defaultFont);
			textLabel->setFontSize(defaultFontSize);
			textLabel->setSize(ARL::UDim2(0,200,0,50));
		}
		else if(ARL::GuiTextButton* textButton = ARL::Instance::fastDynamicCast<ARL::GuiTextButton>(pObjectToInsert.get()))
		{
			textButton->setFont(defaultFont);
			textButton->setFontSize(defaultFontSize);
			textButton->setSize(ARL::UDim2(0,200,0,50));
		}
		else if(ARL::TextBox* textBox = ARL::Instance::fastDynamicCast<ARL::TextBox>(pObjectToInsert.get()))
		{
			textBox->setFont(defaultFont);
			textBox->setFontSize(defaultFontSize);
			textBox->setSize(ARL::UDim2(0,200,0,50)); // Make TextBox more the size of a traditional text box
		}
		else if(ARL::GuiImageButton* imageButton = ARL::Instance::fastDynamicCast<ARL::GuiImageButton>(pObjectToInsert.get()))
			imageButton->setImage(defaultImage);
		else if(ARL::ImageLabel* imageLabel = ARL::Instance::fastDynamicCast<ARL::ImageLabel>(pObjectToInsert.get()))
			imageLabel->setImage(defaultImage);
	}
    else if (ARL::BillboardGui* bbgui = ARL::Instance::fastDynamicCast<ARL::BillboardGui>(pObjectToInsert.get()))
    {
		bbgui->setSize(ARL::UDim2(0,200,0,50));
	}
    else if (ARL::Script* script = ARL::Instance::fastDynamicCast<ARL::Script>(pObjectToInsert.get()))
    {
        script->setEmbeddedCode(ARL::ProtectedString::fromTrustedSource("print 'Hello world!'\n"));
    }
	else if (ARL::SpawnLocation *spawnLocation = ARL::Instance::fastDynamicCast<ARL::SpawnLocation>(pObjectToInsert.get()))
	{
		spawnLocation->setAnchored(true);
		spawnLocation->setPartSizeUi(ARL::Vector3(6, 1, 6));
		spawnLocation->setSurfaceType(ARL::NORM_Y, ARL::NO_SURFACE);
		// create default decal
		boost::shared_ptr<ARL::Decal> decal = ARL::Creatable<ARL::Instance>::create<ARL::Decal>();
		decal->setTexture("rbxasset://Textures/SpawnLocation.png");
		decal->setFace(ARL::NORM_Y);
		decal->setParent(spawnLocation);
	}
	else if (ARL::ModuleScript* moduleScript = ARL::Instance::fastDynamicCast<ARL::ModuleScript>(pObjectToInsert.get()))
	{
		moduleScript->setSource(ARL::ProtectedString::fromTrustedSource("local module = {}\n\nreturn module\n"));
	}

	// FormFactor deprication
	/* if (ARL::FormFactorPart* formFactorPart = ARL::Instance::fastDynamicCast<ARL::FormFactorPart>(pObjectToInsert.get()))
		formFactorPart->setFormFactorUi(ARL::PartInstance::SYMETRIC);
	*/

	if (ARL::VehicleSeat* vehicleSeat = ARL::Instance::fastDynamicCast<ARL::VehicleSeat>(pObjectToInsert.get()))
	{
		vehicleSeat->setPartSizeUi(ARL::Vector3(4, 1, 2));
		vehicleSeat->setColor(ARL::BrickColor::brickBlack());
	}

	if (ARL::Seat* seat = ARL::Instance::fastDynamicCast<ARL::Seat>(pObjectToInsert.get()))
		seat->setColor(ARL::BrickColor::brickBlack());

}

void InsertObjectWidget::InsertObject(
    boost::shared_ptr<ARL::Instance>    pObjectToInsert, 
    boost::shared_ptr<ARL::DataModel>   pDataModel,
	InsertMode insertMode,
	QPoint* mousePosition)
{
	try
	{
		ARL::Instance* pTargetInstance = getTargetInstance(pObjectToInsert.get(), pDataModel);
		bool ribbonBarMode = insertMode == InsertMode_RibbonAction;

		if (ribbonBarMode &&
			ARL::Instance::fastDynamicCast<ARL::PartInstance>(pObjectToInsert.get()) &&
			ARL::Instance::fastDynamicCast<ARL::ModelInstance>(pTargetInstance))
		{
			pTargetInstance = pDataModel->getWorkspace();
		}

		if (!pTargetInstance)
		{
			// in ribbon bar, in case of multiple selections allow part insertion
			if (ribbonBarMode && ARL::Instance::fastDynamicCast<ARL::PartInstance>(pObjectToInsert.get()))
				pTargetInstance = pDataModel->getWorkspace();
			else
				throw std::runtime_error("InsertObject: Cannot do insert object when there are multiple selections. Please make sure you select a valid target object.");
		}

		{
			ARL::DataModel::LegacyLock lock(pDataModel, ARL::DataModelJob::Write);

			if (!pObjectToInsert->canSetParent(pTargetInstance))
			{
				ARL::Selection* selection = ARL::ServiceProvider::create< ARL::Selection >(pDataModel.get());
				ARL::Instance* intendedTarget;
				if (selection && selection->size() > 0)
					intendedTarget = selection->front().get();
				else
					intendedTarget = pTargetInstance;

				throw ARL::runtime_error("%s cannot be a child of %s", pObjectToInsert->getClassNameStr().c_str(), intendedTarget->getName().c_str());
			}

			if (FFlag::GoogleAnalyticsTrackingEnabled)
			{
				ARL::RobloxGoogleAnalytics::trackEvent(GA_CATEGORY_ACTION, "InsertObject", pObjectToInsert->getClassNameStr().c_str());
			}

			ARL::PartInstance* pPartInstance = ARL::Instance::fastDynamicCast<ARL::PartInstance>(pObjectToInsert.get());

			// If it's a part, let's set the material to our current material (dont set a waypoint)
			if (UpdateUIManager::Instance().getMainWindow().isRibbonStyle() && pPartInstance)
			{
				pPartInstance->setRenderMaterial(ARL::MaterialVerb::getCurrentMaterial());
				pPartInstance->setColor(ARL::ColorVerb::getCurrentColor());
			}

			SetObjectDefaultValues(pObjectToInsert);

			if( (pDataModel->getWorkspace() == pTargetInstance) && pPartInstance)
			{
				// get position to insert at
				ARL::Vector3 pos = getInsertLocation(pDataModel, mousePosition);
				// move part up by it's height (required for Smooth Terrain alignment)
				pos.y += pPartInstance->getPartSizeUi().y;
				// paste instances, this will suppress move and use the position hint to insert the part
				ARL::Instances instances;
				instances.push_back(pObjectToInsert);
				pDataModel->getWorkspace()->insertPasteInstances(instances, pTargetInstance, ARL::INSERT_TO_3D_VIEW, ARL::SUPPRESS_PROMPTS, &pos, NULL, false);
				// save camera position
				m_sCameraCFrame = pDataModel->getWorkspace()->getConstCamera()->getCameraCoordinateFrame();
				//TODO: check if we need to position camera, for non mouse insert i.e. doing an insert from TreeView or Ribbon bar?
			}
			else
			{
				pObjectToInsert->setParent(pTargetInstance);
				if (pPartInstance &&
					ARL::PartInstance::nonNullInWorkspace(shared_from(pPartInstance)))
				{
					std::vector<boost::weak_ptr<ARL::PartInstance> > partArray;
					partArray.push_back(weak_from(pPartInstance));
					pDataModel->getWorkspace()->movePartsToCameraFocus(partArray);
				}
				else
				{
					if (ARL::Decal *decal = ARL::Instance::fastDynamicCast<ARL::Decal>(pObjectToInsert.get()))
					{
						pDataModel->getWorkspace()->startDecalDrag(decal, ARL::INSERT_TO_3D_VIEW);
					}
				}
			}

			// If int limit is reached, halve weights
			if (m_objectWeights.value(pObjectToInsert->getClassNameStr().c_str()).toInt() >= std::numeric_limits<int>::max() - 1)
				for (QHash<QString, QVariant>::iterator iter = m_objectWeights.begin(); iter != m_objectWeights.end(); ++iter)
					m_objectWeights[iter.key()] = iter.value().toInt() / 2;

			m_objectWeights[pObjectToInsert->getClassNameStr().c_str()] = m_objectWeights.value(pObjectToInsert->getClassNameStr().c_str()).toInt() + 1;

			//for undo redo (DecalTool will record Decal insertion history)
			if (!ARL::Instance::fastDynamicCast<ARL::Decal>(pObjectToInsert.get()))
			{
				std::string action = ARL::format("Insert %s", pObjectToInsert->getName().c_str());
				ARL::ChangeHistoryService::requestWaypoint(action.c_str(), pDataModel.get());
			}

			// check if we need to select inserted object

			InsertObjectWidget& insertObjectWidget = UpdateUIManager::Instance().getViewWidget<InsertObjectWidget>(eDW_BASIC_OBJECTS);
			if ( insertObjectWidget.isSelectObject() || ((insertMode != InsertMode_InsertWidget) && pPartInstance) )
			{
				ARL::Selection* selection = ARL::ServiceProvider::create<ARL::Selection>(pDataModel.get());
				if (selection) selection->setSelection(pObjectToInsert.get());
			}
			else
			{
				RobloxExplorerWidget& explorer = UpdateUIManager::Instance().getViewWidget<RobloxExplorerWidget>(eDW_OBJECT_EXPLORER);
				RobloxTreeWidget* pTreeWidget = explorer.getTreeWidget();

				if ( pTreeWidget && pTreeWidget->dataModel() )
					QApplication::postEvent(
					pTreeWidget, 
					new RobloxCustomEventWithArg(TREE_SCROLL_TO_INSTANCE, 
					boost::bind(
					&RobloxTreeWidget::scrollToInstance, 
					pTreeWidget, 
					pObjectToInsert.get() ) ) );
				
			}

		}
		// if inserted object is a script then open it
		LuaSourceBuffer buffer = LuaSourceBuffer::fromInstance(pObjectToInsert);
		if (!buffer.empty())
			RobloxDocManager::Instance().openDoc(buffer);
		else
			UpdateUIManager::Instance().updateToolBars(); //update toolbars
	}
	catch(std::bad_alloc&)
	{
		throw;
	}
	catch(std::exception& e)
	{
		ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR, e.what());
	}
}

/**
 * Inserts an object based on a class name.
 *
 * @param	className	name of instance class
 * @param	pDataModel	model to insert object into
 */
void InsertObjectWidget::InsertObject(      
    const QString&                      className,
    boost::shared_ptr<ARL::DataModel>   pDataModel,
	InsertMode insertMode,
	QPoint* mousePosition)
{
    tInstanceMap instanceMap;
    generateInstanceMap(
        &ARL::Reflection::ClassDescriptor::rootDescriptor(),
        &instanceMap,
        NULL );

    InsertObject(instanceMap[className],pDataModel, insertMode, mousePosition);
}

void InsertObjectWidget::setFilter(QString filter)
{
    while(m_pInsertObjectListWidget->count() > 0)
        m_pInsertObjectListWidget->takeItem(0);

    ARL::Instance* pParent = getTargetInstance(NULL, m_pDataModel);
    
    for (int i = 0; i < m_ItemList->count(); ++i)
    {
        InsertObjectListWidgetItem* listItem = m_ItemList->at(i);
        if (listItem->checkFilter(filter, pParent))
            m_pInsertObjectListWidget->addItem(listItem);
    }
    m_pInsertObjectListWidget->sortItems(m_objectWeights, filter);
}

void InsertObjectWidget::startingQuickInsert(QWidget* widget, bool closeDockWhenDoneWithQuickInsert)
{
	if (widget)
	{
		m_quickInsertPreviousWidget = widget;
		connect(widget, SIGNAL(destroyed()), this, SLOT(onPreviousWidgetDestroyed()));
	}

	m_closeDockWhenDoneWithQuickInsert = closeDockWhenDoneWithQuickInsert;
}

void InsertObjectWidget::connectSelectionChangeEvent(bool connectSignal)
{
	//ARL::StandardOut::singleton()->printf(ARL::MESSAGE_INFO, "InsertObjectWidget::connectSelectionChangeEvent - %d", connectSignal);
	m_cSelectionChanged.disconnect();
	
	if (connectSignal && m_pDataModel)
	{
		ARL::Selection* pSelection = m_pDataModel->create<ARL::Selection>();
		if (pSelection)
			m_cSelectionChanged = pSelection->selectionChanged.connect(boost::bind(&InsertObjectWidget::onInstanceSelectionChanged, this, _1));
	}
}

void InsertObjectWidget::onInstanceSelectionChanged(const ARL::SelectionChanged&)
{ 
	requestDialogRedraw(); 
}

void InsertObjectWidget::requestDialogRedraw()
{
	if(m_bRedrawRequested || !m_pDataModel)
		return;
	
    m_bRedrawRequested = true;
    QMetaObject::invokeMethod(
        this, 
        "redrawDialog", 
        Qt::QueuedConnection );
}

void InsertObjectWidget::redrawDialog()
{
    m_bRedrawRequested = false;
    
	if (m_pDataModel)
	{
		if (m_bInitializationRequired)
		{
			//ARL::StandardOut::singleton()->print(ARL::MESSAGE_INFO, "InsertObjectWidget::createWidgets");
            m_pInsertObjectListWidget->setUpdatesEnabled(false);
            m_ItemList->clear();
			populateList(false, m_ItemList, &ARL::Reflection::ClassDescriptor::rootDescriptor(), m_pDataModel);
            m_pInsertObjectListWidget->setUpdatesEnabled(true);
			m_bInitializationRequired = false;
		}
		setFilter(m_pFilterEdit->text());
	}
}

void InsertObjectWidget::onItemInsertRequested()
{
    QListWidgetItem* currentItem = m_pInsertObjectListWidget->currentItem();

    if (m_quickInsertPreviousWidget)
    {
        m_quickInsertPreviousWidget->activateWindow();
        m_quickInsertPreviousWidget->setFocus(Qt::MouseFocusReason);
        m_quickInsertPreviousWidget = NULL;
		UpdateUIManager::Instance().setDockVisibility(eDW_BASIC_OBJECTS, !m_closeDockWhenDoneWithQuickInsert);
		m_closeDockWhenDoneWithQuickInsert = false;
    }

    m_pInsertObjectListWidget->onItemInsertRequested(currentItem);
}

ARL::Instance* InsertObjectWidget::getSelectionFrontPart()
{
	ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);
	ARL::Selection* selection = ARL::ServiceProvider::create< ARL::Selection >(m_pDataModel.get());
	if (selection && selection->size() > 0)
	{
		return selection->front().get();
	}
	return NULL;
}

ARL::Instance* InsertObjectWidget::getTargetInstance(ARL::Instance* pInstance, boost::shared_ptr<ARL::DataModel> pDataModel)
{
	// If there is a selected part, put it under that 
	ARL::DataModel::LegacyLock lock(pDataModel, ARL::DataModelJob::Write);
	if(dynamic_cast<ARL::Service*>(pInstance))
		return pDataModel.get();

	ARL::Selection* selection = ARL::ServiceProvider::create< ARL::Selection >(pDataModel.get());
	if (selection && selection->size() > 0)
	{
        if(selection->size() > 1)
            return NULL;

		ARL::Instance* selFront = selection->front().get();
		if (!pInstance || pInstance->canSetParent(selFront))
			return selFront;
	}

    if(pInstance)
    {
        // Otherwise check to see if there is a preferred parent
        ARL::Reflection::Metadata::Class* pMetadata = ARL::Reflection::Metadata::Reflection::singleton()->get(pInstance->getDescriptor(), false);
        std::string preferredParent = pMetadata->getPreferredParent();	//itemToInsert->getPreferredParent();
        if (preferredParent != "")
        {
            shared_ptr<ARL::Instance> serviceParent = pDataModel->getPublicServiceByClassNameString(preferredParent);
            if (serviceParent)
            {
                return serviceParent.get();
            }
        }
    }
	return pDataModel->getWorkspace(); 
}

void InsertObjectWidget::onFilter( QString filter )
{
	setFilter(filter);
    m_pInsertObjectListWidget->setCurrentRow(0);
}

ARL::Vector3 InsertObjectWidget::getInsertLocation(boost::shared_ptr<ARL::DataModel> pDataModel, QPoint* insertPosition, bool* isOnPart)
{
	ARL::Vector3 location;

	if (RobloxDocManager::Instance().getPlayDoc())
	{
		QWidget* viewer = RobloxDocManager::Instance().getPlayDoc()->getViewer();
		if (viewer)
		{
			ARL::Workspace* workspace = pDataModel->getWorkspace();

			// check if camera has moved
			m_sIsInsertLocationValid = ARL::Math::fuzzyEq(workspace->getConstCamera()->getCameraCoordinateFrame(), m_sCameraCFrame);

			// if we are inserting at screen center and we have a valid insert location, then insert it at the cached insert location
			if (!insertPosition && m_sIsInsertLocationValid)
				return m_sCachedInsertLocation;

			// else calculate the insert location
			int w = viewer->width();
			int h = viewer->height();
			std::vector<const ARL::Primitive*> excludeParts;
			ARL::PartByLocalCharacter filter(workspace);

			// if no insert position then insert at the center on the window
			ARL::RbxRay mouseRay = workspace->getConstCamera()->worldRay(insertPosition ? insertPosition->x() : w/2, 
				                                                         insertPosition ? insertPosition->y() : h/2);
			// check if we have a part at the insert position?
			ARL::PartInstance* foundPart = ARL::MouseCommand::getMousePart(mouseRay,
				                                                          *workspace->getWorld()->getContactManager(),
			                                                           	   excludeParts,
				                                                          &filter,
				                                                           location);

			if (isOnPart)
				*isOnPart = (foundPart != NULL);

			// if no part found, try to get a point based on the camera
			if (!foundPart)
			{
				ARL::Vector3 pos = workspace->getConstCamera()->getCameraFocus().lookVector();
				// if we do not get a valid point, insert at origin
				if (!ARL::Math::intersectRayPlane(ARL::MouseCommand::getSearchRay(mouseRay), ARL::Plane(ARL::Vector3::unitY(), pos), location))
					return ARL::Vector3::zero();
			}
		}
	}

	// cache insert location for insertion done at center of the screen
	if (!insertPosition)
	{
		m_sIsInsertLocationValid = true;
		m_sCachedInsertLocation = location;
	}

	return location;
}
