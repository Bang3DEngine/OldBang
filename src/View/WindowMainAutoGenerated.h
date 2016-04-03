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
    QAction *actionOpen_project;
    QAction *actionSave_project;
    QAction *actionCreateMaterial;
    QAction *actionCreateMesh;
    QAction *actionCreateShaderProgram;
    QAction *actionCreateTexture2D;
    QAction *actionProject_Settings;
    QAction *actionCreate_from_prefab;
    QAction *actionEmptyEntity;
    QAction *actionCreatePrefab;
    QAction *actionRemovePart;
    QAction *actionAddPartMeshRenderer;
    QAction *actionAddPartCamera;
    QAction *actionAddPartTransform;
    QAction *actionAddPartBehaviour;
    QAction *actionCreateEmptyEntity;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_3;
    WindowEventManager *windowEventManager;
    Canvas *canvas;
    QPushButton *buttonPauseResume;
    MenuBar *menubar;
    QMenu *menuAssets;
    QMenu *menuCreate;
    QMenu *menuEntity;
    QMenu *menuProject;
    QMenu *menuParts;
    QMenu *menuAddPart;
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
    QLabel *labelInspectorEntityName;
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
            WindowMain->setObjectName(QString::fromUtf8("WindowMain"));
        WindowMain->setWindowModality(Qt::NonModal);
        WindowMain->resize(941, 594);
        actionOpen_project = new QAction(WindowMain);
        actionOpen_project->setObjectName(QString::fromUtf8("actionOpen_project"));
        actionSave_project = new QAction(WindowMain);
        actionSave_project->setObjectName(QString::fromUtf8("actionSave_project"));
        actionCreateMaterial = new QAction(WindowMain);
        actionCreateMaterial->setObjectName(QString::fromUtf8("actionCreateMaterial"));
        actionCreateMesh = new QAction(WindowMain);
        actionCreateMesh->setObjectName(QString::fromUtf8("actionCreateMesh"));
        actionCreateShaderProgram = new QAction(WindowMain);
        actionCreateShaderProgram->setObjectName(QString::fromUtf8("actionCreateShaderProgram"));
        actionCreateTexture2D = new QAction(WindowMain);
        actionCreateTexture2D->setObjectName(QString::fromUtf8("actionCreateTexture2D"));
        actionProject_Settings = new QAction(WindowMain);
        actionProject_Settings->setObjectName(QString::fromUtf8("actionProject_Settings"));
        actionCreate_from_prefab = new QAction(WindowMain);
        actionCreate_from_prefab->setObjectName(QString::fromUtf8("actionCreate_from_prefab"));
        actionEmptyEntity = new QAction(WindowMain);
        actionEmptyEntity->setObjectName(QString::fromUtf8("actionEmptyEntity"));
        actionCreatePrefab = new QAction(WindowMain);
        actionCreatePrefab->setObjectName(QString::fromUtf8("actionCreatePrefab"));
        actionRemovePart = new QAction(WindowMain);
        actionRemovePart->setObjectName(QString::fromUtf8("actionRemovePart"));
        actionAddPartMeshRenderer = new QAction(WindowMain);
        actionAddPartMeshRenderer->setObjectName(QString::fromUtf8("actionAddPartMeshRenderer"));
        actionAddPartCamera = new QAction(WindowMain);
        actionAddPartCamera->setObjectName(QString::fromUtf8("actionAddPartCamera"));
        actionAddPartTransform = new QAction(WindowMain);
        actionAddPartTransform->setObjectName(QString::fromUtf8("actionAddPartTransform"));
        actionAddPartBehaviour = new QAction(WindowMain);
        actionAddPartBehaviour->setObjectName(QString::fromUtf8("actionAddPartBehaviour"));
        actionCreateEmptyEntity = new QAction(WindowMain);
        actionCreateEmptyEntity->setObjectName(QString::fromUtf8("actionCreateEmptyEntity"));
        centralwidget = new QWidget(WindowMain);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        windowEventManager = new WindowEventManager(centralwidget);
        windowEventManager->setObjectName(QString::fromUtf8("windowEventManager"));

        verticalLayout_3->addWidget(windowEventManager);


        verticalLayout->addLayout(verticalLayout_3);

        canvas = new Canvas(centralwidget);
        canvas->setObjectName(QString::fromUtf8("canvas"));
        canvas->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(canvas->sizePolicy().hasHeightForWidth());
        canvas->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(canvas);

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

        verticalLayout->addWidget(buttonPauseResume);

        WindowMain->setCentralWidget(centralwidget);
        canvas->raise();
        buttonPauseResume->raise();
        menubar = new MenuBar(WindowMain);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 941, 25));
        menubar->setNativeMenuBar(false);
        menuAssets = new QMenu(menubar);
        menuAssets->setObjectName(QString::fromUtf8("menuAssets"));
        menuCreate = new QMenu(menuAssets);
        menuCreate->setObjectName(QString::fromUtf8("menuCreate"));
        menuEntity = new QMenu(menubar);
        menuEntity->setObjectName(QString::fromUtf8("menuEntity"));
        menuProject = new QMenu(menubar);
        menuProject->setObjectName(QString::fromUtf8("menuProject"));
        menuParts = new QMenu(menubar);
        menuParts->setObjectName(QString::fromUtf8("menuParts"));
        menuAddPart = new QMenu(menuParts);
        menuAddPart->setObjectName(QString::fromUtf8("menuAddPart"));
        WindowMain->setMenuBar(menubar);
        statusbar = new QStatusBar(WindowMain);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        WindowMain->setStatusBar(statusbar);
        dockHierarchy = new QDockWidget(WindowMain);
        dockHierarchy->setObjectName(QString::fromUtf8("dockHierarchy"));
        dockHierarchy->setFeatures(QDockWidget::AllDockWidgetFeatures);
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
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(widgetHierarchy);
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem2);
        new QTreeWidgetItem(__qtreewidgetitem1);
        QTreeWidgetItem *__qtreewidgetitem3 = new QTreeWidgetItem(widgetHierarchy);
        QTreeWidgetItem *__qtreewidgetitem4 = new QTreeWidgetItem(__qtreewidgetitem3);
        QTreeWidgetItem *__qtreewidgetitem5 = new QTreeWidgetItem(__qtreewidgetitem4);
        new QTreeWidgetItem(__qtreewidgetitem5);
        new QTreeWidgetItem(__qtreewidgetitem5);
        new QTreeWidgetItem(__qtreewidgetitem4);
        new QTreeWidgetItem(__qtreewidgetitem4);
        widgetHierarchy->setObjectName(QString::fromUtf8("widgetHierarchy"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widgetHierarchy->sizePolicy().hasHeightForWidth());
        widgetHierarchy->setSizePolicy(sizePolicy1);
        widgetHierarchy->setContextMenuPolicy(Qt::CustomContextMenu);
        widgetHierarchy->setAcceptDrops(true);
        widgetHierarchy->setDragEnabled(true);
        widgetHierarchy->setDragDropMode(QAbstractItemView::DragDrop);
        widgetHierarchy->setDefaultDropAction(Qt::MoveAction);
        widgetHierarchy->setAlternatingRowColors(false);
        widgetHierarchy->setSelectionMode(QAbstractItemView::ExtendedSelection);
        widgetHierarchy->setSelectionBehavior(QAbstractItemView::SelectRows);
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
        dockInspector->setFloating(false);
        dockInspector->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockInspector->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_7 = new QWidget();
        dockWidgetContents_7->setObjectName(QString::fromUtf8("dockWidgetContents_7"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dockWidgetContents_7->sizePolicy().hasHeightForWidth());
        dockWidgetContents_7->setSizePolicy(sizePolicy2);
        dockWidgetContents_7->setContextMenuPolicy(Qt::DefaultContextMenu);
        verticalLayout_9 = new QVBoxLayout(dockWidgetContents_7);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        widget = new QWidget(dockWidgetContents_7);
        widget->setObjectName(QString::fromUtf8("widget"));

        verticalLayout_9->addWidget(widget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        labelInspectorEntityName = new QLabel(dockWidgetContents_7);
        labelInspectorEntityName->setObjectName(QString::fromUtf8("labelInspectorEntityName"));

        verticalLayout_2->addWidget(labelInspectorEntityName);

        widgetInspector = new Inspector(dockWidgetContents_7);
        widgetInspector->setObjectName(QString::fromUtf8("widgetInspector"));
        widgetInspector->setContextMenuPolicy(Qt::CustomContextMenu);
        widgetInspector->setDragDropMode(QAbstractItemView::DragOnly);
        widgetInspector->setDefaultDropAction(Qt::IgnoreAction);
        widgetInspector->setAlternatingRowColors(true);
        widgetInspector->setSelectionMode(QAbstractItemView::NoSelection);
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
        dockExplorer->setMinimumSize(QSize(493, 191));
        dockExplorer->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QString::fromUtf8("dockWidgetContents_5"));
        verticalLayout_10 = new QVBoxLayout(dockWidgetContents_5);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        widgetTreeExplorerDirTree = new ExplorerDirTree(dockWidgetContents_5);
        widgetTreeExplorerDirTree->setObjectName(QString::fromUtf8("widgetTreeExplorerDirTree"));
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
        sizePolicy2.setHeightForWidth(widgetListExplorer->sizePolicy().hasHeightForWidth());
        widgetListExplorer->setSizePolicy(sizePolicy2);
        widgetListExplorer->setAcceptDrops(true);
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
        buttonCreatePrefab->setObjectName(QString::fromUtf8("buttonCreatePrefab"));
        sizePolicy2.setHeightForWidth(buttonCreatePrefab->sizePolicy().hasHeightForWidth());
        buttonCreatePrefab->setSizePolicy(sizePolicy2);
        buttonCreatePrefab->setDefault(true);

        verticalLayout_10->addWidget(buttonCreatePrefab);

        verticalLayout_10->setStretch(0, 1);
        dockExplorer->setWidget(dockWidgetContents_5);
        WindowMain->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockExplorer);
        dockLogger = new QDockWidget(WindowMain);
        dockLogger->setObjectName(QString::fromUtf8("dockLogger"));
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
        menubar->addAction(menuEntity->menuAction());
        menubar->addAction(menuAssets->menuAction());
        menubar->addAction(menuParts->menuAction());
        menuAssets->addAction(menuCreate->menuAction());
        menuCreate->addAction(actionCreatePrefab);
        menuCreate->addSeparator();
        menuCreate->addAction(actionCreateMaterial);
        menuCreate->addAction(actionCreateMesh);
        menuCreate->addAction(actionCreateShaderProgram);
        menuCreate->addAction(actionCreateTexture2D);
        menuEntity->addAction(actionCreateEmptyEntity);
        menuEntity->addAction(actionCreate_from_prefab);
        menuProject->addAction(actionOpen_project);
        menuProject->addAction(actionSave_project);
        menuProject->addAction(actionProject_Settings);
        menuParts->addAction(menuAddPart->menuAction());
        menuParts->addAction(actionRemovePart);
        menuAddPart->addAction(actionAddPartBehaviour);
        menuAddPart->addAction(actionAddPartCamera);
        menuAddPart->addAction(actionAddPartMeshRenderer);
        menuAddPart->addAction(actionAddPartTransform);

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
        WindowMain->setWindowTitle(QApplication::translate("WindowMain", "Bang Editor", 0, QApplication::UnicodeUTF8));
        actionOpen_project->setText(QApplication::translate("WindowMain", "Open project", 0, QApplication::UnicodeUTF8));
        actionSave_project->setText(QApplication::translate("WindowMain", "Save project", 0, QApplication::UnicodeUTF8));
        actionCreateMaterial->setText(QApplication::translate("WindowMain", "Material", 0, QApplication::UnicodeUTF8));
        actionCreateMesh->setText(QApplication::translate("WindowMain", "Mesh", 0, QApplication::UnicodeUTF8));
        actionCreateShaderProgram->setText(QApplication::translate("WindowMain", "ShaderProgram", 0, QApplication::UnicodeUTF8));
        actionCreateTexture2D->setText(QApplication::translate("WindowMain", "Texture2D", 0, QApplication::UnicodeUTF8));
        actionProject_Settings->setText(QApplication::translate("WindowMain", "Project Settings", 0, QApplication::UnicodeUTF8));
        actionCreate_from_prefab->setText(QApplication::translate("WindowMain", "Create from prefab", 0, QApplication::UnicodeUTF8));
        actionEmptyEntity->setText(QApplication::translate("WindowMain", "Empty Entity", 0, QApplication::UnicodeUTF8));
        actionCreatePrefab->setText(QApplication::translate("WindowMain", "Prefab", 0, QApplication::UnicodeUTF8));
        actionRemovePart->setText(QApplication::translate("WindowMain", "Remove Part", 0, QApplication::UnicodeUTF8));
        actionAddPartMeshRenderer->setText(QApplication::translate("WindowMain", "Mesh Renderer", 0, QApplication::UnicodeUTF8));
        actionAddPartCamera->setText(QApplication::translate("WindowMain", "Camera", 0, QApplication::UnicodeUTF8));
        actionAddPartTransform->setText(QApplication::translate("WindowMain", "Transform", 0, QApplication::UnicodeUTF8));
        actionAddPartBehaviour->setText(QApplication::translate("WindowMain", "Behaviour", 0, QApplication::UnicodeUTF8));
        actionCreateEmptyEntity->setText(QApplication::translate("WindowMain", "Create Empty Entity", 0, QApplication::UnicodeUTF8));
        buttonPauseResume->setText(QApplication::translate("WindowMain", "Pause", 0, QApplication::UnicodeUTF8));
        menuAssets->setTitle(QApplication::translate("WindowMain", "Assets", 0, QApplication::UnicodeUTF8));
        menuCreate->setTitle(QApplication::translate("WindowMain", "Create", 0, QApplication::UnicodeUTF8));
        menuEntity->setTitle(QApplication::translate("WindowMain", "Entities", 0, QApplication::UnicodeUTF8));
        menuProject->setTitle(QApplication::translate("WindowMain", "Project", 0, QApplication::UnicodeUTF8));
        menuParts->setTitle(QApplication::translate("WindowMain", "Parts", 0, QApplication::UnicodeUTF8));
        menuAddPart->setTitle(QApplication::translate("WindowMain", "Add Part", 0, QApplication::UnicodeUTF8));
        dockHierarchy->setWindowTitle(QApplication::translate("WindowMain", "Hierarchy", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = widgetHierarchy->isSortingEnabled();
        widgetHierarchy->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem = widgetHierarchy->topLevelItem(0);
        ___qtreewidgetitem->setText(0, QApplication::translate("WindowMain", "aaaaaa", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = ___qtreewidgetitem->child(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("WindowMain", "bbbbbb", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(0, QApplication::translate("WindowMain", "cccccc", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem3 = ___qtreewidgetitem->child(1);
        ___qtreewidgetitem3->setText(0, QApplication::translate("WindowMain", "dddddd", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem4 = widgetHierarchy->topLevelItem(1);
        ___qtreewidgetitem4->setText(0, QApplication::translate("WindowMain", "xxxxxxxx", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem5 = ___qtreewidgetitem4->child(0);
        ___qtreewidgetitem5->setText(0, QApplication::translate("WindowMain", "wwwwwwwwwwwww", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem6 = ___qtreewidgetitem5->child(0);
        ___qtreewidgetitem6->setText(0, QApplication::translate("WindowMain", "eeeeeeeeeeeeeee", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem7 = ___qtreewidgetitem6->child(0);
        ___qtreewidgetitem7->setText(0, QApplication::translate("WindowMain", "rrrrrrrrrrrrrrrrrr", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem8 = ___qtreewidgetitem6->child(1);
        ___qtreewidgetitem8->setText(0, QApplication::translate("WindowMain", "wwwwwwww", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem9 = ___qtreewidgetitem5->child(1);
        ___qtreewidgetitem9->setText(0, QApplication::translate("WindowMain", "qqqqqqqqqqqq", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem10 = ___qtreewidgetitem5->child(2);
        ___qtreewidgetitem10->setText(0, QApplication::translate("WindowMain", "qqqqqqqqqqqqqssssssssssssss", 0, QApplication::UnicodeUTF8));
        widgetHierarchy->setSortingEnabled(__sortingEnabled);

        dockInspector->setWindowTitle(QApplication::translate("WindowMain", "Inspector", 0, QApplication::UnicodeUTF8));
        labelInspectorEntityName->setText(QApplication::translate("WindowMain", "Entity Name", 0, QApplication::UnicodeUTF8));
        dockExplorer->setWindowTitle(QApplication::translate("WindowMain", "Explorer", 0, QApplication::UnicodeUTF8));
        buttonExplorerDirUp->setText(QApplication::translate("WindowMain", "Up", 0, QApplication::UnicodeUTF8));
        buttonExplorerChangeViewMode->setText(QApplication::translate("WindowMain", "Change view mode", 0, QApplication::UnicodeUTF8));
        buttonCreatePrefab->setText(QApplication::translate("WindowMain", "Create prefab", 0, QApplication::UnicodeUTF8));
        dockLogger->setWindowTitle(QApplication::translate("WindowMain", "Logger", 0, QApplication::UnicodeUTF8));
        buttonLoggerClear->setText(QApplication::translate("WindowMain", "Clear", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WindowMain: public Ui_WindowMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWMAIN_H
