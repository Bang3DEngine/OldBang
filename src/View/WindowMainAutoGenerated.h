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
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Canvas.h"
#include "Explorer.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "ListLogger.h"
#include "LoggerWidget.h"
#include "WindowEventManager.h"

QT_BEGIN_NAMESPACE

class Ui_WindowMain
{
public:
    QAction *actionOpen_project;
    QAction *actionSave_project;
    QAction *actionMaterial;
    QAction *actionMesh;
    QAction *actionShaderProgram;
    QAction *actionTexture2D;
    QAction *actionProject_Settings;
    QAction *actionCreate_from_prefab;
    QAction *actionPrefab;
    QAction *actionPrefab_2;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_3;
    WindowEventManager *windowEventManager;
    QPushButton *buttonPauseResume;
    Canvas *canvas;
    QMenuBar *menubar;
    QMenu *menuAssets;
    QMenu *menuCreate;
    QMenu *menuEntity;
    QMenu *menuProject;
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
    QPushButton *buttonCreatePrefab;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *buttonExplorerDirUp;
    QToolButton *buttonExplorerChangeViewMode;
    Explorer *widgetTreeExplorer;
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
        actionOpen_project = new QAction(WindowMain);
        actionOpen_project->setObjectName(QStringLiteral("actionOpen_project"));
        actionSave_project = new QAction(WindowMain);
        actionSave_project->setObjectName(QStringLiteral("actionSave_project"));
        actionMaterial = new QAction(WindowMain);
        actionMaterial->setObjectName(QStringLiteral("actionMaterial"));
        actionMesh = new QAction(WindowMain);
        actionMesh->setObjectName(QStringLiteral("actionMesh"));
        actionShaderProgram = new QAction(WindowMain);
        actionShaderProgram->setObjectName(QStringLiteral("actionShaderProgram"));
        actionTexture2D = new QAction(WindowMain);
        actionTexture2D->setObjectName(QStringLiteral("actionTexture2D"));
        actionProject_Settings = new QAction(WindowMain);
        actionProject_Settings->setObjectName(QStringLiteral("actionProject_Settings"));
        actionCreate_from_prefab = new QAction(WindowMain);
        actionCreate_from_prefab->setObjectName(QStringLiteral("actionCreate_from_prefab"));
        actionPrefab = new QAction(WindowMain);
        actionPrefab->setObjectName(QStringLiteral("actionPrefab"));
        actionPrefab_2 = new QAction(WindowMain);
        actionPrefab_2->setObjectName(QStringLiteral("actionPrefab_2"));
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

        buttonPauseResume = new QPushButton(centralwidget);
        buttonPauseResume->setObjectName(QStringLiteral("buttonPauseResume"));
        QIcon icon;
        QString iconThemeName = QStringLiteral("pause");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QStringLiteral(""), QSize(), QIcon::Normal, QIcon::Off);
        }
        buttonPauseResume->setIcon(icon);
        buttonPauseResume->setFlat(false);

        verticalLayout->addWidget(buttonPauseResume);

        canvas = new Canvas(centralwidget);
        canvas->setObjectName(QStringLiteral("canvas"));
        canvas->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(canvas->sizePolicy().hasHeightForWidth());
        canvas->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(canvas);

        WindowMain->setCentralWidget(centralwidget);
        buttonPauseResume->raise();
        canvas->raise();
        menubar = new QMenuBar(WindowMain);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 941, 25));
        menubar->setNativeMenuBar(false);
        menuAssets = new QMenu(menubar);
        menuAssets->setObjectName(QStringLiteral("menuAssets"));
        menuCreate = new QMenu(menuAssets);
        menuCreate->setObjectName(QStringLiteral("menuCreate"));
        menuEntity = new QMenu(menubar);
        menuEntity->setObjectName(QStringLiteral("menuEntity"));
        menuProject = new QMenu(menubar);
        menuProject->setObjectName(QStringLiteral("menuProject"));
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
        widgetHierarchy->setObjectName(QStringLiteral("widgetHierarchy"));
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
        labelInspectorEntityName = new QLabel(dockWidgetContents_7);
        labelInspectorEntityName->setObjectName(QStringLiteral("labelInspectorEntityName"));

        verticalLayout_2->addWidget(labelInspectorEntityName);

        widgetInspector = new Inspector(dockWidgetContents_7);
        widgetInspector->setObjectName(QStringLiteral("widgetInspector"));
        widgetInspector->setDefaultDropAction(Qt::MoveAction);
        widgetInspector->setAlternatingRowColors(true);
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
        dockExplorer->setMinimumSize(QSize(420, 158));
        dockExplorer->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QStringLiteral("dockWidgetContents_5"));
        verticalLayout_10 = new QVBoxLayout(dockWidgetContents_5);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        buttonCreatePrefab = new QPushButton(dockWidgetContents_5);
        buttonCreatePrefab->setObjectName(QStringLiteral("buttonCreatePrefab"));
        sizePolicy2.setHeightForWidth(buttonCreatePrefab->sizePolicy().hasHeightForWidth());
        buttonCreatePrefab->setSizePolicy(sizePolicy2);
        buttonCreatePrefab->setDefault(true);

        horizontalLayout_3->addWidget(buttonCreatePrefab);

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

        widgetTreeExplorer = new Explorer(dockWidgetContents_5);
        widgetTreeExplorer->setObjectName(QStringLiteral("widgetTreeExplorer"));
        sizePolicy2.setHeightForWidth(widgetTreeExplorer->sizePolicy().hasHeightForWidth());
        widgetTreeExplorer->setSizePolicy(sizePolicy2);
        widgetTreeExplorer->setAcceptDrops(true);

        verticalLayout_7->addWidget(widgetTreeExplorer);


        horizontalLayout_3->addLayout(verticalLayout_7);


        verticalLayout_10->addLayout(horizontalLayout_3);

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

        menubar->addAction(menuProject->menuAction());
        menubar->addAction(menuEntity->menuAction());
        menubar->addAction(menuAssets->menuAction());
        menuAssets->addAction(menuCreate->menuAction());
        menuCreate->addAction(actionPrefab);
        menuCreate->addAction(actionPrefab_2);
        menuCreate->addSeparator();
        menuCreate->addAction(actionMaterial);
        menuCreate->addAction(actionMesh);
        menuCreate->addAction(actionShaderProgram);
        menuCreate->addAction(actionTexture2D);
        menuEntity->addAction(actionCreate_from_prefab);
        menuProject->addAction(actionOpen_project);
        menuProject->addAction(actionSave_project);
        menuProject->addAction(actionProject_Settings);

        retranslateUi(WindowMain);
        QObject::connect(widgetHierarchy, SIGNAL(customContextMenuRequested(QPoint)), widgetHierarchy, SLOT(OnCustomContextMenuRequested(QPoint)));
        QObject::connect(widgetHierarchy, SIGNAL(itemSelectionChanged()), widgetHierarchy, SLOT(_NotifyHierarchyItemSelectionChanged()));
        QObject::connect(menubar, SIGNAL(activated(int)), windowEventManager, SLOT(_NotifyMenuBarItemClicked(int)));
        QObject::connect(buttonCreatePrefab, SIGNAL(clicked()), canvas, SLOT(OnTopKekPressed()));
        QObject::connect(buttonPauseResume, SIGNAL(clicked()), canvas, SLOT(OnPauseResumeButtonPressed()));
        QObject::connect(buttonLoggerClear, SIGNAL(clicked()), listLogger, SLOT(clear()));

        QMetaObject::connectSlotsByName(WindowMain);
    } // setupUi

    void retranslateUi(QMainWindow *WindowMain)
    {
        WindowMain->setWindowTitle(QApplication::translate("WindowMain", "Bang Editor", 0));
        actionOpen_project->setText(QApplication::translate("WindowMain", "Open project", 0));
        actionSave_project->setText(QApplication::translate("WindowMain", "Save project", 0));
        actionMaterial->setText(QApplication::translate("WindowMain", "Material", 0));
        actionMesh->setText(QApplication::translate("WindowMain", "Mesh", 0));
        actionShaderProgram->setText(QApplication::translate("WindowMain", "ShaderProgram", 0));
        actionTexture2D->setText(QApplication::translate("WindowMain", "Texture2D", 0));
        actionProject_Settings->setText(QApplication::translate("WindowMain", "Project Settings", 0));
        actionCreate_from_prefab->setText(QApplication::translate("WindowMain", "Create from prefab", 0));
        actionPrefab->setText(QApplication::translate("WindowMain", "Empty Entity", 0));
        actionPrefab_2->setText(QApplication::translate("WindowMain", "Prefab", 0));
        buttonPauseResume->setText(QApplication::translate("WindowMain", "Pause", 0));
        menuAssets->setTitle(QApplication::translate("WindowMain", "Assets", 0));
        menuCreate->setTitle(QApplication::translate("WindowMain", "Create", 0));
        menuEntity->setTitle(QApplication::translate("WindowMain", "Entities", 0));
        menuProject->setTitle(QApplication::translate("WindowMain", "Project", 0));
        dockHierarchy->setWindowTitle(QApplication::translate("WindowMain", "Hierarchy", 0));

        const bool __sortingEnabled = widgetHierarchy->isSortingEnabled();
        widgetHierarchy->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem = widgetHierarchy->topLevelItem(0);
        ___qtreewidgetitem->setText(0, QApplication::translate("WindowMain", "aaaaaa", 0));
        QTreeWidgetItem *___qtreewidgetitem1 = ___qtreewidgetitem->child(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("WindowMain", "bbbbbb", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(0, QApplication::translate("WindowMain", "cccccc", 0));
        QTreeWidgetItem *___qtreewidgetitem3 = ___qtreewidgetitem->child(1);
        ___qtreewidgetitem3->setText(0, QApplication::translate("WindowMain", "dddddd", 0));
        QTreeWidgetItem *___qtreewidgetitem4 = widgetHierarchy->topLevelItem(1);
        ___qtreewidgetitem4->setText(0, QApplication::translate("WindowMain", "xxxxxxxx", 0));
        QTreeWidgetItem *___qtreewidgetitem5 = ___qtreewidgetitem4->child(0);
        ___qtreewidgetitem5->setText(0, QApplication::translate("WindowMain", "wwwwwwwwwwwww", 0));
        QTreeWidgetItem *___qtreewidgetitem6 = ___qtreewidgetitem5->child(0);
        ___qtreewidgetitem6->setText(0, QApplication::translate("WindowMain", "eeeeeeeeeeeeeee", 0));
        QTreeWidgetItem *___qtreewidgetitem7 = ___qtreewidgetitem6->child(0);
        ___qtreewidgetitem7->setText(0, QApplication::translate("WindowMain", "rrrrrrrrrrrrrrrrrr", 0));
        QTreeWidgetItem *___qtreewidgetitem8 = ___qtreewidgetitem6->child(1);
        ___qtreewidgetitem8->setText(0, QApplication::translate("WindowMain", "wwwwwwww", 0));
        QTreeWidgetItem *___qtreewidgetitem9 = ___qtreewidgetitem5->child(1);
        ___qtreewidgetitem9->setText(0, QApplication::translate("WindowMain", "qqqqqqqqqqqq", 0));
        QTreeWidgetItem *___qtreewidgetitem10 = ___qtreewidgetitem5->child(2);
        ___qtreewidgetitem10->setText(0, QApplication::translate("WindowMain", "qqqqqqqqqqqqqssssssssssssss", 0));
        widgetHierarchy->setSortingEnabled(__sortingEnabled);

        dockInspector->setWindowTitle(QApplication::translate("WindowMain", "Inspector", 0));
        labelInspectorEntityName->setText(QApplication::translate("WindowMain", "Entity Name", 0));
        dockExplorer->setWindowTitle(QApplication::translate("WindowMain", "Explorer", 0));
        buttonCreatePrefab->setText(QApplication::translate("WindowMain", "Test: Create prefab", 0));
        buttonExplorerDirUp->setText(QApplication::translate("WindowMain", "Up", 0));
        buttonExplorerChangeViewMode->setText(QApplication::translate("WindowMain", "Change view mode", 0));
        dockLogger->setWindowTitle(QApplication::translate("WindowMain", "Logger", 0));
        buttonLoggerClear->setText(QApplication::translate("WindowMain", "Clear", 0));
    } // retranslateUi

};

namespace Ui {
    class WindowMain: public Ui_WindowMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWMAIN_H
