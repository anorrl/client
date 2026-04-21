/********************************************************************************
** Form generated from reading UI file 'ARLMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARLMAINWINDOW_H
#define UI_ARLMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ARLMainWindow
{
public:
    QAction *fileOpenAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *deleteSelectedAction;
    QAction *selectAllAction;
    QAction *lockAction;
    QAction *anchorAction;
    QAction *advTranslateAction;
    QAction *advRotateAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *tiltUpAction;
    QAction *tiltDownAction;
    QAction *panRightAction;
    QAction *panLeftAction;
    QAction *glueSurfaceAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *smoothSurfaceAction;
    QAction *weldSurfaceAction;
    QAction *studsAction;
    QAction *inletAction;
    QAction *universalsAction;
    QAction *hingeAction;
    QAction *smoothNoOutlinesAction;
    QAction *groupSelectionAction;
    QAction *ungroupSelectionAction;
    QAction *moveUpBrickAction;
    QAction *moveDownBrickAction;
    QAction *rotateSelectionAction;
    QAction *tiltSelectionAction;
    QAction *simulationPlayAction;
    QAction *simulationRunAction;
    QAction *simulationStopAction;
    QAction *testStatsAction;
    QAction *testRenderStatsAction;
    QAction *testNetworkStatsAction;
    QAction *testPhysicsStatsAction;
    QAction *testSummaryStatsAction;
    QAction *testCustomStatsAction;
    QAction *simulationResetAction;
    QAction *advanceJointCreationManualAction;
    QAction *gridToOneAction;
    QAction *gridToOneFifthAction;
    QAction *gridToOffAction;
    QAction *resizeAction;
    QAction *motorRightAction;
    QAction *actionFillColor;
    QAction *actionMaterial;
    QAction *dropperAction;
    QAction *fileSaveAction;
    QAction *executeScriptAction;
    QAction *fileCloseAction;
    QAction *fileSaveAsAction;
    QAction *fileNewAction;
    QAction *fileExitAction;
    QAction *findAction;
    QAction *zoomExtentsAction;
    QAction *viewBasicObjectsAction;
    QAction *insertModelAction;
    QAction *pasteIntoAction;
    QAction *selectionSaveToFileAction;
    QAction *insertIntoFileAction;
    QAction *publishToRobloxAction;
    QAction *publishGameAction;
    QAction *aboutRobloxAction;
    QAction *onlineHelpAction;
    QAction *objectBrowserAction;
    QAction *playSoloAction;
    QAction *startServerAction;
    QAction *startPlayerAction;
    QAction *instanceDumpAction;
    QAction *settingsAction;
    QAction *publishSelectionToRobloxAction;
    QAction *findNextAction;
    QAction *replaceAction;
    QAction *advArrowToolAction;
    QAction *saveToRobloxAction;
    QAction *insertServiceAction;
    QAction *fastLogDumpAction;
    QAction *viewToolboxAction;
    QAction *toggleAxisWidgetAction;
    QAction *toggle3DGridAction;
    QAction *toggleVideoRecordAction;
    QAction *filePublishedProjectsAction;
    QAction *viewDiagnosticsAction;
    QAction *viewTaskSchedulerAction;
    QAction *viewScriptPerformanceAction;
    QAction *viewObjectExplorerAction;
    QAction *viewPropertiesAction;
    QAction *viewOutputWindowAction;
    QAction *actionStartPage;
    QAction *actionFullScreen;
    QAction *commentSelectionAction;
    QAction *uncommentSelectionAction;
    QAction *toggleCommentAction;
    QAction *toggleCollisionCheckAction;
    QAction *toggleBuildModeAction;
    QAction *openPluginsFolderAction;
    QAction *unlockAllAction;
    QAction *shortcutHelpAction;
    QAction *findPreviousAction;
    QAction *goToScriptErrorAction;
    QAction *expandAllFoldsAction;
    QAction *collapseAllFoldsAction;
    QAction *renameObjectAction;
    QAction *screenShotAction;
    QAction *publishToRobloxAsAction;
    QAction *selectChildrenAction;
    QAction *testClearStatsAction;
    QAction *stepIntoAction;
    QAction *stepOverAction;
    QAction *stepOutAction;
    QAction *insertBreakpointAction;
    QAction *deleteBreakpointAction;
    QAction *toggleBreakpointStateAction;
    QAction *addWatchAction;
    QAction *resetScriptZoomAction;
    QAction *viewContextualHelpAction;
    QAction *toggleLocalSpaceAction;
    QAction *resetViewAction;
    QAction *gridSizeToTwoAction;
    QAction *gridSizeToFourAction;
    QAction *gridSizeToSixteenAction;
    QAction *publishAsPluginAction;
    QAction *createNewLinkedSourceAction;
    QAction *managePluginsAction;
    QAction *quickInsertAction;
    QAction *reloadScriptAction;
    QAction *neverBreakOnScriptErrorsAction;
    QAction *breakOnAllScriptErrorsAction;
    QAction *breakOnUnhandledScriptErrorsAction;
    QAction *viewFindResultsWindowAction;
    QAction *findInScriptsAction;
    QAction *explorerFilterAction;
    QAction *exportSelectionAction;
    QAction *exportPlaceAction;
    QAction *toggleAllBreakpointsStateAction;
    QAction *duplicateSelectionAction;
    QAction *gameExplorerAction;
    QAction *customizeQuickAccessAction;
    QAction *unionSelectionAction;
    QAction *negateSelectionAction;
    QAction *separateSelectionAction;
    QAction *cleanupServersAndPlayersAction;
    QAction *playRbxDeviceAction;
    QAction *pairRbxDeviceAction;
    QAction *audioToggleAction;
    QAction *viewTutorialsAction;
    QAction *viewScriptAnalysisAction;
    QAction *viewTeamCreateAction;
    QAction *emulateDeviceAction;
    QAction *manageEmulationDeviceAction;
    QAction *launchHelpForSelectionAction;
    QAction *viewCommandBarAction;
    QAction *fileOpenRecentSavesAction;
    QAction *goToLineAction;
    QAction *downloadPlaceCopyAction;
    QWidget *defaultCentralWidget;
    QStatusBar *statusbar;
    QToolBar *standardToolBar;
    QToolBar *runToolBar;
    QToolBar *advToolsToolBar;
    QToolBar *oldToolsToolBar;
    QToolBar *editCameraToolBar;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuFind_and_Replace;
    QMenu *menuTools;
    QMenu *menuTest;
    QMenu *menuStats;
    QMenu *menuView;
    QMenu *menuToolBars;
    QMenu *menuFormat;
    QMenu *menuInsert;
    QMenu *menuWindow;
    QMenu *menuHelp;
    QMenu *scriptMenu;
    QMenu *breakErrorsIntoDebuggerMenu;
    QDockWidget *outputWindow;
    QWidget *dockWidgetContents_2;
    QGridLayout *gridLayout;
    QToolBar *commandToolBar;
    QDockWidget *objectExplorer;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_3;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_2;
    QDockWidget *propertyBrowser;
    QWidget *dockWidgetContents_3;
    QDockWidget *diagnosticsDockWidget;
    QWidget *dockWidgetContents_4;
    QDockWidget *taskSchedulerDockWidget;
    QWidget *dockWidgetContents_6;
    QDockWidget *toolBoxDockWidget;
    QWidget *dockWidgetContents_5;
    QDockWidget *scriptReviewDockWidget;
    QWidget *dockWidgetContents_7;
    QDockWidget *gameExplorerDockWidget;
    QWidget *dockWidgetContents_11;
    QToolBar *viewToolsToolBar;
    QDockWidget *basicObjectsDockWidget;
    QWidget *dockWidgetContents_8;
    QDockWidget *contextualHelp;
    QWidget *dockWidgetContents_9;
    QDockWidget *findResultsWindow;
    QWidget *dockWidgetContents_10;
    QGridLayout *gridLayout_2;
    QDockWidget *tutorialsDockWidget;
    QWidget *dockWidgetContents_12;
    QDockWidget *teamCreateDockWidget;
    QWidget *dockWidgetContents_13;
    QDockWidget *scriptAnalysisDockWidget;

    void setupUi(QMainWindow *ARLMainWindow)
    {
        if (ARLMainWindow->objectName().isEmpty())
            ARLMainWindow->setObjectName(QString::fromUtf8("ARLMainWindow"));
        ARLMainWindow->resize(1280, 765);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/ANORRLStudio.png"), QSize(), QIcon::Normal, QIcon::Off);
        ARLMainWindow->setWindowIcon(icon);
        ARLMainWindow->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        ARLMainWindow->setUnifiedTitleAndToolBarOnMac(false);
        fileOpenAction = new QAction(ARLMainWindow);
        fileOpenAction->setObjectName(QString::fromUtf8("fileOpenAction"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/open_document_16_h.png"), QSize(), QIcon::Normal, QIcon::Off);
        fileOpenAction->setIcon(icon1);
        undoAction = new QAction(ARLMainWindow);
        undoAction->setObjectName(QString::fromUtf8("undoAction"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/undo_16_h.png"), QSize(), QIcon::Normal, QIcon::Off);
        undoAction->setIcon(icon2);
        redoAction = new QAction(ARLMainWindow);
        redoAction->setObjectName(QString::fromUtf8("redoAction"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/redo_16_h.png"), QSize(), QIcon::Normal, QIcon::Off);
        redoAction->setIcon(icon3);
        deleteSelectedAction = new QAction(ARLMainWindow);
        deleteSelectedAction->setObjectName(QString::fromUtf8("deleteSelectedAction"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/delete_x_16_h.png"), QSize(), QIcon::Normal, QIcon::Off);
        deleteSelectedAction->setIcon(icon4);
        selectAllAction = new QAction(ARLMainWindow);
        selectAllAction->setObjectName(QString::fromUtf8("selectAllAction"));
        lockAction = new QAction(ARLMainWindow);
        lockAction->setObjectName(QString::fromUtf8("lockAction"));
        lockAction->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/lock.png"), QSize(), QIcon::Normal, QIcon::Off);
        lockAction->setIcon(icon5);
        anchorAction = new QAction(ARLMainWindow);
        anchorAction->setObjectName(QString::fromUtf8("anchorAction"));
        anchorAction->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/t_anchor_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        anchorAction->setIcon(icon6);
        advTranslateAction = new QAction(ARLMainWindow);
        advTranslateAction->setObjectName(QString::fromUtf8("advTranslateAction"));
        advTranslateAction->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/move-parts.png"), QSize(), QIcon::Normal, QIcon::Off);
        advTranslateAction->setIcon(icon7);
        advTranslateAction->setShortcutContext(Qt::ApplicationShortcut);
        advRotateAction = new QAction(ARLMainWindow);
        advRotateAction->setObjectName(QString::fromUtf8("advRotateAction"));
        advRotateAction->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/advT_rotateGrid_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        advRotateAction->setIcon(icon8);
        advRotateAction->setShortcutContext(Qt::ApplicationShortcut);
        zoomInAction = new QAction(ARLMainWindow);
        zoomInAction->setObjectName(QString::fromUtf8("zoomInAction"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/c_zoomIn_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        zoomInAction->setIcon(icon9);
        zoomOutAction = new QAction(ARLMainWindow);
        zoomOutAction->setObjectName(QString::fromUtf8("zoomOutAction"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/c_zoomOut_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        zoomOutAction->setIcon(icon10);
        tiltUpAction = new QAction(ARLMainWindow);
        tiltUpAction->setObjectName(QString::fromUtf8("tiltUpAction"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/images/c_tiltUpCamera_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        tiltUpAction->setIcon(icon11);
        tiltDownAction = new QAction(ARLMainWindow);
        tiltDownAction->setObjectName(QString::fromUtf8("tiltDownAction"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/images/c_tiltDownCamera_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        tiltDownAction->setIcon(icon12);
        panRightAction = new QAction(ARLMainWindow);
        panRightAction->setObjectName(QString::fromUtf8("panRightAction"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/images/c_panRight_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        panRightAction->setIcon(icon13);
        panLeftAction = new QAction(ARLMainWindow);
        panLeftAction->setObjectName(QString::fromUtf8("panLeftAction"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/images/c_panLeft_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        panLeftAction->setIcon(icon14);
        glueSurfaceAction = new QAction(ARLMainWindow);
        glueSurfaceAction->setObjectName(QString::fromUtf8("glueSurfaceAction"));
        glueSurfaceAction->setCheckable(true);
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/glue-surface.png"), QSize(), QIcon::Normal, QIcon::Off);
        glueSurfaceAction->setIcon(icon15);
        cutAction = new QAction(ARLMainWindow);
        cutAction->setObjectName(QString::fromUtf8("cutAction"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/cut_clipboard_16_h.png"), QSize(), QIcon::Normal, QIcon::Off);
        cutAction->setIcon(icon16);
        copyAction = new QAction(ARLMainWindow);
        copyAction->setObjectName(QString::fromUtf8("copyAction"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/copy_clipboard_16_h.png"), QSize(), QIcon::Normal, QIcon::Off);
        copyAction->setIcon(icon17);
        pasteAction = new QAction(ARLMainWindow);
        pasteAction->setObjectName(QString::fromUtf8("pasteAction"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/paste_clipboard_16_h.png"), QSize(), QIcon::Normal, QIcon::Off);
        pasteAction->setIcon(icon18);
        smoothSurfaceAction = new QAction(ARLMainWindow);
        smoothSurfaceAction->setObjectName(QString::fromUtf8("smoothSurfaceAction"));
        smoothSurfaceAction->setCheckable(true);
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/smooth-surface.png"), QSize(), QIcon::Normal, QIcon::Off);
        smoothSurfaceAction->setIcon(icon19);
        weldSurfaceAction = new QAction(ARLMainWindow);
        weldSurfaceAction->setObjectName(QString::fromUtf8("weldSurfaceAction"));
        weldSurfaceAction->setCheckable(true);
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/weld-surface.png"), QSize(), QIcon::Normal, QIcon::Off);
        weldSurfaceAction->setIcon(icon20);
        studsAction = new QAction(ARLMainWindow);
        studsAction->setObjectName(QString::fromUtf8("studsAction"));
        studsAction->setCheckable(true);
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/connector-surface.png"), QSize(), QIcon::Normal, QIcon::Off);
        studsAction->setIcon(icon21);
        inletAction = new QAction(ARLMainWindow);
        inletAction->setObjectName(QString::fromUtf8("inletAction"));
        inletAction->setCheckable(true);
        QIcon icon22;
        icon22.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/indents-surface.png"), QSize(), QIcon::Normal, QIcon::Off);
        inletAction->setIcon(icon22);
        universalsAction = new QAction(ARLMainWindow);
        universalsAction->setObjectName(QString::fromUtf8("universalsAction"));
        universalsAction->setCheckable(true);
        QIcon icon23;
        icon23.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/universals-surface.png"), QSize(), QIcon::Normal, QIcon::Off);
        universalsAction->setIcon(icon23);
        hingeAction = new QAction(ARLMainWindow);
        hingeAction->setObjectName(QString::fromUtf8("hingeAction"));
        hingeAction->setCheckable(true);
        QIcon icon24;
        icon24.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/hinge-surface.png"), QSize(), QIcon::Normal, QIcon::Off);
        hingeAction->setIcon(icon24);
        smoothNoOutlinesAction = new QAction(ARLMainWindow);
        smoothNoOutlinesAction->setObjectName(QString::fromUtf8("smoothNoOutlinesAction"));
        smoothNoOutlinesAction->setCheckable(true);
        QIcon icon25;
        icon25.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/smooth-no-outlines-surface.png"), QSize(), QIcon::Normal, QIcon::Off);
        smoothNoOutlinesAction->setIcon(icon25);
        groupSelectionAction = new QAction(ARLMainWindow);
        groupSelectionAction->setObjectName(QString::fromUtf8("groupSelectionAction"));
        QIcon icon26;
        icon26.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/group_16.png"), QSize(), QIcon::Normal, QIcon::Off);
        groupSelectionAction->setIcon(icon26);
        ungroupSelectionAction = new QAction(ARLMainWindow);
        ungroupSelectionAction->setObjectName(QString::fromUtf8("ungroupSelectionAction"));
        QIcon icon27;
        icon27.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/ungroup_16.png"), QSize(), QIcon::Normal, QIcon::Off);
        ungroupSelectionAction->setIcon(icon27);
        moveUpBrickAction = new QAction(ARLMainWindow);
        moveUpBrickAction->setObjectName(QString::fromUtf8("moveUpBrickAction"));
        QIcon icon28;
        icon28.addFile(QString::fromUtf8(":/images/s_moveUp_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        moveUpBrickAction->setIcon(icon28);
        moveDownBrickAction = new QAction(ARLMainWindow);
        moveDownBrickAction->setObjectName(QString::fromUtf8("moveDownBrickAction"));
        QIcon icon29;
        icon29.addFile(QString::fromUtf8(":/images/s_moveDown_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        moveDownBrickAction->setIcon(icon29);
        rotateSelectionAction = new QAction(ARLMainWindow);
        rotateSelectionAction->setObjectName(QString::fromUtf8("rotateSelectionAction"));
        QIcon icon30;
        icon30.addFile(QString::fromUtf8(":/images/s_rotate_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        rotateSelectionAction->setIcon(icon30);
        tiltSelectionAction = new QAction(ARLMainWindow);
        tiltSelectionAction->setObjectName(QString::fromUtf8("tiltSelectionAction"));
        QIcon icon31;
        icon31.addFile(QString::fromUtf8(":/images/s_tilt_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        tiltSelectionAction->setIcon(icon31);
        simulationPlayAction = new QAction(ARLMainWindow);
        simulationPlayAction->setObjectName(QString::fromUtf8("simulationPlayAction"));
        simulationPlayAction->setCheckable(false);
        QIcon icon32;
        icon32.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/playsolo.png"), QSize(), QIcon::Normal, QIcon::Off);
        simulationPlayAction->setIcon(icon32);
        simulationPlayAction->setAutoRepeat(true);
        simulationRunAction = new QAction(ARLMainWindow);
        simulationRunAction->setObjectName(QString::fromUtf8("simulationRunAction"));
        simulationRunAction->setCheckable(false);
        QIcon icon33;
        icon33.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        simulationRunAction->setIcon(icon33);
        simulationRunAction->setAutoRepeat(true);
        simulationStopAction = new QAction(ARLMainWindow);
        simulationStopAction->setObjectName(QString::fromUtf8("simulationStopAction"));
        simulationStopAction->setCheckable(false);
        simulationStopAction->setEnabled(true);
        QIcon icon34;
        icon34.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        simulationStopAction->setIcon(icon34);
        simulationStopAction->setAutoRepeat(true);
        testStatsAction = new QAction(ARLMainWindow);
        testStatsAction->setObjectName(QString::fromUtf8("testStatsAction"));
        testStatsAction->setCheckable(true);
        testRenderStatsAction = new QAction(ARLMainWindow);
        testRenderStatsAction->setObjectName(QString::fromUtf8("testRenderStatsAction"));
        testRenderStatsAction->setCheckable(true);
        testNetworkStatsAction = new QAction(ARLMainWindow);
        testNetworkStatsAction->setObjectName(QString::fromUtf8("testNetworkStatsAction"));
        testNetworkStatsAction->setCheckable(true);
        testPhysicsStatsAction = new QAction(ARLMainWindow);
        testPhysicsStatsAction->setObjectName(QString::fromUtf8("testPhysicsStatsAction"));
        testPhysicsStatsAction->setCheckable(true);
        testSummaryStatsAction = new QAction(ARLMainWindow);
        testSummaryStatsAction->setObjectName(QString::fromUtf8("testSummaryStatsAction"));
        testSummaryStatsAction->setCheckable(true);
        testCustomStatsAction = new QAction(ARLMainWindow);
        testCustomStatsAction->setObjectName(QString::fromUtf8("testCustomStatsAction"));
        testCustomStatsAction->setCheckable(true);
        simulationResetAction = new QAction(ARLMainWindow);
        simulationResetAction->setObjectName(QString::fromUtf8("simulationResetAction"));
        QIcon icon35;
        icon35.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        simulationResetAction->setIcon(icon35);
        advanceJointCreationManualAction = new QAction(ARLMainWindow);
        advanceJointCreationManualAction->setObjectName(QString::fromUtf8("advanceJointCreationManualAction"));
        advanceJointCreationManualAction->setCheckable(true);
        QIcon icon36;
        icon36.addFile(QString::fromUtf8(":/images/advT_jointManual_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        advanceJointCreationManualAction->setIcon(icon36);
        advanceJointCreationManualAction->setProperty("disableForQuickAccess", QVariant(true));
        gridToOneAction = new QAction(ARLMainWindow);
        gridToOneAction->setObjectName(QString::fromUtf8("gridToOneAction"));
        gridToOneAction->setCheckable(true);
        QIcon icon37;
        icon37.addFile(QString::fromUtf8(":/images/advT_gridToOne_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        gridToOneAction->setIcon(icon37);
        gridToOneFifthAction = new QAction(ARLMainWindow);
        gridToOneFifthAction->setObjectName(QString::fromUtf8("gridToOneFifthAction"));
        gridToOneFifthAction->setCheckable(true);
        QIcon icon38;
        icon38.addFile(QString::fromUtf8(":/images/advT_gridToOneFifth_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        gridToOneFifthAction->setIcon(icon38);
        gridToOffAction = new QAction(ARLMainWindow);
        gridToOffAction->setObjectName(QString::fromUtf8("gridToOffAction"));
        gridToOffAction->setCheckable(true);
        QIcon icon39;
        icon39.addFile(QString::fromUtf8(":/images/advT_gridOff_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        gridToOffAction->setIcon(icon39);
        resizeAction = new QAction(ARLMainWindow);
        resizeAction->setObjectName(QString::fromUtf8("resizeAction"));
        resizeAction->setCheckable(true);
        QIcon icon40;
        icon40.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/resize.png"), QSize(), QIcon::Normal, QIcon::Off);
        resizeAction->setIcon(icon40);
        resizeAction->setShortcutContext(Qt::ApplicationShortcut);
        motorRightAction = new QAction(ARLMainWindow);
        motorRightAction->setObjectName(QString::fromUtf8("motorRightAction"));
        motorRightAction->setCheckable(true);
        QIcon icon41;
        icon41.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/motor-surface.png"), QSize(), QIcon::Normal, QIcon::Off);
        motorRightAction->setIcon(icon41);
        actionFillColor = new QAction(ARLMainWindow);
        actionFillColor->setObjectName(QString::fromUtf8("actionFillColor"));
        actionFillColor->setCheckable(true);
        QIcon icon42;
        icon42.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/paintcan.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFillColor->setIcon(icon42);
        actionFillColor->setProperty("disableForQuickAccess", QVariant(true));
        actionMaterial = new QAction(ARLMainWindow);
        actionMaterial->setObjectName(QString::fromUtf8("actionMaterial"));
        actionMaterial->setCheckable(true);
        QIcon icon43;
        icon43.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/material_picker_16.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMaterial->setIcon(icon43);
        actionMaterial->setAutoRepeat(false);
        dropperAction = new QAction(ARLMainWindow);
        dropperAction->setObjectName(QString::fromUtf8("dropperAction"));
        dropperAction->setCheckable(true);
        QIcon icon44;
        icon44.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/color-picker.png"), QSize(), QIcon::Normal, QIcon::Off);
        dropperAction->setIcon(icon44);
        fileSaveAction = new QAction(ARLMainWindow);
        fileSaveAction->setObjectName(QString::fromUtf8("fileSaveAction"));
        QIcon icon45;
        icon45.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/save_16_h.png"), QSize(), QIcon::Normal, QIcon::Off);
        fileSaveAction->setIcon(icon45);
        fileSaveAction->setAutoRepeat(true);
        executeScriptAction = new QAction(ARLMainWindow);
        executeScriptAction->setObjectName(QString::fromUtf8("executeScriptAction"));
        QIcon icon46;
        icon46.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/script_go.png"), QSize(), QIcon::Normal, QIcon::Off);
        executeScriptAction->setIcon(icon46);
        fileCloseAction = new QAction(ARLMainWindow);
        fileCloseAction->setObjectName(QString::fromUtf8("fileCloseAction"));
        fileCloseAction->setProperty("disableForQuickAccess", QVariant(true));
        fileSaveAsAction = new QAction(ARLMainWindow);
        fileSaveAsAction->setObjectName(QString::fromUtf8("fileSaveAsAction"));
        QIcon icon47;
        icon47.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/save_green_16_h.png"), QSize(), QIcon::Normal, QIcon::Off);
        fileSaveAsAction->setIcon(icon47);
        fileSaveAsAction->setAutoRepeat(false);
        fileNewAction = new QAction(ARLMainWindow);
        fileNewAction->setObjectName(QString::fromUtf8("fileNewAction"));
        QIcon icon48;
        icon48.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/new_document_16_h.png"), QSize(), QIcon::Normal, QIcon::Off);
        fileNewAction->setIcon(icon48);
        fileNewAction->setAutoRepeat(false);
        fileExitAction = new QAction(ARLMainWindow);
        fileExitAction->setObjectName(QString::fromUtf8("fileExitAction"));
        fileExitAction->setProperty("disableForQuickAccess", QVariant(true));
        findAction = new QAction(ARLMainWindow);
        findAction->setObjectName(QString::fromUtf8("findAction"));
        findAction->setPriority(QAction::HighPriority);
        zoomExtentsAction = new QAction(ARLMainWindow);
        zoomExtentsAction->setObjectName(QString::fromUtf8("zoomExtentsAction"));
        QIcon icon49;
        icon49.addFile(QString::fromUtf8(":/images/c_zoomExtent_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        zoomExtentsAction->setIcon(icon49);
        zoomExtentsAction->setShortcutContext(Qt::ApplicationShortcut);
        viewBasicObjectsAction = new QAction(ARLMainWindow);
        viewBasicObjectsAction->setObjectName(QString::fromUtf8("viewBasicObjectsAction"));
        viewBasicObjectsAction->setCheckable(true);
        QIcon icon50;
        icon50.addFile(QString::fromUtf8(":/images/TA_BasicObjects.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewBasicObjectsAction->setIcon(icon50);
        insertModelAction = new QAction(ARLMainWindow);
        insertModelAction->setObjectName(QString::fromUtf8("insertModelAction"));
        pasteIntoAction = new QAction(ARLMainWindow);
        pasteIntoAction->setObjectName(QString::fromUtf8("pasteIntoAction"));
        pasteIntoAction->setProperty("disableForQuickAccess", QVariant(true));
        selectionSaveToFileAction = new QAction(ARLMainWindow);
        selectionSaveToFileAction->setObjectName(QString::fromUtf8("selectionSaveToFileAction"));
        insertIntoFileAction = new QAction(ARLMainWindow);
        insertIntoFileAction->setObjectName(QString::fromUtf8("insertIntoFileAction"));
        publishToRobloxAction = new QAction(ARLMainWindow);
        publishToRobloxAction->setObjectName(QString::fromUtf8("publishToRobloxAction"));
        publishGameAction = new QAction(ARLMainWindow);
        publishGameAction->setObjectName(QString::fromUtf8("publishGameAction"));
        aboutRobloxAction = new QAction(ARLMainWindow);
        aboutRobloxAction->setObjectName(QString::fromUtf8("aboutRobloxAction"));
        onlineHelpAction = new QAction(ARLMainWindow);
        onlineHelpAction->setObjectName(QString::fromUtf8("onlineHelpAction"));
        objectBrowserAction = new QAction(ARLMainWindow);
        objectBrowserAction->setObjectName(QString::fromUtf8("objectBrowserAction"));
        objectBrowserAction->setCheckable(true);
        QIcon icon51;
        icon51.addFile(QString::fromUtf8(":/images/TA_ObjectBrowser.png"), QSize(), QIcon::Normal, QIcon::Off);
        objectBrowserAction->setIcon(icon51);
        playSoloAction = new QAction(ARLMainWindow);
        playSoloAction->setObjectName(QString::fromUtf8("playSoloAction"));
        startServerAction = new QAction(ARLMainWindow);
        startServerAction->setObjectName(QString::fromUtf8("startServerAction"));
        startServerAction->setProperty("disableForQuickAccess", QVariant(true));
        startPlayerAction = new QAction(ARLMainWindow);
        startPlayerAction->setObjectName(QString::fromUtf8("startPlayerAction"));
        startPlayerAction->setProperty("disableForQuickAccess", QVariant(true));
        instanceDumpAction = new QAction(ARLMainWindow);
        instanceDumpAction->setObjectName(QString::fromUtf8("instanceDumpAction"));
        settingsAction = new QAction(ARLMainWindow);
        settingsAction->setObjectName(QString::fromUtf8("settingsAction"));
        publishSelectionToRobloxAction = new QAction(ARLMainWindow);
        publishSelectionToRobloxAction->setObjectName(QString::fromUtf8("publishSelectionToRobloxAction"));
        findNextAction = new QAction(ARLMainWindow);
        findNextAction->setObjectName(QString::fromUtf8("findNextAction"));
        findNextAction->setPriority(QAction::HighPriority);
        replaceAction = new QAction(ARLMainWindow);
        replaceAction->setObjectName(QString::fromUtf8("replaceAction"));
        replaceAction->setPriority(QAction::HighPriority);
        advArrowToolAction = new QAction(ARLMainWindow);
        advArrowToolAction->setObjectName(QString::fromUtf8("advArrowToolAction"));
        advArrowToolAction->setCheckable(true);
        QIcon icon52;
        icon52.addFile(QString::fromUtf8(":/images/t_drag_old.png"), QSize(), QIcon::Normal, QIcon::Off);
        advArrowToolAction->setIcon(icon52);
        advArrowToolAction->setShortcutContext(Qt::ApplicationShortcut);
        saveToRobloxAction = new QAction(ARLMainWindow);
        saveToRobloxAction->setObjectName(QString::fromUtf8("saveToRobloxAction"));
        insertServiceAction = new QAction(ARLMainWindow);
        insertServiceAction->setObjectName(QString::fromUtf8("insertServiceAction"));
        fastLogDumpAction = new QAction(ARLMainWindow);
        fastLogDumpAction->setObjectName(QString::fromUtf8("fastLogDumpAction"));
        QIcon icon53;
        icon53.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/bug_error.png"), QSize(), QIcon::Normal, QIcon::Off);
        fastLogDumpAction->setIcon(icon53);
        viewToolboxAction = new QAction(ARLMainWindow);
        viewToolboxAction->setObjectName(QString::fromUtf8("viewToolboxAction"));
        viewToolboxAction->setCheckable(true);
        QIcon icon54;
        icon54.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/toolbox.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewToolboxAction->setIcon(icon54);
        toggleAxisWidgetAction = new QAction(ARLMainWindow);
        toggleAxisWidgetAction->setObjectName(QString::fromUtf8("toggleAxisWidgetAction"));
        toggleAxisWidgetAction->setCheckable(true);
        QIcon icon55;
        icon55.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/axis_widget.png"), QSize(), QIcon::Normal, QIcon::Off);
        toggleAxisWidgetAction->setIcon(icon55);
        toggle3DGridAction = new QAction(ARLMainWindow);
        toggle3DGridAction->setObjectName(QString::fromUtf8("toggle3DGridAction"));
        toggle3DGridAction->setCheckable(true);
        toggleVideoRecordAction = new QAction(ARLMainWindow);
        toggleVideoRecordAction->setObjectName(QString::fromUtf8("toggleVideoRecordAction"));
        toggleVideoRecordAction->setCheckable(true);
        QIcon icon56;
        icon56.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/Record-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toggleVideoRecordAction->setIcon(icon56);
        toggleVideoRecordAction->setAutoRepeat(false);
        filePublishedProjectsAction = new QAction(ARLMainWindow);
        filePublishedProjectsAction->setObjectName(QString::fromUtf8("filePublishedProjectsAction"));
        filePublishedProjectsAction->setIcon(icon1);
        viewDiagnosticsAction = new QAction(ARLMainWindow);
        viewDiagnosticsAction->setObjectName(QString::fromUtf8("viewDiagnosticsAction"));
        viewDiagnosticsAction->setCheckable(true);
        QIcon icon57;
        icon57.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/report.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewDiagnosticsAction->setIcon(icon57);
        viewTaskSchedulerAction = new QAction(ARLMainWindow);
        viewTaskSchedulerAction->setObjectName(QString::fromUtf8("viewTaskSchedulerAction"));
        viewTaskSchedulerAction->setCheckable(true);
        QIcon icon58;
        icon58.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/Task-Scheduler-color.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewTaskSchedulerAction->setIcon(icon58);
        viewScriptPerformanceAction = new QAction(ARLMainWindow);
        viewScriptPerformanceAction->setObjectName(QString::fromUtf8("viewScriptPerformanceAction"));
        viewScriptPerformanceAction->setCheckable(true);
        QIcon icon59;
        icon59.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/jobs.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewScriptPerformanceAction->setIcon(icon59);
        viewObjectExplorerAction = new QAction(ARLMainWindow);
        viewObjectExplorerAction->setObjectName(QString::fromUtf8("viewObjectExplorerAction"));
        viewObjectExplorerAction->setCheckable(true);
        QIcon icon60;
        icon60.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/explorer.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewObjectExplorerAction->setIcon(icon60);
        viewPropertiesAction = new QAction(ARLMainWindow);
        viewPropertiesAction->setObjectName(QString::fromUtf8("viewPropertiesAction"));
        viewPropertiesAction->setCheckable(true);
        QIcon icon61;
        icon61.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/table.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewPropertiesAction->setIcon(icon61);
        viewOutputWindowAction = new QAction(ARLMainWindow);
        viewOutputWindowAction->setObjectName(QString::fromUtf8("viewOutputWindowAction"));
        viewOutputWindowAction->setCheckable(true);
        QIcon icon62;
        icon62.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/output.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewOutputWindowAction->setIcon(icon62);
        actionStartPage = new QAction(ARLMainWindow);
        actionStartPage->setObjectName(QString::fromUtf8("actionStartPage"));
        actionStartPage->setCheckable(true);
        actionStartPage->setChecked(false);
        QIcon icon63;
        icon63.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/world.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStartPage->setIcon(icon63);
        actionStartPage->setIconVisibleInMenu(true);
        actionFullScreen = new QAction(ARLMainWindow);
        actionFullScreen->setObjectName(QString::fromUtf8("actionFullScreen"));
        actionFullScreen->setCheckable(true);
        QIcon icon64;
        icon64.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/fullscreen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFullScreen->setIcon(icon64);
        commentSelectionAction = new QAction(ARLMainWindow);
        commentSelectionAction->setObjectName(QString::fromUtf8("commentSelectionAction"));
        commentSelectionAction->setVisible(true);
        commentSelectionAction->setPriority(QAction::HighPriority);
        uncommentSelectionAction = new QAction(ARLMainWindow);
        uncommentSelectionAction->setObjectName(QString::fromUtf8("uncommentSelectionAction"));
        uncommentSelectionAction->setVisible(true);
        uncommentSelectionAction->setPriority(QAction::HighPriority);
        toggleCommentAction = new QAction(ARLMainWindow);
        toggleCommentAction->setObjectName(QString::fromUtf8("toggleCommentAction"));
        toggleCollisionCheckAction = new QAction(ARLMainWindow);
        toggleCollisionCheckAction->setObjectName(QString::fromUtf8("toggleCollisionCheckAction"));
        toggleCollisionCheckAction->setCheckable(true);
        QIcon icon65;
        icon65.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/no-collision-dragger.png"), QSize(), QIcon::Normal, QIcon::Off);
        toggleCollisionCheckAction->setIcon(icon65);
        toggleBuildModeAction = new QAction(ARLMainWindow);
        toggleBuildModeAction->setObjectName(QString::fromUtf8("toggleBuildModeAction"));
        openPluginsFolderAction = new QAction(ARLMainWindow);
        openPluginsFolderAction->setObjectName(QString::fromUtf8("openPluginsFolderAction"));
        openPluginsFolderAction->setIcon(icon1);
        unlockAllAction = new QAction(ARLMainWindow);
        unlockAllAction->setObjectName(QString::fromUtf8("unlockAllAction"));
        QIcon icon66;
        icon66.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/lock_open.png"), QSize(), QIcon::Normal, QIcon::Off);
        unlockAllAction->setIcon(icon66);
        shortcutHelpAction = new QAction(ARLMainWindow);
        shortcutHelpAction->setObjectName(QString::fromUtf8("shortcutHelpAction"));
        shortcutHelpAction->setProperty("disableForQuickAccess", QVariant(true));
        findPreviousAction = new QAction(ARLMainWindow);
        findPreviousAction->setObjectName(QString::fromUtf8("findPreviousAction"));
        findPreviousAction->setPriority(QAction::HighPriority);
        goToScriptErrorAction = new QAction(ARLMainWindow);
        goToScriptErrorAction->setObjectName(QString::fromUtf8("goToScriptErrorAction"));
        expandAllFoldsAction = new QAction(ARLMainWindow);
        expandAllFoldsAction->setObjectName(QString::fromUtf8("expandAllFoldsAction"));
        expandAllFoldsAction->setProperty("disableForQuickAccess", QVariant(true));
        collapseAllFoldsAction = new QAction(ARLMainWindow);
        collapseAllFoldsAction->setObjectName(QString::fromUtf8("collapseAllFoldsAction"));
        collapseAllFoldsAction->setProperty("disableForQuickAccess", QVariant(true));
        renameObjectAction = new QAction(ARLMainWindow);
        renameObjectAction->setObjectName(QString::fromUtf8("renameObjectAction"));
        screenShotAction = new QAction(ARLMainWindow);
        screenShotAction->setObjectName(QString::fromUtf8("screenShotAction"));
        QIcon icon67;
        icon67.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/picture-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        screenShotAction->setIcon(icon67);
        screenShotAction->setAutoRepeat(false);
        publishToRobloxAsAction = new QAction(ARLMainWindow);
        publishToRobloxAsAction->setObjectName(QString::fromUtf8("publishToRobloxAsAction"));
        selectChildrenAction = new QAction(ARLMainWindow);
        selectChildrenAction->setObjectName(QString::fromUtf8("selectChildrenAction"));
        testClearStatsAction = new QAction(ARLMainWindow);
        testClearStatsAction->setObjectName(QString::fromUtf8("testClearStatsAction"));
        stepIntoAction = new QAction(ARLMainWindow);
        stepIntoAction->setObjectName(QString::fromUtf8("stepIntoAction"));
        QIcon icon68;
        icon68.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/stepInto.png"), QSize(), QIcon::Normal, QIcon::Off);
        stepIntoAction->setIcon(icon68);
        stepOverAction = new QAction(ARLMainWindow);
        stepOverAction->setObjectName(QString::fromUtf8("stepOverAction"));
        QIcon icon69;
        icon69.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/stepOver.png"), QSize(), QIcon::Normal, QIcon::Off);
        stepOverAction->setIcon(icon69);
        stepOutAction = new QAction(ARLMainWindow);
        stepOutAction->setObjectName(QString::fromUtf8("stepOutAction"));
        QIcon icon70;
        icon70.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/stepOut.png"), QSize(), QIcon::Normal, QIcon::Off);
        stepOutAction->setIcon(icon70);
        insertBreakpointAction = new QAction(ARLMainWindow);
        insertBreakpointAction->setObjectName(QString::fromUtf8("insertBreakpointAction"));
        deleteBreakpointAction = new QAction(ARLMainWindow);
        deleteBreakpointAction->setObjectName(QString::fromUtf8("deleteBreakpointAction"));
        toggleBreakpointStateAction = new QAction(ARLMainWindow);
        toggleBreakpointStateAction->setObjectName(QString::fromUtf8("toggleBreakpointStateAction"));
        addWatchAction = new QAction(ARLMainWindow);
        addWatchAction->setObjectName(QString::fromUtf8("addWatchAction"));
        resetScriptZoomAction = new QAction(ARLMainWindow);
        resetScriptZoomAction->setObjectName(QString::fromUtf8("resetScriptZoomAction"));
        viewContextualHelpAction = new QAction(ARLMainWindow);
        viewContextualHelpAction->setObjectName(QString::fromUtf8("viewContextualHelpAction"));
        viewContextualHelpAction->setCheckable(true);
        QIcon icon71;
        icon71.addFile(QString::fromUtf8(":/RibbonBar/images/Studio Ribbon Icons/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewContextualHelpAction->setIcon(icon71);
        toggleLocalSpaceAction = new QAction(ARLMainWindow);
        toggleLocalSpaceAction->setObjectName(QString::fromUtf8("toggleLocalSpaceAction"));
        toggleLocalSpaceAction->setCheckable(true);
        toggleLocalSpaceAction->setShortcutContext(Qt::ApplicationShortcut);
        toggleLocalSpaceAction->setAutoRepeat(true);
        toggleLocalSpaceAction->setVisible(true);
        toggleLocalSpaceAction->setIconVisibleInMenu(false);
        resetViewAction = new QAction(ARLMainWindow);
        resetViewAction->setObjectName(QString::fromUtf8("resetViewAction"));
        QIcon icon72;
        icon72.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/reset_layout.png"), QSize(), QIcon::Normal, QIcon::Off);
        resetViewAction->setIcon(icon72);
        gridSizeToTwoAction = new QAction(ARLMainWindow);
        gridSizeToTwoAction->setObjectName(QString::fromUtf8("gridSizeToTwoAction"));
        QIcon icon73;
        icon73.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/grid_one.png"), QSize(), QIcon::Normal, QIcon::Off);
        gridSizeToTwoAction->setIcon(icon73);
        gridSizeToFourAction = new QAction(ARLMainWindow);
        gridSizeToFourAction->setObjectName(QString::fromUtf8("gridSizeToFourAction"));
        QIcon icon74;
        icon74.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/grid.png"), QSize(), QIcon::Normal, QIcon::Off);
        gridSizeToFourAction->setIcon(icon74);
        gridSizeToSixteenAction = new QAction(ARLMainWindow);
        gridSizeToSixteenAction->setObjectName(QString::fromUtf8("gridSizeToSixteenAction"));
        QIcon icon75;
        icon75.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/grid_sixteen.png"), QSize(), QIcon::Normal, QIcon::Off);
        gridSizeToSixteenAction->setIcon(icon75);
        publishAsPluginAction = new QAction(ARLMainWindow);
        publishAsPluginAction->setObjectName(QString::fromUtf8("publishAsPluginAction"));
        createNewLinkedSourceAction = new QAction(ARLMainWindow);
        createNewLinkedSourceAction->setObjectName(QString::fromUtf8("createNewLinkedSourceAction"));
        managePluginsAction = new QAction(ARLMainWindow);
        managePluginsAction->setObjectName(QString::fromUtf8("managePluginsAction"));
        quickInsertAction = new QAction(ARLMainWindow);
        quickInsertAction->setObjectName(QString::fromUtf8("quickInsertAction"));
        quickInsertAction->setProperty("disableForQuickAccess", QVariant(true));
        reloadScriptAction = new QAction(ARLMainWindow);
        reloadScriptAction->setObjectName(QString::fromUtf8("reloadScriptAction"));
        reloadScriptAction->setEnabled(false);
        reloadScriptAction->setAutoRepeat(false);
        neverBreakOnScriptErrorsAction = new QAction(ARLMainWindow);
        neverBreakOnScriptErrorsAction->setObjectName(QString::fromUtf8("neverBreakOnScriptErrorsAction"));
        neverBreakOnScriptErrorsAction->setCheckable(true);
        neverBreakOnScriptErrorsAction->setProperty("disableForQuickAccess", QVariant(true));
        breakOnAllScriptErrorsAction = new QAction(ARLMainWindow);
        breakOnAllScriptErrorsAction->setObjectName(QString::fromUtf8("breakOnAllScriptErrorsAction"));
        breakOnAllScriptErrorsAction->setCheckable(true);
        breakOnAllScriptErrorsAction->setProperty("disableForQuickAccess", QVariant(true));
        breakOnUnhandledScriptErrorsAction = new QAction(ARLMainWindow);
        breakOnUnhandledScriptErrorsAction->setObjectName(QString::fromUtf8("breakOnUnhandledScriptErrorsAction"));
        breakOnUnhandledScriptErrorsAction->setCheckable(true);
        breakOnUnhandledScriptErrorsAction->setProperty("disableForQuickAccess", QVariant(true));
        viewFindResultsWindowAction = new QAction(ARLMainWindow);
        viewFindResultsWindowAction->setObjectName(QString::fromUtf8("viewFindResultsWindowAction"));
        viewFindResultsWindowAction->setCheckable(true);
        QIcon icon76;
        icon76.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/find.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewFindResultsWindowAction->setIcon(icon76);
        findInScriptsAction = new QAction(ARLMainWindow);
        findInScriptsAction->setObjectName(QString::fromUtf8("findInScriptsAction"));
        findInScriptsAction->setShortcutContext(Qt::ApplicationShortcut);
        findInScriptsAction->setProperty("disableForQuickAccess", QVariant(true));
        explorerFilterAction = new QAction(ARLMainWindow);
        explorerFilterAction->setObjectName(QString::fromUtf8("explorerFilterAction"));
        explorerFilterAction->setProperty("disableForQuickAccess", QVariant(true));
        exportSelectionAction = new QAction(ARLMainWindow);
        exportSelectionAction->setObjectName(QString::fromUtf8("exportSelectionAction"));
        exportPlaceAction = new QAction(ARLMainWindow);
        exportPlaceAction->setObjectName(QString::fromUtf8("exportPlaceAction"));
        toggleAllBreakpointsStateAction = new QAction(ARLMainWindow);
        toggleAllBreakpointsStateAction->setObjectName(QString::fromUtf8("toggleAllBreakpointsStateAction"));
        duplicateSelectionAction = new QAction(ARLMainWindow);
        duplicateSelectionAction->setObjectName(QString::fromUtf8("duplicateSelectionAction"));
        duplicateSelectionAction->setIcon(icon17);
        gameExplorerAction = new QAction(ARLMainWindow);
        gameExplorerAction->setObjectName(QString::fromUtf8("gameExplorerAction"));
        gameExplorerAction->setCheckable(true);
        gameExplorerAction->setIcon(icon62);
        customizeQuickAccessAction = new QAction(ARLMainWindow);
        customizeQuickAccessAction->setObjectName(QString::fromUtf8("customizeQuickAccessAction"));
        customizeQuickAccessAction->setProperty("disableForQuickAccess", QVariant(true));
        unionSelectionAction = new QAction(ARLMainWindow);
        unionSelectionAction->setObjectName(QString::fromUtf8("unionSelectionAction"));
        QIcon icon77;
        icon77.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/union_16.png"), QSize(), QIcon::Normal, QIcon::Off);
        unionSelectionAction->setIcon(icon77);
        negateSelectionAction = new QAction(ARLMainWindow);
        negateSelectionAction->setObjectName(QString::fromUtf8("negateSelectionAction"));
        QIcon icon78;
        icon78.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/negate_16.png"), QSize(), QIcon::Normal, QIcon::Off);
        negateSelectionAction->setIcon(icon78);
        separateSelectionAction = new QAction(ARLMainWindow);
        separateSelectionAction->setObjectName(QString::fromUtf8("separateSelectionAction"));
        QIcon icon79;
        icon79.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/separate_16.png"), QSize(), QIcon::Normal, QIcon::Off);
        separateSelectionAction->setIcon(icon79);
        cleanupServersAndPlayersAction = new QAction(ARLMainWindow);
        cleanupServersAndPlayersAction->setObjectName(QString::fromUtf8("cleanupServersAndPlayersAction"));
        cleanupServersAndPlayersAction->setEnabled(false);
        QIcon icon80;
        icon80.addFile(QString::fromUtf8(":/RibbonBar/images/Studio 2.0 icons/32x32/delete_x_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        cleanupServersAndPlayersAction->setIcon(icon80);
        playRbxDeviceAction = new QAction(ARLMainWindow);
        playRbxDeviceAction->setObjectName(QString::fromUtf8("playRbxDeviceAction"));
        pairRbxDeviceAction = new QAction(ARLMainWindow);
        pairRbxDeviceAction->setObjectName(QString::fromUtf8("pairRbxDeviceAction"));
        QIcon icon81;
        icon81.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/pair_device_16.png"), QSize(), QIcon::Normal, QIcon::Off);
        pairRbxDeviceAction->setIcon(icon81);
        audioToggleAction = new QAction(ARLMainWindow);
        audioToggleAction->setObjectName(QString::fromUtf8("audioToggleAction"));
        QIcon icon82;
        icon82.addFile(QString::fromUtf8(":/16x16/images/RibbonIcons/Test/Audio-Disabled.png"), QSize(), QIcon::Normal, QIcon::Off);
        audioToggleAction->setIcon(icon82);
        viewTutorialsAction = new QAction(ARLMainWindow);
        viewTutorialsAction->setObjectName(QString::fromUtf8("viewTutorialsAction"));
        viewTutorialsAction->setCheckable(true);
        QIcon icon83;
        icon83.addFile(QString::fromUtf8(":/RibbonBar/images/RibbonIcons/View/Tutorials.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewTutorialsAction->setIcon(icon83);
        viewScriptAnalysisAction = new QAction(ARLMainWindow);
        viewScriptAnalysisAction->setObjectName(QString::fromUtf8("viewScriptAnalysisAction"));
        viewScriptAnalysisAction->setCheckable(true);
        viewScriptAnalysisAction->setIcon(icon62);
        viewTeamCreateAction = new QAction(ARLMainWindow);
        viewTeamCreateAction->setObjectName(QString::fromUtf8("viewTeamCreateAction"));
        viewTeamCreateAction->setCheckable(true);
        QIcon icon84;
        icon84.addFile(QString::fromUtf8(":/RibbonBar/images/RibbonIcons/View/TeamCreate.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewTeamCreateAction->setIcon(icon84);
        emulateDeviceAction = new QAction(ARLMainWindow);
        emulateDeviceAction->setObjectName(QString::fromUtf8("emulateDeviceAction"));
        manageEmulationDeviceAction = new QAction(ARLMainWindow);
        manageEmulationDeviceAction->setObjectName(QString::fromUtf8("manageEmulationDeviceAction"));
        QIcon icon85;
        icon85.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/emulate_device_16.png"), QSize(), QIcon::Normal, QIcon::Off);
        manageEmulationDeviceAction->setIcon(icon85);
        launchHelpForSelectionAction = new QAction(ARLMainWindow);
        launchHelpForSelectionAction->setObjectName(QString::fromUtf8("launchHelpForSelectionAction"));
        launchHelpForSelectionAction->setIcon(icon71);
        viewCommandBarAction = new QAction(ARLMainWindow);
        viewCommandBarAction->setObjectName(QString::fromUtf8("viewCommandBarAction"));
        viewCommandBarAction->setCheckable(true);
        QIcon icon86;
        icon86.addFile(QString::fromUtf8(":/RibbonBar/images/RibbonIcons/View/Output.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewCommandBarAction->setIcon(icon86);
        fileOpenRecentSavesAction = new QAction(ARLMainWindow);
        fileOpenRecentSavesAction->setObjectName(QString::fromUtf8("fileOpenRecentSavesAction"));
        fileOpenRecentSavesAction->setIcon(icon1);
        goToLineAction = new QAction(ARLMainWindow);
        goToLineAction->setObjectName(QString::fromUtf8("goToLineAction"));
        QIcon icon87;
        icon87.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/goToLine.png"), QSize(), QIcon::Normal, QIcon::Off);
        goToLineAction->setIcon(icon87);
        downloadPlaceCopyAction = new QAction(ARLMainWindow);
        downloadPlaceCopyAction->setObjectName(QString::fromUtf8("downloadPlaceCopyAction"));
        QIcon icon88;
        icon88.addFile(QString::fromUtf8(":/16x16/images/Studio 2.0 icons/16x16/download_copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        downloadPlaceCopyAction->setIcon(icon88);
        defaultCentralWidget = new QWidget(ARLMainWindow);
        defaultCentralWidget->setObjectName(QString::fromUtf8("defaultCentralWidget"));
        ARLMainWindow->setCentralWidget(defaultCentralWidget);
        statusbar = new QStatusBar(ARLMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ARLMainWindow->setStatusBar(statusbar);
        standardToolBar = new QToolBar(ARLMainWindow);
        standardToolBar->setObjectName(QString::fromUtf8("standardToolBar"));
        standardToolBar->setIconSize(QSize(16, 16));
        ARLMainWindow->addToolBar(Qt::TopToolBarArea, standardToolBar);
        runToolBar = new QToolBar(ARLMainWindow);
        runToolBar->setObjectName(QString::fromUtf8("runToolBar"));
        runToolBar->setEnabled(true);
        runToolBar->setMouseTracking(false);
        runToolBar->setAcceptDrops(false);
        runToolBar->setAutoFillBackground(false);
        runToolBar->setIconSize(QSize(16, 16));
        ARLMainWindow->addToolBar(Qt::TopToolBarArea, runToolBar);
        advToolsToolBar = new QToolBar(ARLMainWindow);
        advToolsToolBar->setObjectName(QString::fromUtf8("advToolsToolBar"));
        advToolsToolBar->setIconSize(QSize(16, 16));
        ARLMainWindow->addToolBar(Qt::TopToolBarArea, advToolsToolBar);
        oldToolsToolBar = new QToolBar(ARLMainWindow);
        oldToolsToolBar->setObjectName(QString::fromUtf8("oldToolsToolBar"));
        oldToolsToolBar->setIconSize(QSize(16, 16));
        ARLMainWindow->addToolBar(Qt::TopToolBarArea, oldToolsToolBar);
        editCameraToolBar = new QToolBar(ARLMainWindow);
        editCameraToolBar->setObjectName(QString::fromUtf8("editCameraToolBar"));
        editCameraToolBar->setIconSize(QSize(16, 16));
        ARLMainWindow->addToolBar(Qt::TopToolBarArea, editCameraToolBar);
        ARLMainWindow->insertToolBarBreak(editCameraToolBar);
        menubar = new QMenuBar(ARLMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1280, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuFind_and_Replace = new QMenu(menuEdit);
        menuFind_and_Replace->setObjectName(QString::fromUtf8("menuFind_and_Replace"));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuTest = new QMenu(menuTools);
        menuTest->setObjectName(QString::fromUtf8("menuTest"));
        menuStats = new QMenu(menuTools);
        menuStats->setObjectName(QString::fromUtf8("menuStats"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuToolBars = new QMenu(menuView);
        menuToolBars->setObjectName(QString::fromUtf8("menuToolBars"));
        menuFormat = new QMenu(menubar);
        menuFormat->setObjectName(QString::fromUtf8("menuFormat"));
        menuInsert = new QMenu(menubar);
        menuInsert->setObjectName(QString::fromUtf8("menuInsert"));
        menuWindow = new QMenu(menubar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        scriptMenu = new QMenu(menubar);
        scriptMenu->setObjectName(QString::fromUtf8("scriptMenu"));
        breakErrorsIntoDebuggerMenu = new QMenu(scriptMenu);
        breakErrorsIntoDebuggerMenu->setObjectName(QString::fromUtf8("breakErrorsIntoDebuggerMenu"));
        ARLMainWindow->setMenuBar(menubar);
        outputWindow = new QDockWidget(ARLMainWindow);
        outputWindow->setObjectName(QString::fromUtf8("outputWindow"));
        outputWindow->setMinimumSize(QSize(300, 104));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        dockWidgetContents_2->setMinimumSize(QSize(300, 0));
        dockWidgetContents_2->setSizeIncrement(QSize(0, 0));
        gridLayout = new QGridLayout(dockWidgetContents_2);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        outputWindow->setWidget(dockWidgetContents_2);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), outputWindow);
        commandToolBar = new QToolBar(ARLMainWindow);
        commandToolBar->setObjectName(QString::fromUtf8("commandToolBar"));
        commandToolBar->setIconSize(QSize(16, 16));
        ARLMainWindow->addToolBar(Qt::BottomToolBarArea, commandToolBar);
        objectExplorer = new QDockWidget(ARLMainWindow);
        objectExplorer->setObjectName(QString::fromUtf8("objectExplorer"));
        objectExplorer->setMinimumSize(QSize(230, 300));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout_3 = new QGridLayout(dockWidgetContents);
        gridLayout_3->setSpacing(0);
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        stackedWidget = new QStackedWidget(dockWidgetContents);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedWidget->addWidget(page_2);

        gridLayout_3->addWidget(stackedWidget, 0, 0, 1, 1);

        objectExplorer->setWidget(dockWidgetContents);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), objectExplorer);
        propertyBrowser = new QDockWidget(ARLMainWindow);
        propertyBrowser->setObjectName(QString::fromUtf8("propertyBrowser"));
        propertyBrowser->setMinimumSize(QSize(266, 250));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        propertyBrowser->setWidget(dockWidgetContents_3);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), propertyBrowser);
        diagnosticsDockWidget = new QDockWidget(ARLMainWindow);
        diagnosticsDockWidget->setObjectName(QString::fromUtf8("diagnosticsDockWidget"));
        diagnosticsDockWidget->setMinimumSize(QSize(100, 42));
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        diagnosticsDockWidget->setWidget(dockWidgetContents_4);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), diagnosticsDockWidget);
        taskSchedulerDockWidget = new QDockWidget(ARLMainWindow);
        taskSchedulerDockWidget->setObjectName(QString::fromUtf8("taskSchedulerDockWidget"));
        taskSchedulerDockWidget->setMinimumSize(QSize(100, 42));
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QString::fromUtf8("dockWidgetContents_6"));
        taskSchedulerDockWidget->setWidget(dockWidgetContents_6);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), taskSchedulerDockWidget);
        toolBoxDockWidget = new QDockWidget(ARLMainWindow);
        toolBoxDockWidget->setObjectName(QString::fromUtf8("toolBoxDockWidget"));
        toolBoxDockWidget->setMinimumSize(QSize(200, 100));
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QString::fromUtf8("dockWidgetContents_5"));
        toolBoxDockWidget->setWidget(dockWidgetContents_5);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), toolBoxDockWidget);
        scriptReviewDockWidget = new QDockWidget(ARLMainWindow);
        scriptReviewDockWidget->setObjectName(QString::fromUtf8("scriptReviewDockWidget"));
        scriptReviewDockWidget->setMinimumSize(QSize(100, 42));
        dockWidgetContents_7 = new QWidget();
        dockWidgetContents_7->setObjectName(QString::fromUtf8("dockWidgetContents_7"));
        scriptReviewDockWidget->setWidget(dockWidgetContents_7);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), scriptReviewDockWidget);
        gameExplorerDockWidget = new QDockWidget(ARLMainWindow);
        gameExplorerDockWidget->setObjectName(QString::fromUtf8("gameExplorerDockWidget"));
        gameExplorerDockWidget->setMinimumSize(QSize(100, 42));
        dockWidgetContents_11 = new QWidget();
        dockWidgetContents_11->setObjectName(QString::fromUtf8("dockWidgetContents_11"));
        gameExplorerDockWidget->setWidget(dockWidgetContents_11);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), gameExplorerDockWidget);
        viewToolsToolBar = new QToolBar(ARLMainWindow);
        viewToolsToolBar->setObjectName(QString::fromUtf8("viewToolsToolBar"));
        viewToolsToolBar->setIconSize(QSize(16, 16));
        ARLMainWindow->addToolBar(Qt::TopToolBarArea, viewToolsToolBar);
        basicObjectsDockWidget = new QDockWidget(ARLMainWindow);
        basicObjectsDockWidget->setObjectName(QString::fromUtf8("basicObjectsDockWidget"));
        basicObjectsDockWidget->setMinimumSize(QSize(100, 42));
        dockWidgetContents_8 = new QWidget();
        dockWidgetContents_8->setObjectName(QString::fromUtf8("dockWidgetContents_8"));
        basicObjectsDockWidget->setWidget(dockWidgetContents_8);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), basicObjectsDockWidget);
        contextualHelp = new QDockWidget(ARLMainWindow);
        contextualHelp->setObjectName(QString::fromUtf8("contextualHelp"));
        dockWidgetContents_9 = new QWidget();
        dockWidgetContents_9->setObjectName(QString::fromUtf8("dockWidgetContents_9"));
        contextualHelp->setWidget(dockWidgetContents_9);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), contextualHelp);
        findResultsWindow = new QDockWidget(ARLMainWindow);
        findResultsWindow->setObjectName(QString::fromUtf8("findResultsWindow"));
        findResultsWindow->setMinimumSize(QSize(300, 104));
        dockWidgetContents_10 = new QWidget();
        dockWidgetContents_10->setObjectName(QString::fromUtf8("dockWidgetContents_10"));
        dockWidgetContents_10->setMinimumSize(QSize(300, 0));
        dockWidgetContents_10->setSizeIncrement(QSize(0, 0));
        gridLayout_2 = new QGridLayout(dockWidgetContents_10);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        findResultsWindow->setWidget(dockWidgetContents_10);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), findResultsWindow);
        tutorialsDockWidget = new QDockWidget(ARLMainWindow);
        tutorialsDockWidget->setObjectName(QString::fromUtf8("tutorialsDockWidget"));
        tutorialsDockWidget->setMinimumSize(QSize(350, 150));
        dockWidgetContents_12 = new QWidget();
        dockWidgetContents_12->setObjectName(QString::fromUtf8("dockWidgetContents_12"));
        tutorialsDockWidget->setWidget(dockWidgetContents_12);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), tutorialsDockWidget);
        teamCreateDockWidget = new QDockWidget(ARLMainWindow);
        teamCreateDockWidget->setObjectName(QString::fromUtf8("teamCreateDockWidget"));
        teamCreateDockWidget->setMinimumSize(QSize(250, 150));
        dockWidgetContents_13 = new QWidget();
        dockWidgetContents_13->setObjectName(QString::fromUtf8("dockWidgetContents_13"));
        teamCreateDockWidget->setWidget(dockWidgetContents_13);
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), teamCreateDockWidget);
        scriptAnalysisDockWidget = new QDockWidget(ARLMainWindow);
        scriptAnalysisDockWidget->setObjectName(QString::fromUtf8("scriptAnalysisDockWidget"));
        scriptAnalysisDockWidget->setMinimumSize(QSize(300, 104));
        ARLMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), scriptAnalysisDockWidget);

        standardToolBar->addAction(fileNewAction);
        standardToolBar->addAction(fileOpenAction);
        standardToolBar->addAction(fileOpenRecentSavesAction);
        standardToolBar->addAction(fileSaveAction);
        standardToolBar->addSeparator();
        standardToolBar->addAction(cutAction);
        standardToolBar->addAction(copyAction);
        standardToolBar->addAction(pasteAction);
        standardToolBar->addSeparator();
        standardToolBar->addAction(undoAction);
        standardToolBar->addAction(redoAction);
        standardToolBar->addSeparator();
        standardToolBar->addAction(groupSelectionAction);
        standardToolBar->addAction(ungroupSelectionAction);
        standardToolBar->addAction(unionSelectionAction);
        standardToolBar->addAction(negateSelectionAction);
        standardToolBar->addAction(separateSelectionAction);
        standardToolBar->addSeparator();
        standardToolBar->addAction(rotateSelectionAction);
        standardToolBar->addAction(tiltSelectionAction);
        standardToolBar->addAction(moveUpBrickAction);
        standardToolBar->addAction(moveDownBrickAction);
        runToolBar->addAction(simulationPlayAction);
        runToolBar->addAction(simulationRunAction);
        runToolBar->addAction(simulationStopAction);
        runToolBar->addAction(simulationResetAction);
        advToolsToolBar->addAction(advArrowToolAction);
        advToolsToolBar->addAction(advTranslateAction);
        advToolsToolBar->addAction(resizeAction);
        advToolsToolBar->addAction(advRotateAction);
        advToolsToolBar->addSeparator();
        advToolsToolBar->addAction(gridToOneAction);
        advToolsToolBar->addAction(gridToOneFifthAction);
        advToolsToolBar->addAction(gridToOffAction);
        advToolsToolBar->addSeparator();
        advToolsToolBar->addAction(advanceJointCreationManualAction);
        advToolsToolBar->addSeparator();
        advToolsToolBar->addAction(lockAction);
        advToolsToolBar->addAction(anchorAction);
        advToolsToolBar->addSeparator();
        advToolsToolBar->addAction(dropperAction);
        advToolsToolBar->addAction(smoothSurfaceAction);
        advToolsToolBar->addAction(glueSurfaceAction);
        advToolsToolBar->addAction(weldSurfaceAction);
        advToolsToolBar->addAction(studsAction);
        advToolsToolBar->addAction(inletAction);
        advToolsToolBar->addAction(universalsAction);
        advToolsToolBar->addAction(hingeAction);
        advToolsToolBar->addAction(motorRightAction);
        editCameraToolBar->addAction(panLeftAction);
        editCameraToolBar->addAction(panRightAction);
        editCameraToolBar->addAction(tiltUpAction);
        editCameraToolBar->addAction(tiltDownAction);
        editCameraToolBar->addAction(zoomInAction);
        editCameraToolBar->addAction(zoomOutAction);
        editCameraToolBar->addAction(zoomExtentsAction);
        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(scriptMenu->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuInsert->menuAction());
        menubar->addAction(menuFormat->menuAction());
        menubar->addAction(menuTools->menuAction());
        menubar->addAction(menuWindow->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(fileNewAction);
        menuFile->addAction(fileOpenAction);
        menuFile->addAction(filePublishedProjectsAction);
        menuFile->addAction(fileCloseAction);
        menuFile->addSeparator();
        menuFile->addAction(fileSaveAction);
        menuFile->addAction(fileSaveAsAction);
        menuFile->addAction(publishToRobloxAction);
        menuFile->addAction(publishToRobloxAsAction);
        menuFile->addAction(publishGameAction);
        menuFile->addAction(downloadPlaceCopyAction);
        menuFile->addSeparator();
        menuFile->addAction(publishSelectionToRobloxAction);
        menuFile->addSeparator();
        menuFile->addAction(exportPlaceAction);
        menuFile->addSeparator();
        menuFile->addAction(fileExitAction);
        menuEdit->addAction(undoAction);
        menuEdit->addAction(redoAction);
        menuEdit->addSeparator();
        menuEdit->addAction(cutAction);
        menuEdit->addAction(copyAction);
        menuEdit->addAction(pasteAction);
        menuEdit->addAction(duplicateSelectionAction);
        menuEdit->addSeparator();
        menuEdit->addAction(pasteIntoAction);
        menuEdit->addAction(deleteSelectedAction);
        menuEdit->addAction(selectAllAction);
        menuEdit->addAction(explorerFilterAction);
        menuEdit->addSeparator();
        menuEdit->addAction(menuFind_and_Replace->menuAction());
        menuEdit->addSeparator();
        menuEdit->addAction(lockAction);
        menuEdit->addAction(unlockAllAction);
        menuFind_and_Replace->addAction(findAction);
        menuFind_and_Replace->addAction(findNextAction);
        menuFind_and_Replace->addAction(findPreviousAction);
        menuFind_and_Replace->addAction(replaceAction);
        menuFind_and_Replace->addSeparator();
        menuFind_and_Replace->addAction(findInScriptsAction);
        menuTools->addAction(executeScriptAction);
        menuTools->addSeparator();
        menuTools->addAction(menuTest->menuAction());
        menuTools->addAction(menuStats->menuAction());
        menuTools->addSeparator();
        menuTools->addAction(openPluginsFolderAction);
        menuTools->addAction(managePluginsAction);
        menuTools->addAction(settingsAction);
        menuTest->addAction(playSoloAction);
        menuTest->addAction(startServerAction);
        menuTest->addAction(startPlayerAction);
        menuTest->addSeparator();
        menuTest->addAction(pairRbxDeviceAction);
        menuStats->addAction(testStatsAction);
        menuStats->addAction(testRenderStatsAction);
        menuStats->addAction(testNetworkStatsAction);
        menuStats->addAction(testPhysicsStatsAction);
        menuStats->addAction(testSummaryStatsAction);
        menuStats->addAction(testCustomStatsAction);
        menuStats->addSeparator();
        menuStats->addAction(testClearStatsAction);
        menuView->addAction(actionStartPage);
        menuView->addSeparator();
        menuView->addAction(actionFullScreen);
        menuView->addAction(toggleAxisWidgetAction);
        menuView->addAction(toggle3DGridAction);
        menuView->addSeparator();
        menuView->addAction(menuToolBars->menuAction());
        menuView->addSeparator();
        menuFormat->addAction(rotateSelectionAction);
        menuFormat->addAction(tiltSelectionAction);
        menuFormat->addSeparator();
        menuFormat->addAction(groupSelectionAction);
        menuFormat->addAction(ungroupSelectionAction);
        menuFormat->addAction(unionSelectionAction);
        menuFormat->addAction(negateSelectionAction);
        menuFormat->addAction(separateSelectionAction);
        menuFormat->addAction(selectChildrenAction);
        menuInsert->addAction(insertModelAction);
        menuInsert->addAction(insertServiceAction);
        menuInsert->addSeparator();
        menuHelp->addAction(aboutRobloxAction);
        menuHelp->addAction(onlineHelpAction);
        menuHelp->addAction(shortcutHelpAction);
        menuHelp->addAction(objectBrowserAction);
        menuHelp->addAction(fastLogDumpAction);
        scriptMenu->addAction(goToScriptErrorAction);
        scriptMenu->addAction(breakErrorsIntoDebuggerMenu->menuAction());
        scriptMenu->addSeparator();
        scriptMenu->addAction(commentSelectionAction);
        scriptMenu->addAction(uncommentSelectionAction);
        scriptMenu->addAction(toggleCommentAction);
        scriptMenu->addSeparator();
        scriptMenu->addAction(expandAllFoldsAction);
        scriptMenu->addAction(collapseAllFoldsAction);
        scriptMenu->addSeparator();
        scriptMenu->addAction(zoomInAction);
        scriptMenu->addAction(zoomOutAction);
        scriptMenu->addAction(resetScriptZoomAction);
        scriptMenu->addAction(reloadScriptAction);
        scriptMenu->addSeparator();
        scriptMenu->addAction(goToLineAction);
        breakErrorsIntoDebuggerMenu->addAction(neverBreakOnScriptErrorsAction);
        breakErrorsIntoDebuggerMenu->addAction(breakOnAllScriptErrorsAction);
        breakErrorsIntoDebuggerMenu->addAction(breakOnUnhandledScriptErrorsAction);

        retranslateUi(ARLMainWindow);

        QMetaObject::connectSlotsByName(ARLMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ARLMainWindow)
    {
        ARLMainWindow->setWindowTitle(QApplication::translate("ARLMainWindow", "ANORRL Studio", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setText(QApplication::translate("ARLMainWindow", "Open...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileOpenAction->setToolTip(QApplication::translate("ARLMainWindow", "Open", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        fileOpenAction->setStatusTip(QApplication::translate("ARLMainWindow", "Open an existing document", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        fileOpenAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        undoAction->setText(QApplication::translate("ARLMainWindow", "Undo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        undoAction->setToolTip(QApplication::translate("ARLMainWindow", "Undo", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        undoAction->setStatusTip(QApplication::translate("ARLMainWindow", "Undo the last action", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        undoAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        redoAction->setText(QApplication::translate("ARLMainWindow", "Redo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        redoAction->setToolTip(QApplication::translate("ARLMainWindow", "Redo", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        redoAction->setStatusTip(QApplication::translate("ARLMainWindow", "Redo the previously undone action", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        redoAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+Z", 0, QApplication::UnicodeUTF8));
        deleteSelectedAction->setText(QApplication::translate("ARLMainWindow", "Delete", 0, QApplication::UnicodeUTF8));
        deleteSelectedAction->setIconText(QApplication::translate("ARLMainWindow", "Delete", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        deleteSelectedAction->setToolTip(QApplication::translate("ARLMainWindow", "Delete", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        deleteSelectedAction->setStatusTip(QApplication::translate("ARLMainWindow", "Erase the selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        deleteSelectedAction->setShortcut(QApplication::translate("ARLMainWindow", "Del", 0, QApplication::UnicodeUTF8));
        selectAllAction->setText(QApplication::translate("ARLMainWindow", "Select All", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        selectAllAction->setToolTip(QApplication::translate("ARLMainWindow", "Select All", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        selectAllAction->setStatusTip(QApplication::translate("ARLMainWindow", "Select the entire document", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        selectAllAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        lockAction->setText(QApplication::translate("ARLMainWindow", "Lock", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lockAction->setToolTip(QApplication::translate("ARLMainWindow", "Lock - prevent selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        lockAction->setStatusTip(QApplication::translate("ARLMainWindow", "Lock objects", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        lockAction->setShortcut(QApplication::translate("ARLMainWindow", "Alt+L", 0, QApplication::UnicodeUTF8));
        anchorAction->setText(QApplication::translate("ARLMainWindow", "Anchor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        anchorAction->setToolTip(QApplication::translate("ARLMainWindow", "Anchor - prevent moving", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        anchorAction->setStatusTip(QApplication::translate("ARLMainWindow", "Anchor objects", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        anchorAction->setShortcut(QApplication::translate("ARLMainWindow", "Alt+A", 0, QApplication::UnicodeUTF8));
        advTranslateAction->setText(QApplication::translate("ARLMainWindow", "Move Tool", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        advTranslateAction->setToolTip(QApplication::translate("ARLMainWindow", "Move Parts", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        advTranslateAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+2", 0, QApplication::UnicodeUTF8));
        advRotateAction->setText(QApplication::translate("ARLMainWindow", "Rotate Tool", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        advRotateAction->setToolTip(QApplication::translate("ARLMainWindow", "Rotate Parts", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        advRotateAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+4", 0, QApplication::UnicodeUTF8));
        zoomInAction->setText(QApplication::translate("ARLMainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        zoomInAction->setToolTip(QApplication::translate("ARLMainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        zoomInAction->setStatusTip(QApplication::translate("ARLMainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        zoomOutAction->setText(QApplication::translate("ARLMainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        zoomOutAction->setToolTip(QApplication::translate("ARLMainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        zoomOutAction->setStatusTip(QApplication::translate("ARLMainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        tiltUpAction->setText(QApplication::translate("ARLMainWindow", "Tilt Up", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        tiltUpAction->setToolTip(QApplication::translate("ARLMainWindow", "Tilt Up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        tiltUpAction->setStatusTip(QApplication::translate("ARLMainWindow", "Tilt Up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        tiltDownAction->setText(QApplication::translate("ARLMainWindow", "Tilt Down", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        tiltDownAction->setStatusTip(QApplication::translate("ARLMainWindow", "Tilt Down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        panRightAction->setText(QApplication::translate("ARLMainWindow", "Pan Right", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        panRightAction->setStatusTip(QApplication::translate("ARLMainWindow", "Pan Right", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        panLeftAction->setText(QApplication::translate("ARLMainWindow", "Pan Left", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        panLeftAction->setStatusTip(QApplication::translate("ARLMainWindow", "Pan Left", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        glueSurfaceAction->setText(QApplication::translate("ARLMainWindow", "Glue Surface", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        glueSurfaceAction->setToolTip(QApplication::translate("ARLMainWindow", "Glue Surface", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        cutAction->setText(QApplication::translate("ARLMainWindow", "Cut", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cutAction->setToolTip(QApplication::translate("ARLMainWindow", "Cut", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        cutAction->setStatusTip(QApplication::translate("ARLMainWindow", "Cut the selection and put it on the Clipboard", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        cutAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        copyAction->setText(QApplication::translate("ARLMainWindow", "Copy", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        copyAction->setToolTip(QApplication::translate("ARLMainWindow", "Copy", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        copyAction->setStatusTip(QApplication::translate("ARLMainWindow", "Copy the selection and put it on the Clipboard", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        copyAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        pasteAction->setText(QApplication::translate("ARLMainWindow", "Paste", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pasteAction->setToolTip(QApplication::translate("ARLMainWindow", "Paste", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        pasteAction->setStatusTip(QApplication::translate("ARLMainWindow", "Insert Clipboard contents", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        pasteAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        smoothSurfaceAction->setText(QApplication::translate("ARLMainWindow", "Smooth Surface", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        smoothSurfaceAction->setToolTip(QApplication::translate("ARLMainWindow", "Smooth Surface", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        weldSurfaceAction->setText(QApplication::translate("ARLMainWindow", "Weld Surface", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        weldSurfaceAction->setToolTip(QApplication::translate("ARLMainWindow", "Weld Surface", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        studsAction->setText(QApplication::translate("ARLMainWindow", "Connectors On Surface", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        studsAction->setToolTip(QApplication::translate("ARLMainWindow", "Connectors on Surface", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        inletAction->setText(QApplication::translate("ARLMainWindow", "Indents on Surface", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        inletAction->setToolTip(QApplication::translate("ARLMainWindow", "Indents on Surface", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        universalsAction->setText(QApplication::translate("ARLMainWindow", "Universals for surface", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        universalsAction->setToolTip(QApplication::translate("ARLMainWindow", "Universals for Surface", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        hingeAction->setText(QApplication::translate("ARLMainWindow", "Hinge on Surface", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        hingeAction->setToolTip(QApplication::translate("ARLMainWindow", "Hinge on Surface", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        smoothNoOutlinesAction->setText(QApplication::translate("ARLMainWindow", "Smooth Surface without Outlines", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        smoothNoOutlinesAction->setToolTip(QApplication::translate("ARLMainWindow", "Smooth Surface without Outlines", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupSelectionAction->setText(QApplication::translate("ARLMainWindow", "Group", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        groupSelectionAction->setToolTip(QApplication::translate("ARLMainWindow", "Group", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        groupSelectionAction->setStatusTip(QApplication::translate("ARLMainWindow", "Group parts or models", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        groupSelectionAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+G", 0, QApplication::UnicodeUTF8));
        ungroupSelectionAction->setText(QApplication::translate("ARLMainWindow", "Ungroup", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ungroupSelectionAction->setToolTip(QApplication::translate("ARLMainWindow", "Ungroup", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        ungroupSelectionAction->setStatusTip(QApplication::translate("ARLMainWindow", "Ungroup models or parts", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        ungroupSelectionAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+U", 0, QApplication::UnicodeUTF8));
        moveUpBrickAction->setText(QApplication::translate("ARLMainWindow", "Move Up Brick", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        moveUpBrickAction->setToolTip(QApplication::translate("ARLMainWindow", "Move Up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        moveUpBrickAction->setStatusTip(QApplication::translate("ARLMainWindow", "Move objects up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        moveDownBrickAction->setText(QApplication::translate("ARLMainWindow", "Move Down", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        moveDownBrickAction->setToolTip(QApplication::translate("ARLMainWindow", "Move Down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        moveDownBrickAction->setStatusTip(QApplication::translate("ARLMainWindow", "Move objects down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        rotateSelectionAction->setText(QApplication::translate("ARLMainWindow", "Rotate", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        rotateSelectionAction->setToolTip(QApplication::translate("ARLMainWindow", "Rotate objects", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        rotateSelectionAction->setStatusTip(QApplication::translate("ARLMainWindow", "Rotate objects", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        rotateSelectionAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        tiltSelectionAction->setText(QApplication::translate("ARLMainWindow", "Tilt", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        tiltSelectionAction->setToolTip(QApplication::translate("ARLMainWindow", "Tilt objects", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        tiltSelectionAction->setStatusTip(QApplication::translate("ARLMainWindow", "Tilt objects", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        tiltSelectionAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+T", 0, QApplication::UnicodeUTF8));
        simulationPlayAction->setText(QApplication::translate("ARLMainWindow", "Play", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        simulationPlayAction->setToolTip(QApplication::translate("ARLMainWindow", "Play", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        simulationPlayAction->setStatusTip(QApplication::translate("ARLMainWindow", "Test - play", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        simulationPlayAction->setShortcut(QApplication::translate("ARLMainWindow", "F5", 0, QApplication::UnicodeUTF8));
        simulationRunAction->setText(QApplication::translate("ARLMainWindow", "Run", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        simulationRunAction->setToolTip(QApplication::translate("ARLMainWindow", "Run", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        simulationRunAction->setStatusTip(QApplication::translate("ARLMainWindow", "Test - run", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        simulationStopAction->setText(QApplication::translate("ARLMainWindow", "Pause", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        simulationStopAction->setToolTip(QApplication::translate("ARLMainWindow", "Pause", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        simulationStopAction->setStatusTip(QApplication::translate("ARLMainWindow", "Pauses the run", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        simulationStopAction->setShortcut(QApplication::translate("ARLMainWindow", "F5", 0, QApplication::UnicodeUTF8));
        testStatsAction->setText(QApplication::translate("ARLMainWindow", "Stats", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        testStatsAction->setToolTip(QApplication::translate("ARLMainWindow", "Render Stats", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        testStatsAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+F1", 0, QApplication::UnicodeUTF8));
        testRenderStatsAction->setText(QApplication::translate("ARLMainWindow", "Render Stats", 0, QApplication::UnicodeUTF8));
        testRenderStatsAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+F2", 0, QApplication::UnicodeUTF8));
        testNetworkStatsAction->setText(QApplication::translate("ARLMainWindow", "Network Stats", 0, QApplication::UnicodeUTF8));
        testNetworkStatsAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+F3", 0, QApplication::UnicodeUTF8));
        testPhysicsStatsAction->setText(QApplication::translate("ARLMainWindow", "Physics Stats", 0, QApplication::UnicodeUTF8));
        testPhysicsStatsAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+F4", 0, QApplication::UnicodeUTF8));
        testSummaryStatsAction->setText(QApplication::translate("ARLMainWindow", "Summary Stats", 0, QApplication::UnicodeUTF8));
        testSummaryStatsAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+F5", 0, QApplication::UnicodeUTF8));
        testCustomStatsAction->setText(QApplication::translate("ARLMainWindow", "Custom Stats", 0, QApplication::UnicodeUTF8));
        testCustomStatsAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+F6", 0, QApplication::UnicodeUTF8));
        simulationResetAction->setText(QApplication::translate("ARLMainWindow", "Stop", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        simulationResetAction->setToolTip(QApplication::translate("ARLMainWindow", "Stop", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        simulationResetAction->setStatusTip(QApplication::translate("ARLMainWindow", "Stops and reloads the document", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        simulationResetAction->setShortcut(QApplication::translate("ARLMainWindow", "Shift+F5", 0, QApplication::UnicodeUTF8));
        advanceJointCreationManualAction->setText(QApplication::translate("ARLMainWindow", "Automatically Create Joints", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        advanceJointCreationManualAction->setToolTip(QApplication::translate("ARLMainWindow", "Automatically create joints between parts...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        advanceJointCreationManualAction->setStatusTip(QApplication::translate("ARLMainWindow", "Automatically create joints between parts...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        gridToOneAction->setText(QApplication::translate("ARLMainWindow", "One Stud Grid", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        gridToOneAction->setToolTip(QApplication::translate("ARLMainWindow", "One Stud Grid", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        gridToOneFifthAction->setText(QApplication::translate("ARLMainWindow", "1/5 Stud Grid", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        gridToOneFifthAction->setToolTip(QApplication::translate("ARLMainWindow", "1/5 Stud Grid", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        gridToOffAction->setText(QApplication::translate("ARLMainWindow", "Grid Off", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        gridToOffAction->setToolTip(QApplication::translate("ARLMainWindow", "Grid Off", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        resizeAction->setText(QApplication::translate("ARLMainWindow", "Resize Tool", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        resizeAction->setToolTip(QApplication::translate("ARLMainWindow", "Resize", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        resizeAction->setStatusTip(QApplication::translate("ARLMainWindow", "Resize Object", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        resizeAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+3", 0, QApplication::UnicodeUTF8));
        motorRightAction->setText(QApplication::translate("ARLMainWindow", "Right Motor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        motorRightAction->setToolTip(QApplication::translate("ARLMainWindow", "Motor on Surface", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFillColor->setText(QApplication::translate("ARLMainWindow", "Fill", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionFillColor->setToolTip(QApplication::translate("ARLMainWindow", "Fill With Color", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionFillColor->setStatusTip(QApplication::translate("ARLMainWindow", "Fills a part with a color", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionMaterial->setText(QApplication::translate("ARLMainWindow", "Material", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionMaterial->setToolTip(QApplication::translate("ARLMainWindow", "Change Material", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionMaterial->setStatusTip(QApplication::translate("ARLMainWindow", "Changes the material of a part", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        dropperAction->setText(QApplication::translate("ARLMainWindow", "Dropper", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        dropperAction->setToolTip(QApplication::translate("ARLMainWindow", "Pick Color", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        dropperAction->setStatusTip(QApplication::translate("ARLMainWindow", "Picks up a color from a part", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        fileSaveAction->setText(QApplication::translate("ARLMainWindow", "Save", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileSaveAction->setToolTip(QApplication::translate("ARLMainWindow", "Save", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        fileSaveAction->setStatusTip(QApplication::translate("ARLMainWindow", "Save the active document", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        fileSaveAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        executeScriptAction->setText(QApplication::translate("ARLMainWindow", "Execute Script...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        executeScriptAction->setToolTip(QApplication::translate("ARLMainWindow", "Execute Script", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fileCloseAction->setText(QApplication::translate("ARLMainWindow", "Close", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        fileCloseAction->setStatusTip(QApplication::translate("ARLMainWindow", "Close the active document", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        fileSaveAsAction->setText(QApplication::translate("ARLMainWindow", "Save As...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        fileSaveAsAction->setStatusTip(QApplication::translate("ARLMainWindow", "Save the active document with a new name", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        fileSaveAsAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        fileNewAction->setText(QApplication::translate("ARLMainWindow", "New", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        fileNewAction->setStatusTip(QApplication::translate("ARLMainWindow", "Create a new document", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        fileNewAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        fileExitAction->setText(QApplication::translate("ARLMainWindow", "Exit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        fileExitAction->setStatusTip(QApplication::translate("ARLMainWindow", "Quit the application; prompts to save documents", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        fileExitAction->setShortcut(QApplication::translate("ARLMainWindow", "Alt+F4", 0, QApplication::UnicodeUTF8));
        findAction->setText(QApplication::translate("ARLMainWindow", "Find...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        findAction->setToolTip(QApplication::translate("ARLMainWindow", "Find", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        findAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        zoomExtentsAction->setText(QApplication::translate("ARLMainWindow", "Zoom to", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        zoomExtentsAction->setToolTip(QApplication::translate("ARLMainWindow", "Zoom to", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        zoomExtentsAction->setStatusTip(QApplication::translate("ARLMainWindow", "Zoom to", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        zoomExtentsAction->setShortcut(QApplication::translate("ARLMainWindow", "F", 0, QApplication::UnicodeUTF8));
        viewBasicObjectsAction->setText(QApplication::translate("ARLMainWindow", "Basic Objects", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewBasicObjectsAction->setToolTip(QApplication::translate("ARLMainWindow", "Insert Basic Objects", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        insertModelAction->setText(QApplication::translate("ARLMainWindow", "Model...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        insertModelAction->setToolTip(QApplication::translate("ARLMainWindow", "Insert Models", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pasteIntoAction->setText(QApplication::translate("ARLMainWindow", "Paste Into", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pasteIntoAction->setToolTip(QApplication::translate("ARLMainWindow", "Paste Into", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pasteIntoAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+V", 0, QApplication::UnicodeUTF8));
        selectionSaveToFileAction->setText(QApplication::translate("ARLMainWindow", "Save to File...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        selectionSaveToFileAction->setToolTip(QApplication::translate("ARLMainWindow", "Save selection to file...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        selectionSaveToFileAction->setStatusTip(QApplication::translate("ARLMainWindow", "Save the selection as a Model to a file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        insertIntoFileAction->setText(QApplication::translate("ARLMainWindow", "Insert from File...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        insertIntoFileAction->setToolTip(QApplication::translate("ARLMainWindow", "Insert from File...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        insertIntoFileAction->setStatusTip(QApplication::translate("ARLMainWindow", "Insert from File...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        publishToRobloxAction->setText(QApplication::translate("ARLMainWindow", "Publish to ANORRL", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        publishToRobloxAction->setToolTip(QApplication::translate("ARLMainWindow", "Publish Active Document to ANORRL", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        publishToRobloxAction->setStatusTip(QApplication::translate("ARLMainWindow", "Save the Active Document to ANORRL Website", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        publishToRobloxAction->setShortcut(QApplication::translate("ARLMainWindow", "Alt+P", 0, QApplication::UnicodeUTF8));
        publishGameAction->setText(QApplication::translate("ARLMainWindow", "Publish Whole Game...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        publishGameAction->setToolTip(QApplication::translate("ARLMainWindow", "Publish all places, developer products, and other parts of your game to ANORRL", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        publishGameAction->setStatusTip(QApplication::translate("ARLMainWindow", "Publish all places, developer products, and other parts of your game to ANORRL", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        aboutRobloxAction->setText(QApplication::translate("ARLMainWindow", "About ANORRL...", 0, QApplication::UnicodeUTF8));
        onlineHelpAction->setText(QApplication::translate("ARLMainWindow", "Online Help...", 0, QApplication::UnicodeUTF8));
        onlineHelpAction->setShortcut(QApplication::translate("ARLMainWindow", "F1", 0, QApplication::UnicodeUTF8));
        objectBrowserAction->setText(QApplication::translate("ARLMainWindow", "Object Browser", 0, QApplication::UnicodeUTF8));
        playSoloAction->setText(QApplication::translate("ARLMainWindow", "Play Solo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        playSoloAction->setToolTip(QApplication::translate("ARLMainWindow", "Play in Solo Mode", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        playSoloAction->setStatusTip(QApplication::translate("ARLMainWindow", "Launch Studio in Solo Player Mode", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        playSoloAction->setShortcut(QApplication::translate("ARLMainWindow", "F6", 0, QApplication::UnicodeUTF8));
        startServerAction->setText(QApplication::translate("ARLMainWindow", "Start Server", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        startServerAction->setToolTip(QApplication::translate("ARLMainWindow", "Start Server", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        startServerAction->setStatusTip(QApplication::translate("ARLMainWindow", "Launch Studio in Server Mode", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        startServerAction->setShortcut(QApplication::translate("ARLMainWindow", "F7", 0, QApplication::UnicodeUTF8));
        startPlayerAction->setText(QApplication::translate("ARLMainWindow", "Start Player", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        startPlayerAction->setToolTip(QApplication::translate("ARLMainWindow", "Start Player", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        startPlayerAction->setStatusTip(QApplication::translate("ARLMainWindow", "Launch Studio in Player Mode", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        startPlayerAction->setShortcut(QApplication::translate("ARLMainWindow", "Alt+F7", 0, QApplication::UnicodeUTF8));
        instanceDumpAction->setText(QApplication::translate("ARLMainWindow", "Instance Dump", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        instanceDumpAction->setStatusTip(QApplication::translate("ARLMainWindow", "Displays ANORRL Studio Active Instances in Output Window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        settingsAction->setText(QApplication::translate("ARLMainWindow", "Settings...", 0, QApplication::UnicodeUTF8));
        settingsAction->setShortcut(QApplication::translate("ARLMainWindow", "Alt+S", 0, QApplication::UnicodeUTF8));
        publishSelectionToRobloxAction->setText(QApplication::translate("ARLMainWindow", "Publish Selection to ANORRL...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        publishSelectionToRobloxAction->setStatusTip(QApplication::translate("ARLMainWindow", "Publish Selection to ANORRL", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        findNextAction->setText(QApplication::translate("ARLMainWindow", "Find Next", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        findNextAction->setToolTip(QApplication::translate("ARLMainWindow", "Find Next", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        findNextAction->setShortcut(QApplication::translate("ARLMainWindow", "F3", 0, QApplication::UnicodeUTF8));
        replaceAction->setText(QApplication::translate("ARLMainWindow", "Replace...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        replaceAction->setToolTip(QApplication::translate("ARLMainWindow", "Replace", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        replaceAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+H", 0, QApplication::UnicodeUTF8));
        advArrowToolAction->setText(QApplication::translate("ARLMainWindow", "Arrow Tool", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        advArrowToolAction->setToolTip(QApplication::translate("ARLMainWindow", "Drag", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        advArrowToolAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+1", 0, QApplication::UnicodeUTF8));
        saveToRobloxAction->setText(QApplication::translate("ARLMainWindow", "Save to ANORRL...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        saveToRobloxAction->setToolTip(QApplication::translate("ARLMainWindow", "Publish the Selection to ANORRL", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        insertServiceAction->setText(QApplication::translate("ARLMainWindow", "Service...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        insertServiceAction->setToolTip(QApplication::translate("ARLMainWindow", "Insert Services.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fastLogDumpAction->setText(QApplication::translate("ARLMainWindow", "Upload Log Files", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fastLogDumpAction->setToolTip(QApplication::translate("ARLMainWindow", "Upload Log Files", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fastLogDumpAction->setShortcut(QApplication::translate("ARLMainWindow", "Shift+F8", 0, QApplication::UnicodeUTF8));
        viewToolboxAction->setText(QApplication::translate("ARLMainWindow", "Toolbox Objects", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewToolboxAction->setToolTip(QApplication::translate("ARLMainWindow", "Insert Objects from Web", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toggleAxisWidgetAction->setText(QApplication::translate("ARLMainWindow", "Axis Widget", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toggleAxisWidgetAction->setToolTip(QApplication::translate("ARLMainWindow", "Shows/Hides Axis Widget", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toggle3DGridAction->setText(QApplication::translate("ARLMainWindow", "3D Grid", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toggle3DGridAction->setToolTip(QApplication::translate("ARLMainWindow", "Shows/Hides 3D Grid", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toggle3DGridAction->setShortcut(QApplication::translate("ARLMainWindow", "Alt+G", 0, QApplication::UnicodeUTF8));
        toggleVideoRecordAction->setText(QApplication::translate("ARLMainWindow", "Video Record", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toggleVideoRecordAction->setToolTip(QApplication::translate("ARLMainWindow", "Toggles video record", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toggleVideoRecordAction->setShortcut(QApplication::translate("ARLMainWindow", "F12", 0, QApplication::UnicodeUTF8));
        filePublishedProjectsAction->setText(QApplication::translate("ARLMainWindow", "Published Projects...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        filePublishedProjectsAction->setToolTip(QApplication::translate("ARLMainWindow", "Open and Configure Published Projects", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        filePublishedProjectsAction->setStatusTip(QApplication::translate("ARLMainWindow", "Open and Configure Published Projects", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        filePublishedProjectsAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+O", 0, QApplication::UnicodeUTF8));
        viewDiagnosticsAction->setText(QApplication::translate("ARLMainWindow", "Diagnostics", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewDiagnosticsAction->setToolTip(QApplication::translate("ARLMainWindow", "View Diagnostics...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        viewTaskSchedulerAction->setText(QApplication::translate("ARLMainWindow", "Task Scheduler", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewTaskSchedulerAction->setToolTip(QApplication::translate("ARLMainWindow", "View Task Scheduler...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        viewScriptPerformanceAction->setText(QApplication::translate("ARLMainWindow", "Script Performance", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewScriptPerformanceAction->setToolTip(QApplication::translate("ARLMainWindow", "View Script Performance...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        viewObjectExplorerAction->setText(QApplication::translate("ARLMainWindow", "Explorer", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewObjectExplorerAction->setToolTip(QApplication::translate("ARLMainWindow", "View Object Explorer", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        viewObjectExplorerAction->setShortcut(QApplication::translate("ARLMainWindow", "Alt+X", 0, QApplication::UnicodeUTF8));
        viewPropertiesAction->setText(QApplication::translate("ARLMainWindow", "Properties", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewPropertiesAction->setToolTip(QApplication::translate("ARLMainWindow", "View Properties...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        viewOutputWindowAction->setText(QApplication::translate("ARLMainWindow", "Output Window", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewOutputWindowAction->setToolTip(QApplication::translate("ARLMainWindow", "View Output Window...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionStartPage->setText(QApplication::translate("ARLMainWindow", "Start Page", 0, QApplication::UnicodeUTF8));
        actionFullScreen->setText(QApplication::translate("ARLMainWindow", "Full Screen", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionFullScreen->setStatusTip(QApplication::translate("ARLMainWindow", "Toggles between full screen and windowed views", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionFullScreen->setShortcut(QApplication::translate("ARLMainWindow", "F11", 0, QApplication::UnicodeUTF8));
        commentSelectionAction->setText(QApplication::translate("ARLMainWindow", "Comment Selection", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        commentSelectionAction->setToolTip(QApplication::translate("ARLMainWindow", "Comment out selected text", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        uncommentSelectionAction->setText(QApplication::translate("ARLMainWindow", "Uncomment Selection", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        uncommentSelectionAction->setToolTip(QApplication::translate("ARLMainWindow", "Uncomment selected text", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toggleCommentAction->setText(QApplication::translate("ARLMainWindow", "Toggle Comment Selection", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toggleCommentAction->setToolTip(QApplication::translate("ARLMainWindow", "Toggle selection commenting", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toggleCommentAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+C", 0, QApplication::UnicodeUTF8));
        toggleCollisionCheckAction->setText(QApplication::translate("ARLMainWindow", "Collision Check", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toggleCollisionCheckAction->setToolTip(QApplication::translate("ARLMainWindow", "Collision Check", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        toggleCollisionCheckAction->setStatusTip(QApplication::translate("ARLMainWindow", "Enables or Disables Collision Check", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        toggleBuildModeAction->setText(QApplication::translate("ARLMainWindow", "Toggle Build Mode", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toggleBuildModeAction->setToolTip(QApplication::translate("ARLMainWindow", "Toggle current build mode", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toggleBuildModeAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Alt+Shift+B", 0, QApplication::UnicodeUTF8));
        openPluginsFolderAction->setText(QApplication::translate("ARLMainWindow", "Open Plugins Folder", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        openPluginsFolderAction->setToolTip(QApplication::translate("ARLMainWindow", "Open Plugins Folder", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        unlockAllAction->setText(QApplication::translate("ARLMainWindow", "Unlock All", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        unlockAllAction->setToolTip(QApplication::translate("ARLMainWindow", "Unlock All", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        shortcutHelpAction->setText(QApplication::translate("ARLMainWindow", "Customize Shortcuts...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        shortcutHelpAction->setToolTip(QApplication::translate("ARLMainWindow", "Show list of shortcut commands", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        findPreviousAction->setText(QApplication::translate("ARLMainWindow", "Find Previous", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        findPreviousAction->setToolTip(QApplication::translate("ARLMainWindow", "Find previous item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        findPreviousAction->setShortcut(QApplication::translate("ARLMainWindow", "Shift+F3", 0, QApplication::UnicodeUTF8));
        goToScriptErrorAction->setText(QApplication::translate("ARLMainWindow", "Go to Script Error", 0, QApplication::UnicodeUTF8));
        expandAllFoldsAction->setText(QApplication::translate("ARLMainWindow", "Expand All Folds", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        expandAllFoldsAction->setToolTip(QApplication::translate("ARLMainWindow", "Expands all folds", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        expandAllFoldsAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+E", 0, QApplication::UnicodeUTF8));
        collapseAllFoldsAction->setText(QApplication::translate("ARLMainWindow", "Collapse All Folds", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        collapseAllFoldsAction->setToolTip(QApplication::translate("ARLMainWindow", "Collapses all folds", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        collapseAllFoldsAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+E", 0, QApplication::UnicodeUTF8));
        renameObjectAction->setText(QApplication::translate("ARLMainWindow", "Rename", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        renameObjectAction->setToolTip(QApplication::translate("ARLMainWindow", "Renames selected object", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        screenShotAction->setText(QApplication::translate("ARLMainWindow", "Take Screenshot", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        screenShotAction->setToolTip(QApplication::translate("ARLMainWindow", "Take Screenshot", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        publishToRobloxAsAction->setText(QApplication::translate("ARLMainWindow", "Publish to ANORRL As...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        publishToRobloxAsAction->setToolTip(QApplication::translate("ARLMainWindow", "Publish Active Document to ANORRL As", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        publishToRobloxAsAction->setShortcut(QApplication::translate("ARLMainWindow", "Alt+Shift+P", 0, QApplication::UnicodeUTF8));
        selectChildrenAction->setText(QApplication::translate("ARLMainWindow", "Select Children", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        selectChildrenAction->setToolTip(QApplication::translate("ARLMainWindow", "Select Children", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        testClearStatsAction->setText(QApplication::translate("ARLMainWindow", "Clear Stats", 0, QApplication::UnicodeUTF8));
        stepIntoAction->setText(QApplication::translate("ARLMainWindow", "Step Into", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        stepIntoAction->setToolTip(QApplication::translate("ARLMainWindow", "Step Into", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        stepIntoAction->setShortcut(QApplication::translate("ARLMainWindow", "F11", 0, QApplication::UnicodeUTF8));
        stepOverAction->setText(QApplication::translate("ARLMainWindow", "Step Over", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        stepOverAction->setToolTip(QApplication::translate("ARLMainWindow", "Step Over", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        stepOverAction->setShortcut(QApplication::translate("ARLMainWindow", "F10", 0, QApplication::UnicodeUTF8));
        stepOutAction->setText(QApplication::translate("ARLMainWindow", "Step Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        stepOutAction->setToolTip(QApplication::translate("ARLMainWindow", "Step Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        stepOutAction->setShortcut(QApplication::translate("ARLMainWindow", "Shift+F11", 0, QApplication::UnicodeUTF8));
        insertBreakpointAction->setText(QApplication::translate("ARLMainWindow", "Insert Breakpoint", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        insertBreakpointAction->setToolTip(QApplication::translate("ARLMainWindow", "Insert Breakpoint", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        deleteBreakpointAction->setText(QApplication::translate("ARLMainWindow", "Delete Breakpoint", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        deleteBreakpointAction->setToolTip(QApplication::translate("ARLMainWindow", "Delete Breakpoint", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toggleBreakpointStateAction->setText(QApplication::translate("ARLMainWindow", "Disable Breakpoint", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toggleBreakpointStateAction->setToolTip(QApplication::translate("ARLMainWindow", "Disable Breakpoint", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        addWatchAction->setText(QApplication::translate("ARLMainWindow", "Add Watch", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        addWatchAction->setToolTip(QApplication::translate("ARLMainWindow", "Add Watch", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        resetScriptZoomAction->setText(QApplication::translate("ARLMainWindow", "Reset Script Zoom", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        resetScriptZoomAction->setToolTip(QApplication::translate("ARLMainWindow", "Resets font size", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        resetScriptZoomAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+0", 0, QApplication::UnicodeUTF8));
        viewContextualHelpAction->setText(QApplication::translate("ARLMainWindow", "Context Help", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewContextualHelpAction->setToolTip(QApplication::translate("ARLMainWindow", "View Context-sensitive Help", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toggleLocalSpaceAction->setText(QApplication::translate("ARLMainWindow", "Toggle Local Space", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toggleLocalSpaceAction->setToolTip(QApplication::translate("ARLMainWindow", "Toggles between local or world space dragging.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        toggleLocalSpaceAction->setStatusTip(QApplication::translate("ARLMainWindow", "Toggles between local or world space dragging.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        toggleLocalSpaceAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+L", 0, QApplication::UnicodeUTF8));
        resetViewAction->setText(QApplication::translate("ARLMainWindow", "Reset View", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        resetViewAction->setToolTip(QApplication::translate("ARLMainWindow", "Resets Widget and Toolbar position", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        gridSizeToTwoAction->setText(QApplication::translate("ARLMainWindow", "Grid Size - 2 studs", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        gridSizeToTwoAction->setToolTip(QApplication::translate("ARLMainWindow", "Set Grid Size to 2 Studs", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        gridSizeToFourAction->setText(QApplication::translate("ARLMainWindow", "Grid Size - 4 Studs", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        gridSizeToFourAction->setToolTip(QApplication::translate("ARLMainWindow", "Set Grid Size to 4 Studs", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        gridSizeToSixteenAction->setText(QApplication::translate("ARLMainWindow", "Grid Size - 16 Studs", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        gridSizeToSixteenAction->setToolTip(QApplication::translate("ARLMainWindow", "Set Grid Size to 16 Studs", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        publishAsPluginAction->setText(QApplication::translate("ARLMainWindow", "Publish as Plugin...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        publishAsPluginAction->setToolTip(QApplication::translate("ARLMainWindow", "Publish the Selection to ANORRL as a Plugin", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        createNewLinkedSourceAction->setText(QApplication::translate("ARLMainWindow", "Create new LinkedSource...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        createNewLinkedSourceAction->setToolTip(QApplication::translate("ARLMainWindow", "Store this script in the cloud so that you can easily re-use it", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        managePluginsAction->setText(QApplication::translate("ARLMainWindow", "Manage Plugins", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        managePluginsAction->setToolTip(QApplication::translate("ARLMainWindow", "Enable, Disable, and Uninstall Plugins", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        quickInsertAction->setText(QApplication::translate("ARLMainWindow", "Quick Insert", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        quickInsertAction->setToolTip(QApplication::translate("ARLMainWindow", "Temporarily set focus to the basic objects widget", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        quickInsertAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+I", 0, QApplication::UnicodeUTF8));
        reloadScriptAction->setText(QApplication::translate("ARLMainWindow", "Reload Script", 0, QApplication::UnicodeUTF8));
        reloadScriptAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        neverBreakOnScriptErrorsAction->setText(QApplication::translate("ARLMainWindow", "Never", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        neverBreakOnScriptErrorsAction->setToolTip(QApplication::translate("ARLMainWindow", "Never break errors into debugger", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        breakOnAllScriptErrorsAction->setText(QApplication::translate("ARLMainWindow", "On All Exceptions", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        breakOnAllScriptErrorsAction->setToolTip(QApplication::translate("ARLMainWindow", "Break all exceptions into debugger", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        breakOnUnhandledScriptErrorsAction->setText(QApplication::translate("ARLMainWindow", "On Unhandled Exceptions", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        breakOnUnhandledScriptErrorsAction->setToolTip(QApplication::translate("ARLMainWindow", "Break unhandled exceptions into debugger", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        viewFindResultsWindowAction->setText(QApplication::translate("ARLMainWindow", "Find Results", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewFindResultsWindowAction->setToolTip(QApplication::translate("ARLMainWindow", "View Find Results", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        findInScriptsAction->setText(QApplication::translate("ARLMainWindow", "Find All", 0, QApplication::UnicodeUTF8));
        findInScriptsAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+F", 0, QApplication::UnicodeUTF8));
        explorerFilterAction->setText(QApplication::translate("ARLMainWindow", "Explorer Filter", 0, QApplication::UnicodeUTF8));
        explorerFilterAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+X", 0, QApplication::UnicodeUTF8));
        exportSelectionAction->setText(QApplication::translate("ARLMainWindow", "Export Selection...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        exportSelectionAction->setToolTip(QApplication::translate("ARLMainWindow", "Export Selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        exportPlaceAction->setText(QApplication::translate("ARLMainWindow", "Export as Obj", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        exportPlaceAction->setToolTip(QApplication::translate("ARLMainWindow", "Export as Obj", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toggleAllBreakpointsStateAction->setText(QApplication::translate("ARLMainWindow", "Toggle All Breakpoints", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toggleAllBreakpointsStateAction->setToolTip(QApplication::translate("ARLMainWindow", "Enable/Disable All Breakpoints", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        duplicateSelectionAction->setText(QApplication::translate("ARLMainWindow", "Duplicate", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        duplicateSelectionAction->setToolTip(QApplication::translate("ARLMainWindow", "Duplicates current selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        duplicateSelectionAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+D", 0, QApplication::UnicodeUTF8));
        gameExplorerAction->setText(QApplication::translate("ARLMainWindow", "Game Explorer", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        gameExplorerAction->setToolTip(QApplication::translate("ARLMainWindow", "View Game Explorer...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        customizeQuickAccessAction->setText(QApplication::translate("ARLMainWindow", "Customize...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        customizeQuickAccessAction->setToolTip(QApplication::translate("ARLMainWindow", "Customize Quick Access", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        unionSelectionAction->setText(QApplication::translate("ARLMainWindow", "Union", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        unionSelectionAction->setToolTip(QApplication::translate("ARLMainWindow", "Union Selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        unionSelectionAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+G", 0, QApplication::UnicodeUTF8));
        negateSelectionAction->setText(QApplication::translate("ARLMainWindow", "Negate", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        negateSelectionAction->setToolTip(QApplication::translate("ARLMainWindow", "Negate Selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        negateSelectionAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+Shift+N", 0, QApplication::UnicodeUTF8));
        separateSelectionAction->setText(QApplication::translate("ARLMainWindow", "Separate", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        separateSelectionAction->setToolTip(QApplication::translate("ARLMainWindow", "Separate parts from a fused selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        cleanupServersAndPlayersAction->setText(QApplication::translate("ARLMainWindow", "Cleans up all clients and servers", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cleanupServersAndPlayersAction->setToolTip(QApplication::translate("ARLMainWindow", "Cleans up all clients and servers", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        playRbxDeviceAction->setText(QApplication::translate("ARLMainWindow", "Play on ARL Dev Device", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        playRbxDeviceAction->setToolTip(QApplication::translate("ARLMainWindow", "Starts a server and your connected Roblox Developer iOS Device", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        playRbxDeviceAction->setStatusTip(QApplication::translate("ARLMainWindow", "Starts a server and your connected Roblox Developer iOS Device", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        playRbxDeviceAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+F7", 0, QApplication::UnicodeUTF8));
        pairRbxDeviceAction->setText(QApplication::translate("ARLMainWindow", "Pair Developer Device", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pairRbxDeviceAction->setToolTip(QApplication::translate("ARLMainWindow", "Pair a device for testing", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        pairRbxDeviceAction->setStatusTip(QApplication::translate("ARLMainWindow", "Pair a device for testing", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        audioToggleAction->setText(QApplication::translate("ARLMainWindow", "Toggle Game Audio", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        audioToggleAction->setToolTip(QApplication::translate("ARLMainWindow", "Toggle Game Audio", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        audioToggleAction->setStatusTip(QApplication::translate("ARLMainWindow", "Toggle Game Audio", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        viewTutorialsAction->setText(QApplication::translate("ARLMainWindow", "Tutorials", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewTutorialsAction->setToolTip(QApplication::translate("ARLMainWindow", "View tutorials", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        viewScriptAnalysisAction->setText(QApplication::translate("ARLMainWindow", "Script Analysis", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewScriptAnalysisAction->setToolTip(QApplication::translate("ARLMainWindow", "View Script Analysis...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        viewTeamCreateAction->setText(QApplication::translate("ARLMainWindow", "Team Create", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewTeamCreateAction->setToolTip(QApplication::translate("ARLMainWindow", "View Team Create Window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        emulateDeviceAction->setText(QApplication::translate("ARLMainWindow", "Emulate Device", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        emulateDeviceAction->setToolTip(QApplication::translate("ARLMainWindow", "Emulate a mobile device ", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        manageEmulationDeviceAction->setText(QApplication::translate("ARLMainWindow", "Manage Devices...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        manageEmulationDeviceAction->setToolTip(QApplication::translate("ARLMainWindow", "Manage emulated devices", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        launchHelpForSelectionAction->setText(QApplication::translate("ARLMainWindow", "Help", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        launchHelpForSelectionAction->setToolTip(QApplication::translate("ARLMainWindow", "Show help for selected object", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        viewCommandBarAction->setText(QApplication::translate("ARLMainWindow", "Command Bar", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        viewCommandBarAction->setToolTip(QApplication::translate("ARLMainWindow", "Command Bar", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fileOpenRecentSavesAction->setText(QApplication::translate("ARLMainWindow", "Open Recent Saves", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileOpenRecentSavesAction->setToolTip(QApplication::translate("ARLMainWindow", "Open Recent Saves Folder", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        fileOpenRecentSavesAction->setStatusTip(QApplication::translate("ARLMainWindow", "Open Recent Saves Folder", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        goToLineAction->setText(QApplication::translate("ARLMainWindow", "Go to line...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        goToLineAction->setToolTip(QApplication::translate("ARLMainWindow", "Go to line", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        goToLineAction->setShortcut(QApplication::translate("ARLMainWindow", "Ctrl+G", 0, QApplication::UnicodeUTF8));
        downloadPlaceCopyAction->setText(QApplication::translate("ARLMainWindow", "Download a Copy...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        downloadPlaceCopyAction->setToolTip(QApplication::translate("ARLMainWindow", "Download a copy of the active document", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        standardToolBar->setWindowTitle(QApplication::translate("ARLMainWindow", "Standard", 0, QApplication::UnicodeUTF8));
        runToolBar->setWindowTitle(QApplication::translate("ARLMainWindow", "Test", 0, QApplication::UnicodeUTF8));
        advToolsToolBar->setWindowTitle(QApplication::translate("ARLMainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        oldToolsToolBar->setWindowTitle(QApplication::translate("ARLMainWindow", "Tools-Classic", 0, QApplication::UnicodeUTF8));
        editCameraToolBar->setWindowTitle(QApplication::translate("ARLMainWindow", "Camera", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("ARLMainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("ARLMainWindow", "&Edit", 0, QApplication::UnicodeUTF8));
        menuFind_and_Replace->setTitle(QApplication::translate("ARLMainWindow", "Find and Replace", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("ARLMainWindow", "&Tools", 0, QApplication::UnicodeUTF8));
        menuTest->setTitle(QApplication::translate("ARLMainWindow", "Test", 0, QApplication::UnicodeUTF8));
        menuStats->setTitle(QApplication::translate("ARLMainWindow", "Stats", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("ARLMainWindow", "&View", 0, QApplication::UnicodeUTF8));
        menuToolBars->setTitle(QApplication::translate("ARLMainWindow", "ToolBars", 0, QApplication::UnicodeUTF8));
        menuFormat->setTitle(QApplication::translate("ARLMainWindow", "For&mat", 0, QApplication::UnicodeUTF8));
        menuInsert->setTitle(QApplication::translate("ARLMainWindow", "&Insert", 0, QApplication::UnicodeUTF8));
        menuWindow->setTitle(QApplication::translate("ARLMainWindow", "Window", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("ARLMainWindow", "Help", 0, QApplication::UnicodeUTF8));
        scriptMenu->setTitle(QApplication::translate("ARLMainWindow", "Script", 0, QApplication::UnicodeUTF8));
        breakErrorsIntoDebuggerMenu->setTitle(QApplication::translate("ARLMainWindow", "Debug Errors", 0, QApplication::UnicodeUTF8));
        outputWindow->setWindowTitle(QApplication::translate("ARLMainWindow", "Output", 0, QApplication::UnicodeUTF8));
        commandToolBar->setWindowTitle(QApplication::translate("ARLMainWindow", "Command", 0, QApplication::UnicodeUTF8));
        objectExplorer->setWindowTitle(QApplication::translate("ARLMainWindow", "Explorer", 0, QApplication::UnicodeUTF8));
        propertyBrowser->setWindowTitle(QApplication::translate("ARLMainWindow", "Properties", 0, QApplication::UnicodeUTF8));
        diagnosticsDockWidget->setWindowTitle(QApplication::translate("ARLMainWindow", "Diagnostics", 0, QApplication::UnicodeUTF8));
        taskSchedulerDockWidget->setWindowTitle(QApplication::translate("ARLMainWindow", "Task Scheduler", 0, QApplication::UnicodeUTF8));
        toolBoxDockWidget->setWindowTitle(QApplication::translate("ARLMainWindow", "Toolbox", 0, QApplication::UnicodeUTF8));
        scriptReviewDockWidget->setWindowTitle(QApplication::translate("ARLMainWindow", "Script Performance", 0, QApplication::UnicodeUTF8));
        gameExplorerDockWidget->setWindowTitle(QApplication::translate("ARLMainWindow", "Game", 0, QApplication::UnicodeUTF8));
        viewToolsToolBar->setWindowTitle(QApplication::translate("ARLMainWindow", "View Tools", 0, QApplication::UnicodeUTF8));
        basicObjectsDockWidget->setWindowTitle(QApplication::translate("ARLMainWindow", "Basic Objects", 0, QApplication::UnicodeUTF8));
        contextualHelp->setWindowTitle(QApplication::translate("ARLMainWindow", "Context Help", 0, QApplication::UnicodeUTF8));
        findResultsWindow->setWindowTitle(QApplication::translate("ARLMainWindow", "Find Results", 0, QApplication::UnicodeUTF8));
        tutorialsDockWidget->setWindowTitle(QApplication::translate("ARLMainWindow", "Tutorials", 0, QApplication::UnicodeUTF8));
        teamCreateDockWidget->setWindowTitle(QApplication::translate("ARLMainWindow", "Team Create - Beta", 0, QApplication::UnicodeUTF8));
        scriptAnalysisDockWidget->setWindowTitle(QApplication::translate("ARLMainWindow", "Script Analysis", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ARLMainWindow: public Ui_ARLMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARLMAINWINDOW_H
