/**
 * RobloxSettingsDialog.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

//this will make sure that we can use qt with boost (else moc compiler errors)
#ifndef QT_NO_KEYWORDS
    #define QT_NO_KEYWORDS
#endif

// Qt Headers
#include <QDialog>
#include <QTableWidget>

// Roblox Studio Headers
#include "RobloxPropertyWidget.h"

namespace ARL {
	class Instance;
	class Selection;
	class SelectionChanged;
}

class QSplitter;

class SettingsItem : public QTableWidgetItem
{	
public:
	SettingsItem(boost::shared_ptr<ARL::Instance> inst, QString name);
	boost::shared_ptr<ARL::Instance> getInstance() { return m_instance; }

private:
	boost::shared_ptr<ARL::Instance> m_instance;
};

class SelectionPropertyTree : public PropertyTreeWidget
{
public:
	SelectionPropertyTree(QWidget* parent = 0);
	~SelectionPropertyTree();
	void setSelection(ARL::Selection* selection);
	ARL::Selection* getSelection() { return m_selection.get(); }

	void requestUpdate();
	
private:
	void initProperty(const ARL::Reflection::ClassDescriptor* pClassDescriptor, 
		              const ARL::Reflection::PropertyDescriptor* pPropertyDescriptor);
	void onSelectionChanged(const ARL::SelectionChanged& evt);
	void addSelectionInstance(boost::shared_ptr<ARL::Instance> pInstance);
	void removeSelectionInstance(boost::shared_ptr<ARL::Instance> pInstance);

	boost::shared_ptr<ARL::Selection>		m_selection;
	rbx::signals::scoped_connection			m_selectionChangedConnection;

	InstanceList							m_selectedInstances;

	QMutex                                  m_propertyWidgetMutex;

};


class RobloxSettingsDialog : public QDialog
{

Q_OBJECT
	
public:
	RobloxSettingsDialog(QWidget* parent);
    virtual ~RobloxSettingsDialog();

private Q_SLOTS:
	void onCategorySelected(QTableWidgetItem* current, QTableWidgetItem* previous);
	void onReset();

	/*override*/bool close();

private:
	void populate();

	bool hasProperties(const ARL::Reflection::ClassDescriptor* pDescriptor);

	void onDescendantAdded(boost::shared_ptr<ARL::Instance> child);
	void onDescendantRemoving(boost::shared_ptr<ARL::Instance> child);

	rbx::signals::scoped_connection m_descendantAddedConnection;
	rbx::signals::scoped_connection m_descendantRemovingConnection;

	QSplitter				*m_hSplitter;
	QTableWidget			*m_SettingsTypesTable;
	SelectionPropertyTree	*m_SettingsTree;

};	
