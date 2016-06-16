/********************************************************************************
** Form generated from reading UI file 'WindowMain.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOWMAIN_H
#define UI_WINDOWMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "Canvas.h"
#include "Explorer.h"
#include "ExplorerDirTree.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "ListLogger.h"
#include "LoggerWidget.h"
#include "MenuBar.h"
#include "WindowEventManager.h"

QT_BEGIN_NAMESPACE

class Ui_WindowMain
{
public:
    QAction *actionOpenScene;
    QAction *actionSaveSceneAs;
    QAction *actionCreateMaterial;
    QAction *actionCreateMesh;
    QAction *actionCreateShaderProgram;
    QAction *actionCreateTexture2D;
    QAction *actionProjectSettings;
    QAction *actionCreateFromPrefab;
    QAction *actionEmptyGameObject;
    QAction *actionCreatePrefab;
    QAction *actionRemoveComponent;
    QAction *actionAddComponentMeshRenderer;
    QAction *actionAddComponentCamera;
    QAction *actionAddComponentTransform;
    QAction *actionAddComponentBehaviour;
    QAction *actionCreateEmptyGameObject;
    QAction *actionNewScene;
    QAction *actionSaveScene;
    QAction *actionAddComponentLineRenderer;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_3;
    WindowEventManager *windowEventManager;
    QHBoxLayout *horizontalLayout;
    QToolButton *buttonTranslateMode;
    QToolButton *buttonRotateMode;
    QToolButton *buttonScaleMode;
    QPushButton *buttonGlobalCoords;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonPauseResume;
    Canvas *canvas;
    MenuBar *menubar;
    QMenu *menuAssets;
    QMenu *menuCreate;
    QMenu *menuGameObject;
    QMenu *menuProject;
    QMenu *menuComponents;
    QMenu *menuAddComponent;
    QStatusBar *statusbar;
    QDockWidget *dockHierarchy;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *verticalLayout_4;
    Hierarchy *widgetHierarchy;
    QDockWidget *dockInspector;
    QWidget *dockWidgetContents_7;
    QVBoxLayout *verticalLayout_9;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelInspectorGameObjectName;
    Inspector *widgetInspector;
    QDockWidget *dockExplorer;
    QWidget *dockWidgetContents_5;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_3;
    ExplorerDirTree *widgetTreeExplorerDirTree;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *buttonExplorerDirUp;
    QToolButton *buttonExplorerChangeViewMode;
    Explorer *widgetListExplorer;
    QDockWidget *dockLogger;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_5;
    LoggerWidget *widgetLogger;
    QVBoxLayout *verticalLayout_6;
    ListLogger *listLogger;
    QPushButton *buttonLoggerClear;

    void setupUi(QMainWindow *WindowMain)
    {
        if (WindowMain->objectName().isEmpty())
            WindowMain->setObjectName(QString::fromUtf8("WindowMain"));
        WindowMain->setWindowModality(Qt::NonModal);
        WindowMain->resize(941, 594);
        WindowMain->setMouseTracking(true);
        WindowMain->setFocusPolicy(Qt::StrongFocus);
        actionOpenScene = new QAction(WindowMain);
        actionOpenScene->setObjectName(QString::fromUtf8("actionOpenScene"));
        actionSaveSceneAs = new QAction(WindowMain);
        actionSaveSceneAs->setObjectName(QString::fromUtf8("actionSaveSceneAs"));
        actionCreateMaterial = new QAction(WindowMain);
        actionCreateMaterial->setObjectName(QString::fromUtf8("actionCreateMaterial"));
        actionCreateMesh = new QAction(WindowMain);
        actionCreateMesh->setObjectName(QString::fromUtf8("actionCreateMesh"));
        actionCreateShaderProgram = new QAction(WindowMain);
        actionCreateShaderProgram->setObjectName(QString::fromUtf8("actionCreateShaderProgram"));
        actionCreateTexture2D = new QAction(WindowMain);
        actionCreateTexture2D->setObjectName(QString::fromUtf8("actionCreateTexture2D"));
        actionProjectSettings = new QAction(WindowMain);
        actionProjectSettings->setObjectName(QString::fromUtf8("actionProjectSettings"));
        actionCreateFromPrefab = new QAction(WindowMain);
        actionCreateFromPrefab->setObjectName(QString::fromUtf8("actionCreateFromPrefab"));
        actionEmptyGameObject = new QAction(WindowMain);
        actionEmptyGameObject->setObjectName(QString::fromUtf8("actionEmptyGameObject"));
        actionCreatePrefab = new QAction(WindowMain);
        actionCreatePrefab->setObjectName(QString::fromUtf8("actionCreatePrefab"));
        actionRemoveComponent = new QAction(WindowMain);
        actionRemoveComponent->setObjectName(QString::fromUtf8("actionRemoveComponent"));
        actionAddComponentMeshRenderer = new QAction(WindowMain);
        actionAddComponentMeshRenderer->setObjectName(QString::fromUtf8("actionAddComponentMeshRenderer"));
        actionAddComponentCamera = new QAction(WindowMain);
        actionAddComponentCamera->setObjectName(QString::fromUtf8("actionAddComponentCamera"));
        actionAddComponentTransform = new QAction(WindowMain);
        actionAddComponentTransform->setObjectName(QString::fromUtf8("actionAddComponentTransform"));
        actionAddComponentBehaviour = new QAction(WindowMain);
        actionAddComponentBehaviour->setObjectName(QString::fromUtf8("actionAddComponentBehaviour"));
        actionCreateEmptyGameObject = new QAction(WindowMain);
        actionCreateEmptyGameObject->setObjectName(QString::fromUtf8("actionCreateEmptyGameObject"));
        actionNewScene = new QAction(WindowMain);
        actionNewScene->setObjectName(QString::fromUtf8("actionNewScene"));
        actionSaveScene = new QAction(WindowMain);
        actionSaveScene->setObjectName(QString::fromUtf8("actionSaveScene"));
        actionAddComponentLineRenderer = new QAction(WindowMain);
        actionAddComponentLineRenderer->setObjectName(QString::fromUtf8("actionAddComponentLineRenderer"));
        centralwidget = new QWidget(WindowMain);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        windowEventManager = new WindowEventManager(centralwidget);
        windowEventManager->setObjectName(QString::fromUtf8("windowEventManager"));

        verticalLayout_3->addWidget(windowEventManager);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonTranslateMode = new QToolButton(centralwidget);
        buttonTranslateMode->setObjectName(QString::fromUtf8("buttonTranslateMode"));

        horizontalLayout->addWidget(buttonTranslateMode);

        buttonRotateMode = new QToolButton(centralwidget);
        buttonRotateMode->setObjectName(QString::fromUtf8("buttonRotateMode"));

        horizontalLayout->addWidget(buttonRotateMode);

        buttonScaleMode = new QToolButton(centralwidget);
        buttonScaleMode->setObjectName(QString::fromUtf8("buttonScaleMode"));

        horizontalLayout->addWidget(buttonScaleMode);

        buttonGlobalCoords = new QPushButton(centralwidget);
        buttonGlobalCoords->setObjectName(QString::fromUtf8("buttonGlobalCoords"));
        buttonGlobalCoords->setCheckable(true);
        buttonGlobalCoords->setChecked(true);
        buttonGlobalCoords->setAutoExclusive(false);

        horizontalLayout->addWidget(buttonGlobalCoords);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonPauseResume = new QPushButton(centralwidget);
        buttonPauseResume->setObjectName(QString::fromUtf8("buttonPauseResume"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("pause");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("../../.designer/backup"), QSize(), QIcon::Normal, QIcon::Off);
        }
        buttonPauseResume->setIcon(icon);
        buttonPauseResume->setFlat(false);

        horizontalLayout->addWidget(buttonPauseResume);


        verticalLayout_3->addLayout(horizontalLayout);

        canvas = new Canvas(centralwidget);
        canvas->setObjectName(QString::fromUtf8("canvas"));
        canvas->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(canvas->sizePolicy().hasHeightForWidth());
        canvas->setSizePolicy(sizePolicy);
        canvas->setMouseTracking(true);
        canvas->setFocusPolicy(Qt::StrongFocus);

        verticalLayout_3->addWidget(canvas);


        verticalLayout->addLayout(verticalLayout_3);

        WindowMain->setCentralWidget(centralwidget);
        menubar = new MenuBar(WindowMain);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 941, 25));
        menubar->setNativeMenuBar(false);
        menuAssets = new QMenu(menubar);
        menuAssets->setObjectName(QString::fromUtf8("menuAssets"));
        menuCreate = new QMenu(menuAssets);
        menuCreate->setObjectName(QString::fromUtf8("menuCreate"));
        menuGameObject = new QMenu(menubar);
        menuGameObject->setObjectName(QString::fromUtf8("menuGameObject"));
        menuProject = new QMenu(menubar);
        menuProject->setObjectName(QString::fromUtf8("menuProject"));
        menuComponents = new QMenu(menubar);
        menuComponents->setObjectName(QString::fromUtf8("menuComponents"));
        menuAddComponent = new QMenu(menuComponents);
        menuAddComponent->setObjectName(QString::fromUtf8("menuAddComponent"));
        WindowMain->setMenuBar(menubar);
        statusbar = new QStatusBar(WindowMain);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        WindowMain->setStatusBar(statusbar);
        dockHierarchy = new QDockWidget(WindowMain);
        dockHierarchy->setObjectName(QString::fromUtf8("dockHierarchy"));
        dockHierarchy->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        dockHierarchy->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        verticalLayout_8 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        widgetHierarchy = new Hierarchy(dockWidgetContents_4);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        widgetHierarchy->setHeaderItem(__qtreewidgetitem);
        widgetHierarchy->setObjectName(QString::fromUtf8("widgetHierarchy"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widgetHierarchy->sizePolicy().hasHeightForWidth());
        widgetHierarchy->setSizePolicy(sizePolicy1);
        widgetHierarchy->setContextMenuPolicy(Qt::CustomContextMenu);
        widgetHierarchy->setAcceptDrops(true);
        widgetHierarchy->setAutoScroll(false);
        widgetHierarchy->setDragEnabled(true);
        widgetHierarchy->setDragDropMode(QAbstractItemView::DragDrop);
        widgetHierarchy->setDefaultDropAction(Qt::MoveAction);
        widgetHierarchy->setAlternatingRowColors(false);
        widgetHierarchy->setSelectionMode(QAbstractItemView::ExtendedSelection);
        widgetHierarchy->setSelectionBehavior(QAbstractItemView::SelectRows);
        widgetHierarchy->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        widgetHierarchy->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        widgetHierarchy->setIndentation(10);
        widgetHierarchy->setRootIsDecorated(true);
        widgetHierarchy->setAnimated(true);
        widgetHierarchy->header()->setVisible(false);

        verticalLayout_4->addWidget(widgetHierarchy);


        verticalLayout_8->addLayout(verticalLayout_4);

        dockHierarchy->setWidget(dockWidgetContents_4);
        WindowMain->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockHierarchy);
        dockInspector = new QDockWidget(WindowMain);
        dockInspector->setObjectName(QString::fromUtf8("dockInspector"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dockInspector->sizePolicy().hasHeightForWidth());
        dockInspector->setSizePolicy(sizePolicy2);
        dockInspector->setMinimumSize(QSize(300, 162));
        dockInspector->setFloating(false);
        dockInspector->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        dockInspector->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_7 = new QWidget();
        dockWidgetContents_7->setObjectName(QString::fromUtf8("dockWidgetContents_7"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(dockWidgetContents_7->sizePolicy().hasHeightForWidth());
        dockWidgetContents_7->setSizePolicy(sizePolicy3);
        dockWidgetContents_7->setContextMenuPolicy(Qt::DefaultContextMenu);
        verticalLayout_9 = new QVBoxLayout(dockWidgetContents_7);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        widget = new QWidget(dockWidgetContents_7);
        widget->setObjectName(QString::fromUtf8("widget"));

        verticalLayout_9->addWidget(widget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        labelInspectorGameObjectName = new QLabel(dockWidgetContents_7);
        labelInspectorGameObjectName->setObjectName(QString::fromUtf8("labelInspectorGameObjectName"));

        verticalLayout_2->addWidget(labelInspectorGameObjectName);

        widgetInspector = new Inspector(dockWidgetContents_7);
        widgetInspector->setObjectName(QString::fromUtf8("widgetInspector"));
        sizePolicy1.setHeightForWidth(widgetInspector->sizePolicy().hasHeightForWidth());
        widgetInspector->setSizePolicy(sizePolicy1);
        widgetInspector->setMinimumSize(QSize(0, 0));
        widgetInspector->setContextMenuPolicy(Qt::CustomContextMenu);
        widgetInspector->setLineWidth(4);
        widgetInspector->setMidLineWidth(4);
        widgetInspector->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        widgetInspector->setAutoScroll(false);
        widgetInspector->setDragDropMode(QAbstractItemView::DragOnly);
        widgetInspector->setDefaultDropAction(Qt::IgnoreAction);
        widgetInspector->setAlternatingRowColors(true);
        widgetInspector->setSelectionMode(QAbstractItemView::NoSelection);
        widgetInspector->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        widgetInspector->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        widgetInspector->setMovement(QListView::Snap);
        widgetInspector->setFlow(QListView::TopToBottom);
        widgetInspector->setViewMode(QListView::ListMode);

        verticalLayout_2->addWidget(widgetInspector);

        verticalLayout_2->setStretch(1, 1);

        verticalLayout_9->addLayout(verticalLayout_2);

        dockInspector->setWidget(dockWidgetContents_7);
        WindowMain->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockInspector);
        dockExplorer = new QDockWidget(WindowMain);
        dockExplorer->setObjectName(QString::fromUtf8("dockExplorer"));
        dockExplorer->setMinimumSize(QSize(493, 210));
        dockExplorer->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QString::fromUtf8("dockWidgetContents_5"));
        verticalLayout_10 = new QVBoxLayout(dockWidgetContents_5);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        widgetTreeExplorerDirTree = new ExplorerDirTree(dockWidgetContents_5);
        widgetTreeExplorerDirTree->setObjectName(QString::fromUtf8("widgetTreeExplorerDirTree"));
        widgetTreeExplorerDirTree->setAutoScroll(false);
        widgetTreeExplorerDirTree->setProperty("showDropIndicator", QVariant(true));
        widgetTreeExplorerDirTree->setSortingEnabled(true);
        widgetTreeExplorerDirTree->setAllColumnsShowFocus(true);

        horizontalLayout_3->addWidget(widgetTreeExplorerDirTree);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        buttonExplorerDirUp = new QToolButton(dockWidgetContents_5);
        buttonExplorerDirUp->setObjectName(QString::fromUtf8("buttonExplorerDirUp"));

        horizontalLayout_4->addWidget(buttonExplorerDirUp);

        buttonExplorerChangeViewMode = new QToolButton(dockWidgetContents_5);
        buttonExplorerChangeViewMode->setObjectName(QString::fromUtf8("buttonExplorerChangeViewMode"));

        horizontalLayout_4->addWidget(buttonExplorerChangeViewMode);


        verticalLayout_7->addLayout(horizontalLayout_4);

        widgetListExplorer = new Explorer(dockWidgetContents_5);
        widgetListExplorer->setObjectName(QString::fromUtf8("widgetListExplorer"));
        sizePolicy3.setHeightForWidth(widgetListExplorer->sizePolicy().hasHeightForWidth());
        widgetListExplorer->setSizePolicy(sizePolicy3);
        widgetListExplorer->setAcceptDrops(true);
        widgetListExplorer->setAutoScroll(false);
        widgetListExplorer->setProperty("showDropIndicator", QVariant(true));
        widgetListExplorer->setDragEnabled(true);
        widgetListExplorer->setDragDropMode(QAbstractItemView::DragDrop);
        widgetListExplorer->setDefaultDropAction(Qt::MoveAction);
        widgetListExplorer->setMovement(QListView::Snap);
        widgetListExplorer->setResizeMode(QListView::Adjust);
        widgetListExplorer->setLayoutMode(QListView::Batched);
        widgetListExplorer->setViewMode(QListView::IconMode);
        widgetListExplorer->setUniformItemSizes(true);
        widgetListExplorer->setWordWrap(true);

        verticalLayout_7->addWidget(widgetListExplorer);


        horizontalLayout_3->addLayout(verticalLayout_7);

        horizontalLayout_3->setStretch(1, 5);

        verticalLayout_10->addLayout(horizontalLayout_3);

        verticalLayout_10->setStretch(0, 1);
        dockExplorer->setWidget(dockWidgetContents_5);
        WindowMain->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockExplorer);
        dockLogger = new QDockWidget(WindowMain);
        dockLogger->setObjectName(QString::fromUtf8("dockLogger"));
        dockLogger->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_5 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        widgetLogger = new LoggerWidget(dockWidgetContents);
        widgetLogger->setObjectName(QString::fromUtf8("widgetLogger"));
        verticalLayout_6 = new QVBoxLayout(widgetLogger);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        listLogger = new ListLogger(widgetLogger);
        listLogger->setObjectName(QString::fromUtf8("listLogger"));
        listLogger->setAlternatingRowColors(false);
        listLogger->setSelectionMode(QAbstractItemView::SingleSelection);
        listLogger->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_6->addWidget(listLogger);

        buttonLoggerClear = new QPushButton(widgetLogger);
        buttonLoggerClear->setObjectName(QString::fromUtf8("buttonLoggerClear"));

        verticalLayout_6->addWidget(buttonLoggerClear);


        verticalLayout_5->addWidget(widgetLogger);

        dockLogger->setWidget(dockWidgetContents);
        WindowMain->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockLogger);
        dockExplorer->raise();

        menubar->addAction(menuProject->menuAction());
        menubar->addAction(menuAssets->menuAction());
        menubar->addAction(menuGameObject->menuAction());
        menubar->addAction(menuComponents->menuAction());
        menuAssets->addAction(menuCreate->menuAction());
        menuCreate->addAction(actionCreatePrefab);
        menuCreate->addSeparator();
        menuCreate->addAction(actionCreateMaterial);
        menuCreate->addAction(actionCreateMesh);
        menuCreate->addAction(actionCreateShaderProgram);
        menuCreate->addAction(actionCreateTexture2D);
        menuGameObject->addAction(actionCreateEmptyGameObject);
        menuGameObject->addAction(actionCreateFromPrefab);
        menuProject->addAction(actionNewScene);
        menuProject->addAction(actionOpenScene);
        menuProject->addAction(actionSaveScene);
        menuProject->addAction(actionSaveSceneAs);
        menuProject->addSeparator();
        menuProject->addAction(actionProjectSettings);
        menuComponents->addAction(menuAddComponent->menuAction());
        menuComponents->addAction(actionRemoveComponent);
        menuAddComponent->addAction(actionAddComponentBehaviour);
        menuAddComponent->addAction(actionAddComponentCamera);
        menuAddComponent->addAction(actionAddComponentMeshRenderer);
        menuAddComponent->addAction(actionAddComponentTransform);
        menuAddComponent->addAction(actionAddComponentLineRenderer);

        retranslateUi(WindowMain);
        QObject::connect(widgetHierarchy, SIGNAL(customContextMenuRequested(QPoint)), widgetHierarchy, SLOT(OnCustomContextMenuRequested(QPoint)));
        QObject::connect(buttonPauseResume, SIGNAL(clicked()), canvas, SLOT(OnPauseResumeButtonPressed()));
        QObject::connect(buttonLoggerClear, SIGNAL(clicked()), listLogger, SLOT(clear()));

        QMetaObject::connectSlotsByName(WindowMain);
    } // setupUi

    void retranslateUi(QMainWindow *WindowMain)
    {
        WindowMain->setWindowTitle(QApplication::translate("WindowMain", "Bang Editor", 0, QApplication::UnicodeUTF8));
        actionOpenScene->setText(QApplication::translate("WindowMain", "Open Scene...", 0, QApplication::UnicodeUTF8));
        actionSaveSceneAs->setText(QApplication::translate("WindowMain", "Save Scene as...", 0, QApplication::UnicodeUTF8));
        actionCreateMaterial->setText(QApplication::translate("WindowMain", "Material", 0, QApplication::UnicodeUTF8));
        actionCreateMesh->setText(QApplication::translate("WindowMain", "Mesh", 0, QApplication::UnicodeUTF8));
        actionCreateShaderProgram->setText(QApplication::translate("WindowMain", "ShaderProgram", 0, QApplication::UnicodeUTF8));
        actionCreateTexture2D->setText(QApplication::translate("WindowMain", "Texture2D", 0, QApplication::UnicodeUTF8));
        actionProjectSettings->setText(QApplication::translate("WindowMain", "Project Settings", 0, QApplication::UnicodeUTF8));
        actionCreateFromPrefab->setText(QApplication::translate("WindowMain", "Create from prefab", 0, QApplication::UnicodeUTF8));
        actionEmptyGameObject->setText(QApplication::translate("WindowMain", "Empty GameObject", 0, QApplication::UnicodeUTF8));
        actionCreatePrefab->setText(QApplication::translate("WindowMain", "Prefab", 0, QApplication::UnicodeUTF8));
        actionRemoveComponent->setText(QApplication::translate("WindowMain", "Remove Component", 0, QApplication::UnicodeUTF8));
        actionAddComponentMeshRenderer->setText(QApplication::translate("WindowMain", "Mesh Renderer", 0, QApplication::UnicodeUTF8));
        actionAddComponentCamera->setText(QApplication::translate("WindowMain", "Camera", 0, QApplication::UnicodeUTF8));
        actionAddComponentTransform->setText(QApplication::translate("WindowMain", "Transform", 0, QApplication::UnicodeUTF8));
        actionAddComponentBehaviour->setText(QApplication::translate("WindowMain", "Behaviour", 0, QApplication::UnicodeUTF8));
        actionCreateEmptyGameObject->setText(QApplication::translate("WindowMain", "Create Empty GameObject", 0, QApplication::UnicodeUTF8));
        actionNewScene->setText(QApplication::translate("WindowMain", "New Scene", 0, QApplication::UnicodeUTF8));
        actionSaveScene->setText(QApplication::translate("WindowMain", "Save Scene", 0, QApplication::UnicodeUTF8));
        actionAddComponentLineRenderer->setText(QApplication::translate("WindowMain", "Line Renderer", 0, QApplication::UnicodeUTF8));
        buttonTranslateMode->setText(QApplication::translate("WindowMain", "T", 0, QApplication::UnicodeUTF8));
        buttonRotateMode->setText(QApplication::translate("WindowMain", "R", 0, QApplication::UnicodeUTF8));
        buttonScaleMode->setText(QApplication::translate("WindowMain", "S", 0, QApplication::UnicodeUTF8));
        buttonGlobalCoords->setText(QApplication::translate("WindowMain", "Global", 0, QApplication::UnicodeUTF8));
        buttonPauseResume->setText(QApplication::translate("WindowMain", "Pause", 0, QApplication::UnicodeUTF8));
        menuAssets->setTitle(QApplication::translate("WindowMain", "Assets", 0, QApplication::UnicodeUTF8));
        menuCreate->setTitle(QApplication::translate("WindowMain", "Create", 0, QApplication::UnicodeUTF8));
        menuGameObject->setTitle(QApplication::translate("WindowMain", "GameObjects", 0, QApplication::UnicodeUTF8));
        menuProject->setTitle(QApplication::translate("WindowMain", "Project", 0, QApplication::UnicodeUTF8));
        menuComponents->setTitle(QApplication::translate("WindowMain", "Components", 0, QApplication::UnicodeUTF8));
        menuAddComponent->setTitle(QApplication::translate("WindowMain", "Add Component", 0, QApplication::UnicodeUTF8));
        dockHierarchy->setWindowTitle(QApplication::translate("WindowMain", "Hierarchy", 0, QApplication::UnicodeUTF8));
        dockInspector->setWindowTitle(QApplication::translate("WindowMain", "Inspector", 0, QApplication::UnicodeUTF8));
        labelInspectorGameObjectName->setText(QApplication::translate("WindowMain", "GameObject Name", 0, QApplication::UnicodeUTF8));
        dockExplorer->setWindowTitle(QApplication::translate("WindowMain", "Explorer", 0, QApplication::UnicodeUTF8));
        buttonExplorerDirUp->setText(QApplication::translate("WindowMain", "Up", 0, QApplication::UnicodeUTF8));
        buttonExplorerChangeViewMode->setText(QApplication::translate("WindowMain", "Change view mode", 0, QApplication::UnicodeUTF8));
        dockLogger->setWindowTitle(QApplication::translate("WindowMain", "Logger", 0, QApplication::UnicodeUTF8));
        buttonLoggerClear->setText(QApplication::translate("WindowMain", "Clear", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WindowMain: public Ui_WindowMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWMAIN_H
