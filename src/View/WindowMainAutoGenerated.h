/********************************************************************************
** Form generated from reading UI file 'WindowMain.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOWMAIN_H
#define UI_WINDOWMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
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
    Canvas *canvas;
    QPushButton *buttonPauseResume;
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
    QPushButton *buttonCreatePrefab;
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
            WindowMain->setObjectName(QStringLiteral("WindowMain"));
        WindowMain->setWindowModality(Qt::NonModal);
        WindowMain->resize(941, 594);
        WindowMain->setMouseTracking(true);
        WindowMain->setFocusPolicy(Qt::StrongFocus);
        actionOpenScene = new QAction(WindowMain);
        actionOpenScene->setObjectName(QStringLiteral("actionOpenScene"));
        actionSaveSceneAs = new QAction(WindowMain);
        actionSaveSceneAs->setObjectName(QStringLiteral("actionSaveSceneAs"));
        actionCreateMaterial = new QAction(WindowMain);
        actionCreateMaterial->setObjectName(QStringLiteral("actionCreateMaterial"));
        actionCreateMesh = new QAction(WindowMain);
        actionCreateMesh->setObjectName(QStringLiteral("actionCreateMesh"));
        actionCreateShaderProgram = new QAction(WindowMain);
        actionCreateShaderProgram->setObjectName(QStringLiteral("actionCreateShaderProgram"));
        actionCreateTexture2D = new QAction(WindowMain);
        actionCreateTexture2D->setObjectName(QStringLiteral("actionCreateTexture2D"));
        actionProjectSettings = new QAction(WindowMain);
        actionProjectSettings->setObjectName(QStringLiteral("actionProjectSettings"));
        actionCreateFromPrefab = new QAction(WindowMain);
        actionCreateFromPrefab->setObjectName(QStringLiteral("actionCreateFromPrefab"));
        actionEmptyGameObject = new QAction(WindowMain);
        actionEmptyGameObject->setObjectName(QStringLiteral("actionEmptyGameObject"));
        actionCreatePrefab = new QAction(WindowMain);
        actionCreatePrefab->setObjectName(QStringLiteral("actionCreatePrefab"));
        actionRemoveComponent = new QAction(WindowMain);
        actionRemoveComponent->setObjectName(QStringLiteral("actionRemoveComponent"));
        actionAddComponentMeshRenderer = new QAction(WindowMain);
        actionAddComponentMeshRenderer->setObjectName(QStringLiteral("actionAddComponentMeshRenderer"));
        actionAddComponentCamera = new QAction(WindowMain);
        actionAddComponentCamera->setObjectName(QStringLiteral("actionAddComponentCamera"));
        actionAddComponentTransform = new QAction(WindowMain);
        actionAddComponentTransform->setObjectName(QStringLiteral("actionAddComponentTransform"));
        actionAddComponentBehaviour = new QAction(WindowMain);
        actionAddComponentBehaviour->setObjectName(QStringLiteral("actionAddComponentBehaviour"));
        actionCreateEmptyGameObject = new QAction(WindowMain);
        actionCreateEmptyGameObject->setObjectName(QStringLiteral("actionCreateEmptyGameObject"));
        actionNewScene = new QAction(WindowMain);
        actionNewScene->setObjectName(QStringLiteral("actionNewScene"));
        actionSaveScene = new QAction(WindowMain);
        actionSaveScene->setObjectName(QStringLiteral("actionSaveScene"));
        actionAddComponentLineRenderer = new QAction(WindowMain);
        actionAddComponentLineRenderer->setObjectName(QStringLiteral("actionAddComponentLineRenderer"));
        centralwidget = new QWidget(WindowMain);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        windowEventManager = new WindowEventManager(centralwidget);
        windowEventManager->setObjectName(QStringLiteral("windowEventManager"));

        verticalLayout_3->addWidget(windowEventManager);


        verticalLayout->addLayout(verticalLayout_3);

        canvas = new Canvas(centralwidget);
        canvas->setObjectName(QStringLiteral("canvas"));
        canvas->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(canvas->sizePolicy().hasHeightForWidth());
        canvas->setSizePolicy(sizePolicy);
        canvas->setMouseTracking(true);
        canvas->setFocusPolicy(Qt::StrongFocus);

        verticalLayout->addWidget(canvas);

        buttonPauseResume = new QPushButton(centralwidget);
        buttonPauseResume->setObjectName(QStringLiteral("buttonPauseResume"));
        QIcon icon;
        QString iconThemeName = QStringLiteral("pause");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QStringLiteral("../../.designer/backup"), QSize(), QIcon::Normal, QIcon::Off);
        }
        buttonPauseResume->setIcon(icon);
        buttonPauseResume->setFlat(false);

        verticalLayout->addWidget(buttonPauseResume);

        WindowMain->setCentralWidget(centralwidget);
        buttonPauseResume->raise();
        canvas->raise();
        menubar = new MenuBar(WindowMain);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 941, 25));
        menubar->setNativeMenuBar(false);
        menuAssets = new QMenu(menubar);
        menuAssets->setObjectName(QStringLiteral("menuAssets"));
        menuCreate = new QMenu(menuAssets);
        menuCreate->setObjectName(QStringLiteral("menuCreate"));
        menuGameObject = new QMenu(menubar);
        menuGameObject->setObjectName(QStringLiteral("menuGameObject"));
        menuProject = new QMenu(menubar);
        menuProject->setObjectName(QStringLiteral("menuProject"));
        menuComponents = new QMenu(menubar);
        menuComponents->setObjectName(QStringLiteral("menuComponents"));
        menuAddComponent = new QMenu(menuComponents);
        menuAddComponent->setObjectName(QStringLiteral("menuAddComponent"));
        WindowMain->setMenuBar(menubar);
        statusbar = new QStatusBar(WindowMain);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        WindowMain->setStatusBar(statusbar);
        dockHierarchy = new QDockWidget(WindowMain);
        dockHierarchy->setObjectName(QStringLiteral("dockHierarchy"));
        dockHierarchy->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockHierarchy->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        verticalLayout_8 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        widgetHierarchy = new Hierarchy(dockWidgetContents_4);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        widgetHierarchy->setHeaderItem(__qtreewidgetitem);
        widgetHierarchy->setObjectName(QStringLiteral("widgetHierarchy"));
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
        dockInspector->setObjectName(QStringLiteral("dockInspector"));
        dockInspector->setFloating(false);
        dockInspector->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockInspector->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_7 = new QWidget();
        dockWidgetContents_7->setObjectName(QStringLiteral("dockWidgetContents_7"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dockWidgetContents_7->sizePolicy().hasHeightForWidth());
        dockWidgetContents_7->setSizePolicy(sizePolicy2);
        dockWidgetContents_7->setContextMenuPolicy(Qt::DefaultContextMenu);
        verticalLayout_9 = new QVBoxLayout(dockWidgetContents_7);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        widget = new QWidget(dockWidgetContents_7);
        widget->setObjectName(QStringLiteral("widget"));

        verticalLayout_9->addWidget(widget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        labelInspectorGameObjectName = new QLabel(dockWidgetContents_7);
        labelInspectorGameObjectName->setObjectName(QStringLiteral("labelInspectorGameObjectName"));

        verticalLayout_2->addWidget(labelInspectorGameObjectName);

        widgetInspector = new Inspector(dockWidgetContents_7);
        widgetInspector->setObjectName(QStringLiteral("widgetInspector"));
        widgetInspector->setContextMenuPolicy(Qt::CustomContextMenu);
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
        dockExplorer->setObjectName(QStringLiteral("dockExplorer"));
        dockExplorer->setMinimumSize(QSize(493, 210));
        dockExplorer->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QStringLiteral("dockWidgetContents_5"));
        verticalLayout_10 = new QVBoxLayout(dockWidgetContents_5);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        widgetTreeExplorerDirTree = new ExplorerDirTree(dockWidgetContents_5);
        widgetTreeExplorerDirTree->setObjectName(QStringLiteral("widgetTreeExplorerDirTree"));
        widgetTreeExplorerDirTree->setAutoScroll(false);
        widgetTreeExplorerDirTree->setProperty("showDropIndicator", QVariant(true));
        widgetTreeExplorerDirTree->setSortingEnabled(true);
        widgetTreeExplorerDirTree->setAllColumnsShowFocus(true);

        horizontalLayout_3->addWidget(widgetTreeExplorerDirTree);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        buttonExplorerDirUp = new QToolButton(dockWidgetContents_5);
        buttonExplorerDirUp->setObjectName(QStringLiteral("buttonExplorerDirUp"));

        horizontalLayout_4->addWidget(buttonExplorerDirUp);

        buttonExplorerChangeViewMode = new QToolButton(dockWidgetContents_5);
        buttonExplorerChangeViewMode->setObjectName(QStringLiteral("buttonExplorerChangeViewMode"));

        horizontalLayout_4->addWidget(buttonExplorerChangeViewMode);


        verticalLayout_7->addLayout(horizontalLayout_4);

        widgetListExplorer = new Explorer(dockWidgetContents_5);
        widgetListExplorer->setObjectName(QStringLiteral("widgetListExplorer"));
        sizePolicy2.setHeightForWidth(widgetListExplorer->sizePolicy().hasHeightForWidth());
        widgetListExplorer->setSizePolicy(sizePolicy2);
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

        buttonCreatePrefab = new QPushButton(dockWidgetContents_5);
        buttonCreatePrefab->setObjectName(QStringLiteral("buttonCreatePrefab"));
        sizePolicy2.setHeightForWidth(buttonCreatePrefab->sizePolicy().hasHeightForWidth());
        buttonCreatePrefab->setSizePolicy(sizePolicy2);
        buttonCreatePrefab->setDefault(true);

        verticalLayout_10->addWidget(buttonCreatePrefab);

        verticalLayout_10->setStretch(0, 1);
        dockExplorer->setWidget(dockWidgetContents_5);
        WindowMain->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockExplorer);
        dockLogger = new QDockWidget(WindowMain);
        dockLogger->setObjectName(QStringLiteral("dockLogger"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_5 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        widgetLogger = new LoggerWidget(dockWidgetContents);
        widgetLogger->setObjectName(QStringLiteral("widgetLogger"));
        verticalLayout_6 = new QVBoxLayout(widgetLogger);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        listLogger = new ListLogger(widgetLogger);
        listLogger->setObjectName(QStringLiteral("listLogger"));
        listLogger->setAlternatingRowColors(false);
        listLogger->setSelectionMode(QAbstractItemView::SingleSelection);
        listLogger->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_6->addWidget(listLogger);

        buttonLoggerClear = new QPushButton(widgetLogger);
        buttonLoggerClear->setObjectName(QStringLiteral("buttonLoggerClear"));

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
        QObject::connect(widgetHierarchy, SIGNAL(itemSelectionChanged()), widgetHierarchy, SLOT(_NotifyHierarchyItemSelectionChanged()));
        QObject::connect(buttonCreatePrefab, SIGNAL(clicked()), canvas, SLOT(OnTopKekPressed()));
        QObject::connect(buttonPauseResume, SIGNAL(clicked()), canvas, SLOT(OnPauseResumeButtonPressed()));
        QObject::connect(buttonLoggerClear, SIGNAL(clicked()), listLogger, SLOT(clear()));

        QMetaObject::connectSlotsByName(WindowMain);
    } // setupUi

    void retranslateUi(QMainWindow *WindowMain)
    {
        WindowMain->setWindowTitle(QApplication::translate("WindowMain", "Bang Editor", 0));
        actionOpenScene->setText(QApplication::translate("WindowMain", "Open Scene...", 0));
        actionSaveSceneAs->setText(QApplication::translate("WindowMain", "Save Scene as...", 0));
        actionCreateMaterial->setText(QApplication::translate("WindowMain", "Material", 0));
        actionCreateMesh->setText(QApplication::translate("WindowMain", "Mesh", 0));
        actionCreateShaderProgram->setText(QApplication::translate("WindowMain", "ShaderProgram", 0));
        actionCreateTexture2D->setText(QApplication::translate("WindowMain", "Texture2D", 0));
        actionProjectSettings->setText(QApplication::translate("WindowMain", "Project Settings", 0));
        actionCreateFromPrefab->setText(QApplication::translate("WindowMain", "Create from prefab", 0));
        actionEmptyGameObject->setText(QApplication::translate("WindowMain", "Empty GameObject", 0));
        actionCreatePrefab->setText(QApplication::translate("WindowMain", "Prefab", 0));
        actionRemoveComponent->setText(QApplication::translate("WindowMain", "Remove Component", 0));
        actionAddComponentMeshRenderer->setText(QApplication::translate("WindowMain", "Mesh Renderer", 0));
        actionAddComponentCamera->setText(QApplication::translate("WindowMain", "Camera", 0));
        actionAddComponentTransform->setText(QApplication::translate("WindowMain", "Transform", 0));
        actionAddComponentBehaviour->setText(QApplication::translate("WindowMain", "Behaviour", 0));
        actionCreateEmptyGameObject->setText(QApplication::translate("WindowMain", "Create Empty GameObject", 0));
        actionNewScene->setText(QApplication::translate("WindowMain", "New Scene", 0));
        actionSaveScene->setText(QApplication::translate("WindowMain", "Save Scene", 0));
        actionAddComponentLineRenderer->setText(QApplication::translate("WindowMain", "Line Renderer", 0));
        buttonPauseResume->setText(QApplication::translate("WindowMain", "Pause", 0));
        menuAssets->setTitle(QApplication::translate("WindowMain", "Assets", 0));
        menuCreate->setTitle(QApplication::translate("WindowMain", "Create", 0));
        menuGameObject->setTitle(QApplication::translate("WindowMain", "GameObjects", 0));
        menuProject->setTitle(QApplication::translate("WindowMain", "Project", 0));
        menuComponents->setTitle(QApplication::translate("WindowMain", "Components", 0));
        menuAddComponent->setTitle(QApplication::translate("WindowMain", "Add Component", 0));
        dockHierarchy->setWindowTitle(QApplication::translate("WindowMain", "Hierarchy", 0));
        dockInspector->setWindowTitle(QApplication::translate("WindowMain", "Inspector", 0));
        labelInspectorGameObjectName->setText(QApplication::translate("WindowMain", "GameObject Name", 0));
        dockExplorer->setWindowTitle(QApplication::translate("WindowMain", "Explorer", 0));
        buttonExplorerDirUp->setText(QApplication::translate("WindowMain", "Up", 0));
        buttonExplorerChangeViewMode->setText(QApplication::translate("WindowMain", "Change view mode", 0));
        buttonCreatePrefab->setText(QApplication::translate("WindowMain", "Create prefab", 0));
        dockLogger->setWindowTitle(QApplication::translate("WindowMain", "Logger", 0));
        buttonLoggerClear->setText(QApplication::translate("WindowMain", "Clear", 0));
    } // retranslateUi

};

namespace Ui {
    class WindowMain: public Ui_WindowMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWMAIN_H
