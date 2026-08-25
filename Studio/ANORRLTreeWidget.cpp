/**
 * ANORRLTreeWidget.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRLTreeWidget.h"

// Qt Headers
#include <QApplication>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QMenu>
#include <QScrollBar>
#include <QLineEdit>
#include <QMetaObject>
#include <QMovie>
#include <QStyledItemDelegate>

// ANORRL Headers
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Selection.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/ChangeHistory.h"
#include "v8datamodel/GuiObject.h"
#include "Script/ModuleScript.h"
#include "script/script.h"
#include "util/ScopedAssign.h"
#include "util/BrickColor.h"
#include "ReflectionMetaData.h"
#include "v8datamodel/PlayerGui.h"

// ANORRL Studio Headers
#include "CommonInsertWidget.h"
#include "LuaSourceBuffer.h"
#include "QtUtilities.h"
#include "ANORRLMainWindow.h"
#include "ANORRLContextualHelp.h"
#include "ANORRLCustomWidgets.h"
#include "ANORRLDocManager.h" 
#include "UpdateUIManager.h"
#include "ANORRLIDEDoc.h"
#include "boost/algorithm/string.hpp"
#include "ANORRLSettings.h"
#include "AuthoringSettings.h"

const static std::string classSearchString = "classname:";
const static std::string nameSearchString = "name:";

static int nearestWidgetItemIndex(ANORRLTreeWidgetItem *pParentTWI, int first, int last, int key);
static QItemSelection mergeModelIndexes(const QList<QModelIndex> &indexes);

FASTINTVARIABLE(StudioTreeWidgetProcessingTime, 100)
FASTINTVARIABLE(StudioTreeWidgetFilterTime, 30)
FASTINTVARIABLE(StudioTreeWidgetEventProcessingTime, 10)

FASTFLAGVARIABLE(StudioDE8774CrashFixEnabled, false)
FASTFLAGVARIABLE(StudioPushTreeWidgetUpdatesToMainThread, false)
FASTFLAGVARIABLE(StudioTreeWidgetCheckDeletingFlagWhenDoingUpdates, false)

FASTFLAG(StudioNewWiki)
FASTFLAG(StudioMimeDataContainsInstancePath)
FASTFLAG(StudioSeparateActionByActivationMethod)
FASTFLAG(TeamCreateOptimizeRemoteSelection)

LOGGROUP(Explorer)

bool DepthCompare::operator() (const ANORRLTreeWidgetItem* lhs, const ANORRLTreeWidgetItem* rhs) const
{
	if (lhs->getTreeWidgetDepth() == rhs->getTreeWidgetDepth())
		return lhs < rhs;

	return rhs->getTreeWidgetDepth() < lhs->getTreeWidgetDepth();
}

//--------------------------------------------------------------------------------------------
// TreeWidgetDelegate
//--------------------------------------------------------------------------------------------
class TreeWidgetDelegate: public QStyledItemDelegate
{
public:
	TreeWidgetDelegate(QObject* parent)
		: QStyledItemDelegate(parent)
	{

	}

	enum Role
	{
		OtherUsersHighlightingRole = Qt::UserRole+1,
	};

private:
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		return QStyledItemDelegate::sizeHint(option, index);
	}

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		// do the normal paint
		QStyledItemDelegate::paint(painter, option, index);

		// now paint other users selection
		// TODO: once we finalize the display, get widget width and add "..."
		QRect newRect(option.rect.topRight(), QSize(15, option.rect.height()));
		PlayersColorCollection playersData = index.data(OtherUsersHighlightingRole).value<PlayersColorCollection >();
		for (PlayersColorCollection::const_iterator iter = playersData.constBegin(); iter != playersData.constEnd(); ++iter)
		{
			painter->fillRect(newRect, QtUtilities::toQColor(ARL::BrickColor(iter.value()).color3()));
			newRect.translate(15, 0);
		}
	}
};

InstanceUpdateHandler::InstanceUpdateHandler(boost::shared_ptr<ARL::Instance> pInstance)
: m_pInstance(pInstance)
{
	// Attach listeners
	if (FFlag::StudioPushTreeWidgetUpdatesToMainThread)
	{
		m_cChildAddedConnection   = m_pInstance->getOrCreateChildAddedSignal()->connect(boost::bind(&InstanceUpdateHandler::childAddedSignalHandler, this, _1));
		m_cChildRemovedConnection = m_pInstance->getOrCreateChildRemovedSignal()->connect(boost::bind(&InstanceUpdateHandler::childRemovedSignalHandler, this, _1));
		m_cPropertyChangedConnection = m_pInstance->propertyChangedSignal.connect(boost::bind(&InstanceUpdateHandler::propertyChangedSignalHandler, this, _1));
	}
	else
	{
		m_cChildAddedConnection   = m_pInstance->getOrCreateChildAddedSignal()->connect(boost::bind(&InstanceUpdateHandler::onChildAdded, this, _1));
		m_cChildRemovedConnection = m_pInstance->getOrCreateChildRemovedSignal()->connect(boost::bind(&InstanceUpdateHandler::onChildRemoved, this, _1));
		m_cPropertyChangedConnection = m_pInstance->propertyChangedSignal.connect(boost::bind(&InstanceUpdateHandler::onPropertyChanged, this, _1));
	}
}

InstanceUpdateHandler::~InstanceUpdateHandler()
{	
	m_cChildRemovedConnection.disconnect();
	m_cChildAddedConnection.disconnect();
	m_cPropertyChangedConnection.disconnect();

	m_PendingItemsToAdd.clear();
}

void InstanceUpdateHandler::populateChildren(ANORRLTreeWidget* pTreeWidget)
{
	shared_ptr<ARL::Instance> inst = getInstance();

	ANORRLTreeWidgetItem* pParent = getItemParent();

	for (size_t i = 0; i < inst->numChildren(); ++i)
	{
		if (ARL::Instance* pChildInst = inst->getChild(i))
		{
			ANORRLTreeWidgetItem* pChildItem = pTreeWidget->findItemFromInstance(pChildInst);

			if (pChildItem)
			{
				bool currentlySelected = false;

				if (ARL::Selection* selection = ARL::ServiceProvider::find<ARL::Selection>(getInstance().get()))
				{
					for (ARL::Instances::const_iterator iter = selection->begin(); iter != selection->end(); ++iter)
					{
						if ((*iter).get() == pChildInst)
						{
							currentlySelected = true;
							break;
						}
					}
				}

				if (pParent)
				{
					int index = pParent->getIndexToInsertAt(shared_from(pChildInst));
					pParent->insertChild(index < 0 ? 0 : index, pChildItem);
				}
				else if (pTreeWidget)
				{
					int index = pTreeWidget->getIndexToInsertAt(shared_from(pChildInst));
					pTreeWidget->insertTopLevelItem(index < 0 ? 0 : index, pChildItem);
				}

				if (currentlySelected)
					pTreeWidget->addItemToSelection(pChildInst);

				pChildItem->populateChildren(pTreeWidget);
			}
		}
	}
}

bool InstanceUpdateHandler::onChildAdded(shared_ptr<ARL::Instance> pChild)
{
	FASTLOG1(FLog::Explorer, "InstanceUpdateHander::onChildAdded, child: %p", pChild.get());

	if (!pChild || !ANORRLTreeWidgetItem::isExplorerItem(pChild))
		return false;

	FASTLOGS(FLog::Explorer, "Child name: %s", pChild->getName());

	ANORRLTreeWidget *pTreeWidget = getTreeWidget();
	if (!pTreeWidget || pTreeWidget->isDeletionRequested())
		return false;

	ANORRLTreeWidgetItem *pTreeWidgetParentItem = pTreeWidget->findItemFromInstance(pChild->getParent());
	
	if (pTreeWidgetParentItem)
		pTreeWidgetParentItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	QMutexLocker lock(pTreeWidget->treeWidgetMutex());

	if (ANORRLIDEDoc::getIsCloudEditSession() && ARL::Security::Context::current().identity == ARL::Security::Replicator_)
		pTreeWidget->markLocationToSuppressScrolling();

	ANORRLTreeWidgetItem* pChildItem = pTreeWidget->findItemFromInstance(pChild.get());

	if (pChildItem && (pTreeWidgetParentItem || pTreeWidget))
	{
		pTreeWidget->removeItemFromRemovalList(pChildItem);
		bool currentlySelected = pChildItem->removeItemFromTreeWidget();

		if (pTreeWidgetParentItem)
		{
			int index = pTreeWidgetParentItem->getIndexToInsertAt(pChild);
			pTreeWidgetParentItem->insertChild(index < 0 ? 0 : index, pChildItem);
		}
		else if (pTreeWidget)
		{
			int index = pTreeWidget->getIndexToInsertAt(pChild);
			pTreeWidget->insertTopLevelItem(index < 0 ? 0 : index, pChildItem);
		}

		pChildItem->setExpandedOnce(false);

		if (currentlySelected)
			pTreeWidget->addItemToSelection(pChild.get());

	}
	else
	{
		m_PendingItemsToAdd.insert(pChild);
	}
	
	if (pTreeWidgetParentItem && pTreeWidgetParentItem->isExpanded())
	{
		pTreeWidgetParentItem->updateFilterItems();
	}
	pTreeWidget->addToUpdateList(pTreeWidgetParentItem);
	return true;
}

bool InstanceUpdateHandler::onChildRemoved(shared_ptr<ARL::Instance> pChild)
{
	FASTLOG1(FLog::Explorer, "InstanceUpdateHander::onChildRemoved, child: %p", pChild.get());
	if (!pChild)
		return false;

	FASTLOGS(FLog::Explorer, "Child name: %s", pChild->getName());

	ANORRLTreeWidget *pTreeWidget = getTreeWidget();
	if (!pTreeWidget || pTreeWidget->isDeletionRequested())
		return false;

	QMutexLocker lock(pTreeWidget->treeWidgetMutex());
	m_PendingItemsToAdd.erase(pChild);
	m_FilterItemsToAdd.erase(pChild);
	ANORRLTreeWidgetItem *pTreeWidgetItem = pTreeWidget->findItemFromInstance(pChild.get());
	if (pTreeWidgetItem)
	{
		pTreeWidget->requestItemDelete(pTreeWidgetItem);

		if (pTreeWidgetItem == pTreeWidget->lastSelectedItem())
			pTreeWidget->eraseLastSelectedItem();

        if (ANORRLIDEDoc::getIsCloudEditSession())
		{
			if (ARL::Security::Context::current().identity == ARL::Security::Replicator_)
				pTreeWidget->markLocationToSuppressScrolling();
			pTreeWidget->itemDeletionRequested(pTreeWidgetItem);
		}
	}

	return true;
}

void InstanceUpdateHandler::onPropertyChanged(const ARL::Reflection::PropertyDescriptor* pDescriptor)
{
	bool isNameChanged = (*pDescriptor==ARL::Instance::desc_Name);
	if (isNameChanged)
	{
		ANORRLTreeWidget *pTreeWidget = getTreeWidget();
		if (pTreeWidget && !pTreeWidget->isDeletionRequested())
		{
			QMutexLocker lock(pTreeWidget->treeWidgetMutex());
			processPropertyChange();
		}
	}
	else if (*pDescriptor==ARL::Instance::propParent)
	{
		if (ANORRLTreeWidget* pTreeWidget = getTreeWidget())
			if (!pTreeWidget->isFilterEmpty() && getInstance()->getParent())
				pTreeWidget->filterWidget();
	}
}

ANORRLTreeWidgetItem* InstanceUpdateHandler::processChildAdd(shared_ptr<ARL::Instance> pInstance)
{
	ANORRLTreeWidget *pTreeWidget = getTreeWidget();
	ANORRLTreeWidgetItem *pWidgetItem = NULL;

	if (pTreeWidget->findItemFromInstance(pInstance.get()))
		return NULL;
	
	if (getItemParent())
	{
		int index = getItemParent()->getIndexToInsertAt(pInstance);			
		pWidgetItem = new ANORRLTreeWidgetItem((index > 0) ? index : 0, getItemParent(), pInstance);			
	}
	else
	{
		int index = pTreeWidget->getIndexToInsertAt(pInstance);
		pWidgetItem = new ANORRLTreeWidgetItem((index > 0) ? index : 0, pTreeWidget, pInstance);
	}

	if (pWidgetItem && pInstance->numChildren() > 0)
		pWidgetItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	if (pWidgetItem)
		pWidgetItem->setViewState();

	pTreeWidget->updateSelectionState(pInstance.get());
	
	m_FilterItemsToAdd.erase(pInstance);
	m_PendingItemsToAdd.erase(pInstance);

	return pWidgetItem;
}

bool InstanceUpdateHandler::processChildrenAdd()
{
	ANORRLTreeWidget *pTreeWidget = getTreeWidget();
	ARLASSERT(pTreeWidget);
	if (!pTreeWidget)
		return false;

	InstanceList_SPTR* instanceList = pTreeWidget->isFilterEmpty() ? &m_PendingItemsToAdd : &m_FilterItemsToAdd;

	if (instanceList->empty())
		return false;

	bool itemsAdded = false;

	//Lock for a few items
	QMutexLocker lock(pTreeWidget->treeWidgetMutex());
	// Do a few items
	for ( int i = 0 ; i < 10 ; ++i )
	{
		InstanceList_SPTR::iterator iter = instanceList->begin();
		if (iter == instanceList->end())
			return itemsAdded;
		shared_ptr<ARL::Instance> pInstance = *iter;

		processChildAdd(pInstance);

		itemsAdded = true;
	}
	return itemsAdded;
}

void InstanceUpdateHandler::removeFromRemovalList(ANORRLTreeWidgetItem *pTreeWidgetItem)
{
	m_PendingItemsToRemove.erase(pTreeWidgetItem);

	int numChild = pTreeWidgetItem->childCount();
	if (!numChild)
		return;

	int currentChild = 0;
	ANORRLTreeWidgetItem *pCurrentItem = NULL;

	while (currentChild < numChild)
	{		
		pCurrentItem = static_cast<ANORRLTreeWidgetItem*>(pTreeWidgetItem->child(currentChild));
		if (pCurrentItem)
			removeFromRemovalList(pCurrentItem);
		++currentChild;
	}
}

void InstanceUpdateHandler::processChildRemove()
{
	if (!m_PendingItemsToRemove.size())
		return;

	ANORRLTreeWidget *pTreeWidget = getTreeWidget();

	if (FFlag::StudioDE8774CrashFixEnabled)
	{
		QMutexLocker lock(pTreeWidget->treeWidgetMutex());

		while (m_PendingItemsToRemove.begin() != m_PendingItemsToRemove.end())
		{
			ANORRLTreeWidgetItem *pTreeWidgetItem = *m_PendingItemsToRemove.begin();
			if (!pTreeWidgetItem)
				continue;

			pTreeWidgetItem->aboutToDelete(pTreeWidget);
			removeFromRemovalList(pTreeWidgetItem);

			delete pTreeWidgetItem;
		}
	}
	else
	{
		std::vector<ANORRLTreeWidgetItem*> itemsToRemove;

		// Copy the items to a temp collection
		{
			QMutexLocker lock(pTreeWidget->treeWidgetMutex());
			for (TreeWidgetItemList::iterator iter = m_PendingItemsToRemove.begin(); iter != m_PendingItemsToRemove.end(); ++iter)
				itemsToRemove.push_back(*iter);
			m_PendingItemsToRemove.clear();
		}

		// Process them
		ANORRLTreeWidgetItem *pTreeWidgetItem = NULL;
		for (std::vector<ANORRLTreeWidgetItem*>::iterator iter = itemsToRemove.begin(); iter!=itemsToRemove.end(); ++iter)
		{
			pTreeWidgetItem = *iter;
			if (!pTreeWidgetItem)
				continue;
			pTreeWidgetItem->aboutToDelete(pTreeWidget);
			//if (pTreeWidgetItem->parent())
			//	pTreeWidgetItem->parent()->removeChild(pTreeWidgetItem);
			delete pTreeWidgetItem;
		}
	}
}

ANORRLTreeWidgetItem::ANORRLTreeWidgetItem(int index, ANORRLTreeWidget *pTreeWidget, boost::shared_ptr<ARL::Instance> pInstance)
: InstanceUpdateHandler(pInstance)
, m_ItemInfo(0)
, m_queuedForDeletion(false)
{
	initData();

	m_treeWidgetDepth = 1;

	//add it to map in tree view for find
	pTreeWidget->addInstance(m_pInstance.get(), this);

	//add item to tree view
	pTreeWidget->insertTopLevelItem(index, this);
}

ANORRLTreeWidgetItem::ANORRLTreeWidgetItem(int index, ANORRLTreeWidgetItem *pParentWidgetItem, boost::shared_ptr<ARL::Instance> pInstance)
: InstanceUpdateHandler(pInstance)
, m_ItemInfo(0)
, m_queuedForDeletion(false)
{
	initData();

	m_treeWidgetDepth = pParentWidgetItem->getTreeWidgetDepth() + 1;

	//add it to map in tree view for find (cannot use current item's getTreeWidget)
	pParentWidgetItem->getTreeWidget()->addInstance(m_pInstance.get(), this);

	//add item as child
	pParentWidgetItem->insertChild(index, this);
}

ANORRLTreeWidgetItem::~ANORRLTreeWidgetItem()
{
	ANORRLTreeWidget* pTreeWidget = getTreeWidget();

	if (!pTreeWidget)
	{
		//May not be in treeWidget
		ANORRLExplorerWidget* anorrlExplorer = static_cast<ANORRLExplorerWidget*>(UpdateUIManager::Instance().getExplorerWidget());
		pTreeWidget = anorrlExplorer->getTreeWidget();
	}

	if (pTreeWidget)
	{
		pTreeWidget->removeItemFromRemovalList(this);
		pTreeWidget->eraseInstance(getInstance().get());
	}
}

void ANORRLTreeWidgetItem::setData(int column, int role, const QVariant & value)
{
	//only editing of column 0 i.e. name change is to be handled
	if (column != 0 || role != Qt::EditRole)
	{
		QTreeWidgetItem::setData(column, role, value);
		return;
	}
	
	try
	{
		QString label = value.toString();
		if (!label.isEmpty() && label != m_pInstance->getName().c_str())
		{
			ARL::DataModel::LegacyLock lock(getTreeWidget()->dataModel(), ARL::DataModelJob::Write);
			m_pInstance->setName(label.toStdString());

            // check to see if rename failed
            if ( m_pInstance->getName() == label.toStdString() )
            {
			    //now update the tree widget data
			    QTreeWidgetItem::setData(column, role, value);
    		
			    //set waypoint
			    ARL::ChangeHistoryService::requestWaypoint("Rename", getTreeWidget()->dataModel().get());
			    getTreeWidget()->dataModel()->setDirty(true);
            }
		}
	}

	catch(std::exception& exp)
	{
		ARL::StandardOut::singleton()->printf(ARL::MESSAGE_ERROR, "Label edit failed : %s", exp.what());
	}
}

void ANORRLTreeWidgetItem::initData()
{
	setText(0, m_pInstance->getName().c_str());
	int imageIndex = getImageIndex(m_pInstance);
	setIcon(0, QIcon(QtUtilities::getPixmap(":/images/ClassImages.PNG", imageIndex)));

	m_ItemType = getItemType(m_pInstance);

	setDirty(true);
	setExpandedOnce(false);

	setData(1, ExpandRole, QVariant(false));

	setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);

	if (hasChildren())
		setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
}

bool ANORRLTreeWidgetItem::hasChildren()
{
	bool result = false;
	if (m_pInstance->getChildren())
	{
		ARL::Instances childInstances = *m_pInstance->getChildren();
		if (!childInstances.empty())
		{
			for (ARL::Instances::const_iterator iter = childInstances.begin(); iter != childInstances.end(); ++iter)
			{
				if (isExplorerItem(*iter))
				{
					result = true;
					break;
				}
			}
		}
	}

	return result;
}

void ANORRLTreeWidgetItem::setViewState()
{
	ANORRLTreeWidget* pTreeWidget = getTreeWidget();

	if (pTreeWidget->isFilterEmpty())
	{
		setExpanded(data(1, ExpandRole).toBool());
		setTextColor(0, Qt::black);
		setHidden(false);
		setDisabled(false);
	}
	else if (pTreeWidget->isFoundItem(getInstance()))
	{
		setExpanded(true);
		setTextColor(0, Qt::black);
		setHidden(false);
		setDisabled(false);
	}
	else if (pTreeWidget->isAncestorOfFoundItem(getInstance()))
	{
		setExpanded(true);
		setTextColor(0, Qt::gray);
		setHidden(false);
		setDisabled(false);
	}
	else
	{
		setHidden(true);
		setDisabled(true);
	}
}

ANORRLTreeWidget *ANORRLTreeWidgetItem::getTreeWidget()
{	return dynamic_cast<ANORRLTreeWidget *>(treeWidget()); }

ANORRLTreeWidgetItem* ANORRLTreeWidgetItem::getItemParent()
{	return this; }

void ANORRLTreeWidgetItem::updateFilterItems()
{
	m_FilterItemsToAdd.clear();

	ANORRLTreeWidget* pTreeWidget = getTreeWidget();
	ARLASSERT(pTreeWidget);
	if (!pTreeWidget)
		return;

	if (!pTreeWidget->isFilterEmpty())
	{
		for (std::set<shared_ptr<ARL::Instance> >::const_iterator iter = m_PendingItemsToAdd.begin(); iter != m_PendingItemsToAdd.end(); ++iter)
			if (pTreeWidget->isAncestorOfFoundItem(*iter) || pTreeWidget->isFoundItem(*iter))
				m_FilterItemsToAdd.insert(*iter);
	}
}

void ANORRLTreeWidgetItem::requestItemExpand()
{
	ANORRLTreeWidget *pTreeWidget = getTreeWidget();

	ARLASSERT(pTreeWidget);
	if (!pTreeWidget)
		return;

	if (pTreeWidget->isFilterEmpty())
		setData(1, ExpandRole, QVariant(true));

	if (m_pInstance->getChildren())
	{
		ARL::Instances childInstances = *m_pInstance->getChildren();
		if (childInstances.empty())
			return;

		//add child in pending queue
		if (!isExpandedOnce())
		{
			QMutexLocker lock(pTreeWidget->treeWidgetMutex());
			for (ARL::Instances::const_iterator iter = childInstances.begin(); iter != childInstances.end(); ++iter)
				if(!pTreeWidget->findItemFromInstance(iter->get()) &&ANORRLTreeWidgetItem::isExplorerItem(*iter))
					m_PendingItemsToAdd.insert(*iter);
		}
	}

	setExpandedOnce(true);
	
	updateFilterItems();

	pTreeWidget->addToUpdateList(this);
}

void ANORRLTreeWidgetItem::takeAllChildren()
{
	while (childCount() > 0)
	{
		QTreeWidgetItem* firstItem = child(0);
		ANORRLTreeWidgetItem* treeItem = dynamic_cast<ANORRLTreeWidgetItem*>(firstItem);

		if (treeItem)
			treeItem->takeAllChildren();

		this->takeChild(0);
	}
}

void ANORRLTreeWidgetItem::addPlayerSelection(int playerId, int color)
{
	PlayersColorCollection playersData = data(0, TreeWidgetDelegate::OtherUsersHighlightingRole).value<PlayersColorCollection >();
	if (playersData.find(playerId) == playersData.end())
	{
		playersData[playerId] = color;
		setData(0, TreeWidgetDelegate::OtherUsersHighlightingRole, QVariant::fromValue<PlayersColorCollection >(playersData));
	}
}


void ANORRLTreeWidgetItem::removePlayerSelection(int playerId)
{
	PlayersColorCollection playersData = data(0, TreeWidgetDelegate::OtherUsersHighlightingRole).value<PlayersColorCollection >();
	if (playersData.remove(playerId) > 0)
		setData(0, TreeWidgetDelegate::OtherUsersHighlightingRole, QVariant::fromValue<PlayersColorCollection >(playersData));
}

bool ANORRLTreeWidgetItem::removeItemFromTreeWidget()
{
	ANORRLTreeWidget *pTreeWidget = getTreeWidget();
	if (!pTreeWidget)
		return false;

	bool currentlySelected = false;

	if (ARL::Selection* selection = ARL::ServiceProvider::find<ARL::Selection>(getInstance().get()))
	{
		for (ARL::Instances::const_iterator iter = selection->begin(); iter != selection->end(); ++iter)
		{
			if (*iter == getInstance())
			{
				currentlySelected = true;
				break;
			}
		}
	}

	if (currentlySelected)
		pTreeWidget->blockSignals(true);


	if (ANORRLTreeWidgetItem* previousParent = dynamic_cast<ANORRLTreeWidgetItem*>(static_cast<QTreeWidgetItem*>(this)->parent()))
	{
		int childIndex = previousParent->indexOfChild(this);
		takeAllChildren();
		previousParent->takeChild(childIndex);
	}
	else
	{
		int childIndex = pTreeWidget->indexOfTopLevelItem(this);
		takeAllChildren();
		pTreeWidget->takeTopLevelItem(childIndex);
	}

	if (currentlySelected)
		pTreeWidget->blockSignals(false);

	return currentlySelected;
}

void ANORRLTreeWidgetItem::aboutToDelete(ANORRLTreeWidget* pTreeWidget)
{
	m_queuedForDeletion = true;

	m_cChildAddedConnection.disconnect();
	m_cChildRemovedConnection.disconnect();
	m_cPropertyChangedConnection.disconnect();
	
	pTreeWidget->eraseInstance(getInstance().get());
	pTreeWidget->removeItemFromRemovalList(this);

	pTreeWidget->removeFromUpdateList(this);
	
	int numChild = childCount();
	if (!numChild)
	{
		removeItemFromTreeWidget();
		return;
	}
	
	int currentChild = 0;
	ANORRLTreeWidgetItem *pCurrentItem = NULL;
	
	while (currentChild < numChild)
	{		
		pCurrentItem = static_cast<ANORRLTreeWidgetItem*>(child(currentChild));
		if (pCurrentItem)
		{
			pCurrentItem->aboutToDelete(pTreeWidget);
			pCurrentItem->deleteLater();			
		}
		++currentChild;
	}

	if (!FFlag::StudioDE8774CrashFixEnabled)
		pTreeWidget->removeFromUpdateList(this);

	removeItemFromTreeWidget();
}

bool ANORRLTreeWidgetItem::onChildAdded(shared_ptr<ARL::Instance> pChild)
{
	if (FFlag::StudioTreeWidgetCheckDeletingFlagWhenDoingUpdates && m_queuedForDeletion) return false;

	FASTLOG1(FLog::Explorer, "ANORRLTreeWidgetItem::onChildAdded, child: %p", pChild.get());

	ANORRLTreeWidget *pTreeWidget = getTreeWidget();
	if (!pTreeWidget)
		return false;

	FASTLOGS(FLog::Explorer, "Child name: %s", pChild->getName());

	//if not expanded once then just set dirty flag
	if (!isExpandedOnce())
	{
		if (ANORRLTreeWidgetItem::isExplorerItem(pChild))
		{
			setDirty(true);	
			setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
			pTreeWidget->requestViewportUpdate();
		}
		return true;
	}

	if (!InstanceUpdateHandler::onChildAdded(pChild))
		return false;

	if (isExpandedOnce())
		pTreeWidget->addToUpdateList(this);

	return true;
}

bool ANORRLTreeWidgetItem::onChildRemoved(shared_ptr<ARL::Instance> pChild)
{		
	if (FFlag::StudioTreeWidgetCheckDeletingFlagWhenDoingUpdates && m_queuedForDeletion) return false;

	FASTLOG1(FLog::Explorer, "ANORRLTreeWidgetItem::onChildRemoved, child: %p", pChild.get());
	//if child item has been added before then let it get removed!
	if (!InstanceUpdateHandler::onChildRemoved(pChild))
		return false;

	FASTLOGS(FLog::Explorer, "Child name: %s", pChild->getName());

	ANORRLTreeWidget* treeWidget = getTreeWidget();

	if (!treeWidget)
		return false;

	if (!hasChildren())
	{
		setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
		getTreeWidget()->requestViewportUpdate();
	}

	//request for an update
	if (isExpandedOnce())
		getTreeWidget()->addToUpdateList(this);

	return true;
}

bool ANORRLTreeWidgetItem::hasChildrenToProcess()
{
	if (ANORRLTreeWidget* pWidget = getTreeWidget())
		if (!pWidget->isFilterEmpty())
			return !m_FilterItemsToAdd.empty();

	return !m_PendingItemsToAdd.empty();
}

void ANORRLTreeWidgetItem::processPropertyChange()
{
	setText(0, m_pInstance->getName().c_str());
}

bool ANORRLTreeWidgetItem::handleOpen()
{
	if(!m_pInstance)
		return false;

	// Open script document
	return ANORRLDocManager::Instance().openDoc(LuaSourceBuffer::fromInstance(m_pInstance));
}

int ANORRLTreeWidgetItem::getIndexToInsertAt(shared_ptr<ARL::Instance> pInstance)
{
	int numChild = childCount();
	if (!numChild)
		return -1;

	int itemTypeToAdd = ANORRLTreeWidgetItem::getItemType(pInstance);
	int nearestIndex = nearestWidgetItemIndex(this, 0, childCount()-1, itemTypeToAdd);

	ANORRLTreeWidgetItem *pCurrentItem = static_cast<ANORRLTreeWidgetItem*>(child(nearestIndex));
	if (!pCurrentItem || (itemTypeToAdd != pCurrentItem->itemType()))
		return nearestIndex;

	//if the type is same then we need to insert in the order of item's name
	if ((pInstance->getName() < pCurrentItem->getInstance()->getName()))
	{		
		while (nearestIndex >= 0)
		{
			pCurrentItem = static_cast<ANORRLTreeWidgetItem*>(child(nearestIndex));
			if (!pCurrentItem)
				break;

			if((itemTypeToAdd != pCurrentItem->itemType()) || (pInstance->getName() > pCurrentItem->getInstance()->getName()))
			{
				nearestIndex++;
				break;
			}

			--nearestIndex;
		}
	}
	else
	{
		while (nearestIndex < childCount())
		{
			pCurrentItem = static_cast<ANORRLTreeWidgetItem*>(child(nearestIndex));
			if (!pCurrentItem || (itemTypeToAdd != pCurrentItem->itemType()) || (pInstance->getName() < pCurrentItem->getInstance()->getName()))
				break;
			++nearestIndex;
		}
	}

	return nearestIndex;
}

bool ANORRLTreeWidgetItem::isExplorerItem(const shared_ptr<ARL::Instance>& pInstance)
{
    // Assume everything that isn't a direct child of data model should be shown in explorer
    if (pInstance->getParent() != ARL::DataModel::get(pInstance.get()))
	{
		ARL::CoreGuiService* cgs = ARL::ServiceProvider::find<ARL::CoreGuiService>(pInstance.get());
		if (cgs && pInstance->isDescendantOf(cgs))
			return false;
		
		return true;
    }

    // For everything else, check the class metadata
	boost::shared_ptr<const ARL::Reflection::Metadata::Reflection> pReflection = ARL::Reflection::Metadata::Reflection::singleton();
	ARL::Reflection::Metadata::Class* pClassData = pReflection ? pReflection->get(pInstance->getDescriptor(), true) : NULL;
	if(pClassData && pClassData->isExplorerItem())
		return true;

	return false;
}

int ANORRLTreeWidgetItem::getImageIndex(const shared_ptr<ARL::Instance>& pInstance)
{
	boost::shared_ptr<const ARL::Reflection::Metadata::Reflection> pReflection = ARL::Reflection::Metadata::Reflection::singleton();
	ARL::Reflection::Metadata::Class* pClassData = pReflection ? pReflection->get(pInstance->getDescriptor(), true) : NULL;
	if(pClassData)
		return pClassData->getExplorerImageIndex();

	return 0;
}

int ANORRLTreeWidgetItem::getItemType(const shared_ptr<ARL::Instance>& pInstance)
{
	boost::shared_ptr<const ARL::Reflection::Metadata::Reflection> pReflection = ARL::Reflection::Metadata::Reflection::singleton();
	ARL::Reflection::Metadata::Class* pClassData = pReflection->get(pInstance->getDescriptor(), true);
	return pClassData ? pClassData->getExplorerOrder() : 0;
}

ANORRLTreeWidget::ANORRLTreeWidget(boost::shared_ptr<ARL::DataModel> pDataModel)
: InstanceUpdateHandler(pDataModel)
, m_pDataModel(pDataModel)
, m_treeWidgetMutex(QMutex::Recursive)
, m_pInstanceSelectionHandler(NULL)
, m_pRubberBand(NULL)
, m_bIgnoreInstanceSelectionChanged(false)
, m_bIgnoreItemSelectionChanged(false)
, m_bUpdateRequested(false)
, m_bViewportUpdateRequested(false)
, m_bDeletionRequested(false)
, m_currentFilter("")
, m_filterRunning(false)
, m_lastSelectedItem(NULL)
, m_isActive(false)
, m_savedMarkerItem(NULL)
{
	setHeaderHidden(true);
	setUniformRowHeights(true);

	setDragEnabled(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setDropIndicatorShown(true);
    setAutoScroll(true);
    setAutoScrollMargin(32);

	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setSelectionBehavior(QAbstractItemView::SelectItems);

    header()->setStretchLastSection(false);
    header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    
    // Set edit trigger only for SelectionClicked and EditKeyPressed 
	setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);

	initTreeView(pDataModel);
    
    connect(this,                                      SIGNAL(helpTopicChanged(const QString&)),
            &ANORRLContextualHelpService::singleton(), SLOT(onHelpTopicChanged(const QString&)));
		
	m_cDescendantAddedConnection = pDataModel->getWorkspace()->getOrCreateDescendantAddedSignal()->connect(boost::bind(&ANORRLTreeWidget::onDescendantAdded, this, _1));
}	

ANORRLTreeWidget::~ANORRLTreeWidget()
{	
	deActivate();

	m_InstanceMap.clear();
	m_itemsToUpdate.clear();
	m_selectedItems.clear();
	m_unSelectedItems.clear();
}

void ANORRLTreeWidget::addInstance(const ARL::Instance *pInstance, ANORRLTreeWidgetItem *pTreeWidgetItem) 
{
	m_InstanceMap[pInstance] = pTreeWidgetItem;
}

void ANORRLTreeWidget::initTreeView(boost::shared_ptr<ARL::DataModel> pDataModel)
{
	if (!pDataModel->getChildren())
		return;

	ARL::Instances instances;
	instances = *pDataModel->getChildren();

	std::for_each(instances.begin(), instances.end(), boost::bind(&ANORRLTreeWidget::createTreeRoot, this, _1));
}

void ANORRLTreeWidget::focusInEvent(QFocusEvent *event)
{
	eraseLastSelectedItem();
    QTreeWidget::focusInEvent(event);
    Q_EMIT focusGained();

    UpdateUIManager::Instance().updateToolBars();
}

void ANORRLTreeWidget::createTreeRoot(const boost::shared_ptr<ARL::Instance> pInstance)
{
	if (ANORRLTreeWidgetItem::isExplorerItem(pInstance))
	{
		int index = getIndexToInsertAt(pInstance);
		ANORRLTreeWidgetItem* item = new ANORRLTreeWidgetItem((index > 0) ? index : 0, this, pInstance);
	}
}

ANORRLTreeWidget* ANORRLTreeWidget::getTreeWidget()
{	return this; }

ARL::Selection* ANORRLTreeWidget::getSelection()
{	return ARL::ServiceProvider::create<ARL::Selection>(m_pDataModel.get()); }

boost::shared_ptr<ARL::DataModel> ANORRLTreeWidget::dataModel()
{	return m_pDataModel; }

void ANORRLTreeWidget::eraseInstance(ARL::Instance *pInstance)
{ 
    QMutexLocker lock(treeWidgetMutex());

	m_selectedItems.erase(pInstance);
	m_unSelectedItems.erase(pInstance);

	InstanceMap::iterator iter = m_InstanceMap.find(pInstance); 
	if (iter != m_InstanceMap.end())
	{
		if (m_lastSelectedItem == iter->second)
			eraseLastSelectedItem();

		removeFromUpdateList(iter->second);
		m_InstanceMap.erase(iter);
	}
}

ANORRLTreeWidgetItem* ANORRLTreeWidget::findItemFromInstance(const ARL::Instance *pInstance)
{
	InstanceMap::iterator iter = m_InstanceMap.find(pInstance);
	if (iter != m_InstanceMap.end())	
		return iter->second;
	return NULL;
}

void ANORRLTreeWidget::scrollToInstance(ARL::Instance *pInstance)
{
	if(!pInstance)
		return;

	if(isValidSelection(pInstance))
	{
		ANORRLTreeWidgetItem* item = findItemFromInstance(pInstance);
		if(item)
			scrollToItem(item);
		else if (ANORRLTreeWidgetItem* parentItem = findItemFromInstance(pInstance->getParent()))
			parentItem->setExpanded(true);

	}
}

int ANORRLTreeWidget::getIndexToInsertAt(shared_ptr<ARL::Instance> pInstance)
{
	int numChild = 	topLevelItemCount();
	if (!numChild)
		return -1;

	int itemTypeToAdd = ANORRLTreeWidgetItem::getItemType(pInstance), currentChild = 0;
	ANORRLTreeWidgetItem *pCurrentItem = NULL;

	while (currentChild < numChild)
	{		
		pCurrentItem = static_cast<ANORRLTreeWidgetItem*>(topLevelItem(currentChild));
		if (itemTypeToAdd < pCurrentItem->itemType())
			break;

		if (itemTypeToAdd == pCurrentItem->itemType())
		{
			if (pInstance->getName() < pCurrentItem->getInstance()->getName())
				break;			
		}

		++currentChild;
	}

	return currentChild;
}

void ANORRLTreeWidget::activate()
{
	if (m_isActive)
		return;
	m_isActive = true;

	connect((QTreeWidget *)this, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelectionChanged()));
	connect((QTreeWidget *)this, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(onItemExpanded(QTreeWidgetItem*)));
	connect((QTreeWidget *)this, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(onItemCollapsed(QTreeWidgetItem*)));
	connect((QTreeWidget *)this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));

	if (m_pDataModel)
	{
		ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);
		ARL::Selection* pSelection = m_pDataModel->create<ARL::Selection>();
		if (pSelection)
			m_cInstanceSelectionChanged = pSelection->selectionChanged.connect(boost::bind(&ANORRLTreeWidget::onInstanceSelectionChanged, this, _1));
	}
}

void ANORRLTreeWidget::deActivate()
{
	if (!m_isActive)
		m_isActive = false;

	disconnect((QTreeWidget *)this, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelectionChanged()));
	m_cInstanceSelectionChanged.disconnect();

	disconnect((QTreeWidget *)this, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(onItemExpanded(QTreeWidgetItem*)));
	disconnect((QTreeWidget *)this, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(onItemCollapsed(QTreeWidgetItem*)));
	disconnect((QTreeWidget *)this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));
}

bool ANORRLTreeWidget::onChildAdded(shared_ptr<ARL::Instance> pChild)
{
	if (FFlag::StudioTreeWidgetCheckDeletingFlagWhenDoingUpdates && m_bDeletionRequested) return false;

	if (!InstanceUpdateHandler::onChildAdded(pChild))
		return false;
	requestUpdate();
	return true;
}

bool ANORRLTreeWidget::onChildRemoved(shared_ptr<ARL::Instance> pChild)
{
	if (FFlag::StudioTreeWidgetCheckDeletingFlagWhenDoingUpdates && m_bDeletionRequested) return false;

	if (!InstanceUpdateHandler::onChildRemoved(pChild))
		return false;
	requestUpdate();
	return true;
}

void ANORRLTreeWidget::onDescendantAdded(shared_ptr<ARL::Instance> pDescendant)
{
	if (!isFilterEmpty())
	{
		if (filterInstance(pDescendant) && filterAncestors(pDescendant))
			requestUpdate();
	}
}

void ANORRLTreeWidget::contextMenuEvent(QContextMenuEvent * evt)
{
    QMenu menu;

	QList<QAction*> commonActions;
	UpdateUIManager::Instance().commonContextMenuActions(commonActions);

	QAction *pAction = NULL;
	for (int ii=0; ii<commonActions.size(); ++ii)
	{
		pAction = commonActions.at(ii);
		if (pAction)
        {
			// hack to add sub menu for inserting basic objects
			if ( pAction == UpdateUIManager::Instance().getMainWindow().insertIntoFileAction )
			{
				ARL::Instance* parent = NULL;
				if ( currentItem() )
				{
					ANORRLTreeWidgetItem* item = static_cast<ANORRLTreeWidgetItem*>(currentItem());
					parent = item->getInstance().get();
				}

				if (!parent)
					parent = m_pDataModel->getWorkspace();

				menu.addAction(
					QIcon(QtUtilities::getPixmap(":/images/ClassImages.PNG",1)),    // part icon 
					"Insert Part",
					this,
					SLOT(onInsertPart()) );
				menu.addMenu(InsertObjectWidget::createMenu(parent,this,SLOT(onInsertObject())));
			}           
			menu.addAction(pAction);
        }
		else
			menu.addSeparator();
	}

	menu.addSeparator();
	menu.addAction(UpdateUIManager::Instance().getMainWindow().launchHelpForSelectionAction);
	
	//TODO: Add other tree view actions
	connect(&menu, SIGNAL(aboutToShow()), &UpdateUIManager::Instance(), SLOT(onMenuShow()));
	connect(&menu, SIGNAL(aboutToHide()), &UpdateUIManager::Instance(), SLOT(onMenuHide()));
	
    menu.exec(evt->globalPos());
}

/**
 * Callback for user clicking on insert part in context menu menu.
 */
void ANORRLTreeWidget::onInsertPart()
{
    InsertObjectWidget::InsertObject("Part",m_pDataModel,InsertObjectWidget::InsertMode_TreeWidget);
}

/**
 * Callback for user clicking on insert basic object in context menu sub menu.
 */
void ANORRLTreeWidget::onInsertObject()
{
    QAction* action = static_cast<QAction*>(sender());
    QString className = action->text();
    InsertObjectWidget::InsertObject(className,m_pDataModel,InsertObjectWidget::InsertMode_TreeWidget);
}

void ANORRLTreeWidget::keyPressEvent(QKeyEvent *evt)
{	
	if ( (state() != EditingState) && ((evt->key() == Qt::Key_Enter) || (evt->key() == Qt::Key_Return)) )
	{
		ANORRLTreeWidgetItem* pTreeWidgetItem = dynamic_cast<ANORRLTreeWidgetItem*>(currentItem());
		//handle appropriate document open
		if (pTreeWidgetItem && pTreeWidgetItem->handleOpen())
		{
			//accept event
			evt->accept();
			return;
		}
	}

    if (QKeySequence(evt->key() | evt->modifiers()) == UpdateUIManager::Instance().getMainWindow().zoomExtentsAction->shortcut())
    {
        UpdateUIManager::Instance().getMainWindow().zoomExtentsAction->activate(QAction::Trigger);
        evt->accept();
        return;
    }

	QTreeWidget::keyPressEvent(evt);
}

void ANORRLTreeWidget::processChildrenRemoval()
{
	TreeWidgetItemList::const_iterator iter = m_itemsToRemove.begin();
	while (iter != m_itemsToRemove.end())
	{
		if (ANORRLTreeWidgetItem* pTreeWidgetItem = *iter)
		{
			pTreeWidgetItem->aboutToDelete(this);
			pTreeWidgetItem->deleteLater();
		}
		else
		{
			m_itemsToRemove.erase(iter);
		}

		iter = m_itemsToRemove.begin();
	}
}

void ANORRLTreeWidget::requestItemDelete(ANORRLTreeWidgetItem* item)
{
	for (int i = 0; i < item->childCount(); ++i)
		if (ANORRLTreeWidgetItem* childItem = dynamic_cast<ANORRLTreeWidgetItem*>((item)->child(i)))
			requestItemDelete(childItem);

	m_itemsToRemove.insert(item);
	removeFromUpdateList(item);
}

void ANORRLTreeWidget::onInstanceSelectionChanged(const ARL::SelectionChanged& evt)
{
	if (evt.addedItem != NULL)
		if(ARL::Instance *pInstance = evt.addedItem.get())
			if(ARL::GuiObject* guiObject = dynamic_cast<ARL::GuiObject*>(pInstance))
				guiObject->setSelectionBox(true);

	if (evt.removedItem != NULL)
		if(ARL::Instance *pInstance = evt.removedItem.get())
			if(ARL::GuiObject* guiObject = dynamic_cast<ARL::GuiObject*>(pInstance))
				guiObject->setSelectionBox(false);


	if (m_bIgnoreInstanceSelectionChanged)
		return;
	
	ARL::ScopedAssign<bool> ignoreSelectionEvent(m_bIgnoreItemSelectionChanged, true);

	QMutexLocker lock(treeWidgetMutex());
	bool isModificationRequired = false;

	if (evt.addedItem != NULL)
	{
		ARL::Instance* pInstance = evt.addedItem.get();
		if (isValidSelection(pInstance))
		{
			m_unSelectedItems.erase(pInstance);
			m_selectedItems.insert(pInstance);

			isModificationRequired = true;
		}
	}

	if (evt.removedItem != NULL)
	{
		ARL::Instance* pInstance = evt.removedItem.get();
		
		m_selectedItems.erase(pInstance);
		m_unSelectedItems.insert(pInstance);
		isModificationRequired = true;
	}

	if (isModificationRequired)
		requestUpdate();
}

void ANORRLTreeWidget::onCloudEditSelectionChanged(int playerId)
{
	ARLASSERT(QThread::currentThread() == qApp->thread());
	if (!m_pPlayersDataManager)
	{
		ARLASSERT(false);
		return;
	}

	bool enabled = updatesEnabled();
	setUpdatesEnabled(false);

	if (FFlag::TeamCreateOptimizeRemoteSelection)
	{
		shared_ptr<const ARL::Instances> oldSelection;
		PlayerSelectionMap::iterator iter = m_prevCloudEditSelection.find(playerId);
		if (iter != m_prevCloudEditSelection.end())
			oldSelection = iter->second;
		else
			oldSelection.reset(new ARL::Instances());

		ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);
		ARL::Instances::const_iterator findIter;
		shared_ptr<const ARL::Instances> currentSelection = m_pPlayersDataManager->getLastKnownSelection(playerId);

		bool blockSignals = model()->blockSignals(true);

		for (auto& selectedItem : *currentSelection)
		{
			findIter = std::find(oldSelection->begin(), oldSelection->end(), selectedItem);
			if (findIter == oldSelection->end())
			{
				if (ANORRLTreeWidgetItem* item = findItemFromInstance(selectedItem.get()))
					item->addPlayerSelection(playerId, m_pPlayersDataManager->getPlayerColor(playerId));
			}
		}

		for (auto& oldSelectedItem : *oldSelection)
		{
			findIter = std::find(currentSelection->begin(), currentSelection->end(), oldSelectedItem);
			if (findIter == currentSelection->end())
			{
				if (ANORRLTreeWidgetItem* item = findItemFromInstance(oldSelectedItem.get()))
					item->removePlayerSelection(playerId);
			}
		}

		model()->blockSignals(blockSignals);

		// TODO: check if we need to break it into smaller chunks to clear main thread asap?

		// save current selection so we can update unselected instances in next change
		m_prevCloudEditSelection[playerId] = currentSelection;
		setUpdatesEnabled(enabled);
	}
	else
	{
		ARL::Instances itemsToUnselect;
		PlayerSelectionMap::iterator iter = m_prevCloudEditSelection.find(playerId);
		if (iter != m_prevCloudEditSelection.end())
			itemsToUnselect = *(iter->second);

		ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);

		// TODO: check if we need to break it into smaller chunks to clear main thread asap?
		ARL::Instances::iterator findIter;
		shared_ptr<const ARL::Instances> currentSelection = m_pPlayersDataManager->getLastKnownSelection(playerId);
		for (auto& selectedItem : *currentSelection)
		{
			findIter = std::find(itemsToUnselect.begin(), itemsToUnselect.end(), selectedItem);
			if (findIter == itemsToUnselect.end())
			{
				if (ANORRLTreeWidgetItem* item = findItemFromInstance(selectedItem.get()))
					item->addPlayerSelection(playerId, m_pPlayersDataManager->getPlayerColor(playerId));
			}
			else
			{
				itemsToUnselect.erase(findIter);
			}
		}

		for (auto& unselectedItem : itemsToUnselect)
		{
			if (ANORRLTreeWidgetItem* item = findItemFromInstance(unselectedItem.get()))
				item->removePlayerSelection(playerId);
		}

		// save current selection so we can update unselected instances in next change
		m_prevCloudEditSelection[playerId] = currentSelection;
		setUpdatesEnabled(enabled);
	}
}

bool ANORRLTreeWidget::isSearchTimeUp(const ARL::Time& startTime, int duration)
{
	return (ARL::Time::now<ARL::Time::Fast>() - startTime).msec() > duration;
}

void ANORRLTreeWidget::addFilterItemsToUpdateList()
{
	for (std::set<shared_ptr<ARL::Instance> >::const_iterator iter = m_ancestorsOfFoundItems.begin(); iter != m_ancestorsOfFoundItems.end(); ++iter)
		if (ANORRLTreeWidgetItem* item = findItemFromInstance(iter->get()))
			addToUpdateList(item);

	for (std::set<shared_ptr<ARL::Instance> >::const_iterator iter = m_foundItems.begin(); iter != m_foundItems.end(); ++iter)
		if (ANORRLTreeWidgetItem* item = findItemFromInstance(iter->get()))
			addToUpdateList(item);
}

void ANORRLTreeWidget::onFilterWidgetUpdate()
{
	if (isHidden())
		return;

	Q_EMIT startedProcessing();

	FilterDeque_SPTR previousFilterStack = FilterDeque_SPTR(m_currentFilterStack.begin(), m_currentFilterStack.end());
	m_currentFilterStack.clear();

	{
		ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Read);
		filterWidgetRecursive(dataModel(), previousFilterStack, ARL::Time::now<ARL::Time::Fast>());
	}
	
	if (!m_currentFilterStack.empty())
	{
		QTimer::singleShot(FInt::StudioTreeWidgetEventProcessingTime, this, SLOT(onFilterWidgetUpdate()));
	}
	else
	{
		m_filterRunning = false;
		addFilterItemsToUpdateList();
		requestUpdate();
		Q_EMIT filterSearchFinished();
	}
}

bool ANORRLTreeWidget::filterAncestors(shared_ptr<ARL::Instance> instance)
{
	if (!instance->getParent())
		return true;

	if (filterAncestors(shared_from(instance->getParent())))
	{
		if (!ANORRLTreeWidgetItem::isExplorerItem(instance))
			return false;

		if (filterInstance(instance))
			m_foundItems.insert(instance);
		else
			m_ancestorsOfFoundItems.insert(instance);

		if (ANORRLTreeWidgetItem* item = findItemFromInstance(instance.get()))
			addToUpdateList(item);

		return true;
	}
	return false;
}

void ANORRLTreeWidget::filterWidget(const QString& searchString)
{
	setWidgetFilter(searchString);
	filterWidget();
}

void ANORRLTreeWidget::filterWidget()
{
	m_currentFilterStack.clear();
	m_foundItems.clear();
	m_ancestorsOfFoundItems.clear();
	m_itemsToUpdate.clear();

	for (int i = 0; i < invisibleRootItem()->childCount(); ++i)
	{
		invisibleRootItem()->child(i)->setDisabled(true);
		invisibleRootItem()->child(i)->setHidden(true);
	}

	if (!m_filterRunning)
	{
		m_filterRunning = true;
		QTimer::singleShot(0, this, SLOT(onFilterWidgetUpdate()));
	}
}

bool ANORRLTreeWidget::filterInstance(shared_ptr<ARL::Instance> instance)
{
	for (ParamList::const_iterator iter = m_searchParamList.begin(); iter != m_searchParamList.end(); ++iter)
	{
		switch (iter->second)
		{
		case ParamAny:
			if (!boost::icontains(instance->getName(), iter->first) && !boost::icontains(instance->getClassNameStr(), iter->first))
				return false;
			break;
		case ParamName:
			if (!boost::icontains(instance->getName(), iter->first))
				return false;
			break;
		case ParamType:
			if (!boost::icontains(instance->getClassNameStr(), iter->first))
				return false;
			break;
		}
	}
	return true;
}

bool ANORRLTreeWidget::filterWidgetRecursive(shared_ptr<ARL::Instance> instance, FilterDeque_SPTR& previousFilterStack, const ARL::Time& startTime)
{
	if (!ANORRLTreeWidgetItem::isExplorerItem(instance))
		return false;

	bool parentOfShownItem = false;

	if (instance->getChildren())
	{
		shared_ptr<ARL::Instance> startingChild;
		bool stackFrontFound = true;

		//using previousFilterStack to start from last search location
		if (!previousFilterStack.empty())
		{
			stackFrontFound = false;
			
			startingChild = previousFilterStack.front();
			previousFilterStack.pop_front();
			
			//if stack is broken, we continue through all children
			if (startingChild->getParent() != instance.get())
			{
				previousFilterStack.clear();
				stackFrontFound = true;
			}
		}

		for (ARL::Instances::const_iterator iter = instance->getChildren()->begin(); iter != instance->getChildren()->end(); ++iter)
		{
			if (!stackFrontFound && *iter == startingChild)
				stackFrontFound = true;

			m_currentFilterStack.push_back(*iter);

			if (stackFrontFound && filterWidgetRecursive(*iter, previousFilterStack, startTime))
				parentOfShownItem = true;

			if (isSearchTimeUp(startTime, FInt::StudioTreeWidgetFilterTime))
				break;

			m_currentFilterStack.pop_back();
		}
	}
	else
	{
		previousFilterStack.clear();
	}

	//This is a check to see if filter is empty or instance is the datamodel
	if (isFilterEmpty() || !instance->getParent())
	{
		//Restore item to it's original state
		if (ANORRLTreeWidgetItem* item = findItemFromInstance(instance.get()))
			item->setViewState();
		return false;
	}

	if (filterInstance(instance))
	{
		m_foundItems.insert(instance);
		return true;
	}
	else if (parentOfShownItem)
	{
		m_ancestorsOfFoundItems.insert(instance);
		return true;
	}
	else
	{
		//This is hiding the item if there is a filter
		if (ANORRLTreeWidgetItem* item = findItemFromInstance(instance.get()))
			item->setViewState();
	}

	return false;
}

void ANORRLTreeWidget::setWidgetFilter(const QString& filterString)
{
	m_currentFilter = filterString;

	QStringList filterList = filterString.split(" ", QString::SkipEmptyParts);

	bool nextIsName = false;
	bool nextIsType = false;
	m_searchParamList.clear();
	
	for (QStringList::const_iterator iter = filterList.begin(); iter != filterList.end(); ++iter)
	{
		if (nextIsName)
		{
			nextIsName = false;
			m_searchParamList.push_back(StringParamPair(iter->toStdString(), ParamName));
		}
		else if (nextIsType)
		{
			nextIsType = false;
			m_searchParamList.push_back(StringParamPair(iter->toStdString(), ParamType));
		}
		else if (iter->startsWith(nameSearchString.c_str(), Qt::CaseInsensitive))
		{
			QString rightSide = iter->right(iter->size() - nameSearchString.size());

			if (rightSide.isEmpty())
				nextIsName = true;
			else
				m_searchParamList.push_back(StringParamPair(rightSide.toStdString(), ParamName));
		}
		else if (iter->startsWith(classSearchString.c_str(), Qt::CaseInsensitive))
		{
			QString rightSide = iter->right(iter->size() - classSearchString.size());

			if (rightSide.isEmpty())
				nextIsType = true;
			else
				m_searchParamList.push_back(StringParamPair(rightSide.toStdString(), ParamType));
		}
		else
		{
			m_searchParamList.push_back(StringParamPair(iter->toStdString(), ParamAny));
		}
	}
}

bool ANORRLTreeWidget::isFoundItem(shared_ptr<ARL::Instance> instance)
{
	return m_foundItems.count(instance) > 0;
}

bool ANORRLTreeWidget::isAncestorOfFoundItem(shared_ptr<ARL::Instance> instance)
{
	return m_ancestorsOfFoundItems.count(instance) > 0;
}

bool ANORRLTreeWidget::isValidSelection(ARL::Instance *pInstance)
{
	if (!pInstance)
		return false;

	if (!pInstance->getParent())
		return true;

	if (isValidSelection(pInstance->getParent()))
	{
		if (!ANORRLTreeWidgetItem::isExplorerItem(shared_from(pInstance)))
			return false;

		return true;
	}

	return false;
}

void ANORRLTreeWidget::onItemSelectionChanged()
{
	if (m_bIgnoreItemSelectionChanged)
		return;

	ARL::Selection* pSelection(getSelection());
	if (!pSelection)
		return;

	if (m_pInstanceSelectionHandler)
	{
		boost::shared_ptr<ARL::Instance> selectedInstance;
		QList<QTreeWidgetItem *> selectedItems = this->selectedItems();
		if (selectedItems.size() > 0)
		{
			ANORRLTreeWidgetItem *pTreeWidgetItem = dynamic_cast<ANORRLTreeWidgetItem*>(selectedItems.at(0));
			if (pTreeWidgetItem)
				selectedInstance = pTreeWidgetItem->getInstance();
		}
		m_pInstanceSelectionHandler->onInstanceSelected(selectedInstance);
		return;
	}

	ARL::ScopedAssign<bool> ignoreSelectionEvent(m_bIgnoreInstanceSelectionChanged, true);
	ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);

	QList<QTreeWidgetItem *> selectedItems = this->selectedItems();

	shared_ptr<ARL::Instances> instances(new ARL::Instances());
	for (QList<QTreeWidgetItem*>::const_iterator iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
	{
		if (ANORRLTreeWidgetItem* pTreeWidgetItem = dynamic_cast<ANORRLTreeWidgetItem*>(*iter))
		{
			if (shared_ptr<ARL::Instance> instance = pTreeWidgetItem->getInstance())
				instances->push_back(instance);
		}
	}

	pSelection->setSelection(instances);
	
	if (currentItem())
	{
		ANORRLTreeWidgetItem *pTreeWidgetItem = dynamic_cast<ANORRLTreeWidgetItem*>(currentItem());
		QString className = pTreeWidgetItem->getInstance()->getClassName().c_str();
		if (FFlag::StudioNewWiki) 
			className.prepend("API:Class/");
		Q_EMIT(helpTopicChanged(className));
	}
	
	//HACK: to get toolbars updated for object selection
	UpdateUIManager::Instance().updateToolBars();
}

void ANORRLTreeWidget::modifyItemsSelectionState(InstanceList_PTR &instancesList, bool select)
{
	QList<QModelIndex> modelIndexes;
	
	for (InstanceList_PTR::const_iterator iter = instancesList.begin(); iter != instancesList.end(); )
	{
		if (ANORRLTreeWidgetItem *pTreeWidgetItem = findItemFromInstance(*iter))
		{
			modelIndexes.push_back(indexFromItem(pTreeWidgetItem));

			if (select)
			{
				m_lastSelectedItem = pTreeWidgetItem;
				instancesList.erase(iter++);
			}
			else
			{
				++iter;
			}
		}
		else
		{
			++iter;
		}
	}

	qStableSort(modelIndexes.begin(), modelIndexes.end());	
	selectionModel()->select(mergeModelIndexes(modelIndexes), select ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

void ANORRLTreeWidget::onItemExpanded(QTreeWidgetItem *pItem)
{
	if (!pItem)
		return;

	ANORRLTreeWidgetItem* anorrlTreeWidgetItem = static_cast<ANORRLTreeWidgetItem*>(pItem);
	anorrlTreeWidgetItem->requestItemExpand();
}

void ANORRLTreeWidget::onItemCollapsed(QTreeWidgetItem *pItem)
{
	if (!pItem)
		return;

	if (isFilterEmpty())
		pItem->setData(1, ExpandRole, QVariant(false));
}

void ANORRLTreeWidget::onItemDoubleClicked(QTreeWidgetItem *pWidgetItem, int)
{
	if (!pWidgetItem)
		return;
	
	static_cast<ANORRLTreeWidgetItem*>(pWidgetItem)->handleOpen();
}

void ANORRLTreeWidget::addToUpdateList(ANORRLTreeWidgetItem *pTreeWidgetItem)
{
	if (!pTreeWidgetItem)
		return;

	QMutexLocker lock(treeWidgetMutex());

	if (pTreeWidgetItem->isItemPendingDeletion())
		return;
	pTreeWidgetItem->setViewState();
	if (existsInRemovalList(pTreeWidgetItem) || !pTreeWidgetItem->isInTreeWidget())
		return;

	std::pair<TreeWidgetItemList::iterator, bool> retIter = m_itemsToUpdate.insert(pTreeWidgetItem);
	if (retIter.second)
		requestUpdate();
}

void ANORRLTreeWidget::removeFromUpdateList(ANORRLTreeWidgetItem *pTreeWidgetItem)
{
	QMutexLocker lock(treeWidgetMutex());
	m_itemsToUpdate.erase(pTreeWidgetItem);
}

void ANORRLTreeWidget::requestUpdate()
{
	if (m_bUpdateRequested)
		return;
	//qt will take ownership of the event so no need to delete
	QApplication::postEvent(this, new ANORRLCustomEvent(TREE_WIDGET_UPDATE));
		
	m_bUpdateRequested = true;
}

void ANORRLTreeWidget::requestViewportUpdate()
{
	if (m_bViewportUpdateRequested || m_bUpdateRequested)
		return;
	//qt will take ownership of the event so no need to delete
	QApplication::postEvent(this, new ANORRLCustomEvent(TREE_WIDGET_VIEWPORT_UPDATE));
	m_bViewportUpdateRequested = true;
}

void ANORRLTreeWidget::updateSelectionState(ARL::Instance *pInstance)
{
	ARL::Selection* pSelection(getSelection());
	if (pSelection && pSelection->isSelected(pInstance))
	{
		QMutexLocker lock(treeWidgetMutex());
		m_unSelectedItems.erase(pInstance);
		m_selectedItems.insert(pInstance);			
	}

	// If items are added after updating the highlighting state for CloudEdit mode, update now
	ARL::Instances::const_iterator selectedIter;
	for (auto& playersSelection : m_prevCloudEditSelection)
	{
		selectedIter = std::find(playersSelection.second->begin(), playersSelection.second->end(), shared_from(pInstance));
		if (selectedIter != playersSelection.second->end())
		{
			if (ANORRLTreeWidgetItem* pTreeWidgetItem = findItemFromInstance(pInstance))
				pTreeWidgetItem->addPlayerSelection(playersSelection.first, m_pPlayersDataManager->getPlayerColor(playersSelection.first));
		}
	}
}

bool ANORRLTreeWidget::event(QEvent* evt)
{
	bool retVal = false;
	if(evt->type() == TREE_WIDGET_UPDATE)
	{
		onTreeWidgetUpdate();
		m_bUpdateRequested = false;
		retVal = true;
	}
	else if(evt->type() == TREE_WIDGET_FILTER_UPDATE)
	{
		onFilterWidgetUpdate();
	}
	else if(evt->type() == TREE_SCROLL_TO_INSTANCE)
	{
		ANORRLCustomEventWithArg *pCustomEvent = dynamic_cast<ANORRLCustomEventWithArg*>(evt);
		if (pCustomEvent)
		{
			boost::function<void()>* pFunctionObj = pCustomEvent->m_pEventArg;
			if (pFunctionObj)
				(*pFunctionObj)();		
		}
		retVal = true;
	}
	else if (evt->type() == TREE_WIDGET_VIEWPORT_UPDATE)
	{
		m_bViewportUpdateRequested = false;
		viewport()->update();
		
		retVal = true;
	}
    else if ( evt->type() == QEvent::ShortcutOverride )
    {
        retVal = QTreeWidget::event(evt);
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(evt);
        QChar key = keyEvent->key();

        // capture single key alpha numerical presses - we want these to be considered
        //  as shortcuts in the tree to jumped to items starting with that key, not global shortcuts
        if ( !(keyEvent->key() >= Qt::Key_F1 && keyEvent->key() <= Qt::Key_F35) && 
             key.isLetterOrNumber() && 
             keyEvent->modifiers() == Qt::NoModifier )
        {
            keyEvent->accept();
            retVal = true;
        }
    }
	else
		retVal = QTreeWidget::event(evt);

	return retVal;
}

void ANORRLTreeWidget::onTreeWidgetUpdate()
{
	if (isHidden() || m_filterRunning)
		return;
		
	QCoreApplication::removePostedEvents(this, TREE_WIDGET_VIEWPORT_UPDATE);

	//prevents screen flicker
	setUpdatesEnabled(false);
	ARL::ScopedAssign<bool> ignoreSelectionEvent(m_bIgnoreItemSelectionChanged, true);

	QMutexLocker treeLock(treeWidgetMutex());

	if (!m_itemsToRemove.empty())
		processChildrenRemoval();
		
	int selectionSize = m_selectedItems.size();
	//modify selection first, items not removed from list and items that have yet been populated
	{
		blockSignals(true);
		QMutexLocker lock(treeWidgetMutex());

		if (!m_unSelectedItems.empty())
		{
			modifyItemsSelectionState(m_unSelectedItems, false);
			m_unSelectedItems.clear();
		}

		if (!m_selectedItems.empty())
			modifyItemsSelectionState(m_selectedItems, true);	

		selectionSize = m_selectedItems.size();
		blockSignals(false);
	}

	bool itemsAdded = false;
		
	InstanceList_PTR itemsToSelect;

	ARL::Time startTime = ARL::Time::now<ARL::Time::Fast>();

	std::vector<ANORRLTreeWidgetItem*> recentlyAddedItems;

	//Processing Selection
	while (!m_selectedItems.empty() && isFilterEmpty() && !isSearchTimeUp(startTime, FInt::StudioTreeWidgetProcessingTime))
	{
		ARL::Instance* pInstance = *m_selectedItems.begin();

		ARL::Instance* pChild = pInstance;
		while (pChild->getParent() && !findItemFromInstance(pChild->getParent()))
			pChild = pChild->getParent();

		ANORRLTreeWidgetItem* newChildNode = NULL;

		if (ANORRLTreeWidgetItem* pParentItem = findItemFromInstance(pChild->getParent()))
		{
			pParentItem->requestItemExpand();
			newChildNode = pParentItem->processChildAdd(shared_from(pChild));
		}
		else
		{
			m_selectedItems.erase(pInstance);
		}

		if (pChild == pInstance && newChildNode)
		{
			recentlyAddedItems.push_back(newChildNode);
			m_lastSelectedItem = newChildNode;
			m_selectedItems.erase(pInstance);
		}
	}

	//Processing items requesting update
	while (!m_itemsToUpdate.empty() && !isSearchTimeUp(startTime, FInt::StudioTreeWidgetProcessingTime))
	{
		ANORRLTreeWidgetItem* itemNode = *m_itemsToUpdate.begin();

		if (itemNode)
		{
			itemNode->updateFilterItems();
			if (itemNode->processChildrenAdd())
				itemsAdded = true;

			if (!itemNode->hasChildrenToProcess())
			{
				removeFromUpdateList(itemNode);
				itemNode->setDirty(false);
			}
		}
	}

	//recently added items
	if (!recentlyAddedItems.empty())
	{
		QList<QModelIndex> modelIndexes;

		for (std::vector<ANORRLTreeWidgetItem*>::const_iterator iter = recentlyAddedItems.begin(); iter != recentlyAddedItems.end(); ++iter)
			modelIndexes.push_back(indexFromItem(*iter));

		qStableSort(modelIndexes.begin(), modelIndexes.end());
		selectionModel()->select(mergeModelIndexes(modelIndexes), QItemSelectionModel::Select);
	}
		
	processChildrenAdd();

	setUpdatesEnabled(true);

	m_bUpdateRequested = false;

	//again request for an update
	if (!m_itemsToRemove.empty() || !m_itemsToUpdate.empty() || itemsAdded || (!m_selectedItems.empty() && isFilterEmpty()))
	{
		QTimer::singleShot(0, this, SLOT(requestUpdate()));
	}
	else
	{
		if (m_lastSelectedItem)
		{
			scrollToItem(m_lastSelectedItem);
			eraseLastSelectedItem();
		}

		if (m_savedMarkerItem)
			QMetaObject::invokeMethod(this, "scrollBackToLastMarkedLocation");

		Q_EMIT finishedProcessing();
	}
}

void ANORRLTreeWidget::requestDelete()
{
	m_bDeletionRequested = true;
	
	deActivate();

	m_pInstance.reset();
	m_pDataModel.reset();
	setPlayersDataManager(boost::shared_ptr<PlayersDataManager>());

	deleteLater();
}

void ANORRLTreeWidget::mousePressEvent(QMouseEvent * evt)
{
	QTreeWidget::mousePressEvent(evt);

    ANORRLTreeWidgetItem* pTreeWidgetItem = dynamic_cast<ANORRLTreeWidgetItem*>(itemAt(evt->pos()));
    
    if (pTreeWidgetItem)
    {
        QString className = pTreeWidgetItem->getInstance()->getClassName().c_str();
		if (FFlag::StudioNewWiki) 
			className.prepend("API:Class/");
        Q_EMIT(helpTopicChanged(className));
    }
    
	// Check if ContextMenu was opened in the middle of dragging.
	if (m_pRubberBand)
	{    
		m_pRubberBand->hide();
		m_pRubberBand->deleteLater();
		m_pRubberBand = NULL;
	}
	else
	{
		//although it will be set to NULL in mouseReleaseEvent but just to be sure setting it to NULL again
		m_pRubberBand = NULL;
	}


	//if we cannot drag select then continue with Qt event propagation
	if (!canDragSelect(evt))
	{
		if (FFlag::StudioMimeDataContainsInstancePath)
		{
			QDrag *drag = new QDrag(this);
			QMimeData *mimeData = new QMimeData;
			mimeData->setText(("Game." + pTreeWidgetItem->getInstance()->getFullName()).c_str());
			drag->setMimeData(mimeData);
			drag->exec();
		}

		return;
	}

	//begin drag select
	m_initialOffset = QPoint(horizontalOffset(), verticalOffset());
	m_rubberBandOrigin = evt->pos();

	m_pRubberBand = new QRubberBand(QRubberBand::Rectangle, this);
	m_pRubberBand->setGeometry(QRect(m_rubberBandOrigin, QSize()));
	m_pRubberBand->show();
}

void ANORRLTreeWidget::mouseMoveEvent(QMouseEvent * evt)
{
	if (!m_pRubberBand)
	{
        QTreeWidget::mouseMoveEvent(evt);
        if (m_pInstanceSelectionHandler)
        {
            boost::shared_ptr<ARL::Instance> hoveredInstance;
            ANORRLTreeWidgetItem* pTreeWidgetItem = dynamic_cast<ANORRLTreeWidgetItem*>(itemAt(evt->pos()));
            if (pTreeWidgetItem)
                hoveredInstance = pTreeWidgetItem->getInstance();
            m_pInstanceSelectionHandler->onInstanceHovered(hoveredInstance);
        }

		return;
	}

	//--- drag selection is ON (do the required updates)
	if (verticalScrollBar())
	{
		if (evt->y() <= 1)
			verticalScrollBar()->setValue(verticalScrollBar()->value() + evt->y() - 1);
		else if (evt->y() >= (viewport()->height() - 1))
			verticalScrollBar()->setValue(verticalScrollBar()->value() + evt->y() - viewport()->height() - 1);
	}

	if (horizontalScrollBar())
	{
		if (evt->x() <= 1)
			horizontalScrollBar()->setValue(horizontalScrollBar()->value() + evt->x() - 1);
		else if (evt->y() >= (viewport()->width() - 1))
			horizontalScrollBar()->setValue(horizontalScrollBar()->value() + evt->x() - viewport()->width() - 1);
	}

	QPoint offset = QPoint(horizontalOffset() - m_initialOffset.x(), verticalOffset() - m_initialOffset.y());
	QRect rubberBandRect = QRect(m_rubberBandOrigin - offset, evt->pos()).normalized();

	m_pRubberBand->setGeometry(rubberBandRect);
	setSelection(QRect(rubberBandRect.bottomLeft(), rubberBandRect.topLeft()), QItemSelectionModel::ClearAndSelect);
}

void ANORRLTreeWidget::mouseReleaseEvent(QMouseEvent * evt)
{
	if (!m_pRubberBand)
	{
		QTreeWidget::mouseReleaseEvent(evt);
		return;
	}

	//clean up rubberband
    m_pRubberBand->hide();
	m_pRubberBand->deleteLater();
	m_pRubberBand = NULL;
}

bool ANORRLTreeWidget::canDragSelect(QMouseEvent * evt)
{
	if (evt->buttons() != Qt::LeftButton || evt->modifiers() != Qt::NoModifier || m_pInstanceSelectionHandler)
		return false;

	QPoint mousePos = evt->pos();
	QTreeWidgetItem *pTreeWidgetItem = itemAt(mousePos);
	if (pTreeWidgetItem)
	{
		// - make sure if the mouse is clicked outside the item label we must be able to drag select
		// - we must not activate drag selection if mouse click is on the left of the label (to ensure the expand keys work fine)
		// - item drag/drop will work when user clicks on the item label
		QRect itemRect = visualItemRect(pTreeWidgetItem);
		if ( !itemRect.contains(mousePos) ||
			 (itemRect.contains(mousePos) && (mousePos.x() < (itemRect.bottomLeft().x() + 28 + fontMetrics().width(pTreeWidgetItem->text(0))))) )
			return false;
	}

	//-- we can do a drag select --

	// item under mouse is already selected and the only selected item, we don't proceed further
	if (pTreeWidgetItem && pTreeWidgetItem->isSelected() && (selectedItems().size() == 1))
		return true;

	//clear previously selected items
	clearSelection();
	//if we have an item then select it
	if (pTreeWidgetItem)
		pTreeWidgetItem->setSelected(true);

	return true;
}

void ANORRLTreeWidget::dragEnterEvent(QDragEnterEvent *evt)
{
	const QMimeData *mime = evt->mimeData();
	QString data = mime->text();
	QTreeWidget::dragEnterEvent(evt);
}

void ANORRLTreeWidget::dragMoveEvent(QDragMoveEvent * evt)
{
	// call base class to handle auto scrolling
    QTreeWidget::dragMoveEvent(evt);

	evt->ignore();

	if (ANORRLTreeWidgetItem *pTreeWidgetItem = dynamic_cast<ANORRLTreeWidgetItem*>(itemAt(evt->pos())))
	{
		if (shared_ptr<ARL::Instance> pInstance = pTreeWidgetItem->getInstance())
		{
			ARL::Selection* selection = getSelection();
			for (ARL::Instances::const_iterator iter = selection->begin(); iter != selection->end(); ++iter)
				if (pInstance == *iter || (*iter)->isAncestorOf2(pInstance) || (*iter)->getIsParentLocked())
					return;

			evt->acceptProposedAction();
		}
	}
}

static void setChildren(ARL::Instance* pParent, shared_ptr<ARL::Instance> pChild);

void ANORRLTreeWidget::dropEvent(QDropEvent * evt)
{
	try
	{
		ANORRLTreeWidgetItem *pTreeWidgetItem = dynamic_cast<ANORRLTreeWidgetItem*>(itemAt(evt->pos()));
		if (pTreeWidgetItem)
		{
			boost::shared_ptr<ARL::Instance> pInstance = pTreeWidgetItem->getInstance();
			if (pInstance)
			{
				ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);
				pTreeWidgetItem->requestItemExpand();
		
				std::for_each(getSelection()->begin(), getSelection()->end(), boost::bind(&setChildren, pInstance.get(), _1));

				//add in history for undo/redo
				ARL::ChangeHistoryService::requestWaypoint("Drop", m_pDataModel.get());
				m_pDataModel->setDirty(true);
			}
		}
	}
	catch (std::exception& e)
	{
		QtUtilities::ARLMessageBox msgBox;
		msgBox.setText(e.what());
		msgBox.exec();
	}

	evt->setDropAction(Qt::IgnoreAction);
    setState(NoState);
}

static void setChildren(ARL::Instance* pParent, shared_ptr<ARL::Instance> pChild)
{
	//self drag case (ideally qt should have already taken care of this)
	if (!pChild || pChild->getParent() == pParent)
		return;

	pChild->setParent(pParent);
}

static int nearestWidgetItemIndex(ANORRLTreeWidgetItem *pParentWidgetItem, int first, int last, int itemTypeToAdd) 
{
	while (first <= last) 
	{
		int mid = (first + last) / 2;
		ANORRLTreeWidgetItem *pCurrentItem = static_cast<ANORRLTreeWidgetItem*>(pParentWidgetItem->child(mid));
		ARLASSERT(pCurrentItem != NULL);

		if (itemTypeToAdd == pCurrentItem->itemType()) 
			return mid; 
		else if (itemTypeToAdd > pCurrentItem->itemType()) 
			first = mid+1;
		else
			last = mid-1;
	}

	return first;
}

static QItemSelection mergeModelIndexes(const QList<QModelIndex> &indexes)
{
    QItemSelection colSpans;
    // merge columns
    int i = 0;
    while (i < indexes.count()) {
        QModelIndex tl = indexes.at(i);
        QModelIndex br = tl;
        while (++i < indexes.count()) {
            QModelIndex next = indexes.at(i);
            if ((next.parent() == br.parent())
                 && (next.row() == br.row())
                 && (next.column() == br.column() + 1))
                br = next;
            else
                break;
        }
        colSpans.append(QItemSelectionRange(tl, br));
    }
    // merge rows
    QItemSelection rowSpans;
    i = 0;
    while (i < colSpans.count()) {
        QModelIndex tl = colSpans.at(i).topLeft();
        QModelIndex br = colSpans.at(i).bottomRight();
        QModelIndex prevTl = tl;
        while (++i < colSpans.count()) {
            QModelIndex nextTl = colSpans.at(i).topLeft();
            QModelIndex nextBr = colSpans.at(i).bottomRight();

            if (nextTl.parent() != tl.parent())
                break;

            if ((nextTl.column() == prevTl.column()) && (nextBr.column() == br.column())
                && (nextTl.row() == prevTl.row() + 1) && (nextBr.row() == br.row() + 1)) {
                br = nextBr;
                prevTl = nextTl;
            } else {
                break;
            }
        }
        rowSpans.append(QItemSelectionRange(tl, br));
    }
    return rowSpans;
}

void ANORRLTreeWidget::setInstanceSelectionHandler(InstanceSelectionHandler* handler)
{
	setMouseTracking(handler);
	m_pInstanceSelectionHandler = handler;
}

void ANORRLTreeWidget::markLocationToSuppressScrolling()
{
	if (m_savedMarkerItem == NULL)
	{
		m_savedMarkerItem = getMarkerItem();
	}
}

void ANORRLTreeWidget::scrollBackToLastMarkedLocation()
{
	if (m_savedMarkerItem)
	{
		QTreeWidgetItem* currenMarkerItem = getMarkerItem();
		if (currenMarkerItem != m_savedMarkerItem)
		{
			int savedRow = indexFromItem(m_savedMarkerItem).row();
			int currentRow = indexFromItem(currenMarkerItem).row();

			verticalScrollBar()->setValue(verticalScrollBar()->value() + (savedRow - currentRow) * verticalScrollBar()->singleStep());
		}
		m_savedMarkerItem = NULL;
	}
}

QTreeWidgetItem* ANORRLTreeWidget::getMarkerItem()
{
	QList<QTreeWidgetItem*> selection = selectedItems();
	// if we've a single selection and the selected item is not hidden, the use it as benchmark
	if (selection.count() == 1 && !selection.at(0)->isHidden())
		return selection.at(0);
	// or else use the top item as benchmark
	return itemAt(rect().topLeft());
}

void ANORRLTreeWidget::setPlayersDataManager(boost::shared_ptr<PlayersDataManager> playersDataManager)
{
	if (m_pPlayersDataManager == playersDataManager)
		return;

	if (m_pPlayersDataManager)
		disconnect(m_pPlayersDataManager.get(), SIGNAL(cloudEditSelectionChanged(int)), this, SLOT(onCloudEditSelectionChanged(int)));
	m_TreeWidgetDelegate.reset();
	m_pPlayersDataManager = playersDataManager;

	if (m_pPlayersDataManager)
	{
		m_TreeWidgetDelegate.reset(new TreeWidgetDelegate(this));
		setItemDelegate(m_TreeWidgetDelegate.get());
		connect(m_pPlayersDataManager.get(), SIGNAL(cloudEditSelectionChanged(int)), this, SLOT(onCloudEditSelectionChanged(int)));
	}
}

// TODO: Merge this with eraseLastSelectedItem
void ANORRLTreeWidget::itemDeletionRequested(ANORRLTreeWidgetItem* pItem)
{
	if (pItem == m_savedMarkerItem)
		m_savedMarkerItem = NULL;
}

ANORRLExplorerWidget::ANORRLExplorerWidget(QWidget *pParent)
: QWidget(pParent)
, m_anorrlTreeWidget(NULL)
, m_sendingCounter(false)
{
	m_lineEdit = new QLineEdit();
	m_lineEdit->setPlaceholderText(QString("Filter workspace (%1)").arg(UpdateUIManager::Instance().getMainWindow().explorerFilterAction->shortcut().toString()));
	m_lineEdit->hide();

	m_loadingMovie = new QMovie(":/images/loading.gif");
	m_loadingLabel = new QLabel(m_lineEdit);
	m_loadingLabel->setMovie(m_loadingMovie);
	
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addStretch();
	layout->addWidget(m_loadingLabel);
	layout->addSpacing(5);

	m_lineEdit->setLayout(layout);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setSpacing(4);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(m_lineEdit);
	setLayout(mainLayout);

	setFocusProxy(m_lineEdit);
}

void ANORRLExplorerWidget::setCurrentWidget(ANORRLTreeWidget* treeWidget)
{
	if (m_anorrlTreeWidget == treeWidget)
		return;

	QLayout* mainLayout = layout();
	
	if (m_anorrlTreeWidget)
	{
		m_lineEdit->hide();

		onProcessingFinished();
		
		m_anorrlTreeWidget->hide();
		mainLayout->removeWidget(m_anorrlTreeWidget);

		disconnect(m_anorrlTreeWidget, SIGNAL(finishedProcessing()), this, SLOT(onProcessingFinished()));
		disconnect(m_anorrlTreeWidget, SIGNAL(startedProcessing()), this, SLOT(onProcessingStarted()));
		disconnect(m_anorrlTreeWidget, SIGNAL(focusGained()), this, SIGNAL(focusGained()));
		disconnect(m_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(filterTreeWidget(QString)));
	}

	m_anorrlTreeWidget = treeWidget;
	
	if (m_anorrlTreeWidget)
	{
		mainLayout->addWidget(m_anorrlTreeWidget);
		m_anorrlTreeWidget->show();
		m_lineEdit->show();

		m_lineEdit->setText(m_anorrlTreeWidget->currentFilter());

		connect(m_anorrlTreeWidget, SIGNAL(finishedProcessing()), this, SLOT(onProcessingFinished()));
		connect(m_anorrlTreeWidget, SIGNAL(startedProcessing()), this, SLOT(onProcessingStarted()));
		connect(m_anorrlTreeWidget, SIGNAL(focusGained()), this, SIGNAL(focusGained()));
		connect(m_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(filterTreeWidget(QString)));
		
		if (!m_anorrlTreeWidget->isFilterEmpty())
		{
			onProcessingStarted();
			m_anorrlTreeWidget->filterWidget();
		}
	}
}

void ANORRLExplorerWidget::onProcessingStarted()
{
	m_loadingMovie->start();
	m_loadingLabel->show();
}

void ANORRLExplorerWidget::onProcessingFinished()
{
	m_loadingLabel->hide();
	m_loadingMovie->stop();
}

void ANORRLExplorerWidget::filterTreeWidget(const QString& text)
{
	if (m_anorrlTreeWidget)
	{
		if (!m_sendingCounter)
		{
			m_sendingCounter = true;
			QTimer::singleShot(1000, this, SLOT(sendWidgetCounter()));
		}
		
		m_anorrlTreeWidget->filterWidget(text);
	}	
}

void ANORRLExplorerWidget::sendWidgetCounter()
{
	m_sendingCounter = false;

	ANORRLSettings settings;
	bool previouslyFiltered = settings.value("ExplorerWidgetFilterUsed", false).toBool();
	settings.setValue("ExplorerWidgetFilterUsed", true);
}
