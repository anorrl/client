/**
 * RobloxObjectBrowserDoc.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QListWidget>
#include <QTextBrowser>

// Roblox Headers
#include "reflection/EnumConverter.h"

// Roblox Studio Headers
#include "RobloxBasicDoc.h"

class QSplitter;

namespace ARL
{
	namespace Reflection
	{
		class Descriptor;
		class ClassDescriptor;
		class PropertyDescriptor;
		class FunctionDescriptor;
		class YieldFunctionDescriptor;
		class EventDescriptor;
		class CallbackDescriptor;
		namespace Metadata
		{
			class Reflection;
		}
	}
}

class ObjectBrowserItem : public QListWidgetItem
{	
public:
	ObjectBrowserItem(const ARL::Reflection::Descriptor* desc);
	const ARL::Reflection::Descriptor* getDescriptor() { return m_descriptor; }

private:
	const ARL::Reflection::Descriptor *m_descriptor;
};

class DeclarationView : public QTextBrowser
{
public:
	DeclarationView(QWidget* parent);

	void updateDeclarationView(ObjectBrowserItem* item);

private:
	void constructView(std::string& decl, std::string& owner, bool backend, bool bRestricted, bool bDeprecated, std::string& summary);
	const ARL::Reflection::Descriptor *m_descriptor;
};

class MemberListWidget : public QListWidget
{
public:
	MemberListWidget(QWidget* parent);

	void updateMemberList(ObjectBrowserItem* item);

private:
	void insertProperty(const ARL::Reflection::PropertyDescriptor* d);
	void insertFunction(const ARL::Reflection::FunctionDescriptor* d);
	void insertYieldFunction(const ARL::Reflection::YieldFunctionDescriptor* d);
	void insertSignal(const ARL::Reflection::EventDescriptor* d);
	void insertCallback(const ARL::Reflection::CallbackDescriptor* d);
	void insertEnumItem(const ARL::Reflection::EnumDescriptor::Item* e);

	std::vector<ObjectBrowserItem*> m_MemberItemVector;
	const ARL::Reflection::Descriptor *m_descriptor;
	bool m_showInheritedMembers;

	int toBeDeleted_Cnt;
};

class ClassListWidget : public QListWidget
{
public:
	ClassListWidget(QWidget* parent);

	void init();
	QSize sizeHint() const;
	
private:
	void insertClass(const ARL::Reflection::ClassDescriptor* d);
	void insertEnum(const ARL::Reflection::EnumDescriptor* enumDesc);

	std::vector<ObjectBrowserItem*> m_ClassItemVector;
};

class RobloxObjectBrowserDoc: public QObject, public RobloxBasicDoc
{
	Q_OBJECT

public:

	RobloxObjectBrowserDoc();
	~RobloxObjectBrowserDoc();
	
	bool open(RobloxMainWindow *pMainWindow, const QString &fileName = "");
	
	IRobloxDoc::ARLCloseRequest requestClose() { return IRobloxDoc::NO_SAVE_NEEDED; }

	IRobloxDoc::ARLDocType docType() { return IRobloxDoc::OBJECTBROWSER; }

	QString fileName() const        { return ""; }
	QString displayName() const     { return m_displayName; } 
	QString keyName() const         { return "ObjectBrowser"; }

	bool save(){ return false; }
	bool saveAs(const QString &){ return false; }

	QString saveFileFilters() { return ""; }
	
	QWidget* getViewer() { return m_ObjectBrowser; }
	
	bool isModified(){ return false; }
	
	void activate();
	void deActivate();
	
	bool actionState(const QString &actionID, bool &enableState, bool &checkedState);

	bool handlePluginAction(void *, void *) { return false; }

	void handleScriptCommand(const QString &) { return; }	

	bool supportsZeroPlaneGrid()        { return false; }

private Q_SLOTS:
	void onClassSelected(QListWidgetItem* current, QListWidgetItem* previous);
	void onMemberSelected(QListWidgetItem* current, QListWidgetItem* previous);
	
private:
	virtual bool doClose();
	void initializeAllViews();
	void clearListWidget(QListWidget* listWidget);

	std::vector<ObjectBrowserItem*> m_MemberItemVector;

	QWidget				*m_ObjectBrowser;	
	QSplitter			*m_vSplitter;
	QSplitter			*m_hSplitter;
	ClassListWidget		*m_ClassList;
	MemberListWidget	*m_MemberList;
	DeclarationView		*m_DeclarationView;
	QString              m_displayName;

	bool state;

	static int           sOBCount;
};

