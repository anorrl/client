/**
 * RobloxStudioVerbs.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QProcess>
#include <QMutex>

// Roblox Headers
#include "v8tree/Verb.h"
#include "v8datamodel/Commands.h"
#include "MobileDevelopmentDeployer.h"

//Studio Headers
#include "RobloxIDEDoc.h"
#include "RobloxDocManager.h"
#include "PairRbxDeviceDialog.h"
#include "ManageEmulationDeviceDialog.h"
#include "ScriptPickerDialog.h"

namespace ARL {
	class Instance;
	class DataModel;
	class VideoControl;
	class ViewBase;
	class ChangeHistoryService;
	class LuaSourceContainer;
}

class RobloxMainWindow;
class WebDialog;
class IRobloxDoc;
class InsertAdvancedObjectDialog;


class GroupSelectionVerb : public ARL::EditSelectionVerb
{
private:
	typedef ARL::EditSelectionVerb Super;

public:
	GroupSelectionVerb(ARL::DataModel* dataModel);
	virtual bool isEnabled() const;
	virtual void doIt(ARL::IDataState* dataState);
};

class UngroupSelectionVerb : public ARL::EditSelectionVerb
{
private:
	typedef ARL::EditSelectionVerb Super;

public:
	UngroupSelectionVerb(ARL::DataModel* dataModel);
	virtual bool isEnabled() const;
	virtual void doIt(ARL::IDataState* dataState);
};

//////////////////////////////////////////////////////////////////////////
// CSG Operations

class UnionSelectionVerb : public ARL::EditSelectionVerb
{
private:
	typedef ARL::EditSelectionVerb Super;

public:
	UnionSelectionVerb(ARL::DataModel* dataModel);

    void performUnion(ARL::IDataState* dataState);

	virtual bool isEnabled() const;
	virtual void doIt(ARL::IDataState* dataState);
};

class NegateSelectionVerb : public ARL::EditSelectionVerb
{
private:
	typedef ARL::EditSelectionVerb Super;

public:
	NegateSelectionVerb(ARL::DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(ARL::IDataState* dataState);
};

class SeparateSelectionVerb : public ARL::EditSelectionVerb
{
private:
	typedef ARL::EditSelectionVerb Super;

public:
	SeparateSelectionVerb(ARL::DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(ARL::IDataState* dataState);
    void performSeparate(ARL::IDataState* dataState);
};

class CutVerb : public ARL::DeleteSelectionVerb
{
public:
	CutVerb(ARL::DataModel* dataModel);
	virtual void doIt(ARL::IDataState* dataState);
};

class CopyVerb : public ARL::EditSelectionVerb
{
public:
	CopyVerb(ARL::DataModel* dataModel);
	virtual void doIt(ARL::IDataState* dataState);
};

class PasteVerb :public QObject, public ARL::Verb
{
	Q_OBJECT
public:
	PasteVerb(ARL::DataModel* dataModel, bool pasteInto);
	virtual bool isEnabled() const;
	virtual void doIt(ARL::IDataState* dataState);

private Q_SLOTS:
    void onClipboardModified();
	
private:
	void createInstancesFromClipboard(shared_ptr<ARL::Instances> items);
	void insertInstancesIntoParent(shared_ptr<ARL::Instances> items);
	void createInstancesFromClipboardDep(ARL::Instances& itemsToPaste);
	void insertInstancesIntoParentDep(ARL::Instances& itemsToPaste);
	
	bool isPasteInfoAvailable() const;

	ARL::DataModel *m_pDataModel;
	const bool      m_bPasteInto;
    bool            m_bIsPasteInfoAvailable;
};

class DuplicateSelectionVerb :public QObject, public ARL::EditSelectionVerb
{
	Q_OBJECT
public:
	DuplicateSelectionVerb(ARL::DataModel* dataModel);
	virtual void doIt(ARL::IDataState* dataState);
};

class UndoVerb: public ARL::Verb
{
public:
	UndoVerb(ARL::VerbContainer* pVerbContainer);
	void doIt(ARL::IDataState* dataState);

	virtual bool isChecked() const 
	{ return false; }

	virtual bool isEnabled() const;
	virtual std::string getText() const;
private:
	ARL::DataModel*                              m_pDataModel;
	boost::shared_ptr<ARL::ChangeHistoryService> m_pChangeHistory;
};

class RedoVerb: public ARL::Verb
{
public:
	RedoVerb(ARL::VerbContainer* pVerbContainer);
	void doIt(ARL::IDataState* dataState);

	virtual bool isChecked() const
	{ return false; }

	virtual bool isEnabled() const;
	virtual std::string getText() const;
private:
	ARL::DataModel*                              m_pDataModel;
	boost::shared_ptr<ARL::ChangeHistoryService> m_pChangeHistory;
};

class InsertModelVerb:public ARL::Verb
{
public:
	InsertModelVerb(ARL::VerbContainer* pVerbContainer, bool insertInto);
	void doIt(ARL::IDataState* dataState);

	virtual bool isChecked() const
	{ return false; }

	virtual bool isEnabled() const { return true; };

private:
	void insertModel();
	
private:
	ARL::DataModel *m_pDataModel;
	const bool      m_bInsertInto;
};

class SelectionSaveToFileVerb:public ARL::Verb
{
public:
	SelectionSaveToFileVerb(ARL::VerbContainer* pVerbContainer);

	virtual void doIt(ARL::IDataState* dataState);
	virtual bool isEnabled() const;
	
private:
	void saveToFile();
	std::auto_ptr<XmlElement> writeSelection();

	ARL::DataModel *m_pDataModel;
};

class PublishToRobloxAsVerb: public ARL::Verb
{
public:
	PublishToRobloxAsVerb(ARL::VerbContainer* pVerbContainer, RobloxMainWindow* mainWnd);
    virtual ~PublishToRobloxAsVerb();

	void doIt(ARL::IDataState* dataState);

	virtual bool isChecked() const
	{ return false; }

	virtual bool isEnabled() const;
	void initDialog();

	QDialog* getPublishDialog();
private:
	ARL::DataModel *m_pDataModel;
	RobloxMainWindow *m_pMainWindow;
	WebDialog *m_dlg;
};

class PublishToRobloxVerb : public ARL::Verb
{
public:
	PublishToRobloxVerb(ARL::VerbContainer* pVerbContainer, RobloxMainWindow* mainWnd);
	
	virtual void doIt(ARL::IDataState* dataState);
	virtual bool isChecked() const { return false; }
	virtual bool isEnabled() const;
	void onEventPublishingFinished();

private:

    void save(ARL::ContentId contentID,bool* outError,QString* outErrorTitle,QString* outErrorText);

	bool m_bIsPublishInProcess;
	ARL::DataModel *m_pDataModel;
	RobloxMainWindow *m_pMainWindow;
	QMutex publishingMutex;
};

class PublishSelectionToRobloxVerb: public ARL::Verb
{
public:
	PublishSelectionToRobloxVerb(ARL::VerbContainer* pVerbContainer, RobloxMainWindow* mainWnd);
    virtual ~PublishSelectionToRobloxVerb();

	void doIt(ARL::IDataState* dataState);
	
	virtual bool isChecked() const
	{ return false; }
	
	virtual bool isEnabled() const;
	
private:
	ARL::DataModel *m_pDataModel;
	RobloxMainWindow *m_pMainWindow;
	WebDialog* m_dlg;
};

class CreateNewLinkedSourceVerb : public QObject, public ARL::Verb
{
	Q_OBJECT
public:
	CreateNewLinkedSourceVerb(ARL::DataModel* pVerbContainer);

	virtual bool isEnabled() const;

	virtual void doIt(ARL::IDataState* dataState);
	
private:
	ARL::DataModel *m_pDataModel;

	shared_ptr<ARL::LuaSourceContainer> getLuaSourceContainer() const;
	void doItThread(std::string source, int currentGameId, boost::optional<int> groupId, QString name, bool* success);
};

class PublishAsPluginVerb: public ARL::Verb
{
public:
	PublishAsPluginVerb(ARL::VerbContainer* pVerbContainer, RobloxMainWindow* mainWnd);

	virtual void doIt(ARL::IDataState* dataState);

	virtual bool isEnabled() const;
	
private:
	ARL::DataModel *m_pDataModel;
	RobloxMainWindow *m_pMainWindow;
	boost::scoped_ptr<WebDialog> m_dlg;
};

class LaunchInstancesVerb: public ARL::Verb
{
public:
	enum SimulationType
	{
		PLAYSOLO = 1,
		SERVERONEPLAYER = 2,
		SERVERFOURPLAYERS = 5
	};

	LaunchInstancesVerb(ARL::VerbContainer* pVerbContainer);
	void doIt(ARL::IDataState* dataState);

	virtual bool isEnabled() const { return true; }


private:	
	ARL::VerbContainer *m_pVerbContainer;
};

class StartServerAndPlayerVerb: public ARL::Verb
{
public:
	StartServerAndPlayerVerb(ARL::VerbContainer* pVerbContainer);
	void doIt(ARL::IDataState* dataState);

	void launchPlayers(int numPlayers);

private:
	static void launchStudioInstances(ARL::VerbContainer *pVerbContainer, int numPlayers);
	ARL::VerbContainer *m_pVerbContainer;
};

class ServerPlayersStateInitVerb: public ARL::Verb
{
public:
	ServerPlayersStateInitVerb(ARL::VerbContainer* pVerbContainer);
	void doIt(ARL::IDataState* dataState);

	virtual bool isChecked() const;
};

class CreatePluginVerb: public ARL::Verb
{
public:
	CreatePluginVerb(ARL::VerbContainer* pVerbContainer);
	void doIt(ARL::IDataState* dataState);

	virtual bool isEnabled() const { return true; }

private:	
	ARL::VerbContainer *m_pVerbContainer;
};

class PairRbxDevVerb: public ARL::Verb
{
public:
	PairRbxDevVerb(ARL::VerbContainer* pVerbContainer, QWidget* newParent);
	void doIt(ARL::IDataState* dataState);

	virtual bool isEnabled() const { return true; }

private:
	shared_ptr<PairRbxDeviceDialog> dialog;
	ARL::DataModel *m_pDataModel;
};

class ManageEmulationDevVerb: public ARL::Verb
{
public:
	ManageEmulationDevVerb(ARL::VerbContainer* pVerbContainer, QWidget* newParent);
	void doIt(ARL::IDataState* dataState);

	virtual bool isEnabled() const { return true; }

private:
	shared_ptr<ManageEmulationDeviceDialog> dialog;
	ARL::DataModel *m_pDataModel;
};

class AudioToggleVerb: public ARL::Verb
{
public:
	AudioToggleVerb(ARL::VerbContainer* pVerbContainer);
	void doIt(ARL::IDataState* dataState);

	virtual bool isEnabled() const { return true; }
	virtual bool isChecked() const;

private:
	ARL::DataModel *m_pDataModel;
};

class AnalyzePhysicsToggleVerb: public ARL::Verb
{
public:
    AnalyzePhysicsToggleVerb(ARL::DataModel*);
    virtual void doIt(ARL::IDataState* dataState);

    void startAnalyze();
    void stopAnalyze();

    virtual bool isEnabled() const;
    virtual bool isChecked() const;

private:
    ARL::DataModel* m_pDataModel; 
};

class PlaySoloVerb: public ARL::Verb
{

public:
	PlaySoloVerb(ARL::VerbContainer* pVerbContainer);
	void doIt(ARL::IDataState* dataState);
	
	virtual bool isEnabled() const { return true; }
		
	
private:	
	ARL::DataModel *m_pDataModel;
};

class StartServerVerb: public ARL::Verb
{
	
public:
	StartServerVerb(ARL::VerbContainer* pVerbContainer);
	void doIt(ARL::IDataState* dataState);
	
	virtual bool isEnabled() const { return true; }
	
	
private:	
	ARL::DataModel *m_pDataModel;
};

class StartPlayerVerb: public ARL::Verb
{
	
public:
	StartPlayerVerb(ARL::VerbContainer* pVerbContainer);
	void doIt(ARL::IDataState* dataState);
	
	virtual bool isEnabled() const { return true; }
	
	
private:	
	ARL::DataModel *m_pDataModel;
};

class ToggleFullscreenVerb : public ARL::Verb
{
public:
	ToggleFullscreenVerb(ARL::VerbContainer* container);
	virtual void doIt(ARL::IDataState* dataState);
	virtual bool isEnabled() const;
};

class ShutdownClientVerb : public ARL::Verb
{
public:
	ShutdownClientVerb(ARL::VerbContainer* container, IRobloxDoc *pDoc);
	virtual void doIt(ARL::IDataState* dataState);
	
private:
	IRobloxDoc *m_pIDEDoc;
};

class ShutdownClientAndSaveVerb : public ARL::Verb
{
public:
	ShutdownClientAndSaveVerb(ARL::VerbContainer* container, IRobloxDoc* pDoc);
	virtual void doIt(ARL::IDataState* dataState);
	
private:
	IRobloxDoc* m_pIDEDoc;
};

class LeaveGameVerb : public ARL::Verb
{
public:
	LeaveGameVerb(ARL::VerbContainer* container, IRobloxDoc *pDoc);
	virtual void doIt(ARL::IDataState* dataState);
	
private:
	IRobloxDoc *m_pIDEDoc;
};



class ToggleAxisWidgetVerb : public ARL::Verb
{
public:
	ToggleAxisWidgetVerb(ARL::DataModel*);
	virtual void doIt(ARL::IDataState* dataState);
	virtual bool isEnabled() const  { return true; }
	virtual bool isChecked() const;
private:
	ARL::DataModel *m_pDataModel;
};

class Toggle3DGridVerb : public ARL::Verb
{
public:
	Toggle3DGridVerb(ARL::DataModel*);
	virtual void doIt(ARL::IDataState* dataState);
	virtual bool isEnabled() const;
	virtual bool isChecked() const;
private:
	ARL::DataModel *m_pDataModel;
};

class ToggleCollisionCheckVerb : public ARL::Verb
{
public:
	ToggleCollisionCheckVerb(ARL::DataModel*);
	virtual void doIt(ARL::IDataState* dataState);
	virtual bool isEnabled() const { return true; }
	virtual bool isChecked() const;
};

class ToggleLocalSpaceVerb : public ARL::Verb
{
public:
	ToggleLocalSpaceVerb(ARL::DataModel*);
	virtual void doIt(ARL::IDataState* dataState);
	virtual bool isEnabled() const { return true; }
	virtual bool isChecked() const;
private:
	ARL::DataModel* m_pDataModel;
};

class ScreenshotVerb : public QObject, public ARL::Verb
{
	Q_OBJECT
public:
	ScreenshotVerb(ARL::DataModel* pDataModel);

	virtual void doIt(ARL::IDataState*);
	virtual bool isEnabled() const;

	static void DoPostImage(shared_ptr<ARL::DataModel> spDataModel, const QString &fileName, const QString &seoStr);

	void reconnectScreenshotSignal();

private Q_SLOTS:
	void showPostImageWebDialog();
	void copyImageToClipboard();

private:
	void onScreenshotFinished(const std::string &filename);
	void onUploadSignal(bool finished);
	QString getSEOStr();


	static void PostImageFinished(std::string *pResponse, std::exception *pException, weak_ptr<ARL::DataModel> pWeakDataModel);
	static void showMessage(shared_ptr<ARL::DataModel> pWeakDataModel, const char* message);

	boost::shared_ptr<ARL::DataModel>  m_spDataModel;
	QString                            m_fileToUpload;
};

class InsertAdvancedObjectViewVerb: public ARL::Verb
{
public:
	InsertAdvancedObjectViewVerb(ARL::VerbContainer* pVerbContainer);
	virtual ~InsertAdvancedObjectViewVerb();

	void doIt(ARL::IDataState* dataState);
	virtual bool isEnabled() const;
	virtual bool isChecked() const;
};

class JointToolHelpDialogVerb: public ARL::Verb
{
    
public:
	JointToolHelpDialogVerb(ARL::VerbContainer* pVerbContainer);
	void doIt(ARL::IDataState* dataState);
    
	virtual bool isEnabled() const { return true; }
	virtual bool isChecked() const { return false; }
};

class StudioMaterialVerb: public ARL::MaterialVerb
{
public:
	StudioMaterialVerb(ARL::DataModel* dataModel);
	void doIt(ARL::IDataState* dataState);

	virtual bool isEnabled() const { return true; }
	virtual bool isChecked() const;

	static bool sMaterialActionActAsTool;
};

class StudioColorVerb: public ARL::ColorVerb
{
public:
	StudioColorVerb(ARL::DataModel* dataModel);
	void doIt(ARL::IDataState* dataState);

	virtual bool isEnabled() const { return true; }
	virtual bool isChecked() const;

	static bool sColorActionActAsTool;

private:
	void addColorToIcon();
};

class OpenToolBoxWithOptionsVerb: public QObject, public ARL::Verb
{
	Q_OBJECT
public:
	OpenToolBoxWithOptionsVerb(ARL::VerbContainer* pVerbContainer);
	~OpenToolBoxWithOptionsVerb();

	void doIt(ARL::IDataState* dataState);
	virtual bool isEnabled();

private Q_SLOTS:
	void handleDockVisibilityChanged(bool isVisible);
};

class InsertBasicObjectVerb: public ARL::Verb
{
public:
	InsertBasicObjectVerb(ARL::DataModel* dataModel);
	void doIt(ARL::IDataState* dataState);

	virtual bool isEnabled();

private:
	ARL::DataModel *m_pDataModel;
};

class JointCreationModeVerb: public ARL::Verb
{
public:
	JointCreationModeVerb(ARL::DataModel* dataModel);
	void doIt(ARL::IDataState* dataState);	

private:
	void updateMenuIcon();
	void updateMenuActions();
};

#ifdef Q_WS_WIN32   // must be Q_WS_WIN32 so moc picks it up

class RecordToggleVerb:public QObject, public ARL::Verb
{
	Q_OBJECT
public:
	RecordToggleVerb(ARL::DataModel*, ARL::ViewBase* pViewGfx);
	~RecordToggleVerb();

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual bool isSelected() const;
	virtual void doIt(ARL::IDataState* dataState);

	void startRecording();
	void stopRecording(bool showUploadDialog = true);
	bool isRecording() const;

private Q_SLOTS:
	void uploadVideo();

private:
	void action();

	boost::scoped_ptr<ARL::VideoControl> m_pVideoControl;
	boost::scoped_ptr<boost::thread>     m_helperThread;

	ARL::DataModel                      *m_pDataModel; 
	boost::function<void()>              m_job;

	ARL::CEvent                          m_jobWait;
	ARL::CEvent                          m_jobDone;
	ARL::CEvent                          m_threadDone;

	bool                                 m_bStop;
	bool                                 m_bIsBusy;
};

#endif

class ExportSelectionVerb:public QObject, public ARL::Verb
{
	Q_OBJECT
public:
	ExportSelectionVerb(ARL::DataModel*);

	virtual bool isEnabled() const { return (RobloxDocManager::Instance().getPlayDoc() != NULL); }
	virtual bool isChecked() const { return false; }
	virtual bool isSelected() const { return false; }
	virtual void doIt(ARL::IDataState* dataState);

private:
	ARL::DataModel                      *m_pDataModel;
};

class ExportPlaceVerb : public QObject, public ARL::Verb
{
	Q_OBJECT
public:
	ExportPlaceVerb(ARL::DataModel*);

	virtual bool isEnabled() const { return (RobloxDocManager::Instance().getPlayDoc() != NULL); }
	virtual bool isChecked() const { return false; }
	virtual bool isSelected() const { return false; }
	virtual void doIt(ARL::IDataState* dataState);

private:
	ARL::DataModel						*m_pDataModel;
};

class LaunchHelpForSelectionVerb : public ARL::Verb
{
public:
	LaunchHelpForSelectionVerb(ARL::DataModel*);

	virtual bool isEnabled() const;
	virtual void doIt(ARL::IDataState* dataState);

private:
	ARL::DataModel						*m_pDataModel;
};

class DummyVerb : public ARL::Verb {
public:
	DummyVerb(ARL::VerbContainer* container, const char* name)
	:Verb(container, name)
	{
	}
	virtual bool isEnabled() const {return false;}
	
	virtual void doIt(ARL::IDataState* dataState) {}
};
