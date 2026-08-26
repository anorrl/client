/**
 * ANORRLBasicDoc.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRLBasicDoc.h"

// Qt Headers
#include <QString>

// ANORRL Studio Headers
#include "IExternalHandler.h"
#include "UpdateUIManager.h"
#include "ANORRLDocManager.h"

ANORRLBasicDoc::ANORRLBasicDoc()
    : m_bActive(false)
{
}

ANORRLBasicDoc::~ANORRLBasicDoc()
{
}

void ANORRLBasicDoc::addExternalHandler(IExternalHandler* extHandler)
{
	if(extHandler == NULL) 
		return;

	if(extHandler->handlerId().isEmpty())
		return;

	m_handlersMap[extHandler->handlerId()] = extHandler;
}

void ANORRLBasicDoc::removeExternalHandler(const QString &handlerId)
{
	if(handlerId.isEmpty())
		return;

	m_handlersMap.erase(handlerId);
}

IExternalHandler* ANORRLBasicDoc::getExternalHandler(const QString &handlerId)
{
	return m_handlersMap[handlerId];
}

bool ANORRLBasicDoc::handleAction( const QString &actionID, bool isChecked /*= true*/ )
{
	// Try to see if inherited class handles this action
	return doHandleAction(actionID, isChecked);
}

/** do not override **/
bool ANORRLBasicDoc::close()
{
	if (doClose())
	{
		ANORRLDocManager::Instance().updateWindowMenu();
		return true;
	}
	return false;
}
