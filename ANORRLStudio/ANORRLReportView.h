/**
 * ANORRLReportView.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QTreeWidget>
#include <QStyledItemDelegate>

class ANORRLCategoryItemDelegate : public QStyledItemDelegate
{
public:
	ANORRLCategoryItemDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
	void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const;
};

class ANORRLCategoryItem: public QTreeWidgetItem
{
public:
	ANORRLCategoryItem();
};

class ANORRLReportView : public QTreeWidget
{
public:
	ANORRLReportView(); 
	virtual ~ANORRLReportView();

	void addCategoryItem(ANORRLCategoryItem* pItem);
	bool isCategoryItem(const QModelIndex &index);
	ANORRLCategoryItem* findCategoryItem(const QString& category);
	bool addToCategory(const QString& category, QTreeWidgetItem* pItem);
		
private:
	/*override*/void drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	/*override*/void drawBranches(QPainter * painter, const QRect & rect, const QModelIndex & index ) const;
};


