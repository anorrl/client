/**
 * ARLWorkspace.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QObject>

// boost headers
#include <boost/enable_shared_from_this.hpp>

// ANORRL Headers
#include "v8xml/XmlElement.h"
#include "v8tree/Instance.h"

// If you comment the ENABLE_EDIT_PLACE_IN_STUDIO then the Play Button will start working
// The reason is if the Webpage has a StartGame implementation then the NPANORRL Plugin does not get loaded & that prevents the Play button to invoke the Player
#define ENABLE_EDIT_PLACE_IN_STUDIO
#define ENABLE_DRAG_DROP_TOOLBOX

namespace ARL
{
	class DataModel;
	class Workspace;
}

class ARLContent;
class QMessageBox;
class QAbstractButton;

// This class is used as the JS Hookup from the WebView
class ARLWorkspace
	: public QObject
	, public boost::enable_shared_from_this<ARLWorkspace>
{
Q_OBJECT
// Edit Buton Show up only if we return true for the IsANORRLAppIDE property
Q_PROPERTY(bool IsANORRLAppIDE READ getIsANORRLAppIDE) // MFC Studio && QT Studio (All Studio)
Q_PROPERTY(bool IsANORRL2App READ getIsANORRL2App) // QT Studio only
Q_PROPERTY(bool IsANORRLABApp READ getIsANORRLABApp) // Admin Build -- Play button will call window.external.StartGame to play inside of Studio as opposed to launching the ANORRL Player
  
public:
	ARLWorkspace(QObject *parent, ARL::DataModel* dm);
	~ARLWorkspace();
	bool getIsANORRLAppIDE() const 	{ return true; }
	bool getIsANORRL2App() const 	{ return true; }
	bool getIsANORRLABApp() const 	
	{ 
#ifdef STUDIO_ADMIN_BUILD
		return true; 
#else
		return false;
#endif
	}

private:
	QString m_selectorForFileUpload;
	QMessageBox* m_screenShotInProgressMsgBox;
	ARL::DataModel *m_pDataModel;
	ARLContent *m_pContent;
	QObject *m_pParent;
	
	std::auto_ptr<XmlElement> writeSelectionToXml();
	void writeToStream(const XmlElement* root, std::ostream& stream);
	ARL::Workspace* getWorkspace() const;
	
	void insert(ARL::ContentId contentId, bool insertInto);
	void insert(std::istream& stream, bool insertInto);
	void insert(ARL::Instances& instances, bool insertInto);
	void loadContent(ARL::ContentId contentId, ARL::Instances& instances, bool& hasError);
	
	void Close() { Q_EMIT closeEvent(); }
	void Hide() { Q_EMIT hideEvent(); }
	void Show() { Q_EMIT showEvent(); }

	static void pluginInstallCompleteHelper(weak_ptr<ARLWorkspace> weakWorkspace, bool succeeded, int assetId);

public:
	static bool isScriptAssetUploadEnabled;
	static bool isAnimationAssetUploadEnabled;
	static bool isImageModelAssetUploadEnabled;
	void onScreenshotFinished(const std::string &fileName);
	Q_INVOKABLE void onScreenShotFinished_MT(QString fileName);

    void setDataModel(ARL::DataModel *pDataModel) { m_pDataModel = pDataModel; }
    
Q_SIGNALS:
	void thumbnailTaken(int width, int height);
	void closeEvent();
	void hideEvent();
	void showEvent();
	void PluginInstallComplete(bool succeeded, int assetId);

public Q_SLOTS:
	// Publish To ANORRL JS Callback
    bool SaveUrl( const QString &saveUrl );
    bool Save();

	void handleTakeThumbnail(QAbstractButton*);

#ifdef ENABLE_EDIT_PLACE_IN_STUDIO
	// Build & Edit Button Hookup for JS Callback to open the Place in Studio
	bool StartGame(const QString &ticket, const QString &url, const QString &script);
#endif
	
	// Opens a recent file -- called from the StartPage -- file name is looked up in recent files for safety (prevent arbitrary file opening)
	bool OpenRecentFile(const QString &recentFileName);

	// Publish Selection As Model to ANORRL JS Callback, it returns another JS Callboack object ARLContent which does the actual publishing
	QObject* WriteSelection();
	
	// HTML Toolbox related JS Callback
	bool Insert(const QString &url);

	bool TakeThumbnail(QString selector);

#ifdef ENABLE_DRAG_DROP_TOOLBOX
	bool StartDrag(const QString& url);
#endif

	// Opens picture folder
	bool OpenPicFolder();
	
	// Posts given image to ANORRL site
	void PostImage(bool doPost, int postSetting, const QString &fileName, const QString &seoStr);

	void InstallPlugin(int assetId, int assetVersion);
	QString GetInstalledPlugins();
	void SetPluginEnabled(int assetId, bool enabled);
	void UninstallPlugin(int assetId);

    void OpenUniverse(int universeId);
	void PublishAsNewUniverse();
	void PublishAsNewGroupUniverse(int groupId);
	void PublishToUniverse(int universeId);

	void ImportAsset(int assetId);
	QString GetGameAnimations(int universeId);
};
